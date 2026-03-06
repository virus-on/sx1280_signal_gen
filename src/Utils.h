#pragma once

#define DEBUG_MODE 1
#if DEBUG_MODE
  #define DEBUG_LOG(...) Serial.printf(__VA_ARGS__);
#else
  #define DEBUG_LOG(...) ;
#endif