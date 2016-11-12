//======================================
// ARFS System Code
//======================================
// Matt Bernstein
// mattbbernstein@gatech.edu
// Last updated: 10/14/2016

#include "globals.h"

void setup() {
  // SETUP INPUT AND OUTPUT
  setUpPins();

  BUZZ_ON(buzzerPin);
  delay(1000);
  BUZZ_OFF(buzzerPin);
  delay(1000);

  // CHECK THE MODE
  TURN_OFF(modeSourcePin);
  delay(10);
  if(READ(epeeModePin)==LOW){
    mode = EPEE;
    lockout_us = EPEE_LOCKOUT;
    debounce_us = EPEE_DEBOUNCE;
    attachInterrupt(digitalPinToInterrupt(ALinePin),hitRegistered,RISING);
  } else if(READ(foilModePin)==LOW){
    mode = FOIL;
    lockout_us = FOIL_LOCKOUT;
    debounce_us = FOIL_DEBOUNCE;
    attachInterrupt(digitalPinToInterrupt(CLinePin),hitRegistered,FALLING);
    attachInterrupt(digitalPinToInterrupt(ALinePin),touchRegistered,RISING);
  } else if(READ(sabreModePin)==LOW){
    mode = SABRE;
    lockout_us = SABRE_LOCKOUT;
    debounce_us = SABRE_DEBOUNCE;
    attachInterrupt(digitalPinToInterrupt(ALinePin),touchRegistered,RISING);
  }

  TURN_OFF(modeSourcePin);

  // BUZZ AND BLINK ALL THE LIGHTS TO INDICATE SET UP IS OVER
  for(int i = 0; i <= mode; i++){
    BUZZ_ON(buzzerPin);
    TURN_ON(hitLEDPin);
    TURN_ON(touchLEDPin);
    delay(500);
    BUZZ_OFF(buzzerPin);
    TURN_OFF(hitLEDPin);
    TURN_OFF(touchLEDPin);
    delay(500);
  }

  // Wait 1 second before hitting the loop
  delay(1000);
}

void loop() {
  // Alerting the all go
  resetBeeps();

  TURN_ON(BLinePin);

  bool execute = checkResidual(); // Checks if the button is being held down or any residual/consistent connection;

  while(execute){
   // startTime = micros();

    // If we've been locked out, stop everything
    if(lockoutStart){
      if(micros() >= lockoutStartTime + lockout_us){
        break;
      }
    }
    
    // If we have hit + debounce time
    if(hit && micros() >= hitTime + debounce_us){
      // Check to see if we're still depressing the button
      bool validHit = false;
      if(mode == EPEE) validHit = (READ(ALinePin) == HIGH);
      if(mode == FOIL) validHit = (READ(CLinePin) == LOW);
      // If not, reset the hit flag
      if(!validHit){
        hit = false;
      }else{ // If it is valid
        TURN_ON(hitLEDPin);           // Light goes on
        BUZZ_ON(buzzerPin);           // Buzz buzz
        if(!lockoutStart){  // Start the lockout period if we havent already
          lockoutStartTime = micros();  // Set the lockout timer
          lockoutStart = true;          // Flag lockout
        }
      }
    } // END IF HIT

    // If we have been touched + debounce time
    if(touch && micros() >= touchTime + debounce_us){
      bool validTouch = true;
      if(mode == FOIL){ // Account for debounce time in foil
        validTouch = (READ(ALinePin) == HIGH);
        if(!validTouch) touch = false;
      }
      // If the touch is valid
      if(validTouch){
        TURN_ON(touchLEDPin);           // Light goes on
        BUZZ_ON(buzzerPin);           // Buzz buzz
        if(!lockoutStart){  // Start the lockout period if we havent already
          lockoutStartTime = micros();  // Set the lockout timer
          lockoutStart = true;          // Flag lockout
        }
      }
    } // END IF TOUCH
  }

  // End routines before reset
  delay(BUZZER_LINGER_ms);
  BUZZ_OFF(buzzerPin);
  delay(LIGHTS_LINGER_ms - BUZZER_LINGER_ms);
  TURN_OFF(touchLEDPin);
  TURN_OFF(hitLEDPin);
  delay(1000);
}

void hitRegistered() {
  if(!hit){
    hitTime = micros();
    hit = true;
  }
}

void touchRegistered() {
  if(!touch){
    touchTime = micros();
    touch = true;
  }
}

void setUpPins(){
  POUTPUT(modeSourcePin);
  PPULLUP(epeeModePin);
  PPULLUP(foilModePin);
  PPULLUP(sabreModePin);
  PINPUT(ALinePin);
  POUTPUT(BLinePin);
  PINPUT(CLinePin);
  POUTPUT(hitLEDPin);
  POUTPUT(touchLEDPin);
  POUTPUT(buzzerPin);
}

void chirp(int time_ms){
  BUZZ_ON(buzzerPin);
  delay(time_ms);
  BUZZ_OFF(buzzerPin);
}

void resetBeeps(){
  chirp(100);
  delay(100);
  chirp(100);
  delay(100);  
  chirp(100);
  delay(100);
  chirp(500);

  hit = false;
  touch = false;
  lockedOut = false;
  lockoutStart = false;
}

bool checkResidual(){
  bool execute = true;
  if(mode == FOIL && READ(CLinePin)==LOW){
    hit = true;
    execute = false;
    hitTime = micros();
  }

  if(mode == FOIL && READ(ALinePin)==HIGH){
    touch = true;
    execute = false;
    touchTime = micros();
  }

  if(mode == SABRE && READ(ALinePin)==HIGH){
    touch = true;
    execute = false;
    touchTime = micros();
  }

  if(mode == EPEE && READ(ALinePin)==HIGH){
    hit = true;
    execute = false;
    hitTime = micros();
  }

  execute = true;
  return execute;
}

