/*
 * AUTHOR: Gector
 * Written: 12/21/2017
 * Last Edit: 12/21/2017 - Comments
 */

// -------------------------------------- \\
// ------------ VARIABLES --------------- \\
// -------------------------------------- \\

// Our time units.
int second=0, minute1=1, minute2=1, hour=1, halfDay=1;

// Physical pin numbers. 
const int inHourPin = 12, inMinutePin = 13;
const int outEnable = 2;
const int clockPin = 3;
const int dataPin = 5;
const int latchPin = 4;

const int clearAmount = 16; // If you add more shift registers to clear.

int wait = 0; // Add delay to the clock to see it shift stuff


// -------------------------------------- \\
// ------------ FUNCTIONS --------------- \\
// -------------------------------------- \\

// Shift bits down by one (positive edge triggered)
void clockReg(int hold = wait){
  digitalWrite(clockPin, HIGH);
  delay(hold);
  digitalWrite(clockPin, LOW);
  delay(hold);
}

// Grab a new bit from input (positive edge triggered)
void latch(int hold = wait){
  digitalWrite(latchPin, HIGH);
  delay(hold);
  digitalWrite(latchPin, LOW);
  delay(hold);
}

// Activate a single bit
void activateBit(int hold = wait){
  digitalWrite(dataPin, HIGH);
  latch(hold);
  clockReg(hold);
  digitalWrite(dataPin, LOW);
}

// set whether or not the LEDs are lit or not. (if this isn't done they flicker with the calculations) 
void enableOutput(boolean state){
  if(state) {
    digitalWrite(outEnable, LOW);
  } else {
    digitalWrite(outEnable, HIGH);
  }
}

//... clear all.
void clearAll(){
  digitalWrite(dataPin, LOW);
  for(int x = 0;x <= clearAmount; x++){
    clockReg(0);
    latch(0);
  }
}

// Display hours
void displayHour(int hour){
  if(hour >= 1) {
    for(int x = 3; x >= 0; x--) {
      int bit = bitRead(hour, x);
      if(bit == 1){ // Clock and data
        digitalWrite(dataPin, HIGH);
        latch();
        clockReg();
        digitalWrite(dataPin, LOW);
      } else { // Just clock
        latch();
        clockReg();
      }
    }
  }
}

// Sends on extra bit if it's PM, sends one extra clock if it's AM
void AmPm(int state){
  if(state >= 1) {
    digitalWrite(dataPin, HIGH);
    latch();
    clockReg();
    digitalWrite(dataPin, LOW);
  } else {
    latch();
    clockReg();
  }
}

// Set the first line of LEDs designated to minutes
void displayMinL1(int tens){
  for(int x = 3; x >= 0; x--) {
    int bit = bitRead(tens, x);
    if(bit == 1){ // Clock and data
      digitalWrite(dataPin, HIGH);
      latch();
      clockReg();
      digitalWrite(dataPin, LOW);
    } else { // Just clock
      latch();
      clockReg();
    }
  }
}

// Set the second line of LEDs designated to minutes
void displayMinL2(int ones){
    for(int x = 3; x >= 0; x--) {
      int bit = bitRead(ones, x);
      if(bit == 1){ // Clock and data
        digitalWrite(dataPin, HIGH);
        latch();
        clockReg();
        digitalWrite(dataPin, LOW);
      } else { // Just clock
        latch();
        clockReg();
      }
    }
}

// Set all bits in sequence - usefuly for both lookinng cool and for debugging.
void view(int hold = wait){
  for(int x = 0;x <= clearAmount; x++){
    activateBit(hold);
  }
}

// -------------------------------------- \\
// -------------- MAIN ------------------ \\
// -------------------------------------- \\

// Code that runs first
void setup()
{
  
  //Serial.begin(115200); // Commented out code doesn't do anything.

  // Tell the microcontroller what pins we are using for output.
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(outEnable, OUTPUT);

  // Tell the microcontroller what pins we are using... for input.
  pinMode(inHourPin, INPUT);
  pinMode(inMinutePin, INPUT);
  clearAll();
  enableOutput(true);
}

// Code that runs repeatedly.  
void loop()
{
  /*
  clearAll();
  delay(100);
  view(100);
  delay(100);
  */
  
  // This will keep ticking away and not get reset even though it's in the loop. 
  
  static unsigned long lastTick = 0;

  // One second every 1000ms
  if(millis() - lastTick >= 1000){ // fix this ***
    lastTick = millis();
    second++;
  }

  // One minute every 60s
  if(second >= 60){
    minute2++;
    second = 0; 
  }
  if(minute2 >= 10){
    minute1++;
    minute2=0;
    second=0;
  }

  // One hour every 60m
  if(minute1 >= 6){
    hour++;
    minute1 = 0;
  }

  // One half-day every 12 hours
  if(hour >= 13){
    halfDay++;
    hour=1;
    minute1=0;
    minute2=0;
  }

  // Am/Pm
  if(halfDay >= 2){
    halfDay=0;
    hour=1;
    minute1=0;
    minute2=0;
  }

  // 
  enableOutput(false);
  //clearAll();
  //Serial.print(String(second) + " seconds\n");
  //Serial.print(minute1);
  //Serial.print(String(minute2) + " minutes\n");
  //Serial.println(String(hour) + "hours");
  //if

  AmPm(halfDay);
  displayMinL2(minute2);
  displayMinL1(minute1);
  displayHour(hour);
  latch();
  clockReg(); // Since we don't use the first bit, we translate everything down one bit.
  enableOutput(true);
  delay(1);

  int readHour = digitalRead(inHourPin);
  //Serial.println("InHour: " + String(readHour));
  if(readHour == HIGH){
    hour++;
    second=0;
    delay(250);
  }

  int readMinute = digitalRead(inMinutePin);
  //Serial.println("InMinute: " + String(readMinute));
  if(readMinute == HIGH){
    minute2++;
    second=0;
    delay(250);
  }
  
}


