#ifndef MYLOG_H
#define MYLOG_H
#include "cocos2d.h"
using namespace cocos2d;

#define  LOGD_ON 1

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <android/log.h>

#define  LOG_TAG  "chtzj"
#if (LOGD_ON)
#define LD(...)     __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#else
#define LD(...)  do {} while (0);
#endif


#else
#if (LOGD_ON)
#define LD(format, ...)      CCLog(format, ##__VA_ARGS__)
#else
#define LD(...)  do {} while (0);
#endif
#endif
#endif