#ifndef _ULTRASONIC_H_
#define _ULTRASONIC_H_


#define UL_LIMIT_MIN 5
#define UL_LIMIT_MID 10
#define UL_LIMIT_MAX 400

#define RGB_RED     0x220000
#define RGB_GREEN   0x002200
#define RGB_BLUE    0x000022
#define RGB_YELLOW  0x222200
#define RGB_PURPLE  0x220022
#define RGB_WHITE   0x222222

typedef enum
{
    E_RGB_ALL = 0,
    E_RGB_RIGHT = 1,
    E_RGB_LEFT = 2
} E_RGB_INDEX;

typedef enum
{
    E_EFFECT_BREATHING = 0,
    E_EFFECT_ROTATE = 1,
    E_EFFECT_FLASH = 2
} E_RGB_EFFECT;

class M5Ultrasonic
{
  public:
    uint16_t FrontDistance;
    M5Ultrasonic(byte signal_pin, byte rgb_pin);
    uint16_t GetUltrasonicDistance(void);
    void SetRgbColor(E_RGB_INDEX index, long Color);
    void SetRgbEffect(E_RGB_INDEX index, long Color, uint8_t effect);
  private:
    float farTime;
};
#endif
