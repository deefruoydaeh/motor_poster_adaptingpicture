// @author Fabian Moron Zirfas
// for 
// Laura Russ
// 2015.07.03
// LICENSE MIT
#define MOTORDIR1 11 // pin for direction 1 of motor 
#define MOTORDIR2 10 // pin for direction 2 of motor
#define MOTORSPEED 9 // pin for speed of motor
#define BUFFERLENGTH 5 // numbers of places in buffer
#define MOTORMIN 50  //justierung je nach motor
#define MOTORMAX 200 //same


int sensorreadingraw; // raw value reading from the shapr sensor

// this is for calibrationg the reading
int maxsensorval = 0; // will hold the max value
int minsensorval = 1023; // will hold the min value
int average = 0; // will hold the calculated average from sensor reading

int sensorbuffer  [BUFFERLENGTH] = {0, 0, 0, 0, 0}; // buffer for averaging

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //just for debugging
 // setu the outputs
  pinMode(MOTORDIR1, OUTPUT);       // pin A declared as OUTPUT
  pinMode(MOTORDIR2, OUTPUT);       // pin B declared as OUTPUT
  pinMode(MOTORSPEED, OUTPUT);       // pin C declared as OUTPUT

  // define the direction we want to turn in
  // exchange HIGH vs LOW if the motor should turn into the other direction
  digitalWrite(MOTORDIR2, LOW);
  digitalWrite(MOTORDIR1, HIGH);
}// end of setup

void loop() {

  sensorreadingraw = analogRead(1); // read in the sharp sensor 
  average = calcaverage(sensorreadingraw); // calculate the average
  calibrate(average); // calibrate our range of sensor values
  Serial.println(average); // just fpr debugging oputput
//  delay(10);

  
  
// Here we map the vaslues read from the sensor 
// into the range of our motors movement
// MOTOR MAX = 255
// MOTOR MIN = 0 
  
  int movement = map(average, minsensorval, maxsensorval, MOTORMIN, MOTORMAX); 
  
  // turn the motor according to the distance from the sensor
  analogWrite(MOTORSPEED, movement);


}



// this function calculates the average of our read values
// see
// https://www.sparkfun.com/products/8958 
// in the comments

// @param int sensorreadingraw = the value from analogRead
// @return int  = the calulated average
int calcaverage(int sensorreadingraw) {

   // loop the buffer from the end
  for (int j = BUFFERLENGTH - 1; j >= 0; j--) {
    if (j == BUFFERLENGTH - 1) {
      sensorbuffer[j] = sensorreadingraw;
    } else {
      sensorbuffer[j] = sensorbuffer[j + 1];
    }
  }

  int ave = 0;
  for (int i = 0; i <  BUFFERLENGTH; i++) {
    ave += sensorbuffer[i];
  }

  return ave / BUFFERLENGTH;

}

// calibrate our min and max values
// @param int ave = the value to calibrate
void calibrate(int ave) {
  if (ave < minsensorval ) {
    minsensorval = ave;
  }
  if (ave > maxsensorval) {
    maxsensorval = ave;
  }

}

//4m ca 270
//3m ca 290
//2,5m ca 310
//2m ca 340
//1,5m ca 380
//1m ca 460
//0,8m ca 550
