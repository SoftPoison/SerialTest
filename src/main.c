#include <fxlib.h>
#include <stdio.h>

#include "serial.h"

/**
 * Entry point for the addin
 * 
 * @param isAppli 1 if launched by main menu, 0 if launched in eACT strip
 * @param OptionNum strip number (0-3). Only used if isAppli = 0
 * 
 * @return 1 if okay, 0 if error
 */
int AddIn_main(int isAppli, unsigned short OptionNum) {
    unsigned int key;
    unsigned char spBuf[16] = { 0 };
    unsigned char buffer[128] = { 0 };
    int result;

    Bdisp_AllClr_DDVRAM();

    locate(1, 1);
    Print((unsigned char*) "Initializing...");
    Bdisp_PutDisp_DD();

    Sleep(1000);

    // attempt to open the serial port
    result = serialInit();

    Bdisp_AllClr_DDVRAM();
    if (result || Serial_IsOpen() != 1) {
        locate(1,1);
        Print((unsigned char*) "Failed!");
        locate(1, 2);
        sprintf((char*) spBuf, "Result: %d", result);
        Print(spBuf);
        goto END;
    }
    else {
        locate(1, 1);
        Print((unsigned char*) "Initialised!");
        Bdisp_PutDisp_DD();
    }

    Sleep(1000);

    Bdisp_AllClr_DDVRAM();
    locate(1, 1);
    Print((unsigned char*) "Connecting...");
    Bdisp_PutDisp_DD();

    Sleep(1000);

    result = serialConnect();

    if (!result) {
        Bdisp_AllClr_DDVRAM();
        locate(1, 1);
        Print((unsigned char*) "Connected!");
    }

    locate(1, 2);
    sprintf((char*) spBuf, "Result: %d", result);
    Print(spBuf);
    Bdisp_PutDisp_DD();

    Sleep(1000);

    while (key != KEY_CTRL_EXIT) { 
        GetKey(&key);
    }

    END:
    Serial_Close(1);
    while (1) { // hang until menu is pressed
        GetKey(&key);
    }

    return 1;
}


// do not change below this line

#pragma section _BR_Size
unsigned long BR_Size;
#pragma section

#pragma section _TOP
int InitializeSystem(int isAppli, unsigned short OptionNum) {
    return INIT_ADDIN_APPLICATION(isAppli, OptionNum);
}
#pragma section

