#include <OneWire.h> // Include the OneWire library.

OneWire TemperatureSensor(2); // Temperature sensor on pin 2.

const int FloatLow = 3; // INPUT-Low water float on pin 3.
const int FloatHigh = 4; // INPUT-High water float on pin 4.
const int Humidistat = 5; // INPUT-Humidistat contact on pin 5.
const int FillValve = 6; // OUTPUT-Water filling valve on pin 6.
const int FlushValve = 7; // OUTPUT-Water flushing valve on pin 7.
const int ON = 8; // OUTPUT-ON on pin 8.
const int Fault = 9; // OUTPUT-Fault on pin 9.
const int TemperatureThreshold = 640; // Temperature at which the humidifier stops (40 deg C x 16 = 640).
int OutputTimerFloatLow = 0; // Self explanatory.
int OutputTimerFlush = 0; // Self explanatory.
int OutputTimerFillDelay = 0; // Self explanatory.
int OutputTimerFillTooLong = 0; // Self explanatory.
int OutputTimerFlushTooLong = 0; // Self explanatory.
int OutputTimerElementFault = 0; // Self explanatory.
int TemperatureTooHigh = 0; // Self explanatory.
int TemperatureRequested = 0; // Self explanatory.
int Temperature = 0; // Variable for the temperature used to do the logic.
int FloatLowState = 0; // Variable for the low water float used to do the logic.
int FloatHighState = 0; // Variable for the high water float used to do the logic.
int HumidistatState = 0; // Variable for the humidistat contact used to do the logic.
int FillValveState = 1; // Variable for the fill valve output used to do the logic. State is reversed because of "active on low" relay board.
int FlushValveState = 1; // Variable for the flush valve output used to do the logic. State is reversed because of "active on low" relay board.
int ONState = 1; // Variable for the ON output used to do the logic. State is reversed because of "active on low" relay board.
int FaultState = 1; // Variable for the fault output used to do the logic. State is reversed because of "active on low" relay board.
int NoAlarm1stFill = 0; // Variable used to cancel the false fault on startup.
unsigned long currentMillis; // Used to store the current ms count since the program started.
unsigned long Delay750msStartMillis; // Used to calculate a delay of 750 milliseconds for the temperature request.
const unsigned long Delay750ms = 750; // Used to calculate a delay of 750 milliseconds for the temperature request.
unsigned long Delay10sStartMillis; // Used to calculate a delay of 10 seconds for the temperature request.
const unsigned long Delay10s = 10000; // Used to calculate a delay of 10 seconds for the temperature request.
unsigned long TimerFloatLowStartMillis; // Used to calculate a delay of 1 minute for the TimerFloatLow timer off.
const unsigned long TimerFloatLowDelay = 60000; // Used to calculate a delay of 1 minute for the TimerFloatLow timer off.
unsigned long TimerFlushONStartMillis; // Used to calculate a delay of 13 minutes for the TimerFlush timer on/off.
const unsigned long TimerFlushONDelay = 780000; // Used to calculate a delay of 13 minutes for the TimerFlush timer on/off.
unsigned long TimerFlushOFFStartMillis; // Used to calculate a delay of 24 hours for the TimerFlush timer on/off.
const unsigned long TimerFlushOFFDelay = 86400000; // Used to calculate a delay of 24 hours for the TimerFlush timer on/off.
unsigned long TimerFillDelayStartMillis; // Used to calculate a delay of 6 minutes for the TimerFillDelay timer off.
const unsigned long TimerFillDelayDelay = 360000; // Used to calculate a delay of 6 minutes for the TimerFillDelay timer off.
unsigned long TimerFillTooLongStartMillis; // Used to calculate a delay of 20 minutes for the TimerFillTooLong timer on.
const unsigned long TimerFillTooLongDelay = 1200000; // Used to calculate a delay of 20 minutes for the TimerFillTooLong timer on.
unsigned long TimerFlushTooLongStartMillis; // Used to calculate a delay of 16 minutes for the TimerFlushTooLong timer on.
const unsigned long TimerFlushTooLongDelay = 960000; // Used to calculate a delay of 16 minutes for the TimerFlushTooLong timer on.
unsigned long TimerElementFaultStartMillis; // Used to calculate a delay of 20 hours for the TimerElementFault timer on.
const unsigned long TimerElementFaultDelay = 72000000; // Used to calculate a delay of 20 hours for the TimerElementFault timer on.

