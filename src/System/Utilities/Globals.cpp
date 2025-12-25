#include "Globals.h"

Timestamp deltaTime = 1;
u32 frameCount = 0;
u32 boopCount = 0;
u16 micValue = 0;
char btStatus[16] = "No Bluetooth";
char uptime[16] = "";

const char* PROJECT_NAME = "Toaster Blaster";
const char* VERSION_NUMBER = "v0.5.2";
const char* COMPILE_TIMESTAMP = __DATE__ " " __TIME__;
const char* LINK_1 = "github.com/diodeface/";
const char* LINK_2 = "ToasterBlaster";