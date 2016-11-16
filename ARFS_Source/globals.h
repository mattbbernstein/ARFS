#ifndef GLOBALS_H
#define GLOBALS_H

// TIMING DEFINITIONS (all in micros)
#define EPEE_DEBOUNCE 14000
#define EPEE_LOCKOUT  45000

#define FOIL_DEBOUNCE 15000
#define FOIL_LOCKOUT  300000

#define SABRE_DEBOUNCE 0
#define SABRE_LOCKOUT 170000

// PIN DEFINITIONS
const int modeSourcePin = 17;
const int epeeModePin = 16;
const int foilModePin = 18;
const int sabreModePin = 19;
const int ALinePin = A0; //Pin 14
const int BLinePin = 22;
const int CLinePin = 23;
const int hitLEDPin = 2;
const int touchLEDPin = 1;
const int buzzerPin = 7;


// GLOBALS AND CONSTANTS

#define BUZZER_LINGER_ms 1500
#define LIGHTS_LINGER_ms 2000
#define BUZZ_TONE        4000
#define PULSE            512    // Pulse singal, 50% duty cycle
#define PULSE_RNG        50

#define PINPUT(x)   pinMode((x),INPUT)
#define POUTPUT(x)  pinMode((x),OUTPUT)
#define PPULLUP(x)  pinMode((x),INPUT_PULLUP)
#define TURN_ON(x)  digitalWrite((x),HIGH)
#define TURN_OFF(x) digitalWrite((x),LOW)
#define PULSE_ON(x) analogWrite((x),PULSE);
#define ATURN_OFF(x)analogWrite((x),0);
#define READ(x)     digitalRead(x)
#define AREAD(x)    analogRead(x)
#define BUZZ_ON(x)  tone((x),BUZZ_TONE);
#define BUZZ_OFF(x) noTone((x))

  
#define EPEE  0
#define FOIL  1
#define SABRE 2

int mode;
unsigned long lockout_us;
unsigned long debounce_us;
unsigned long hitTime;
unsigned long touchTime;
unsigned long lockoutStartTime;
bool hit;
bool validHit;
bool touch;
bool validTouch;
bool lockoutStart;
bool lockedOut;

#endif
