#ifndef __MYWEBSERVER__h__
#define __MYWEBSERVER__h__

#include <WebServer.h>

void initWebServer(void);

void handleNotFound(void);
void handleRoot(void);

//extern WebServer  webServer;


#endif