void setup() {
  pinMode (FloatLow, INPUT); // Set pin 3 to input mode.
  pinMode (FloatHigh, INPUT); // Set pin 4 to input mode.
  pinMode (Humidistat, INPUT); // Set pin 5 to input mode.
  pinMode (FillValve, OUTPUT); // Set pin 6 to ouput mode.
  pinMode (FlushValve, OUTPUT); // Set pin 7 to ouput mode.
  pinMode (ON, OUTPUT); // Set pin 8 to ouput mode.
  pinMode (Fault, OUTPUT); // Set pin 9 to ouput mode.
  Delay750msStartMillis = millis(); // Set the 750 milliseconds timer to 0.
  Delay10sStartMillis = millis(); // Set the 10 seconds timer to 0.  
  TimerFloatLowStartMillis = millis(); // Set the TimerFloatLow timer off to 0.
  TimerFlushONStartMillis = millis(); // Set the TimerFlush timer on/off to 0.
  TimerFlushOFFStartMillis = millis(); // Set the TimerFlush timer on/off to 0.
  TimerFillDelayStartMillis = millis(); // Set the TimerFillDelay timer off to 0.
  TimerFillTooLongStartMillis = millis(); // Set the TimerFillTooLong timer on to 0.
  TimerFlushTooLongStartMillis = millis(); // Set the TimerFlushTooLong timer on to 0.
  TimerElementFaultStartMillis = millis(); // Set the TimerElementFault timer on to 0.
}

