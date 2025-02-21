#include <netdb.h>
void sethostent(int x) {}
void setnetent(int x) {}
void setprotoent(int x) {}
void setservent(int x) {}
void endhostent(void) {}
void endnetent(void) {}
void endprotoent(void) {}
void endservent(void) {}

struct hostent *gethostent(void) { return 0; }
struct netent *getnetent(void) { return 0; }
struct protoent *getprotoent(void) { return 0; }
struct servent *getservent(void) { return 0; }

struct netent *getnetbyaddr(uint32_t x, int y) { return 0; }
struct netent *getnetbyname(const char *n) { return 0; }
struct protoent *getprotobynumber(int x) { return 0; }
struct protoent *getprotobyname(const char *n) { return 0; }
struct servent *getservbyport(int p, const char *s) { return 0; }
struct servent *getservbyname(const char *n, const char *s) { return 0; }

