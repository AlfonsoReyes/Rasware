// ServoTest.c
// Contains the functions that manipulate servos, motors, etc. that will be used in 
// RoboHub.c
// Included Files
#include <RASLib/inc/uart.h>        // Contains functions to utilize UART
#include <RASLib/inc/common.h>      // Common file
#include <RASLib/inc/gpio.h>        // Contains functions to utilize ports of the controller
#include <RASLib/inc/time.h>        // Contains functions to delay and manipulate system time
#include <RASLib/inc/motor.h>       // Contains functions to manipulate motors
#include <RASLib/inc/servo.h>       // Contains functions to manipulate servos
#include <RASLib/inc/adc.h>         // Contains functions dealing with ADC
#include <RASLib/inc/linesensor.h>

// Variable Definitions
static tADC *adc[4];                  // Array for using ADC 
static tMotor *motor1;                // Variable to manipulate a motor
static tMotor *motor2;                // Variable to manipulate a motor
static tMotor *motor3;                // Variable to manipulate a motor
static tBoolean initialized = false;  // Flag that indicates whether initialization occurred
static tBoolean blink_on = true;
static tLineSensor *gls;              // Variable to hold line sensor values
static tServo *servo1;                // Variable to manipulate a servo

// Initializations
void initMotors(void) {
    if (!(initialized)){              // If initialization has not occurred, continue.
        initialized = true;           // Set the initialization flag.
        servo1 = InitializeServo(PIN_E4);
        motor1 = InitializeServoMotor(PIN_B0,false);
        motor2 = InitializeServoMotor(PIN_B1,false);
        motor3 = InitializeServoMotor(PIN_E5,false);
        adc[0] = InitializeADC(PIN_D0);      // FORWARD
        adc[1] = InitializeADC(PIN_D1);      // LATERAL
        adc[2] = InitializeADC(PIN_D2);
        adc[3] = InitializeADC(PIN_D3);

        gls = InitializeGPIOLineSensor( // Line Sensor Initialization 
        PIN_E0,     //1
        PIN_E2,     //2
        PIN_D0,     //3
        PIN_D1,     //4
        PIN_D2,     //5
        PIN_D3,     //6
        PIN_E3,     //7
        PIN_E1);    //8
    }
}

// Function Definitions

// turnWindmill sets the robot to move forward at MAXIMUM OVERDRIVE.
void turnWindmill(void){
    while(1){    
         SetMotor(motor3, 1.0); // Spin "windmill".
     }
}

// forward moves the robot forward.
void forward(void){             
    SetMotor(motor1, -0.85);
    SetMotor(motor2, 1.0);
}

// stop stops the robot.
void stop(void){
    SetMotor(motor1, 0);
    SetMotor(motor2, 0);
}

//turn90 turns the robot 90 degrees.
void turn90(void){
    SetMotor(motor1, 1.0);
    SetMotor(motor2, 1.0);
    WaitUS(1600000);
    SetMotor(motor1, 0);
    SetMotor(motor2, 0);


}

void shake(void){

SetMotor(motor1,-0.95);
SetMotor(motor2, -0.95);
WaitUS(200000);

SetMotor(motor1, 0.95);
SetMotor(motor2, 0.95);
WaitUS(200000);


}

//servo conditions, @ 0.50, in middle, @ 1.0, top, @ 0.0 , bottom
//moves servo gate up -> MARBLES EXIT, PING PONG BALLS BLOCKED
void gateOpenUp(void){
SetServo(servo1, 0.50);

shake();
shake();
shake();
shake();
shake();
}
//moves servo gate down -> MARBLES BLOCKED, PING PONG BALLS EXIT
void gateOpenDown(void){
SetServo(servo1, 0.10);
shake();
shake();
shake();
shake();
shake();
}
//servo gate closes -> MARBLES & PING PONG BALLS BLOCKED
void gateClose(void){

SetServo(servo1, 0.30);
}


//motor 1 is right motor, negative is forward
//motor 2 is left motor, positive is forward
void motorTest(void){       //only uses one IR sensor for wall sensing & turning

    float speedOne;
    float speedTwo;


    while(1) {

    if(ADCRead(adc[3]) > 0.60){

        speedOne = -0.95;
        speedTwo = -0.95;

    }
    else if(ADCRead(adc[0]) < 0.35){         //forward
        speedOne = -0.30;
        speedTwo = 0.30;

    }
    else if(ADCRead(adc[0]) > 0.35){    //turn right
        speedOne = -0.95;
        speedTwo = 0.10;

    }


        SetMotor(motor1,speedOne);
        SetMotor(motor2,speedTwo);
        SetMotor(motor3, 0.50);
    }

    
    
}


void duoIRSensors(void){    //uses two IR sensors for wall sensing & turning

    float forwardIR = ADCRead(adc[0]);
    float sideIR = ADCRead(adc[1]);

    //(far)0 <==> 1(close)
    while(1){

        if(forwardIR < 0.50 && forwardIR < 0.60 && sideIR > 0.50 ){ 
        //robot has clear path ahead && lateral fair interval distance goldilocks
        
        //robot will move FORWARD
        
        SetMotor(motor1, 0.20);
        SetMotor(motor2, -0.20);

        }
        else if(sideIR < 0.50){     //robot is too far away from wall LATERALLY
        //robot will turn TOWARDS side wall
        }
        else if(sideIR > 0.60){     //robot is too close to wall LATERALL
        //robot will turn AWAY from side wall
        }
        else if(forwardIR > 0.5){   //robot is close to wall AHEAD
        //robot will turn
        }
        else{       //LED will turn on if exceptional case occurs
        
        SetPin(PIN_F2,true);

        }
    
    SetPin(PIN_F2, false);
    forwardIR = ADCRead(adc[0]);
    sideIR = ADCRead(adc[1]);

    }


}

