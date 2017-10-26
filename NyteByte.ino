/*
  Nite-Light

  This set of code uses a light sensing diode in order to detect when the
  lights go off. Then the light display routine runs.

  Grid Layout:

  1   2   3   4
  5   6   7   8
  9   10  11  12
  13  14  15  16

  Pin Layout:

  1   A2  A0  A1
  4   5   2   3
  6   9   8   7
  13  11  12  10

  Josh Gonzales
 */

const int photocellPin = A5;     // the cell and 10K pulldown are connected to a5
int photocellReading;     // the analog reading from the analog resistor divider

// initialize the two-dimensional arrays for the LED grid

const int led[16] = {1,A2,A0,A1,4,5,2,3,6,9,8,7,13,11,12,10};


// the setup function runs once when you press reset or power the board
void setup() {
  // We'll send debugging information via the Serial monitor
  //Serial.begin(9600);
  // initialize all digital and analog pins as output.
  int i = 0;
  while(i< 16){
    pinMode(led[i], OUTPUT);
    i++;
  }
  BootAnimation();
}



// the loop function runs over and over again forever
void loop() {
  //photocellReading = analogRead(photocellPin);

  //Serial.print("Analog reading = ");
  //Serial.print(photocellReading);     // the raw analog reading

  // We'll have a few threshholds, qualitatively determined
  //if (photocellReading < 10) {
    //Serial.println(" - Dark");
  //} else if (photocellReading < 200) {
   // Serial.println(" - Dim");
  //} else if (photocellReading < 500) {
  //  Serial.println(" - Light");
 // } else if (photocellReading < 800) {
   // Serial.println(" - Bright");
 // } else {
  //  Serial.println(" - Very bright");
 // }

 /* The following calls on the animation routines. The variable i
 is used to seed the refresh speed */
 for (int i=15;i<25;i++){
   SingleOff(i);
 }
 for (int i=35;i<45;i++){
   DiagonalShiftRight(i);
 };

 for (int i=45;i>35;i--){
   DiagonalShiftLeft(i);
 };

 for (int i=15;i<30;i++){
   SquareZoom(i);
 };

 for (int i=5;i<10;i++){
   SquareShift(i);
 };

 for (int i=5;i<10;i++){
   RainAnimation(i);
 };

 for (int i=5;i<10;i++){
   SquareSpinLeft(i);
 };

 for (int i=5;i<10;i++){
   SquareSpinRight(i);
 };

}

void UpdateDisplay(byte display_byte[2]) {
// Receives an array of two bytes. Using bit anding, the lowest to
//highest order bits are checked to determine if it is a 1 for on
//or a 0 for off. Due to reverse order checking, the second byte is
//checked first and then the first byte.

 //iterate through second byte and determine if on or off
 int i = 15;
 while(i>=8){
   if (display_byte[1] & 0x1) {
     digitalWrite(led[i], HIGH);
   }
   else {
     digitalWrite(led[i], LOW);
   }
   i--;
   display_byte[1] >>= 1;
 }

 // iterate through first byte and determine to turn LED on or off
 while(i>=0){
   if (display_byte[0] & 0x1) {
     digitalWrite(led[i], HIGH);
   }
   else {
     digitalWrite(led[i], LOW);
   }
   i--;
   display_byte[0] >>=1;
 }
}

void RainAnimation(int speed) {
// Animation that looks like rain.
byte data[14][2] = {
  {B10000000, B00000000},
  {B00011000, B00000000},
  {B01000001, B10000000},
  {B00100100, B00011000},
  {B10000010, B01000001},
  {B00001000, B00100100},
  {B00010000, B10000010},
  {B00000001, B00001000},
  {B00000000, B00010000},
  {B00000000, B00000001},
  {B01000000, B00000000},
  {B00000100, B00000000},
  {B00000000, B01000000},
  {B00000000, B00000100}
};

 //run through the data array to display the rain animation
 for (int x=0; x < 14; x++) {
   UpdateDisplay(data[x]);
   delay(500 - (20 * speed));
  };
}

void SquareSpinLeft(int speed) {
// Runs a spinning square animation

byte data[14][2] = {
  {B00000000, B00000000},
  {B00000110, B01100000},
  {B00101000, B00010100},
  {B01000001, B10000010},
  {B10010000, B00001001},
  {B00101000, B00010100},
  {B01000001, B10000010},
  {B10010000, B00001001},
  {B00101000, B00010100},
  {B01000001, B10000010},
  {B10010000, B00001001},
  {B00101000, B00010100},
  {B01000001, B10000010},
  {B10010000, B00001001}
};

 for (int x=0; x < 14; x++) {
   UpdateDisplay(data[x]);
   delay(200 - (10 * speed));
 };

}

