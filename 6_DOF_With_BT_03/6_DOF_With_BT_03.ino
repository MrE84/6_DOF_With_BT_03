#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27,16,4);
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
SoftwareSerial bt1(2,3); /* (Rx,Tx) */

const int MIN_PULSE_WIDTH = 500;
const int MAX_PULSE_WIDTH = 2500;
const int DEFAULT_PULSE_WIDTH = 1500;
const int FREQUENCY = 50;
const float FREQUENCY_SCALE = (float)FREQUENCY * 4096 / 1000000;

int pulseWidth(int angle) {
    int pulse_wide = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
    int analog_value = int(pulse_wide * FREQUENCY_SCALE);
    Serial.println(analog_value);
    return analog_value;
}



int leftButton = 7;
int rightButton = 8;

int potentiometer1 = A0;
int potentiometer2 = A1;
int potentiometer3 = A2;
int potentiometer4 = A3;
int potentiometer5 = A4;

//Record section
int buttonStartRecord = 9;
int buttonStopRecord = 10;
int buttonPlayRecord = 11;
int ledStopRecord = 12;
int ledStartRecord = 13;

bool isRecord = false;
bool isPlay = false;
int indexRecord = 0;

const int moveCount = 10;
const int servoNumber = 4;
int movesServos[moveCount][servoNumber]; //max of 10 moves

void setup() {
    Serial.begin(9600);
    bt1.begin(9600);

   //Wire.begin();
  //initialize lcd screen
  lcd.init();
   // turn on the backlight
  lcd.backlight();
  lcd.clear();



  lcd.begin(16, 4);      // set up the LCD's number of columns and rows
  lcd.setCursor(0, 0);
  lcd.print("2023 GDIP F1 :");
  lcd.setCursor(0, 1);
  lcd.print("6 DOF ROBOTIC ARM ");
  lcd.setCursor(0, 2);
  lcd.print("PROTOTYPE 1");
    pwm.begin();
    pwm.setPWMFreq(FREQUENCY);
    pwm.setPWM(1,0,pulseWidth(125));
    pwm.setPWM(2,0,pulseWidth(80));
    pwm.setPWM(3,0,pulseWidth(97));
    pwm.setPWM(4,0,pulseWidth(88));
    pwm.setPWM(5,0,pulseWidth(0));
    pwm.setPWM(6,0,pulseWidth(80));
}

void loop() {

  
  
    String command = "";

    if (Serial.available()) {
        command = Serial.readStringUntil('\n');
    } else if (bt1.available()) {
        command = bt1.readStringUntil('\n');
        Serial.println(command); // echo command from bt to Serial
    }

    if (command != "") {
        executeCommand(command);
    }
}

void executeCommand(String command) {
    struct ServoCommand {
        const char* name;
        int channel;
    };

    ServoCommand commands[] = {
        {"HIP ", 1},
        {"WAIST ", 2},
        {"SHOULDER ", 3},
        {"ELBOW ", 4},
        {"WRIST ", 5},
        {"CRAW ", 6}
    };

    for (const auto& cmd : commands) {
        if (command.startsWith(cmd.name)) {
            int angle = command.substring(strlen(cmd.name)).toInt();
            pwm.setPWM(cmd.channel, 0, pulseWidth(angle));
            Serial.println(String(cmd.name) + "moved to " + String(angle) + " degrees");
            return;
        }
    }

    Serial.println("Unknown command: " + command);
}
