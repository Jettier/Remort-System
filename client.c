#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <errno.h>
#include "socket_IO.h"
#include "gpio_IO.h"
#include <wiringPi.h>  //wiringPiをインストールする必要あり

//サーバー側のIPアドレスを指定
#define IPADDR "10.42.0.184"

//GPIO OUTPUTの始まりピンを指定
#define GPIO_OUTPUT_BGN 4
//GPIO OUTPUTの終わりピンを指定
#define GPIO_OUTPUT_END 11
//GPIO INPUTの始まりピンを指定
#define GPIO_INPUT_BGN 12
//GPIO INPUTの終わりピンを指定
#define GPIO_INPUT_END 21

void initialize_socket(int* sd, struct sockaddr_in* addr) {
  // IPv4 TCP のソケットを作成する
  if((*sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    printf("error\n");
  }

  // 送信先アドレスとポート番号を設定する
  addr->sin_family = AF_INET;
  addr->sin_port = htons(22222);
  addr->sin_addr.s_addr = inet_addr(IPADDR);

  // サーバ接続（TCP の場合は、接続を確立する必要がある）
  connect(*sd, (struct sockaddr *)addr, sizeof(struct sockaddr_in));

}


int main(int argc, char** argv)
{
  int sd;
  struct sockaddr_in addr;

  unsigned long buf = 0;

  // ソケット通信初期化し、サーバーに接続する
  initialize_socket(&sd, &addr);

  // GPIO初期化
  initialize_gpio(GPIO_INPUT_BGN, GPIO_INPUT_END, GPIO_OUTPUT_BGN, GPIO_OUTPUT_END);

  while(1) {
    for(int i = 0; i < 50; i++) { printf("-"); }
    printf("\n");

    // GPIOの状態を取得
    read_gpio(&buf, GPIO_INPUT_BGN, GPIO_INPUT_END, GPIO_OUTPUT_BGN);

    // パケットを TCP で送信
    send_message(sd, &buf);

    // パケット受信。バッファを空にする。
    receive_message(sd, &buf);

    // GPIOへデータを書き込み
    write_gpio(&buf, GPIO_OUTPUT_BGN, GPIO_OUTPUT_END, GPIO_OUTPUT_BGN);

    // 受信データの出力
    //printf("Received : %32lu\n", buf);

    usleep(100000);
  }

  close(sd);

  return 0;
}
