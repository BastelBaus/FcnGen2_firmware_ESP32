
#include "config.h"
#include "MySerial.h"



varInfo_t configParameters[] =
                          {  {"c0m0A2LSB","A2LSB_mode0","V/LSB",FLOAT,0},
                             {"c0m1A2LSB","A2LSB_mode1","V/LSB",FLOAT,0},
                             {"c0m2A2LSB","A2LSB_mode2","V/LSB",FLOAT,0},
                             {"c0m3A2LSB","A2LSB_mode3","V/LSB",FLOAT,0},
                             {"c0m0O2LSBofs","O2LSB_ofs_mode0","LSB",INT,0},
                             {"c0m1O2LSBofs","O2LSB_ofs_mode1","LSB",INT,0},
                             {"c0m2O2LSBofs","O2LSB_ofs_mode2","LSB",INT,0},
                             {"c0m3O2LSBofs","O2LSB_ofs_mode3","LSB",INT,0},
                             {"c0m0O2LSBgain","O2LSB_gain_mode0","LSB/V",FLOAT,0},
                             {"c0m1O2LSBgain","O2LSB_gain_mode1","LSB/V",FLOAT,0},
                             {"c0m2O2LSBgain","O2LSB_gain_mode2","LSB/V",FLOAT,0},
                             {"c0m3O2LSBgain","O2LSB_gain_mode3","LSB/V",FLOAT,0},

                             {"c1m0A2LSB","A2LSB_mode0","LSB/V",FLOAT,1},
                             {"c1m1A2LSB","A2LSB_mode1","LSB/V",FLOAT,1},
                             {"c1m2A2LSB","A2LSB_mode2","LSB/V",FLOAT,1},
                             {"c1m3A2LSB","A2LSB_mode3","LSB/V",FLOAT,1},
                             {"c1m0O2LSBofs","O2LSB_ofs_mode0","LSB/V",INT,1},
                             {"c1m1O2LSBofs","O2LSB_ofs_mode1","LSB/V",INT,1},
                             {"c1m2O2LSBofs","O2LSB_ofs_mode2","LSB/V",INT,1},
                             {"c1m3O2LSBofs","O2LSB_ofs_mode3","LSB/V",INT,1},
                             {"c1m0O2LSBgain","O2LSB_gain_mode0","LSB/V",FLOAT,1},
                             {"c1m1O2LSBgain","O2LSB_gain_mode1","LSB/V",FLOAT,1},
                             {"c1m2O2LSBgain","O2LSB_gain_mode2","LSB/V",FLOAT,1},
                             {"c1m3O2LSBgain","O2LSB_gain_mode3","LSB/V",FLOAT,1},

                             {"ssid1","SSID1","",STRING,2},
                             {"pass1","PASS1","",STRING,2},
                             {"ssid2","SSID2","",STRING,2},
                             {"pass2","PASS2","",STRING,2},
                             {"ssid3","SSID3","",STRING,2},
                             {"pass3","PASS3","",STRING,2}

                          };

uint8_t n_configParameters = sizeof(configParameters)/sizeof(configParameters[0]);



String Config::getConfigList(uint8_t channel) {
  String ret("");
  for(uint8_t k=0; k< n_configParameters ;k++) {
      if( configParameters[k].group == channel) {
        if (ret != "") ret += ",";
        ret += configParameters[k].id;
      } // if( configParameters[k].group == channel) {
  } //for(uint8_t k=0; k++;k<n) {
  return ret;
}

bool Config::updateConfig(String val,String newVal) {
  return false;
}

bool Config::readConfig(String val,varInfo_t* res) {
  for(uint8_t k=0; k< n_configParameters ;k++) {
      if( String(configParameters[k].id) == val) {
        //(*res) = configParameters[k];
        return true;
      } // if( configParameters[k].group == channel) {
  } //for(uint8_t k=0; k++;k<n) {
  return false;
}




const char* config_namespace = "mywlan";

Config conf;


Config::Config() {
}


#define ERROR_AND_RETURN(msg) { Serial.println(msg); return false; }

bool Config::begin(void) {
  if ( ! prefs.begin(config_namespace) ) ERROR_AND_RETURN(F("could not initialize section"));
  return true;
}
void Config::end(void) {
  prefs.end();
}