void loop() {

// Store the current ms count since the program started.
  currentMillis = millis();

// Reads the temperature of the sensor + some logic.
  byte i;
  byte Data1[12];
  byte Address1[8];

  if ( !TemperatureSensor.search(Address1)) {
    TemperatureSensor.reset_search();
    delay(250);
    return;
  }

  if ((currentMillis - Delay10sStartMillis >= Delay10s) && (TemperatureRequested == 0)){
    TemperatureSensor.reset();
    TemperatureSensor.select(Address1);
    TemperatureSensor.write(0x44, 0);
    TemperatureRequested = 1;
    Delay750msStartMillis = currentMillis;
    Delay10sStartMillis = currentMillis;
  }
  
  if ((currentMillis - Delay750msStartMillis >= Delay750ms) && (TemperatureRequested == 1)){
    TemperatureSensor.reset();
    TemperatureSensor.select(Address1);    
    TemperatureSensor.write(0xBE);
    for ( i = 0; i < 9; i++) {
    Data1[i] = TemperatureSensor.read();
    }
    Temperature=(Data1[1]*256)+Data1[0];
    Delay750msStartMillis = currentMillis;
    TemperatureRequested = 0;
  }

  if (Temperature >= TemperatureThreshold){
    TemperatureTooHigh = 1;
  }
  else{
    TemperatureTooHigh = 0;
  }

// Reads the current state of the low water float.
  FloatLowState = digitalRead(FloatLow);
// Reads the current state of the high water float.
  FloatHighState = digitalRead(FloatHigh);
// Reads the current state of the humidistat.
  HumidistatState = digitalRead(Humidistat);

// Logic for the TimerFloatLow timer off.
  if (FloatLowState == 0 || OutputTimerFillDelay == 1){
    OutputTimerFloatLow = 1;
    TimerFloatLowStartMillis = currentMillis;
  }
  else {
    if (currentMillis - TimerFloatLowStartMillis >= TimerFloatLowDelay){
      OutputTimerFloatLow = 0;
    }
  }

// Logic for the TimerFlush timer on/off.
  if (currentMillis - TimerFlushOFFStartMillis >= TimerFlushOFFDelay){
    OutputTimerFlush = 1;
    TimerFlushONStartMillis = currentMillis;
    TimerFlushOFFStartMillis = currentMillis;
  }
  if (currentMillis - TimerFlushONStartMillis >= TimerFlushONDelay){
    OutputTimerFlush = 0;
  }

// Logic for the TimerFillDelay timer off.
  if (OutputTimerFlush == 1){
    OutputTimerFillDelay = 1;
    TimerFillDelayStartMillis = currentMillis;
  }
  else {
    if (currentMillis - TimerFillDelayStartMillis >= TimerFillDelayDelay){
      OutputTimerFillDelay = 0;
    }
  }

// Logic for the TimerFillTooLong timer on.
  if (FillValveState == 0){ // FillValveState is reversed because of "active on low" relay board.
    if (currentMillis - TimerFillTooLongStartMillis >= TimerFillTooLongDelay){
    OutputTimerFillTooLong = 1;
    }
  } 
  else {
    OutputTimerFillTooLong = 0;
    TimerFillTooLongStartMillis = currentMillis;
  }

// Logic for the TimerFlushTooLong timer on.
  if ((FlushValveState == 0) && (FloatLowState == 0)){ // FlushValveState is reversed because of "active on low" relay board.
    if (currentMillis - TimerFlushTooLongStartMillis >= TimerFlushTooLongDelay){
    OutputTimerFlushTooLong = 1;
    }
  } 
  else {
    OutputTimerFlushTooLong = 0;
    TimerFlushTooLongStartMillis = currentMillis;
  }

// Logic for the TimerElementFault timer on.
  if ((HumidistatState == 1) && ((FloatLowState == 0) || (OutputTimerFillDelay == 1))){
    if (currentMillis - TimerElementFaultStartMillis >= TimerElementFaultDelay){
    OutputTimerElementFault = 1;
    }
  } 
  else {
    OutputTimerElementFault = 0;
    TimerElementFaultStartMillis = currentMillis;
  }

// Logic for the fill valve output activation.
  if (((FloatLowState == 1) || (FillValveState == 0)) && (FloatHighState == 0) && (OutputTimerFlush == 0) && (FaultState == 1)){ // FillValveState and FaultState are reversed because of "active on low" relay board.
    FillValveState = 0; // State is reversed because of "active on low" relay board.
  }
  else {
    FillValveState = 1; // State is reversed because of "active on low" relay board.
  }

// Logic for the flush valve output activation.
  if ((OutputTimerFlush == 1) && (FaultState == 1)){ // FaultState is reversed because of "active on low" relay board.
    FlushValveState = 0; // State is reversed because of "active on low" relay board.
  }
  else {
    FlushValveState = 1; // State is reversed because of "active on low" relay board.
  }

// Logic for the ON output activation.
  if ((HumidistatState == 1) && (OutputTimerFillDelay == 0) && (FaultState == 1)){ // FaultState is reversed because of "active on low" relay board.
    ONState = 0; // State is reversed because of "active on low" relay board.
  }
  else {
    ONState = 1; // State is reversed because of "active on low" relay board.
  }

// Logic for the fault output activation.
  if (((FloatLowState == 1) && (FloatHighState == 1)) || (TemperatureTooHigh == 1) || ((OutputTimerFloatLow == 0) && (NoAlarm1stFill == 1)) || (OutputTimerFillTooLong == 1) || (OutputTimerFlushTooLong == 1) || (OutputTimerElementFault == 1) || (FaultState == 0)){ // FaultState is reversed because of "active on low" relay board.
    FaultState = 0; // State is reversed because of "active on low" relay board.
  }
  else {
    FaultState = 1; // State is reversed because of "active on low" relay board.
  }

// Logic to cancel the false fault on startup.
  if ((FloatHighState == 1) || (NoAlarm1stFill == 1)){
    NoAlarm1stFill = 1;
  }
  else {
    NoAlarm1stFill = 0;
  }

// Writes the value for the fill valve contact to the output.
  digitalWrite (FillValve, FillValveState);
// Writes the value for the flush valve contact to the output.
  digitalWrite (FlushValve, FlushValveState);
// Writes the value for the ON contact to the output.
  digitalWrite (ON, ONState);
// Writes the value for the fault contact indicator to the output.
  digitalWrite (Fault, FaultState);
}
