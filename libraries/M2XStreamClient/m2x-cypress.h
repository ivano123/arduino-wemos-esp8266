#define USER_AGENT "User-Agent: M2X Cypress Client/" M2X_VERSION

#ifdef DEBUG
#define DBG(fmt_, data_)
#define DBGLN(fmt_, data_)
#define DBGLNEND
#endif  /* DEBUG */

/* TODO: Enable time service after we figure out how to include timer in Cypress */
#define M2X_DISABLE_TIME_SERVICE

#ifdef __cplusplus
extern "C" {
#endif

#include <project.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

void delay(int ms) {
  CyDelay(ms);
}

char* strdup(const char* s) {
  char* ret = (char*) malloc(strlen(s) + 1);
  if (ret == NULL) { return ret;}
  return strcpy(ret, s);
}

#ifdef __cplusplus
}
#endif

class Print {
public:
  size_t print(const char* s);
  size_t print(char c);
  size_t print(int n);
  size_t print(long n);
  size_t print(double n, int digits = 2);

  size_t println(const char* s);
  size_t println(char c);
  size_t println(int n);
  size_t println(long n);
  size_t println(double n, int digits = 2);
  size_t println();

  /* Dummy implementation to prevent this class from being pure virutal */
  virtual size_t write(uint8_t c) { return -1; }
  virtual size_t write(const uint8_t* buf, size_t size);
};

size_t Print::write(const uint8_t* buf, size_t size) {
  size_t ret = 0;
  while (size--) {
    ret += write(*buf++);
  }
  return ret;
}

size_t Print::print(const char* s) {
  return write((const uint8_t*)s, strlen(s));
}

size_t Print::print(char c) {
  return write(c);
}

size_t Print::print(int n) {
  return print((long) n);
}

size_t Print::print(long n) {
  char buf[8 * sizeof(long) + 1];
  snprintf(buf, sizeof(buf), "%ld", n);
  return print(buf);
}

// Digits are ignored for now
size_t Print::print(double n, int digits) {
  char buf[65];
  snprintf(buf, sizeof(buf), "%g", n);
  return print(buf);
}

size_t Print::println(const char* s) {
  return print(s) + println();
}

size_t Print::println(char c) {
  return print(c) + println();
}

size_t Print::println(int n) {
  return print(n) + println();
}

size_t Print::println(long n) {
  return print(n) + println();
}

size_t Print::println(double n, int digits) {
  return print(n, digits) + println();
}

size_t Print::println() {
  return print('\r') + print('\n');
}

/*
 * TCP Client
 */
class Client : public Print {
public:
  Client();
  ~Client();

  virtual int connect(const char *host, uint16_t port);
  virtual size_t write(uint8_t);
  virtual size_t write(const uint8_t *buf, size_t size);
  virtual int available();
  virtual int read();
  virtual void flush();
  virtual void stop();
  virtual uint8_t connected();
private:
  virtual int read(uint8_t *buf, size_t size);
  uint8_t _socket;
};

Client::Client() : _socket(0xFF) {
}

Client::~Client() {
}

int Client::connect(const char *host, uint16_t port) {
  int status;

  _socket = ETH0_TcpOpen(12344);
  ETH0_TcpConnect(_socket, ETH0_ParseIP(host), port);
  status = ETH0_TcpConnected(_socket);
  return status;
}

size_t Client::write(uint8_t b) {
  return write(&b, 1);
}

size_t Client::write(const uint8_t *buf, size_t size) {
  return ETH0_TcpSend(_socket, const_cast<uint8*>(buf), size);
}

int Client::available() {
  int len = ETH0_SocketRxDataWaiting(_socket);
  return len > 0;
}

int Client::read() {
  uint8_t buf;
  int ret = read(&buf, 1);

  if (ret > 0) {
    return buf;
  }
  return -1;
}

int Client::read(uint8_t *buf, size_t size) {
  return ETH0_TcpReceive(_socket, buf, size);
}

void Client::flush() {
  // does nothing, TCP stack takes care of this
}

void Client::stop() {
  ETH0_TcpDisconnect(_socket);
  ETH0_SocketClose(_socket);
}

uint8_t Client::connected() {
  return ETH0_TcpConnected(_socket);
}
