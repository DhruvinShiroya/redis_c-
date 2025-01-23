

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

static void die(const char *msg) {
  int err = errno;
  fprintf(stderr, "[%d] %s/n", err, msg);
  abort();
}

int main() {

  // create fd socket
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    die("socket()");
  }

  // create client to
  struct sockaddr_in addr = {};
  addr.sin_family = AF_INET;
  addr.sin_port = ntohs(1234);
  addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK);

  // connect to the addr
  int rv = connect(fd, (const struct sockaddr *)&addr, sizeof(addr));
  if (rv < 0) {
    die("connect()");
  }

  // send message hello
  char msg[] = "hello";
  write(fd, msg, strlen(msg));

  char rbuf[64] = {};
  ssize_t n = read(fd, &rbuf, sizeof(rbuf) - 1);
  if (n < 0) {
    die("read()");
  }
  printf("server says: %s\n", rbuf);
  close(fd);

  return 0;
}
