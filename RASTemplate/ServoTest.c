#include <RASLib/inc/uart.h>
#include <RASLib/inc/common.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>
#include <RASLib/inc/motor.h>
#include <RASLib/inc/servo.h>
#include <RASLib/inc/adc.h>

static tADC *adc[4];
static tServo *servo1;
static tServo *servo2;
static tBoolean initialized = false;

// The 'main' function is the entry point of the program
int main(void) {
    // Initialization code can go here
if (!(initialized)){

    initialized = true;

    servo1 = InitializeServo(PIN_B0);
    servo2 = InitializeServo(PIN_B1);
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
    

    if(ADCRead(adc[0]) > 0.90){
        SetServo(servo1, 0.55);
        SetServo(servo2, 0.45);
    }
    else{
        SetServo(servo1, 0.45);
        SetServo(servo2, 0.55);;
    }

    }
}    

//    Printf(" w - forward \n" "s - reverse\n" "a- left \n" "d - right \n");
      
//    ch = Getc();
//    Printf("%c\n", ch);
//    while (1) {
//        // Runtime code can go here
//        switch(ch){
//      case 'w':           //forward
//            pos1 = 0.75f;
//            pos2 = 0.25f;           
//            break;
//           
//        case 's':           //reverse
//            pos1 = 0.25f;
//
//            pos2 = 0.75f;
//           break;
//           
//        case 'a':           //left
//            pos1 = 0.75f;
//            pos2 = 0.50;
//            break;
//        case 'd':           //right
//           pos1 = 0.50;
//            pos2 = 0.75f;
//            break;
//           
//        case 'x':           //stpp
//            pos1 = 0.5;
//            pos2 = 0.5;
//            break;
//        case 'l':
//            return 0;
//        default:
//            pos1 = pos1;
//            pos2 = pos2;       
//        }
  
      

//    SetServo(servo1,pos1);
//    SetServo(servo2,pos2);

   // ch = Getc();
   
//    }


