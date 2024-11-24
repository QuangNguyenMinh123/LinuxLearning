#define BUTTON_0    2
#define BUTTON_1    3
#define BUTTON_2    4
#define BUTTON_3    5
#define BUTTON_4    6
#define BUTTON_5    7
int button[6] = {BUTTON_0, BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4, BUTTON_5};
int buttonState[6];
int analogPin = A7;
int potentio;
/*  Connection : BUTTON_0 -> BUTTON_5 connected to D2 - D7 
                Potentiometer 10k connected to A7, pull down by 10k resistor
    Message example: B01B11B21B31B41B51A513
    Baudrate: 115200
 */
void setup() {
  Serial.begin(115200);
  for (int i =0; i < 6; i++)
    pinMode(button[i], INPUT);
}

void loop() {
  for (int i =0; i < 6; i++)
    buttonState[i] = digitalRead(button[i]);
  potentio = analogRead(analogPin);
  for (int i =0; i < 6; i++)
  {
    Serial.print("B");
    Serial.print(i);
    Serial.print(buttonState[i]);
  }
  Serial.print("A");
  Serial.println(potentio);
  delay(50);
}