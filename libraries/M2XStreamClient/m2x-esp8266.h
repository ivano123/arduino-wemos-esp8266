#include "Arduino.h"
#define USER_AGENT "User-Agent: M2X ESP8266 Client/" M2X_VERSION

#ifdef DEBUG
#define DBG(fmt_, data_) Serial.print(data_)
#define DBGLN(fmt_, data_) Serial.println(data_)
#define DBGLNEND Serial.println()
#endif  /* DEBUG */

#ifdef M2X_ENABLE_READER
#error "As of now, ESP8266 does not support reader functions!"
#endif  /* M2X_ENABLE_READER */

class M2XTimer {
public:
  void start() {}

  unsigned long read_ms() { return millis(); }
};
