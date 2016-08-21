#include <stdio.h>

#include "LedControl.h"
#include "Arduino.h"

#include "wordclock.c"

// data, clock, cs
LedControl led(4, 2, 3, 1);

void setup(void)
{
    Serial.begin(115200);
    Serial.println("Hello world!");

    led.shutdown(0, false);
    led.setIntensity(0, 15);
    led.clearDisplay(0);
}

uint8_t matrix[8];

void loop(void)
{
    static int h = 0;
    static int m = 0;
    
    // update time
    m++;
    if (m >= 60) {
        m = 0;
        h++;
        if (h > 23) {
            h = 0;
        }
    }

    // calculate it
    memset(matrix, 0, sizeof(matrix));
    fill(matrix, h, m);
    int i;
    for (i = 0; i < 8; i++) {
        led.setRow(0, i, matrix[i]);
    }
    
    char buf[16];
    sprintf(buf, "%02d:%02d", h, m);
    Serial.println(buf);

    delay(500);
}

