
#ifndef __MYWIFI_H__
#define __MYWIFI_H__



void initWiFi(void);
void connectWiFi(void);

void configureOTA(void);

void handleSocketClient(void);


extern const char* ssid[];
extern const char* password[] ;


#endif
