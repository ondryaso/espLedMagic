#include <HardwareSerial.h>
#include <math.h>
#include "LedMan.h"

LedMan::LedMan() {
    this->rgb = new float[3];
    this->r = this->rgb;
    this->g = this->r + 1;
    this->b = this->g + 1;

    this->mode = MANUAL;
    this->modeSettings = NULL;
    this->sinrbSettings = NULL;
}

LedMode LedMan::getMode() {
    return this->mode;
}

void LedMan::setMode(LedMode mode) {
    this->mode = mode;
    this->black();

    if(mode == SIN_RAINBOW) {
        this->setModeSettings(new SinRainbowModeSettings());
    }
}

void LedMan::black() {
    *this->r = 0;
    *this->g = 0;
    *this->b = 0;
}

void LedMan::white() {
    *this->r = 1;
    *this->g = 1;
    *this->b = 1;
}

void LedMan::setR(float r) {
    *this->r = r;
}

void LedMan::setG(float g) {
    *this->g = g;
}

void LedMan::setB(float b) {
    *this->b = b;
}

void LedMan::setRGB(float r, float g, float b) {
    *this->r = r;
    *this->g = g;
    *this->b = b;
}

void LedMan::setRGB(float * rgb) {
    memcpy(this->rgb, rgb, 3);
}

float LedMan::getR() {
    return *this->r;
}

float LedMan::getG() {
    return *this->g;
}

float LedMan::getB() {
    return *this->b;
}

float * LedMan::getRGB() {
    return this->rgb;
}

void LedMan::setModeSettings(ModeSettings * settings) {
    if (settings->targetMode == this->mode) {
        delete this->modeSettings;
        this->sinrbSettings = NULL;

        this->modeSettings = settings;

        if (this->mode == SIN_RAINBOW) {
            this->sinrbSettings = (SinRainbowModeSettings *) settings;
        }
    }
}

void LedMan::calculateStep(unsigned long time) {
    switch (this->mode) {
        case MANUAL:
            return;
        case SIN_RAINBOW:
            this->calculateSinRainbow(time);
            break;
    }
}

void LedMan::calculateSinRainbow(unsigned long time) {
    *this->r = (float) fabs(sin(((LM_PI * (time + this->sinrbSettings->redOffset)) /
                (this->sinrbSettings->peakTime * 2)))) * this->sinrbSettings->intensityMultiplier;
    *this->g = (float) fabs(sin(((LM_PI * (time + this->sinrbSettings->blueOffset)) /
                (this->sinrbSettings->peakTime * 2)))) * this->sinrbSettings->intensityMultiplier;
    *this->b = (float) fabs(sin(((LM_PI * (time + this->sinrbSettings->greenOffset)) /
                (this->sinrbSettings->peakTime * 2)))) * this->sinrbSettings->intensityMultiplier;
}
