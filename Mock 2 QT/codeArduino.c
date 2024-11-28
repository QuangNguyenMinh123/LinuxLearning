#define BUTTON_0    A5          // not use
#define BUTTON_1    A6          // not use
#define BUTTON_2    4
#define BUTTON_3    5
#define BUTTON_4    6
#define BUTTON_5    7
/*************************************************************/
int button[6] = {BUTTON_0, BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4, BUTTON_5};
int buttonState[6];
int analogPin = A7;
int potentio;
volatile int pulseCount = 0;  // Đếm số xung
unsigned long prevTime = 0;  // Thời gian trước đó
const int pulsesPerRevolution = 210;  // Số xung mỗi vòng (encoder datasheet)
/*************************************************************/
// Hàm ngắt ngoài để đếm xung
void countPulses() {
  pulseCount++;
}
/*************************************************************/
void setup() {
  Serial.begin(115200);
  for (int i =0; i < 6; i++)
    pinMode(button[i], INPUT);
  pinMode(2, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), countPulses, RISING);
}
/*************************************************************/
void loop() {
  unsigned long currentTime = millis();
  if (currentTime - prevTime >= 50) {
    noInterrupts();  // Dừng ngắt tạm thời để tính toán an toàn
    int pulses = pulseCount;
    pulseCount = 0;  // Reset số xung
    interrupts();    // Bật lại ngắt
    /* Read GPIO */
    for (int i =0; i < 6; i++)
      buttonState[i] = digitalRead(button[i]);
    potentio = analogRead(analogPin);
    for (int i =0; i < 6; i++)
    {
      Serial.print("B");
      Serial.print(i);
      Serial.print(buttonState[i]);
    }
    float rpm = (pulses / (float)pulsesPerRevolution) * 60 * 20;  // (vòng/phút)
    Serial.print("A");
    Serial.println((int)rpm);
    prevTime = currentTime;
  }
}