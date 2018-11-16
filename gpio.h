#ifndef gpio_H
#define gpio_H

#define IN  0
#define OUT 1

#define LOW  0
#define HIGH 1

#define OFF  0
#define ON 1

#define GPIO_MIN 0
#define GPIO_MAX 27

extern const unsigned int displays[];
extern const unsigned int botones[];
extern const unsigned int leds[];

void GPIO_OFF(void);
int GPIO_CLR(unsigned char ngpio);
void GPIO_CONFIG(void);
int GPIO_SET(unsigned char ngpio, unsigned char val);
int GPIO_GET(unsigned char ngpio);

int IN_BOTON(unsigned int b);
int OUT_LED(unsigned int b, unsigned char val);
void OUT_LEDS(unsigned int out);

#endif // gpio_H 

