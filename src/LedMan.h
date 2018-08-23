#ifndef ESPTEST_LEDMAN_H
#define ESPTEST_LEDMAN_H

#include <cmath>
#include <cstring>
#include <Arduino.h>
#include <HardwareSerial.h>

#define LM_PI 3.141592653f
#define LM_SET(chan, val) *this->chan = (((val) > 1) ? 1 : (((val) < 0) ? 0 : (val)))

enum LedMode {
    MANUAL = 0, SIN_RAINBOW = 1
};

struct ModeSettings {
    LedMode targetMode = MANUAL;
};

struct SinRainbowModeSettings : public ModeSettings {
    int peakTime;
    int redOffset;
    int greenOffset;
    int blueOffset;
    float intensityMultiplier;

    SinRainbowModeSettings() {
        targetMode = SIN_RAINBOW;
        peakTime = 2000;
        redOffset = 0;
        greenOffset = 1000;
        blueOffset = 3000;
        intensityMultiplier = 1;
    }
};

class LedMan {
public:
    float * r;
    float * g;
    float * b;
    float * w;

    LedMan();

    LedMode getMode();
    void setMode(LedMode mode);
    void setModeSettings(ModeSettings * settings);

    void calculateStep(unsigned long time);
    void black();
    void white();
    void fullWhite();

    float getR();
    float getG();
    float getB();
    float getW();
    float * getRGBW();

    void setR(float r);
    void setG(float g);
    void setB(float b);
    void setW(float w);
    void setRGBW(float r, float g, float b, float w);
    void setRGB(float r, float g, float b);
    void setRGBW(float * rgbw);
    void setRGB(float * rgb);

    static ModeSettings * createModeSettings(LedMode mode, const char * setstr) {
        switch (mode) {
            case MANUAL:
                return nullptr;
            case SIN_RAINBOW: {
                char * end;
                SinRainbowModeSettings * set = new SinRainbowModeSettings();
                set->peakTime = (int) strtol(setstr, &end, 10);
                set->redOffset = (int) strtol(end, &end, 10);
                set->greenOffset = (int) strtol(end, &end, 10);
                set->blueOffset = (int) strtol(end, &end, 10);
                set->intensityMultiplier = (float) strtod(end, &end);
                return set;
            }
            default:
                return nullptr;
        }
    }

    static const char * createSettingsString(ModeSettings * set) {
        switch (set->targetMode) {
            case MANUAL: {
                return nullptr;
            }
            case SIN_RAINBOW: {
                char * ret = new char[50];
                SinRainbowModeSettings * srs = (SinRainbowModeSettings *)(set);
                int p = os_sprintf(ret, "%d %d %d %d %f", srs->peakTime, srs->redOffset, srs->greenOffset,
                                srs->blueOffset, srs->intensityMultiplier);
                if(p > 0) {
                    return ret;
                } else {
                    return nullptr;
                }
            }
            default:
                return nullptr;
        }
    }

private:
    float * rgb;
    LedMode mode;

    ModeSettings * modeSettings;
    SinRainbowModeSettings * sinrbSettings;

    void calculateSinRainbow(unsigned long time);
};

#endif //ESPTEST_LEDMAN_H