void SquareSpinRight(int speed) {
// Runs a spinning square animation

byte data[14][2] = {
  {B00000000, B00000000},
  {B00000110, B01100000},
  {B01000001, B10000010},
  {B00101000, B00010100},
  {B10010000, B00001001},
  {B01000001, B10000010},
  {B00101000, B00010100},
  {B10010000, B00001001},
  {B01000001, B10000010},
  {B00101000, B00010100},
  {B10010000, B00001001},
  {B01000001, B10000010},
  {B00101000, B00010100},
  {B10010000, B00001001},
};

 for (int x=0; x < 14; x++) {
   UpdateDisplay(data[x]);
   delay(200 - (10 * speed));
 };
}

void SquareZoom(int speed) {
// Runs a spinning square animation

byte data[3][2] = {
  {B00000000, B00000000},
  {B00000110, B01100000},
  {B11111001, B10011111}
};

 for (int x=0; x < 3; x++) {
   UpdateDisplay(data[x]);
   delay(500 - (15 * speed));
 };
}

void SquareShift(int speed) {
// Runs a moving square animation

byte data[16][2] = {
  {B11001100, B00000000},
  {B01100110, B00000000},
  {B00110011, B00000000},
  {B00000011, B00110000},
  {B00000110, B01100000},
  {B00001100, B11000000},
  {B00000000, B11001100},
  {B00000000, B01100110},
  {B00000000, B00110011},
  {B00000000, B01100110},
  {B00000000, B11001100},
  {B00001100, B11000000},
  {B00000110, B01100000},
  {B00000011, B00110000},
  {B00110011, B00000000},
  {B01100110, B00000000}
};

 for (int x=0; x < 16; x++) {
   UpdateDisplay(data[x]);
   delay(500 - (25 * speed));
 };
}

void DiagonalShiftRight(int speed) {
// A sweeping diagonal animation

byte data[8][2] = {
  {B00000000, B00000000},
  {B10000000, B00000000},
  {B01001000, B00000000},
  {B00100100, B10000000},
  {B00010010, B01001000},
  {B00000001, B00100100},
  {B00000000, B00010010},
  {B00000000, B00000001}
};

for (int x=0; x < 8; x++) {
  UpdateDisplay(data[x]);
  delay(500 - (10 * speed));
};
}

void DiagonalShiftLeft(int speed) {
// A sweeping diagonal animation

byte data[8][2] = {
  {B00000000, B00000000},
  {B10000000, B00000000},
  {B01001000, B00000000},
  {B00100100, B10000000},
  {B00010010, B01001000},
  {B00000001, B00100100},
  {B00000000, B00010010},
  {B00000000, B00000001}
};

for (int x=7; x >= 0; x--) {
  UpdateDisplay(data[x]);
  delay(500 - (10 * speed));
};
}

void SingleOff(int speed) {
// All LEDs on at once and a single LED turns off at a time.
  byte data[17][2] = {
    {B11111111, B11111111},
    {B11101111, B11111111},
    {B11011111, B11111111},
    {B10111111, B11111111},
    {B01111111, B11111111},
    {B11110111, B11111111},
    {B11111011, B11111111},
    {B11111101, B11111111},
    {B11111110, B11111111},
    {B11111111, B11101111},
    {B11111111, B11011111},
    {B11111111, B10111111},
    {B11111111, B01111111},
    {B11111111, B11110111},
    {B11111111, B11111011},
    {B11111111, B11111101},
    {B11111111, B11111110}
  };

  for (int x=0; x < 17; x++) {
    UpdateDisplay(data[x]);
    delay(500 - (10 * speed));
  };
}

void BootAnimation() {
// Runs a smiley face animation upon bootup
byte data[12][2] = {
  {B00000110, B00000000},
  {B00000000, B00000000},
  {B00000110, B00000000},
  {B00000000, B00000000},
  {B00000110, B00000000},
  {B00000110, B00000110},
  {B00000110, B00001111},
  {B00000110, B10011111},
  {B00000110, B10011111},
  {B00000110, B10011111},
  {B00000110, B10011111},
  {B00000110, B10011111},
};

for (int x=0; x < 12; x++) {
  UpdateDisplay(data[x]);
  delay(400);
};

byte dataJG[2][2] = {
  {B01110001, B10011111},
  {B11111000, B10111111}
};

for (int x=0; x < 2; x++) {
  UpdateDisplay(dataJG[x]);
  delay(2200);
};

}
