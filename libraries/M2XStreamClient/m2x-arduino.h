#include "Arduino.h"
#define USER_AGENT "User-Agent: M2X Arduino Client/" M2X_VERSION

#ifdef DEBUG
#define DBG(fmt_, data_) Serial.print(data_)
#define DBGLN(fmt_, data_) Serial.println(data_)
#define DBGLNEND Serial.println()
#endif  /* DEBUG */

class M2XTimer {
public:
  void start() {}

  unsigned long read_ms() { return millis(); }
};
