!#include <ADXL345.h>
#include <Keyboard.h>
#include <Joystick.h>
#include <CD4021BE.h>
#include <C74HC595N.h>
#include <CTimer.h>
#include <CPlunger.h>

#include "Wire.h"
#include "I2Cdev.h"

#define LeftShift     0
#define RightShift    1
#define Enter         2
#define Echap         3
#define Start         4
#define Credit        7

CPlunger plunger(10);
CTimer *accelTimer;
CCD4021BE *inputs;
ADXL345 accel; //I2C pin on arduino Leonardo are SCL Digital pin 03, SDA Digital pin 02
C74HC595N outputs(7, 8, 9);

/*****************************************************************************************
 * 
 *  --------------------------------------------------------------
 *  |    |    |    |    |    |    |    |    | 12 | 11 |  13 | 5V | 
 *  --------------------------------------------------------------
 *  |    |    |    |    |    |    |    |    | 7  | 8  |   9 | GND| 
 *  --------------------------------------------------------------
 *  
 *****************************************************************************************/

//fonction déclenchée par le timer accelTimer, cette function recupere les infos de l'accelerometre est initialise les axes X et Y du joystick
void getAccel()
{
  //saisie des valeurs de l'accel en tant que position des axes x et y du joystick
  Joystick.setXAxis(accel.getAccelerationX());
  Joystick.setYAxis(accel.getAccelerationY());
}

//fonction appelle par l'objet CCD4021BE inputs à chaque tour de boucle loop
//permet de réaliser des actions en fonction de l'etats des boutons
void testCD4021Btn(CCD4021BE *cd4021)
{
  if(cd4021->getInputState(LeftShift))
  {
     Joystick.setButton(LeftShift, true);
     //Keyboard.press(KEY_LEFT_SHIFT);
     /*outputs.setRegisterPin(0, HIGH);
     outputs.setRegisterPin(1, HIGH);
     outputs.setRegisterPin(2, HIGH);
     outputs.setRegisterPin(3, HIGH);
     outputs.setRegisterPin(4, HIGH);
     outputs.setRegisterPin(5, HIGH);
     outputs.setRegisterPin(6, HIGH);
     outputs.setRegisterPin(7, HIGH);*/
  }
  else
  {
    Joystick.setButton(LeftShift, false);    
    //Keyboard.release(KEY_LEFT_SHIFT);    
    /*outputs.setRegisterPin(0, LOW);
    outputs.setRegisterPin(1, LOW);
    outputs.setRegisterPin(2, LOW);
    outputs.setRegisterPin(3, LOW);
    outputs.setRegisterPin(4, LOW);
    outputs.setRegisterPin(5, LOW);
    outputs.setRegisterPin(6, LOW);
    outputs.setRegisterPin(7, LOW);*/
    
  }
  if(cd4021->getInputState(RightShift))
  {
     Joystick.setButton(RightShift, true);
     //Keyboard.press(KEY_RIGHT_SHIFT);
  }
  else
  {
    Joystick.setButton(RightShift, false);
    //Keyboard.release(KEY_RIGHT_SHIFT);
  }
  if(cd4021->getInputState(Enter))
  {
     Joystick.setButton(Enter, true);
     Keyboard.press(KEY_RETURN);
  }
  else
  {
    Joystick.setButton(Enter, false);
    Keyboard.release(KEY_RETURN);
  }
  if(cd4021->getInputState(Echap))
  {
     Joystick.setButton(Echap, true);
     Keyboard.press(KEY_ESC);
  }
  else
  {
    Joystick.setButton(Echap, false);
    Keyboard.release(KEY_ESC);
  }
  if(cd4021->getInputState(Start))
  {
     Joystick.setButton(Start, true);
     Keyboard.press('1');
  }
  else
  {
    Joystick.setButton(Start, false);
    Keyboard.release('1');
  }
  if(cd4021->getInputState(5))
  {
     Joystick.setButton(5, true);    
  }
  else
  {
    Joystick.setButton(5, false);
  }
  if(cd4021->getInputState(6))
  {
     Joystick.setButton(6, true);
  }
  else
  {
    Joystick.setButton(6, false);
  }
  if(cd4021->getInputState(Credit))
  {
     Joystick.setButton(Credit, true);
     Keyboard.press('5');
  }
  else
  {
    Joystick.setButton(7, false);
    Keyboard.release('5');
  }
}

void setup() 
{  
  // Initialize Button Pins
  // join I2C bus (I2Cdev library doesn't do this automatically)
  Wire.begin();
  // initialize serial communication
  // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
  // it's really up to you depending on your project)
  //Serial.begin(38400);
  // initialize device
  //Serial.println("Initializing I2C devices...");
  accel.initialize();
  //verify connection
  //Serial.println("Testing device connections...");
  //Serial.println(accel.testConnection() ? "ADXL345 connection successful" : "ADXL345 connection failed");
  //creation de l'objet CCD4021BE
  inputs = new CCD4021BE(11/*vert latch*/, 12/*bleu data*/, 13/*jaune clock*/, &testCD4021Btn);
  accelTimer = new CTimer(20, &getAccel);
  //init de l'objet accelTimer, outputs et inputs
  accelTimer->setup();
  inputs->setup();
  outputs.setup();
  //configuration du pin potpin en tant qu'entrée
  plunger.setup();
  //init de l'arduino en tant que joystick
  Joystick.begin();
  //init de l'arduino en tant que clavier
  Keyboard.begin();
}

void loop() 
{
  //traitement des entrées inputs
  inputs->loop();
  //on execute le timer qui lors du declenchement fait appel à la fonction getAccel()
  accelTimer->loop();
  //traitement des sorties outputs
  outputs.loop();
  //traitement du plunger
  plunger.loop();
 }
