
#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <Arduino.h>
#include <Preferences.h>

typedef enum {STRING=1,FLOAT=2,INT=3} var_t;


typedef struct {
  const char* id;
  const char* name;
  const char* unit;
  const var_t type;
  const uint8_t group;
} varInfo_t;


extern const varInfo_t configParameters[];

class MyPreferences : public Preferences {
  public:
    size_t usedEntries(void);
    size_t activeEntries(void);
};

struct wlan_t {
  char password[64] = { 0 }; // max. 63 bytes per standard (min. 8)
  char ssid[33] = { 0 };     // max. 32 bytes per standard
};

struct channel_t {
  // A*sin(2*pi*f) + O
  // a*LSB
  float A2LSB;
  float O2LSBofs;
  float O2LSBgain;
};


class Config {
  public:
    Config(void);
    bool begin(void);
    void end(void);


    bool load(void);
    bool store(void);

    bool    updateFloat(float v, const char* key);
    bool    updateInt(int32_t v, const char* key);
    bool    updateString(String v, const char* key);

    float    readFloat(const char* key);
    int32_t  readInt(const char* key);
    String   readString(const char* key);

    String getConfigList(uint8_t channel);
    bool   updateConfig(String val,String newVal);
    bool   readConfig(String val,varInfo_t* res);



    wlan_t wlan[3];

    #define NUMBER_OF_MODES 4
    #define NUMBER_OF_CHANNELS 2
    channel_t channel[NUMBER_OF_CHANNELS][NUMBER_OF_MODES];

  private:
    bool initialized = false;
    MyPreferences prefs;
    char currentSection[16] = {0};
};

extern Config conf;

#endif
