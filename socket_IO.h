/* socket_IO.h */
#ifndef SOCKET_IO_H
#define SOCKET_IO_H

int send_message(int sd, unsigned long* buf) {
  if(send(sd, buf, sizeof(*buf), 0) < 0) {
    perror("send_message");
    return -1;
  }
  printf("Sent : %32lu\n", *buf);
  return 1;
}

int receive_message(int sd, unsigned long* buf) {
  *buf = 0;
  while(recv(sd, buf, sizeof(*buf), MSG_DONTWAIT) > 0) { }
  if(errno != EAGAIN || errno != EWOULDBLOCK) {
    perror("recv");
    return -1;
  }
  printf("Received : %32lu\n", *buf);
  return 1;
}

#endif
