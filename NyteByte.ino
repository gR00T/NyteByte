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
  
  Josh Gonzales - 18 APR 20
 */
#define arrayLen(A) sizeof(A)/sizeof(A[0])
#define photocellPin A5    // the cell and 10K pulldown are connected to a5
const int led[16] = {1,A2,A0,A1,4,5,2,3,6,9,8,7,13,11,12,10};

// define custom struct that holds a pointer to the pattern array
// stores the array length, and the number of bytes in each array
typedef struct patterns {
  unsigned char *ptr;
  int p_len;
} pattern;

void BootAnimation();
void clear();

const unsigned char rainAnimation[14][2] = {
  // Animation that looks like rain.
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
  
const unsigned char squareSpinLeft[14][2] = {
  // Runs a spinning square animation
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

const unsigned char squareSpinRight[14][2] = {
  // Runs a spinning square animation
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

const unsigned char squareZoom[3][2] = {
  // Runs a spinning square animation
  {B00000000, B00000000},
  {B00000110, B01100000},
  {B11111001, B10011111}
};

const unsigned char squareShift[16][2] = {
  // Runs a moving square animation
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

const unsigned char diagonalShiftRight[8][2] = {
  // A sweeping diagonal animation
  {B00000000, B00000000},
  {B10000000, B00000000},
  {B01001000, B00000000},
  {B00100100, B10000000},
  {B00010010, B01001000},
  {B00000001, B00100100},
  {B00000000, B00010010},
  {B00000000, B00000001}
};

const unsigned char diagonalShiftLeft[8][2] = {
  // A sweeping diagonal animation
  {B00000000, B00000000},
  {B10000000, B00000000},
  {B01001000, B00000000},
  {B00100100, B10000000},
  {B00010010, B01001000},
  {B00000001, B00100100},
  {B00000000, B00010010},
  {B00000000, B00000001}
};

const unsigned char singleOff[17][2] = {
  // All LEDs on at once and a single LED turns off at a time.
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

pattern pArray[8] = {
  { (unsigned char *) &rainAnimation, arrayLen(rainAnimation)},
  { (unsigned char *) &squareSpinLeft, arrayLen(squareSpinLeft)},
  { (unsigned char *) &squareSpinRight, arrayLen(squareSpinRight)},
  { (unsigned char *) &squareZoom, arrayLen(squareZoom)},
  { (unsigned char *) &squareShift, arrayLen(squareShift)},
  { (unsigned char *) &diagonalShiftRight, arrayLen(diagonalShiftRight)},
  { (unsigned char *) &diagonalShiftLeft, arrayLen(diagonalShiftLeft)},
  { (unsigned char *) &singleOff, arrayLen(singleOff)},
};

const int a_len = (int) arrayLen(pArray);

void setup() {
  //Serial.begin(9600);
  int i = 0;
  while(i< 16){
    pinMode(led[i], OUTPUT);
    i++;
  }
  BootAnimation();
}

int current = 0;

void loop() {
  int photocellReading = analogRead(photocellPin);

  // Determine if it is dark enough to display lights
  if (photocellReading < 3) {
    Serial.println(" - Dark");
    current = (current + 1) % a_len;
  } else if (photocellReading < 200) {
    Serial.println(" - Dim");
    current = 0;
  } else if (photocellReading < 500) {
    Serial.println(" - Light");
    current = 0;
  } else if (photocellReading < 800) {
    Serial.println(" - Bright");
    current = 0;
  } else {
    Serial.println(" - Very bright");
    current = 0;
  }

  if (current) {
    for (int i = 0; i < 10; i++ )
      runAnimation(pArray[current], i);
  } 
  else
    clear();
}

void runAnimation(pattern p, int duration) {

  int *current_animation = (int *) p.ptr;

  for (int i = 0; i < p.p_len ; i++) {
    UpdateDisplay( (const unsigned char *) &current_animation[i] );
    delay(100 - (5 * duration));
  }
}

void UpdateDisplay(const unsigned char *byteArray) {
// Receives an array of two bytes. Using bit anding, the lowest to
//highest order bits are checked to determine if it is a 1 for on
//or a 0 for off. Due to reverse order checking, the second byte is
//checked first and then the first byte.

 // create a copy of the byte array in memory so we don't blow it away
 // with the bit shifting
 byte display_byte[2];
 memcpy(display_byte, byteArray, 2);

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

void clear()
{
  Serial.println("Clearing display");
  for (int i = 0; i < arrayLen(led); i++){
    digitalWrite(led[i], LOW);
  }
}
