
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
