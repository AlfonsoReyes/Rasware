#include <RASTemplate/ServoTest.h>
#include <RASLib/inc/common.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>

//tBoolean led_on;

//void blink(void){
    
//    SetPin(PIN_F1, led_on);
//    SetPin(PIN_F2, !led_on);
//
//    led_on = !led_on;;
//}


void main(void){

//    CallEvery(blink, 0.25f);

    initMotors();

//    turnWindmill();

//    forward();
//    WaitUS(8000000);
//    turn90();
//    WaitUS(500000);
//    turn90();
//    WaitUS(500000);
//    forward();
//    WaitUS(8000000);
//    stop();
    turnWindmill();
//    motorTest();;
//    LineSensorTestGate();
}

