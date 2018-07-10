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
    PrintInit(115200);
    EditInit(line, sizeof(line));

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

    if (Serial.available()) {
        char cin = Serial.read();
        char cout;
        boolean haveLine = EditLine(cin, &cout);
        print("%c", cout);
        if (haveLine) {
            int res = cmd_process(cmds, line);
            print("< %d\n", res);
            print("> ");
        }
    }
}

