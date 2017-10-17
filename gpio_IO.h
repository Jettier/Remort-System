/* gpio_IO.h */
#ifndef GPIO_IO_H
#define GPIO_IO_H

#include <wiringPi.h>  //wiringPiをインストールする必要あり

void initialize_gpio(int input_bgn, int input_end, int output_bgn, int output_end) {
  wiringPiSetupGpio();
  for(int i = input_bgn; i <= input_end; i++) {
    pinMode(i, INPUT);
    pullUpDnControl(i, PUD_DOWN);
  }
  for(int i = output_bgn; i <= output_end; i++) {
    pinMode(i, OUTPUT);
  }
}

//INPUT設定のGPIOからHIGH/LOWを読む
void read_gpio(unsigned long* data, int input_bgn, int input_end, int gpio_bgn) {
  *data = 0;
  for(int i = input_bgn; i <= input_end; i++) {
    unsigned long status = digitalRead(i);
    printf("status of GPIO %2d is %lu\n", i, status);
    for(int j = i; j > gpio_bgn; j--) { 
      status = status << 1;
    }
    *data = *data | status;
  }
}

//OUTPUT設定のGPIOへHIGH/LOWを書き込む
void write_gpio(unsigned long* data, int output_bgn, int output_end, int gpio_bgn) {
  for(int i = output_bgn; i <= output_end; i++) {
    unsigned long status = 1;
    for(int j = i; j > gpio_bgn; j--) { 
      status = status << 1;
    }
    printf("writing %32lu to  GPIO %2d\n", (*data & status), i);
    digitalWrite(i, *data & status);
  }
}

#endif
