#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include <WiFiManager.h>

#include "editline.h"
#include "cmdproc.h"
#include "print.h"

#include "Arduino.h"

static char line[256];

void setup(void)
{
    WiFiManager wifiManager;

    // Set up serial output
    Serial.begin(115200);
    EditInit(line, sizeof(line));

    // set up WIFI connection
    wifiManager.autoConnect("ESP_CLOCK");
    
    printf("Hello World!\n");
}

static int do_help(int argc, char *argv[]);

static const cmd_t cmds[] = {
    {"help", do_help, "Shows this help text"},
    {"", NULL, ""}
};

static int do_help(int argc, char *argv[])
{
    for (const cmd_t *cmd = cmds; cmd->name[0] != 0; cmd++) {
        printf("%10s %s\n", cmd->name, cmd->help);
    }
    return 0;
}

void loop(void)
{

    if (Serial.available()) {
        char cin = Serial.read();
        char cout;
        boolean haveLine = EditLine(cin, &cout);
        printf("%c", cout);
        if (haveLine) {
            int res = cmd_process(cmds, line);
            printf("< %d\n", res);
            printf("> ");
        }
    }
}

