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
    //lockout_us = EPEE_LOCKOUT;
    //debounce_us = EPEE_DEBOUNCE;
    //attachInterrupt(digitalPinToInterrupt(ALinePin),hitRegistered,RISING);
  } else if(READ(foilModePin)==LOW){
    mode = FOIL;
    //lockout_us = FOIL_LOCKOUT;
    //debounce_us = FOIL_DEBOUNCE;
    //attachInterrupt(digitalPinToInterrupt(CLinePin),hitRegistered,FALLING);
    //attachInterrupt(digitalPinToInterrupt(ALinePin),touchRegistered,RISING);
  } else if(READ(sabreModePin)==LOW){
    mode = SABRE;
    //lockout_us = SABRE_LOCKOUT;
    //debounce_us = SABRE_DEBOUNCE;
   // attachInterrupt(digitalPinToInterrupt(ALinePin),touchRegistered,RISING);
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

  //bool execute = checkResidual(); // Checks if the button is being held down or any residual/consistent connection;

  if(mode == EPEE){
    epeeLoop();
  } else if(mode == FOIL){
    foilLoop();
  } else if(mode == SABRE){
    sabreLoop();
  }

  TURN_OFF(BLinePin);

  // End routines before reset
  delay(BUZZER_LINGER_ms);
  BUZZ_OFF(buzzerPin);
  delay(LIGHTS_LINGER_ms - BUZZER_LINGER_ms);
  TURN_OFF(touchLEDPin);
  TURN_OFF(hitLEDPin);
  delay(1000);
}

//=============================================================//
//=============================================================//

void epeeLoop(){
  while(true){
    int loopTime = micros();

    if(lockoutStart && loopTime - lockoutStartTime >= EPEE_LOCKOUT){  // If we're in the lockout period, check time
      break;          // Break the loop if lockout ends
    }
    
    if(!hit && READ(ALinePin)==HIGH){               // If we haven't already hit and we're pressing the button
      hit = true;
      hitTime = loopTime;
    }

    if(hit && loopTime-hitTime >= EPEE_DEBOUNCE){ // If we're debounce time after the hit
      if(READ(ALinePin) == HIGH){  // If we're still pressing the button and we haven't started lockout
        TURN_ON(hitLEDPin);
        BUZZ_ON(buzzerPin);
        if(!lockoutStart){                            // If we haven't started the lockout period
          lockoutStart = true;                        // Start lockout zone
          lockoutStartTime = loopTime;                // Markout lockout start time
        }
      } else {                                    // Else, we didn't meet debounce
        hit = false;
      }
    }// End hit check
  
  }// End infinite while
}

//=============================================================//
//=============================================================//

void foilLoop(){
  while(true){
    int loopTime = micros();

    if(lockoutStart && loopTime-lockoutStartTime >= FOIL_LOCKOUT){  // If we're in the lockout period
      break;    // Break the loop if lockout ends
    }

    if(!hit && READ(CLinePin) == LOW){          // If we havent hit and we're pressing the button
      hit = true;
      hitTime = loopTime;
    }

    if(!touch && READ(ALinePin) == HIGH){       // If we haven't been touched and the lame is going high
      touch = true;
      touchTime = loopTime;
    }

    if(hit && loopTime-hitTime >= FOIL_DEBOUNCE){   // If we're debounce time after the hit
      if(READ(CLinePin) == LOW){   // If we're still pressing the button and we haven't started lockout
        TURN_ON(hitLEDPin);
        BUZZ_ON(buzzerPin);
        if(!lockoutStart){                            // If we haven't started the lockout period
          lockoutStart = true;                        // Start lockout period
          lockoutStartTime = loopTime;                // Mark the lockout time
        }
      } else {                                      // Else we didnt meet debounce
        hit = false;
      }
    }// End hit check

    if(touch && loopTime-touchTime >= FOIL_DEBOUNCE){   // If we're debounce time after the hit
      if(READ(ALinePin) == HIGH){   // If we're still pressing the button and we haven't started lockout
        TURN_ON(touchLEDPin);
        BUZZ_ON(buzzerPin);
        if(!lockoutStart){                            // If we haven't started the lockout period
          lockoutStart = true;                        // Start lockout period
          lockoutStartTime = loopTime;                // Mark the lockout time
        }
      } else {                                      // Else we didnt meet debounce
        touch = false;
      }
    }// End touch check
    
  }// End infinte while
}

//=============================================================//
//=============================================================//

void sabreLoop(){
   while(true){
    int loopTime = micros();

    if(lockoutStart && loopTime-lockoutStartTime >= SABRE_LOCKOUT){  // If we're in the lockout period, check time
      break;          // Break the loop if lockout ends
    }
    
    if(!touch && READ(ALinePin)==HIGH){               // If we haven't already hit and we're pressing the button
      touch = true;
      touchTime = loopTime;
    }

    if(touch){                                    // If we've been touched
      TURN_ON(touchLEDPin);
      BUZZ_ON(buzzerPin);
      if(!lockoutStart){                            // If we haven't started the lockout period
        lockoutStart = true;                        // Start lockout zone
        lockoutStartTime = loopTime;                // Markout lockout start time
      }
    }// End touch check
  
  }// End infinite while
}


