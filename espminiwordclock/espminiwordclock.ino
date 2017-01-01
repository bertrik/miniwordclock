#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include <WiFiManager.h>

#include "editline.h"
#include "cmdproc.h"

#include "Arduino.h"

static int serial_putc(char c, FILE *t)
{
    Serial.write(c);
    return 0;
}

static int print(const char *fmt, ...)
{
    char buf[256];
    va_list args;
    va_start (args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end (args);

    // send it to serial
    char *p = buf;
    while (*p != 0) {
        serial_putc(*p++, 0);
    }
}

void setup(void)
{
    WiFiManager wifiManager;

    // Set up serial output
    Serial.begin(115200);
    edit_init(&serial_putc);

    // set up WIFI connection
    wifiManager.autoConnect("ESP_CLOCK");
    
    print("Hello World!\n");
}

static int do_help(int argc, char *argv[]);

static const cmd_t cmds[] = {
    {"help", do_help, "Shows this help text"},
    {"", NULL, ""}
};

static int do_help(int argc, char *argv[])
{
    for (const cmd_t *cmd = cmds; cmd->name[0] != 0; cmd++) {
        print("%10s %s\n", cmd->name, cmd->help);
    }
    return 0;
}

void loop(void)
{
    static char line[64];
    
    if (Serial.available()) {
        char c = Serial.read();
        if (edit_line(c, line, sizeof(line))) {
            int res = cmd_process(cmds, line);
            print("< %d\n", res);
            print("> ");
        }
    }
}

