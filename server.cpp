#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <errno.h>
#include <linux/limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

static void msg(const char *msg) { fprintf(stderr, "%s\n", msg); }

static void die(const char *msg) {
  int err = errno;
  fprintf(stderr, "{%d} %s\n", err, msg);
  abort();
}

static void process_request(int connfd) {
  char rbuf[64] = {};
  ssize_t n = read(connfd, rbuf, sizeof(rbuf) - 1);
  if (n < 0) {
    msg("read() error");
    return;
  }
  printf("client says %s \n", rbuf);
  char wbuf[] = "hello from startgate , abubakar alfagdadi";
  write(connfd, wbuf, sizeof(wbuf));
}

int main() {
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    die("socket()");
  }

  // options needed for most server applicaitoncon
  int val = 1;
  setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

  // bind address to socket
  struct sockaddr_in addr = {};
  addr.sin_family = AF_INET;
  addr.sin_port = ntohs(1234);     // port number 0.0.0.0:1234
  addr.sin_addr.s_addr = ntohl(0); // for 0.0.0.0.
  int rv = bind(fd, (const struct sockaddr *)&addr, sizeof(addr));
  if (rv) {
    die("bind()");
  }
  // listen socket
  rv = listen(fd, SOMAXCONN);
  if (rv) {
    die("listen()");
  }

  while (1) {
    // accept the request and return the response
    struct sockaddr_in client_addr = {};
    socklen_t socklen = sizeof(client_addr);
    int confd = accept(fd, (struct sockaddr *)&client_addr, &socklen);
    if (confd < 0) {
      continue; // error
    }

    process_request(confd);
    close(confd);
  }

  return 0;
}
