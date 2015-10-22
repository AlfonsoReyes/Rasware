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

// The 'main' function is the entry point of the program
int main(void) {
    // Initialization code can go here
if (!(initialized)){

    initialized = true;

    motor1 = InitializeServoMotor(PIN_B0,false);
    motor2 = InitializeServoMotor(PIN_B1,false);
    adc[0] = InitializeADC(PIN_D0);
    adc[1] = InitializeADC(PIN_D1);
    adc[2] = InitializeADC(PIN_D2);
    adc[3] = InitializeADC(PIN_D3);    
    
}
    
    float pos1 = 0;
    float pos2 = 0;
    char ch;

    while(!KeyWasPressed()) {
    
    Printf( 
        "IR values: %1.3f %1.3f %1.3f %1.3f\r", 
        ADCRead(adc[0]),
        ADCRead(adc[1]),
        ADCRead(adc[2]),
        ADCRead(adc[3])
        );

//adc is Analog to Digital Conversion

    if(ADCRead(adc[0]) > 0.90){
        SetMotor(motor1, 0.25);
        SetMotor(motor2, -0.25);
    }
    else{
        SetMotor(motor1, -0.25);
        SetMotor(motor2, 0.25);
    }

    }
}

