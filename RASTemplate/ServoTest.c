#include <RASLib/inc/uart.h>
#include <RASLib/inc/common.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>
#include <RASLib/inc/motor.h>
#include <RASLib/inc/servo.h>
#include <RASLib/inc/adc.h>

static tADC *adc[4];
static tMotor *motor1;
static tMotor *motor2;
static tBoolean initialized = false;
static tBoolean blink_on = true;

// The 'main' function is the entry point of the program
void initMotors(void) {
    // Initialization code can go here
if (!(initialized)){

    initialized = true;

    motor1 = InitializeServoMotor(PIN_B0,false);
    motor2 = InitializeServoMotor(PIN_B1,false);
    adc[0] = InitializeADC(PIN_D0);     //FORWARD
    adc[1] = InitializeADC(PIN_D1);     //LATERAL
    adc[2] = InitializeADC(PIN_D2);
    adc[3] = InitializeADC(PIN_D3);
    
}
}
    
void motorTest(void){       //only uses one IR sensor for wall sensing & turning

    while(1) {

    if(ADCRead(adc[0]) < 0.50){         //forward
        SetMotor(motor1, 0.25);
        SetMotor(motor2, -0.25);
    }
    else if(ADCRead(adc[0]) > 0.50){    //turn                                      
        SetMotor(motor1, 0.15);
        SetMotor(motor2, -0.05);
    }

    }
}

void duoIRSensors(void){    //uses two IR sensors for wall sensing & turning

    float forwardIR = ADCRead(adc[0]); 
    float sideIR = ADCRead(adc[1]);
    
    //(far)0 <==> 1(close)
    while(1){
        
//
        if(forwardIR < 0.50 && forwardIR < 0.60 && sideIR > 0.50 ){ 
        //robot has clear path ahead && lateral fair interval distance goldilocks
        
        //robot will move FORWARD
        
        SetMotor(motor1, 0.25);
        SetMotor(motor2, -0.25);
        
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

//Chicken and Waffles
