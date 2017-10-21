#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include "socket_IO.h"
#include "gpio_IO.h"
#include <wiringPi.h>  //wiringPiをインストールする必要あり

//GPIO OUTPUTの始まりピンを指定
#define GPIO_OUTPUT_BGN 12
//GPIO OUTPUTの終わりピンを指定
#define GPIO_OUTPUT_END 21
//GPIO INPUTの始まりピンを指定
#define GPIO_INPUT_BGN 4 
//GPIO INPUTの終わりピンを指定
#define GPIO_INPUT_END 11

int initialize_socket(int* sd, struct sockaddr_in* addr, int* acc_sd, struct sockaddr_in* from_addr, socklen_t* sin_size) {
  int option = 1;

  // IPv4 TCP のソケットを作成する
  if((*sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    printf("error\n");
    return -1;
  }

  // 送信先アドレスとポート番号を設定する
  addr->sin_family = AF_INET;
  addr->sin_port = htons(22222);
  addr->sin_addr.s_addr = htonl(INADDR_ANY);

  // TCPのTIME_WAIT問題回避
  if(setsockopt(*sd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0) {
    perror("setsockopt(SO_REUSEADDR) error : ");
    close(*sd);
    return -1;
  }

  // バインドする
  if(bind(*sd, (struct sockaddr *)addr, sizeof(*addr)) < 0) {
    perror("bind");
    return -1;
  }

  // パケット受信待ち状態とする
  // 待ちうけキューを１０としている
  if(listen(*sd, 10) < 0) {
    perror("listen");
    return -1;
  }

  // クライアントからコネクト要求が来るまで停止する
  // 以降、サーバ側は acc_sd を使ってパケットの送受信を行う
  if((*acc_sd = accept(*sd, (struct sockaddr *)from_addr, sin_size)) < 0) {
    perror("accept");
    return -1;
  }

  printf("accepted connection from %s, port = %d\n", inet_ntoa(from_addr->sin_addr), ntohs(from_addr->sin_port));
  return 1;
}


int main(int argc, char** argv) {
  int sd;
  int acc_sd;
  struct sockaddr_in addr;

  socklen_t sin_size = sizeof(struct sockaddr_in);
  struct sockaddr_in from_addr;

  unsigned long buf = 0;

  // ソケット通信初期化
  initialize_socket(&sd, &addr, &acc_sd, &from_addr, &sin_size);

  // GPIO初期化
  initialize_gpio(GPIO_INPUT_BGN, GPIO_INPUT_END, GPIO_OUTPUT_BGN, GPIO_OUTPUT_END);

  while(1) {
    //for(int i = 0; i < 50; i++) { printf("-"); }
    //printf("\n");

    // パケット受信。バッファを空にする。
    receive_message(acc_sd, &buf);

    // GPIOへデータを書き込み
    write_gpio(&buf, GPIO_OUTPUT_BGN, GPIO_OUTPUT_END, GPIO_INPUT_BGN);

    // 受信データの出力
    //printf("Received : %32lu\n", buf);

    // GPIOの状態を取得
    read_gpio(&buf, GPIO_INPUT_BGN, GPIO_INPUT_END, GPIO_INPUT_BGN);

    // パケットを TCP で送信
    send_message(acc_sd, &buf);

    usleep(100000);
  }

  // パケット送受信用ソケットのクローズ
  close(acc_sd);

  // 接続要求待ち受け用ソケットをクローズ
  close(sd);

  return 0;
}
