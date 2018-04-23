//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// DC MOTOR
//leftside forward = LF 8,9
//leftside backward = LB 10,11
//rightside forward = RF 4,5
//rightside backward = RB 6,7

int LFI = 9;
int LBI = 10;
int LFO = 8;
int LBO = 11;
int RFI = 4;
int RBI = 7;
int RFO = 5;
int RBO = 6;

char check;

int check_amp = 534;

long long temp_time = 0;

char detect = 'F';

void forward(){
  digitalWrite(LFI, HIGH);
  digitalWrite(LBI, HIGH);
  digitalWrite(RFI, HIGH);
  digitalWrite(RBI, HIGH);
  digitalWrite(LFO, LOW);
  digitalWrite(LBO, LOW);
  digitalWrite(RFO, LOW);
  digitalWrite(RBO, LOW);
  
}

void backward(){
  digitalWrite(LFO, HIGH);
  digitalWrite(LBO, HIGH);
  digitalWrite(RFO, HIGH);
  digitalWrite(RBO, HIGH);
  digitalWrite(LFI, LOW);
  digitalWrite(LBI, LOW);
  digitalWrite(RFI, LOW);
  digitalWrite(RBI, LOW);
  }

void left(){
  digitalWrite(LFO, HIGH);
  digitalWrite(LBO, HIGH);
  digitalWrite(RFI, HIGH);
  digitalWrite(RBI, HIGH);
  digitalWrite(LFI, LOW);
  digitalWrite(LBI, LOW);
  digitalWrite(RFO, LOW);
  digitalWrite(RBO, LOW);
}

void right(){
  digitalWrite(LFI, HIGH);
  digitalWrite(LBI, HIGH);
  digitalWrite(RFO, HIGH);
  digitalWrite(RBO, HIGH);
  digitalWrite(LFO, LOW);
  digitalWrite(LBO, LOW);
  digitalWrite(RFI, LOW);
  digitalWrite(RBI, LOW);
}

void Stop(){
  digitalWrite(LFI, LOW);
  digitalWrite(LBI, LOW);
  digitalWrite(RFO, LOW);
  digitalWrite(RBO, LOW);
  digitalWrite(LFO, LOW);
  digitalWrite(LBO, LOW);
  digitalWrite(RFI, LOW);
  digitalWrite(RBI, LOW);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// MICROWAVE SENSOR
//******** USER CONFIG *********************

//Define the "zero" point of the AC waveform as a DC value between 0 and 1023. This needs to be correct when the sensitivity is low ( <100 )

#define MIDPOINT 22

// Define the sensitivity of analog detection. With a very clean & consistent waveform, the sensitivity can be raised significantly.

// This value is important in determining if detected peak-to-peak values constitute a 'wave'

#define SENSITIVITY 75

// The analog pin to use

#define ADCPin A0



// Incoming data is summed, so fetching the results every second

// will indicate speed over the previous second

// How often in mS to display the results ( 0 = print all results if possible)

#define printDelay 250



//*****************************************/

////////////////////////////////////////////////////////////////////////////////////// DC MOTOR

#include "AnalogFrequency.h"



long long displayTimer = 0;

uint32_t amplitude = 0, samples = 0;

void setup() {
  // put your setup code here, to run once:
  //char check = 'Y';
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);  
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  Serial.begin(9600);
////////////////////////////////////////////////////////////////////////////////////// MICROWAVE SENSOR
  Serial.begin(115200);

  setupADC(ADCPin);
  
  Serial.println("Ready...");
}

void loop() {
  // put your main code here, to run repeatedly:
   static long int cnt = 0;
   
//temp_time = millis();
if (check=='Y'){
    if (fAvailable() && millis() - displayTimer > printDelay) {
  
      displayTimer = millis();

      //Serial.println(displayTimer);
  
      uint32_t frequency = getFreq(&amplitude, &samples);
  
  
  
      //Filter out low amplitude signals with a low count
  
      //if (amplitude < 540 && samples < 2) {
  
  //    if ((amplitude < 700 && samples < 2) || (frequency > 45)) {
  //
  //      return;
  //
  //    }
  
      float speedKM = frequency / 19.49;
  
        Serial.println(++cnt);
  
      Serial.print("\t\tKM/h\t");
  
      Serial.print(speedKM);
  
      Serial.print("\t\tHz\t");
  
      Serial.print(frequency);
  
      Serial.print("\t\tAmplitude\t");
  
      Serial.print(amplitude);
  
      Serial.print("\t\tSamples\t");
  
      Serial.println(samples);

      if ((amplitude > check_amp + 1) || (amplitude < check_amp - 1)){
        detect = 'T';
      }

      if(displayTimer - temp_time >= 2000){
  //      Serial.write(temp_time);
    //    Serial.println();
//        Serial.write(displayTimer);
      //  Serial.println();
        Serial.println(detect);
        detect = 'F';
        check = 'N';
      }
    }
}
  
  char state = '0';

   if(Serial.available() > 0){ // Checks whether data is comming from the serial port
      state = Serial.read(); // Reads the data from the serial port
   }
   if (state == 'w') {
    Serial.println("Forward"); // Send back, to the phone, the String "LED: ON"
    state = '0';
    check = 'N';
    forward();
   }
   else if (state == 's') {
    Serial.println("Backward"); // Send back, to the phone, the String "LED: ON" 
    state = '0';
    check = 'N';
    backward();
   }   
  else if (state=='a'){
    Serial.println("Left"); // Send back, to the phone, the String "LED: ON"
    state = '0';
    check = 'N';
    left();
  }
  else if (state=='d'){
    Serial.println("Right"); // Send back, to the phone, the String "LED: ON"
    state = '0';
    check = 'N';
    right();
  }
  else if (state=='q'){
    Serial.println("Stop");
    state = '0';
    check = 'Y';
    temp_time = millis();
    Stop();
  }

}
