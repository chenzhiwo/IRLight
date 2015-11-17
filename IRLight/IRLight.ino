#include <IRremote.h>

#define LED_PIN 6
#define IR_PIN 12
#define VAR_TIME 8

#define RSME  0xFFC23D
#define MDOWN 0xFF22DD
#define MUP   0xFF02FD
#define DOWN  0xFFE01F
#define UP    0xFFA857
#define L0		0xFF6897
#define L1 		0xFF30CF
#define L2 		0xFF18E7
#define L3 		0xFF7A85
#define L4 		0xFF10EF
#define L5 		0xFF10EF
#define L5 		0xFF38C7
#define L6 		0xFF5AA5
#define L7 		0xFF42BD
#define L8 		0xFF4AB5
#define L9 		0xFF52AD


int currBright = 0;
int lastBright = 0;
IRrecv irrecv(IR_PIN);
decode_results results;

void setBrightSmon(int newBright)
{
  digitalWrite(LED_BUILTIN, HIGH);
  newBright = (newBright < 0) ? 0 : newBright;
  newBright = (newBright > 255) ? 255 : newBright;

  if (newBright != currBright)
  {
    int steps = 0;
    int count = 0;
    int diff = 0;

    Serial.print("FROM: ");
    Serial.print(currBright);
    Serial.print(" TO: ");
    Serial.print(newBright);

    if (newBright > currBright)
    {
      steps = 1;
    } else {
      steps = -1;
    }

    diff = abs(newBright - currBright);

    for (count = 0; count < diff; count++)
    {
      currBright = currBright + steps;
      analogWrite(LED_PIN, currBright);
      delay(VAR_TIME);
    }
    Serial.println(" OK");
    digitalWrite(LED_BUILTIN, LOW);
  }

}

void setup() {
  Serial.begin(57600);
  irrecv.enableIRIn();
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.print("------\nIRCODE: ");
    Serial.println(results.value, HEX);

    switch (results.value)
    {
      case RSME:
        if (currBright == 0)
        {
          setBrightSmon(lastBright);
        } else {
          lastBright = currBright;
          setBrightSmon(0);
        }
        break;

      case MDOWN:
        setBrightSmon(currBright - 2);
        break;

      case MUP:
        setBrightSmon(currBright + 2);
        break;

      case DOWN:
        setBrightSmon(currBright - 25);
        break;

      case UP:
        setBrightSmon(currBright + 25);
        break;

      case L0:
        setBrightSmon(0);
        break;

      case L1:
        setBrightSmon(1);
        break;

      case L9:
        setBrightSmon(255);
        break;
    }

    irrecv.resume();
  }
}
