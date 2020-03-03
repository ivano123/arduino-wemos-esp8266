#include "Energia.h"
#define USER_AGENT "User-Agent: M2X Energia Client/" M2X_VERSION

#ifdef DEBUG
#define DBG(fmt_, data_) Serial.print(data_)
#define DBGLN(fmt_, data_) Serial.println(data_)
#define DBGLNEND Serial.println()
#endif  /* DEBUG */

/* By default Energia board uses aJson package */
#ifdef M2X_ENABLE_READER
#if !(defined(M2X_READER_JSONLITE) || defined(M2X_READER_AJSON))
#define M2X_READER_AJSON
#endif
#endif  /* M2X_ENABLE_READER */

class M2XTimer {
public:
  void start() {}

  unsigned long read_ms() { return millis(); }
};

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus
double atof(const char *s)
{
	double a = 0.0;
	int e = 0;
	int c;
	while ((c = *s++) != '\0' && isdigit(c)) {
		a = a*10.0 + (c - '0');
	}
	if (c == '.') {
		while ((c = *s++) != '\0' && isdigit(c)) {
			a = a*10.0 + (c - '0');
			e = e-1;
		}
	}
	if (c == 'e' || c == 'E') {
		int sign = 1;
		int i = 0;
		c = *s++;
		if (c == '+')
			c = *s++;
		else if (c == '-') {
			c = *s++;
			sign = -1;
		}
		while (isdigit(c)) {
			i = i*10 + (c - '0');
			c = *s++;
		}
		e += i*sign;
	}
	while (e > 0) {
		a *= 10.0;
		e--;
	}
	while (e < 0) {
		a *= 0.1;
		e++;
	}
	return a;
}
#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
