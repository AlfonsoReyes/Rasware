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
    motorTest();    




}
