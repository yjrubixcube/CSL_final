/***************************************************************/
/* Sample code for CSL final project                           */
/* Demonstrating 1) IR sensor, 2) Servo motor, and 3) DC Motor */
/***************************************************************/

// All grounds (GND) should be connected
// After finishing prototyping, you can power Nano by connecting a 9V battery to Vin pin 

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
#define MIN_SERVO 30
#define MAX_SERVO 120
#define MID_SERVO 75
int servo_output = 0;


/***** DC Motor (L298N) *****/
#define ENA 5 // (pwm)
#define IN1 6
#define IN2 7
// +12V -> (+) of 9V battery
// GND -> GND
// OUT1 & OUT2 -> DC Motor

#define WHITE 0
#define GRAY 1
#define BLACK 2

//IR Sensor white gray threshold
#define WG0 45
#define WG1 40
#define WG2 38
#define WG3 35

//IR Sensor black gray threshold
#define BG 80

int dc_dir = 0;
int dc_output = 0;
int black_count = 0;
int ir0_val = 0;
int ir1_val = 0;
int ir2_val = 0;
int ir3_val = 0;
int ir0_col = 0;
int ir1_col = 0;
int ir2_col = 0;
int ir3_col = 0;
int ir0_col_pre = 0;
int ir1_col_pre = 0;
int ir2_col_pre = 0;
int ir3_col_pre = 0;



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
    ir0_val = analogRead(ir_sensor0);
    ir1_val = analogRead(ir_sensor1);
    ir2_val = analogRead(ir_sensor2);
    ir3_val = analogRead(ir_sensor3);

    //PRINT IR COLOR
    ir0_col = ir0_color(ir0_val);
    ir1_col = ir1_color(ir1_val);
    ir2_col = ir2_color(ir2_val);
    ir3_col = ir3_color(ir3_val);
    Serial.print("ir0: ");
    Serial.print(ir0_col);
    Serial.print(", ir1: ");
    Serial.print(ir1_col);
    Serial.print(", ir2: ");
    Serial.print(ir2_col);
    Serial.print(", ir3: ");
    Serial.print(ir3_col);

  // Count black box
  if(ir1_col_pre != BLACK && ir1_col == BLACK)
    black_count += 1;
  else if(ir2_col_pre != BLACK && ir2_col == BLACK)
    black_count += 1;

  //Turning via servo
  if(ir0_col==BLACK){
    Serial.print("right ");
    servo_output = MID_SERVO + 20;
    if(servo_output > MAX_SERVO)
      servo_output = MAX_SERVO;
  }
  else if(ir3_col==BLACK){
    Serial.print("left ");
    servo_output = MID_SERVO - 20;
    if(servo_output < MIN_SERVO)
      servo_output = MIN_SERVO;
  }
  else{
    Serial.print("mid ");
    servo_output = MID_SERVO;
  }
  //print servo val
  myservo.write(servo_output);
  Serial.print(", servo_output: ");
  Serial.print(servo_output);

  //print black count
  Serial.print(", black count: ");
  Serial.print(black_count);
  Serial.print("\n");

  //Stop 3s when 10 black
  if(black_count == 10){
    black_count = 0;
    dc_dir = 1;
    setDirection(dc_dir);
    analogWrite(ENA, 0);
    delay(3000);
  }
  else{
    dc_dir = 0;
    setDirection(dc_dir);
    analogWrite(ENA, 255);
    delay(500);
  }
  ir0_col_pre = ir0_col;
  ir1_col_pre = ir1_col;
  ir2_col_pre = ir2_col;
  ir3_col_pre = ir3_col;
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

// white:0 gray:1 black:2
int ir0_color(int val){
  if(val<=WG0)
    return WHITE;
  else if(val<=BG)
    return GRAY;
  else
    return BLACK;
}

int ir1_color(int val){
  if(val<=WG1)
    return WHITE;
  else if(val<=BG)
    return GRAY;
  else
    return BLACK;
}

int ir2_color(int val){
  if(val<=WG2)
    return WHITE;
  else if(val<=BG)
    return GRAY;
  else
    return BLACK;
}

int ir3_color(int val){
  if(val<=WG3)
    return WHITE;
  else if(val<=BG)
    return GRAY;
  else
    return BLACK;
}
