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
#define MIN_SERVO 37
#define MAX_SERVO 117
#define MID_SERVO 77
int servo_output = MID_SERVO;
int servo_step = 5;

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

// car state
#define READY_TO_MOVE 0
#define MOVING 1
#define READY_TO_STOP 2
#define STOP 3
int car_state = 0;
int speed = 0;
int start_speed = 200;
int min_speed = 120;
int max_speed = 200;

int dc_dir = 0;
int dc_output = 0;
int black_count = 0;

int ir0_val = 0;
int ir1_val = 0;
int ir2_val = 0;
int ir3_val = 0;
int ir0_col = -1;
int ir1_col = -1;
int ir2_col = -1;
int ir3_col = -1;
int ir0_col_pre = -1;
int ir1_col_pre = -1;
int ir2_col_pre = -1;
int ir3_col_pre = -1;

int pre_color = -1;
int cur_color = -1;






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

  
  // current color touched
  if(ir1_col == BLACK || ir2_col == BLACK)
    cur_color = BLACK;
  else
    cur_color = -1;

  // Count black box
  if(pre_color != BLACK && cur_color == BLACK)
    black_count += 1;

  //print black count
  Serial.print(", black count: ");
  Serial.print(black_count);
  Serial.print("\n");

  // Turn
  if(ir0_col == BLACK || ir0_col == WHITE){
    Serial.print("right ");
    servo_output += servo_step;
    if(servo_output > MAX_SERVO)
      servo_output = MAX_SERVO;
  }
  else if(ir3_col == BLACK || ir3_col == WHITE){
    Serial.print("left ");
    servo_output -= servo_step;
    if(servo_output < MIN_SERVO)
      servo_output = MIN_SERVO;
  }
  else{
    Serial.print("mid ");
  }

  //print servo val
  myservo.write(servo_output);
  Serial.print(", servo_output: ");
  Serial.print(servo_output);

  // speed control
  speed = min_speed + (max_speed - min_speed) * pow(abs(servo_output -  MID_SERVO) / (MAX_SERVO - MID_SERVO), 2);

  // decide car state
  // start moving
  if (black_count == 1){
    car_state = MOVING;
  }
  // slow down
  if (black_count == 9){
    car_state = READY_TO_STOP;
  }
  // stop
  else if (black_count == 10){
    black_count = 0;
    car_state = STOP;
  }
  
  if (car_state == READY_TO_MOVE){
    setDirection(0);
    analogWrite(ENA, start_speed);
    delay(20);
  }
  else if (car_state == MOVING){
    analogWrite(ENA, speed);
    delay(20);
  }
  else if (car_state == READY_TO_STOP){
    setDirection(1);
    analogWrite(ENA, 100);
    delay(20);
  }
  else if (car_state == STOP){
    analogWrite(ENA, 0);
    delay(3000);
    car_state = READY_TO_MOVE;
  }
  
  pre_color = cur_color;

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
