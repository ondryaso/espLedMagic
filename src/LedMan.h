#ifndef ESPTEST_LEDMAN_H
#define ESPTEST_LEDMAN_H

#include <string.h>
#include <Arduino.h>

#define LM_PI 3.141592653f

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

    LedMan();

    LedMode getMode();
    void setMode(LedMode mode);
    void setModeSettings(ModeSettings * settings);

    void calculateStep(unsigned long time);
    void black();
    void white();

    float getR();
    float getG();
    float getB();
    float * getRGB();
    void setR(float r);
    void setG(float g);
    void setB(float b);
    void setRGB(float r, float g, float b);
    void setRGB(float * rgb);

    static ModeSettings * createModeSettings(LedMode mode, const char * setstr) {
        switch (mode) {
            case MANUAL:
                return NULL;
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
                return NULL;
        }
    }

    static const char * createSettingsString(ModeSettings * set) {
        switch (set->targetMode) {
            case MANUAL: {
                return NULL;
            }
            case SIN_RAINBOW: {
                char * ret = new char[50];
                SinRainbowModeSettings * srs = static_cast<SinRainbowModeSettings *>(set);
                int p = os_sprintf(ret, "%d %d %d %d %f", srs->peakTime, srs->redOffset, srs->greenOffset,
                                srs->blueOffset, srs->intensityMultiplier);
                if(p > 0) {
                    return ret;
                } else {
                    return NULL;
                }
            }
            default:
                return NULL;
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
