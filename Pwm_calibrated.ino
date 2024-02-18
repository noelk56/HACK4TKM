#define X_AXIS_PIN 32
#define Y_AXIS_PIN 34
#define SWITCH_PIN 5

int xVal;
int yVal;
int Xpwm;
int Ypwm;

void setup() {
  Serial.begin(115200);
  pinMode(X_AXIS_PIN,INPUT);
  pinMode(Y_AXIS_PIN,INPUT);

}

void loop() {
 xVal=analogRead(X_AXIS_PIN);
 yVal=analogRead(Y_AXIS_PIN);



 if (xVal >= 1400)
    {
      Xpwm = map(xVal, 1100, 2640, 90, 180);
    }
    else if (xVal <= 1100)
    {
      Xpwm = map(xVal, 1100, 0, 90, 0);  
    }
    else
    {
      Xpwm = 90;
  }

   if (yVal >= 1400)
    {
      Ypwm = map(yVal, 1100, 2640, 90, 180);
    }
    else if (yVal <= 1100)
    {
      Ypwm = map(yVal, 1100, 0, 90, 0);  
    }
    else
    {
      Ypwm = 90;
  }

Serial.print("X : ");
Serial.print(Xpwm);
Serial.print("   Y : ");
Serial.println(Ypwm);
delay(50);

}
