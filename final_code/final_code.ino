/***************************************************************/
/* Sample code for CSL final project                           */
/* Demonstrating 1) IR sensor, 2) Servo motor, and 3) DC Motor */
/***************************************************************/

// All grounds (GND) should be connected
// After finishing prototyping, you can power Nano by connecting a 9V battery to Vin pin 

#include <string.h>

/***** IR Sensor *****/
#define ir_sensor0 A0 // A0 pin on the sensor
#define ir_sensor1 A1 // A1 pin on the sensor
#define ir_sensor2 A2 // A2 pin on the sensor
#define ir_sensor3 A3 // A2 pin on the sensor
// Vcc -> 5V
// GND -> GND


/***** Servo Motor *****/
#define servo_pin 11 // PWM pin (orange)
// Red -> 5V
// Brown -> GND

#include <Servo.h>
Servo myservo;
int servo_output = 0;
// TODO: calibrate this by hand
int MIN_SERVO = 30;
int MAX_SERVO = 120;
int MID_SERVO = 75;
// midpoint: 75
// min max 30, 120

/***** DC Motor (L298N) *****/
#define ENA 5 // (pwm)
#define IN1 6
#define IN2 7
// +12V -> (+) of 9V battery
// GND -> GND
// OUT1 & OUT2 -> DC Motor

int dc_dir = 0;
int dc_output = 0;


void setup() {
  /****** IR Sensor ******/
  pinMode(ir_sensor0, INPUT);
  pinMode(ir_sensor1, INPUT);
  pinMode(ir_sensor2, INPUT);
  pinMode(ir_sensor3, INPUT);

  /***** Servo Motor *****/
  pinMode(servo_pin, OUTPUT);
  myservo.attach(servo_pin);

  /** DC Motor (L298N) ***/
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  setDirection(dc_dir);
  analogWrite(ENA, dc_output);
  
  Serial.begin(38400);
}

void loop() {
  /* Read from the IR Sensor */
  Serial.print("ir0: ");
  Serial.print(analogRead(ir_sensor0));
  Serial.print(", ir1: ");
  Serial.print(analogRead(ir_sensor1));
  Serial.print(", ir2: ");
  Serial.print(analogRead(ir_sensor2));
  Serial.print(", ir3: ");
  Serial.print(analogRead(ir_sensor3));
  Serial.print("\n");

  

  /* Rotate the servo motor (by degree) */
  // myservo.write((servo_output += 10) % 180);
  /* Rotate the servo by input*/
//   String str = "";
//   int integer = "";
// //   Serial.println("start");
//   while (Serial.available() > 0){
//     int angle = Serial.read();
//     if (isDigit(angle)) str += char(angle);
//     if (angle == '\n'){
//       integer = str.toInt();
//       Serial.println(integer);
//       Serial.println(str);
//       str = "";

//       myservo.write(integer);
//     }
//   }

  /* Set DC motor direction and power */
  setDirection(dc_dir);
  analogWrite(ENA, 255);
  
  delay(1000);
  analogWrite(ENA, 125);
  delay(1000);
  setDirection(1);
  analogWrite(ENA, 100);
  delay(750);
}

void setDirection(int dir){
  if (dir == 0){
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else if (dir == 1){
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }
}
