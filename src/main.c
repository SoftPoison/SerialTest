#include <fxlib.h>
#include <stdio.h>

#include "serial.h"

int loopState = 0;

int connectionLoop();

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

    // attempt to open the serial port
    result = serialInit();

    if (result || Serial_IsOpen() != 1) {
        locate(1,1);
        Print((unsigned char*) "Init Failed!");
        locate(1, 2);
        sprintf((char*) spBuf, "Result: %d", result);
        Print(spBuf);
        goto END;
    }

    locate(1,1);
    Print((unsigned char*) "Connecting...");
    Bdisp_PutDisp_DD();

    Sleep(1);

    result = serialConnect();

    Bdisp_AllClr_DDVRAM();
    if (result || Serial_IsOpen() != 1) {
        locate(1,1);
        Print((unsigned char*) "Connect Failed!");
        locate(1, 2);
        sprintf((char*) spBuf, "Result: %d", result);
        Print(spBuf);
        goto END;
    }

    locate(1, 1);
    Print((unsigned char*) "Connected!");
    Bdisp_PutDisp_DD();
    Sleep(1000);

    while (key != KEY_CTRL_EXIT) {
        result = connectionLoop();
        if (result)
            break;

        GetKey(&key); // TODO: find alternative that doesn't hang
        // if (IsKeyDown(KEY_CTRL_EXIT))
        //     break;

        Sleep(1);
    }

    locate(1, 1);
    Print((unsigned char*) "Something went wrong");
    locate(1, 2);
    sprintf((char*) spBuf, "Result: %d", result);
    Print(spBuf);
    Bdisp_PutDisp_DD();
    Sleep(1);

    END:
    Serial_Close(1);
    while (1) { // hang until menu is pressed
        GetKey(&key);
    }

    return 1;
}

int connectionLoop() {
    unsigned char status;
    int result;

    result = getConStatus(&status);
    if (result)
        return result;

    if (status == 1) { // connecting
        locate(1,1);
        Print((unsigned char*) "WiFi Connecting...");
        Bdisp_PutDisp_DD();
        Sleep(1000);

        return 0;
    }
    else if (status == 2) { // not connected
        result = getSsidList();
        if (result)
            return result + 10;
    }

    locate(1,1);
    Print((unsigned char*) "WiFi Connected!");
    Bdisp_PutDisp_DD();
    Sleep(1000);

    return 0;
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

