
#define ESP32DEBUGGING
#include "esp_log.h"
#include <Arduino.h>
#include <Wire.h>

#include <ESP32Logger.h>

#ifndef _burner_power_h
#define _burner_power_h

#define USEPRINTF 1

#ifdef USE_LOGGER
#define BLog_d(tag, logmsg, ...) { String logstr = tag + ":" + String(__func__) + ": " + String(logmsg); Log.log(ESP32LogLevel::Debug, logstr.c_str(), ##__VA_ARGS__); }
#define BLog_i(tag, logmsg, ...) { String logstr = tag + ":" + String(__func__) + ": " + String(logmsg); Log.log(ESP32LogLevel::Info, logstr.c_str(), ##__VA_ARGS__); }
#define BLog_e(tag, logmsg, ...) { String logstr = tag + ":" + String(__func__) + ": " + String(logmsg); Log.log(ESP32LogLevel::Error, logstr.c_str(), ##__VA_ARGS__); }
#endif

#ifdef USEM5LOGGER
//#define  BLog_d(TAG, format, ...) M5.Log(ESP_LOG_DEBUG  , M5UNIFIED_LOG_FORMAT(D, format), ##__VA_ARGS__)
#define  BLog_d(TAG, format, ...) M5.Log(ESP_LOG_DEBUG  , M5UNIFIED_LOG_FORMAT(D, format), ##__VA_ARGS__)
#define  BLog_i(TAG, format, ...) M5.Log(ESP_LOG_DEBUG  , M5UNIFIED_LOG_FORMAT(D, format), ##__VA_ARGS__)
#define  BLog_e(TAG, format, ...) M5.Log(ESP_LOG_DEBUG  , M5UNIFIED_LOG_FORMAT(D, format), ##__VA_ARGS__)
#endif

#ifdef USEESPLOGGER
#define  BLog_d(TAG, str, ...) ESP_LOGD(TAG,str, ##__VA_ARGS__)
#define  BLog_i(TAG, str, ...) ESP_LOGI(TAG,str, ##__VA_ARGS__)
#define  BLog_e(TAG, str, ...) ESP_LOGE(TAG,str, ##__VA_ARGS__)
#endif

#ifdef USEPRINTF
#define  BLog_d(TAG, logmsg, ...) { char buf[128]; String logstr = TAG + String(":") + String(__func__) + ": " + String(logmsg);  sprintf(buf, logstr.c_str(), ##__VA_ARGS__); Serial.println(buf);}
#define  BLog_i(TAG, logmsg, ...) { char buf[128]; String logstr = TAG + String(":") + String(__func__) + ": " + String(logmsg);  sprintf(buf, logstr.c_str(), ##__VA_ARGS__); Serial.println(buf);}
#define  BLog_e(TAG, logmsg, ...) { char buf[128]; String logstr = TAG + String(":") + String(__func__) + ": " + String(logmsg);  sprintf(buf, logstr.c_str(), ##__VA_ARGS__); Serial.println(buf);}
#endif



//#endif #define M5_LOGE(format, ...) M5.Log(ESP_LOG_ERROR  , M5UNIFIED_LOG_FORMAT(E, format), ##__VA_ARGS__)


#endif // _burner_power_h
