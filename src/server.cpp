#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 5000

int main(int argc, char **argv) {
  int sock, in_sock;
  long val_read;
  struct sockaddr_in address; int addr_len = sizeof(address);

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("In socket init");
    exit(1);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

//  memset(address->sin_zero, '\0', sizeof(address->sin_zero));
  
  if ((bind(sock, (sockaddr*) &address, (socklen_t) sizeof(address)) < 0)) {
    perror("In socket bind");
    exit(2);
  }

  if ((listen(sock, 1) < 0)) {
    perror("In socket listen");
    exit(3);
  }

  char *msg = "Server says hello!";

  while (1) {
    printf("* Waiting for new connection");
    if ((in_sock = accept(sock, (struct sockaddr*) &address, (socklen_t*) &addr_len)) < 0) {
      perror("In socket accept");
      exit(4);
    }

    char buffer[30000] = {0};
    val_read = read(in_sock, buffer, 30000);
    printf("%s\n\n", buffer);
    write(in_sock, msg, strlen(msg));
    printf("* Msg sent *");
    close(in_sock);
  }



}