bool Config::load() {
  for(uint8_t k=0; k<3; k++) {
    String pass = String("pass")+String(k+1);
    String ssid = String("ssid")+String(k+1);
    if ( (prefs.getString(ssid.c_str(), wlan[k].ssid, 32) == 0) ||
         (prefs.getString(pass.c_str(), wlan[k].password, 63) == 0))
      { mySerial.error("Failed to load wlan parameters");
        //return false;
       }
  } // for(uint8_t k=0; k<3; k++) {

    channel[0][0].A2LSB = prefs.getFloat("c0m0A2LSB", 0);
    channel[0][1].A2LSB = prefs.getFloat("c0m1A2LSB", 0);
    channel[0][2].A2LSB = prefs.getFloat("c0m2A2LSB", 0);
    channel[0][3].A2LSB = prefs.getFloat("c0m3A2LSB", 0);

    channel[1][0].A2LSB = prefs.getFloat("c1m0A2LSB", 0);
    channel[1][1].A2LSB = prefs.getFloat("c1m1A2LSB", 0);
    channel[1][2].A2LSB = prefs.getFloat("c1m2A2LSB", 0);
    channel[1][3].A2LSB = prefs.getFloat("c1m3A2LSB", 0);

    channel[0][0].O2LSBofs = prefs.getFloat("c0m0O2LSBofs", 0);
    channel[0][1].O2LSBofs = prefs.getFloat("c0m1O2LSBofs", 0);
    channel[0][2].O2LSBofs = prefs.getFloat("c0m2O2LSBofs", 0);
    channel[0][3].O2LSBofs = prefs.getFloat("c0m3O2LSBofs", 0);
    channel[0][0].O2LSBgain = prefs.getFloat("c0m0O2LSBgain", 0);
    channel[0][1].O2LSBgain = prefs.getFloat("c0m1O2LSBgain", 0);
    channel[0][2].O2LSBgain = prefs.getFloat("c0m2O2LSBgain", 0);
    channel[0][3].O2LSBgain = prefs.getFloat("c0m3O2LSBgain", 0);

    channel[1][0].O2LSBofs = prefs.getFloat("c1m0O2LSBofs", 0);
    channel[1][1].O2LSBofs = prefs.getFloat("c1m1O2LSBofs", 0);
    channel[1][2].O2LSBofs = prefs.getFloat("c1m2O2LSBofs", 0);
    channel[1][3].O2LSBofs = prefs.getFloat("c1m3O2LSBofs", 0);
    channel[1][0].O2LSBgain = prefs.getFloat("c1m0O2LSBgain", 0);
    channel[1][1].O2LSBgain = prefs.getFloat("c1m1O2LSBgain", 0);
    channel[1][2].O2LSBgain = prefs.getFloat("c1m2O2LSBgain", 0);
    channel[1][3].O2LSBgain = prefs.getFloat("c1m3O2LSBgain", 0);


  return true;
}

bool Config::updateFloat(float v, const char* key) {
  //if ( ! prefs.begin(config_namespace) ) ERROR_AND_RETURN(F("could not initialize section"));
  prefs.putFloat(key,v);
  return load();
}

bool Config::updateInt(int32_t v, const char* key) {
  prefs.putInt(key,v);
  return load();
}
bool Config::updateString(String v, const char* key) {
  prefs.putString(key,v);
  return load();
}




float Config::readFloat(const char* key) {
  //if ( ! prefs.begin(config_namespace) ) ERROR_AND_RETURN(F("could not initialize section"));
  return prefs.getFloat(key,0);
}

int32_t Config::readInt(const char* key) {
  //if ( ! prefs.begin(config_namespace) ) ERROR_AND_RETURN(F("could not initialize section"));
  return prefs.getInt(key,0);
}

String Config::readString(const char* key) {
  //if ( ! prefs.begin(config_namespace) ) ERROR_AND_RETURN(F("could not initialize section"));
  String ret = prefs.getString(key,String("n/a"));
  //mySerial.debug("Key:  ", key);
  //mySerial.debug("Ret:  ", ret);
  return ret;
}


bool Config::store(void) {
  if (!initialized) Serial.println("NVS not initialized");
  Serial.printf("Free entries %d\n", prefs.freeEntries());
  prefs.clear();
  Serial.printf("Anzahl freier Einträge nach dem Init./Löschen: %d\n", prefs.freeEntries());

  if ( ! prefs.putString("ssid1", wlan[0].ssid) ) {
    return false;
  }
  if ( ! prefs.putString("pass1", wlan[0].password) ) {
    return false;
  }
  Serial.printf("Final %d\n", prefs.freeEntries());

  prefs.putFloat("c0m0A2LSB", channel[0][0].A2LSB);
  prefs.putFloat("c0m1A2LSB", channel[0][1].A2LSB);
  prefs.putFloat("c0m2A2LSB", channel[0][2].A2LSB);
  prefs.putFloat("c0m3A2LSB", channel[0][3].A2LSB);
  prefs.putFloat("c1m0A2LSB", channel[1][0].A2LSB);
  prefs.putFloat("c1m1A2LSB", channel[1][1].A2LSB);
  prefs.putFloat("c1m2A2LSB", channel[1][2].A2LSB);
  prefs.putFloat("c1m3A2LSB", channel[1][3].A2LSB);


  return true  ;
}
