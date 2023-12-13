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


#define START_MOTOR 255 // starting motor speed
#define STOP_MOTOR 150 // reverse motor speed
#define MAX_MOTOR_SPEED 200 // max motor speed for constant speed

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

int servo_angle = 0;


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

	servo_angle = map(value, from min, from max, MIN_SERVO, MAXSERVO);

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
	// start();
	// delay(1000);
	// analogWrite(ENA, MAX_MOTOR_SPEED);
	// delay(1000);
	if (count_blocks == 10){
		reverse_and_stop();
		start();
	}
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


void reverse_and_stop(int dir){
	setDirection(1); // reverse motor
	analogWrite(ENA, STOP_MOTOR);
	delay(3000); // wait for 3 seconds
}

void start(){
	setDirection(0);
	analogWrite(ENA, START_MOTOR); // forward motor
}

float follow_line(){
	float angle = 0; // (-10, 10)
	return angle;
}

int count_blocks(){
	return 10;
}

void handle_start(){

}