#include <Adafruit_NeoPixel.h>

// Create the neopixel strip with the built in definitions NUM_NEOPIXEL and PIN_NEOPIXEL
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_NEOPIXEL, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

const byte numChars = 32;
char receivedChars[numChars]; // an array to store the received data

boolean newData = false;
int color[3] = {0,0,0};

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(20);
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
 recvWithEndMarker();
 showNewData();
}

void recvWithEndMarker() {
 static byte ndx = 0;
 char endMarker = '\n';
 char rc;
 
 while (Serial.available() > 0 && newData == false) {
  rc = Serial.read();
  if (rc != endMarker) {
    receivedChars[ndx] = rc;
    ndx++;
    if (ndx >= numChars) {
      ndx = numChars - 1;
    }
   }
  else {
    receivedChars[ndx] = '\0'; // terminate the string
    ndx = 0;
    newData = true;
  }
 }
}

void showNewData() {
 if (newData == true) {
  String property = getValue(String(receivedChars),':',0);
  String value = getValue(String(receivedChars),':',1);
  if(property.equals("color")) {
     setColor(value);
     Serial.print("Set color to ");
     Serial.println(value);
  }
  
  newData = false;
 }
}

void setColor(String hex) {
 long number = (long) strtol( &hex[0], NULL, 16);
  int r = number >> 16;
  int g = number >> 8 & 0xFF;
  int b = number & 0xFF;
  for(int p = 0;p<4;p++) {
    strip.setPixelColor(p,r,g,b);
  }
  strip.setBrightness(100);
  strip.show();
}



String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
