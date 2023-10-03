#include <Mouse.h>
#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>

bool countingTime = false; //to know if the variable has not changed below the threshold
uint32_t startBeing3orMore; //to save the milliseconds when the variable started to be 3 or more


const int FLEX_PIN = A0; // Pin connected to voltage divider output

// Measure the voltage at 5V and the actual resistance of your
// 47k resistor, and enter them below:
const float VCC = 4.98; // Measured voltage of Ardunio 5V line
const float R_DIV = 2000.0; // Measured resistance of 3.3k resistor

// Upload the code, then try to adjust these values to more
// accurately calculate bend degree.
const float STRAIGHT_RESISTANCE = 10200.0; 
const float BEND_RESISTANCE = 100000.0;

MPU6050 mpu;
int16_t ax, ay, az, gx, gy, gz;
int vx, vy;
int button1 = 6;
int button2 = 7;
int buttonState1 = 0; 
int buttonState2 = 0; 

void setup() {

  Serial.begin(9600);
  Wire.begin();
  /*pinMode(button1, INPUT);
  pinMode(button2, INPUT);*/
  mpu.initialize();
  if (!mpu.testConnection()) { while (1); }
      //Serial.begin(9600);
      pinMode(FLEX_PIN, INPUT);
      pinMode(2, OUTPUT);
}

void loop() {
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  vx = -((gx+15)/150);  
  vy = -(gz-100)/150;

  Serial.print("gx = ");
  Serial.print(gx);
  Serial.print(" | gz = ");
  Serial.print(gz);
  
  Serial.print("        | X = ");
  Serial.print(vx);
  Serial.print(" | Y = ");
  Serial.println(vy);
  
  Mouse.move(vy, vx);

// Read the ADC, and calculate voltage and resistance from it
          int flexADC = analogRead(FLEX_PIN);
         float flexV = flexADC * VCC / 1023.0;
          float flexR = R_DIV * (VCC / flexV - 1.0);
          Serial.println("Resistance: " + String(flexR) + " ohms");
  
          Serial.println("flexADC: " + String(flexADC) + " ohms");

          // Use the calculated resistance to estimate the sensor's
          // bend angle:
         float angle = map(flexR, STRAIGHT_RESISTANCE, BEND_RESISTANCE,
                   0, 200.0);
         Serial.println("Bend: " + String(angle) + " degrees");
          Serial.println();
  
          //delay(50);

//para activar el motor
if (angle >= 3) {
       if (!countingTime) {
       startBeing3orMore = millis();
       }
   countingTime = true;
   } else {
   countingTime = false;
   }
   if (countingTime && millis() - startBeing3orMore > 0) {
    Mouse.press(MOUSE_LEFT);
    Serial.print("motor = ON");
    delay(10);
   } 
   if (angle < 3) {
    Mouse.release(MOUSE_LEFT); 
    delay(10);
   }

  /*
  buttonState1 = digitalRead(button1);
  buttonState2 = digitalRead(button2);
  
 if (buttonState1 == HIGH) {
    Mouse.press(MOUSE_LEFT);
    delay(100);
    Mouse.release(MOUSE_LEFT);
    delay(200);
  } 
  else if(buttonState2 == HIGH) {
    Mouse.press(MOUSE_RIGHT);
    delay(100);
    Mouse.release(MOUSE_RIGHT);
    delay(200);
  }*/
  delay(10);
}

