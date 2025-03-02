#if 0

#include "RgbUltrasonic.h"

RgbUltrasonic::RgbUltrasonic(byte signal_pin, byte rgb_pin)
{
    SignalPin = signal_pin;
    RgbPin = rgb_pin;
    mRgb = new RGBLed(RgbPin, 6);
}

uint16_t RgbUltrasonic::GetUltrasonicDistance(void)
{
  float distance;
  unsigned long Time_Echo_us = 0;
  pinMode(SignalPin, OUTPUT);
  digitalWrite(SignalPin, LOW);
  delayMicroseconds(2);
  digitalWrite(SignalPin, HIGH);
  delayMicroseconds(20);
  digitalWrite(SignalPin, LOW);
  pinMode(SignalPin, INPUT);
  Time_Echo_us = pulseIn(SignalPin, HIGH);
  if ((Time_Echo_us < 60000) && (Time_Echo_us > 1)) {
      FrontDistance = Time_Echo_us / 58.00;
      Serial.print("distance is :");
      Serial.print(FrontDistance);
      Serial.print("cm");
      Serial.println();  
  }
    return FrontDistance;
}

void RgbUltrasonic::SetRgbColor(E_RGB_INDEX index, long Color)
{
    if (index == E_RGB_ALL) {
        mRgb->setColor(0, Color);
    } else if (index == E_RGB_RIGHT) {
        mRgb->setColor(1, Color);
        mRgb->setColor(2, Color);
        mRgb->setColor(3, Color);
    } else if (index == E_RGB_LEFT) {
        mRgb->setColor(4, Color);
        mRgb->setColor(5, Color);
        mRgb->setColor(6, Color);
    }
    mRgb->show();
}

void RgbUltrasonic::SetRgbEffect(E_RGB_INDEX index, long Color, uint8_t effect)
{
    switch((E_RGB_EFFECT)effect) {
        case E_EFFECT_BREATHING:
            long rgb[3];
            for (byte c = 0 ; c<3; c++){
                rgb[c] = Color>>(2-c)*8 & 0x0000FF;
            }
            for (byte i = 5; i < 120; i++) {
//                SetRgbColor(index, (i<<16)|(i<<8)|i);
                long color = (max(rgb[0]-i,(long int)5) << 16) + (max(rgb[1]-i,(long int)5) << 8) + max(rgb[2]-i,(long int)5);
                SetRgbColor(index, color);
                delay((i < 18) ? 18: (256/i));
            }
            for (byte i = 120; i >= 5; i--) {
//                SetRgbColor(index, (i<<16)|(i<<8)|i);
                long color = (max(rgb[0]-i,(long int)5) << 16) + (max(rgb[1]-i,(long int)5) << 8) + max(rgb[2]-i,(long int)5);
                SetRgbColor(index, color);
                delay((i < 18) ? 18: (256/i));
            }

            break;
        case E_EFFECT_ROTATE:
            SetRgbColor(E_RGB_ALL, 0);
            mRgb->setColor(1, Color);
            mRgb->setColor(4, Color);
            mRgb->show();
            delay(100);
            mRgb->setColor(1, 0);
            mRgb->setColor(4, 0);
            mRgb->setColor(2, Color);
            mRgb->setColor(5, Color);
            mRgb->show();
            delay(100);
            mRgb->setColor(2, 0);
            mRgb->setColor(5, 0);
            mRgb->setColor(3, Color);
            mRgb->setColor(6, Color);
            mRgb->show();
            delay(100);
            mRgb->setColor(3, 0);
            mRgb->setColor(6, 0);
            mRgb->show();
            delay(100);
            mRgb->setColor(2, Color);
            mRgb->setColor(5, Color);
            mRgb->show();
            break;
        case E_EFFECT_FLASH:
            for (byte i = 0; i < 3; i++) {
                SetRgbColor(E_RGB_ALL, 0);
                delay(20);
                SetRgbColor(E_RGB_ALL, Color);
            }
            break;
    }
}
#endif