//robot will correct course, do 60 degree turns, or move forward
//use line sensor (8 points of data) to determine action to carry out
//yall should check out my dank mixtape btdubz

    //motor 1 is on right, motor 2 is on left

    //MOTOR 2           MOTOR 1
    //          FRONT
    // 8  7  6  5  4  3  2  1 (Line Sensor)
    //          BACK
    //    5  4  3  2  1  0      (vals array)


void LineSensorTest(void){

    int i;
    float line[8];
    int vals[6];
    int sumOuters;
    float threshold = 0.40;
    float spdOne;
    float spdTwo;
//    LineSensorReadArray(gls,line);
//  vals[2] and [3] are the middle ones

    //1 is carpet, 0 is black
    while(1){
//    LineSensorReadArray(gls,line);
    if(line[1] < threshold){
    vals[0] = 1;
    }
    else{
    vals[0] = 0;
    }
    
    if(line[2] < threshold){     
    vals[1] = 1;
    }
    else{
    vals[1] = 0;
    }


    if(line[3] < threshold){     //middle
    vals[2] = 1;
    }
    else{
    vals[2] = 0;
    }

    if(line[4] < threshold){     //middle
    vals[3] = 1;
    }
    else{
    vals[3] = 0;
    }

    if(line[5] < threshold){
    vals[4] = 1;
    }
    else{
    vals[4] = 0;
    }

    if(line[6] < threshold){
    vals[5] = 1;
    }
    else{
    vals[5] = 0;
    }

    sumOuters = vals[1] + vals[4];

    //new approach, each different condition INCREMENTS speed in certain manner
    // 1 is non black tape
    // 0 is black tape    

    if( vals[2] == 0 && vals[3] == 0  && vals[1] ==1 && vals[4] == 1){
    //ONLY 2 and 3 on black tape, others on carpet --> move forward
    spdOne = -0.35;
    spdTwo = 0.35;
    //move forward
    }
    else if(vals[1] == 0){      //turn right
    spdOne = -0.35;
    spdTwo = 0.25;
    }
    else if(vals[4] == 0){      //turn left
    spdOne = -0.25;
    spdTwo = 0.35;
    
    }
    else if(vals[2] = 1 && vals[3] ==1 && vals[4] ==1 && vals[5] ==1){
    //stick to preveous values
    spdOne = spdOne;
    spdTwo = spdTwo;

    

    }

//    SetMotor(motor1, spdOne);
//    SetMotor(motor2, spdTwo);


    }




}

//code to run for competition


//uses Line Sensor to operate servo gate allowing marbles/ping pong balls to exit
void LineSensorTestGate(void){

    int ls1;
    int ls2;
    float lne[8];
    int gateState;
    //if 1, open up, marbles leave, @home post
    //if 0, open down, ping pong balls leave, @enemy post
    gateState = 1;

    while(1){

    LineSensorReadArray(gls, lne);
    //Determine state of each line sensor node
    //1 -> carpet, 0 -> black tape
    if(lne[3] < 0.40){
        ls1 = 1;

    }
    else{
        ls1 = 0;
    }
   
    if(lne[4] < 0.40){
        ls2 = 1;
    }
    else{
        ls2 = 0;
    }
    
    //all active line sensor nodes @ black tape
    // ==> trigger gate event

    if(ls1 == 0 && ls2 == 0){
        //stop moving motors
        SetMotor(motor1, 0);
        SetMotor(motor2, 0);

        if(gateState == 1){
            gateOpenUp();
        }
        else{
            gateOpenDown();
        }
        
        gateClose();

        SetMotor(motor1, -0.40);
        SetMotor(motor2, 0.40);
        WaitUS(1000000);
    
        SetMotor(motor1, 0);
        SetMotor(motor2, 0);

        if(gateState == 1){
            gateState = 0;
        }
        else if(gateState != 1){
            gateState = 1;
        }

    }
    else{
        
        gateClose();
    }

    }
}



void fullRun(void){

float m1Spd;
float m2Spd;
float m3Spd;

int lsf1;
int lsf2;
float line[8];
int gateCond;

gateCond = 1;

while(1){
    
    gateClose();

    LineSensorReadArray(gls, line);

    if(line[3] < 0.90){
        lsf1 = 1;
    }
    else{
        lsf1 = 0;
    }

    if(line[4] < 0.90){
        lsf2 = 1;
    }
    else{
        lsf2 = 0;
    }

    if(lsf1 == 0 && lsf2 == 0){
        SetMotor(motor1, 0);
        SetMotor(motor2, 0);

        if(gateCond == 1){
            gateOpenUp();
        }
        else{
            gateOpenDown();
        }

        gateClose();
    

        SetMotor(motor1, -0.40);
        SetMotor(motor2, 0.40);
        WaitUS(1000000);

        SetMotor(motor1, 0);
        SetMotor(motor2, 0);
    
        if(gateCond == 1){
           gateCond = 0;
        }
        else if(gateCond == 0){
            gateCond = 1;
        }

    }
    else{

        gateClose();

    }


    if(ADCRead(adc[3]) > 0.60){
        m1Spd = -0.95;
        m2Spd = -0.95;
    }
    else if(ADCRead(adc[0]) < 0.40){
        m1Spd = -0.30;
        m2Spd = 0.30;

    }
    else if(ADCRead(adc[0]) > 0.40){
        m1Spd = -0.95;
        m2Spd = 0.10;

    }
    
    SetMotor(motor1, m1Spd);
    SetMotor(motor2, m2Spd);
    SetMotor(motor3, 0.40);




}




}

//Chicken and Waffles
