/*
			Programa: 	GPIO
*/
///*************************************************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include "gpio.h"

#define BCM2708_PERI_BASE        0x3F000000  /*RPI-3*/
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */
#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

int  mem_fd;
void *gpio_map=NULL;

// I/O access
volatile unsigned *gpio;

// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) (*(gpio+((g)/10)) &= ~(7<<(((g)%10)*3)))
#define OUT_GPIO(g) (*(gpio+((g)/10)) |=  (1<<(((g)%10)*3)))
#define SET_GPIO_ALT(g,a) (*(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3)))

#define GPIO_set (*(gpio + 7))            // sets   bits which are 1 ignores bits which are 0
#define GPIO_clr (*(gpio + 10))           // clears bits which are 1 ignores bits which are 0

#define GET_GPIO(g) (*(gpio+13)&(1<<g)) // 0 if LOW, (1<<g) if HIGH

#define GPIO_PULL     (*(gpio+37))      // Pull up/pull down
#define GPIO_PULLCLK0 (*(gpio+38))      // Pull up/pull down clock


const unsigned int displays[] = {4, 17, 18, 27, 23, 22, 24, 10};
const unsigned int botones[] = {25, 9, 8, 11, 7, 1, 0, 5};
const unsigned int leds[] =  {6, 12, 13, 16, 19, 20, 26, 21};

void setup_io(void)
{
	if(gpio_map==NULL){
		/* open /dev/mem */
		if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
			printf("can't open /dev/mem \n");
			exit(-1);
		}
		/* mmap GPIO */
		gpio_map = mmap(
				NULL,             //Any adddress in our space will do
				BLOCK_SIZE,       //Map length
				PROT_READ|PROT_WRITE,// Enable reading & writting to mapped memory
				MAP_SHARED,       //Shared with other processes
				mem_fd,           //File to map
				GPIO_BASE         //Offset to GPIO peripheral
		);
		close(mem_fd); //No need to keep mem_fd open after mmap
		if (gpio_map == MAP_FAILED) {
			printf("mmap error %p\n", gpio_map);//errno also set!
			exit(-1);
		}
		// Always use volatile pointer!
		gpio = (volatile unsigned *)gpio_map;
	}
}
void close_io(void){
    munmap(gpio_map, BLOCK_SIZE);
}

void GPIO_CONFIG(void)
{
    int g;
    setup_io();

    /************************************************************************\
     * You are about to change the GPIO settings of your computer.          *
     * Mess this up and it will stop working!                               *
     * It might be a good idea to 'sync' before running this program        *
     * so at least you still have your code changes written to the SD-card! *
    \************************************************************************/

    for(g = GPIO_MIN; g <= GPIO_MAX; g++) {
        INP_GPIO(g); // must use INP_GPIO before we can use OUT_GPIO
    }
    // Set GPIO LED pins  to output
    for(g = 0; g < 8; g++) {
        OUT_GPIO(leds[g]);
        OUT_GPIO(displays[g]);
    }
}

int GPIO_SET(unsigned char ngpio, unsigned char val)
{
	if((ngpio<GPIO_MIN)||(ngpio>GPIO_MAX)){
		printf("GPIO_CLR: Error GPIO%hhu no existe. GPIO[%d-%d]\n",ngpio,GPIO_MIN,GPIO_MAX);
		return -1;
	}
	if(gpio_map==NULL){ return -1;}
	
	if(val==OFF){
		GPIO_clr = 1<<ngpio;
	} else{
		GPIO_set = 1<<ngpio;
	}
	return 0;
}

int GPIO_GET(unsigned char ngpio)
{
	if((ngpio<GPIO_MIN)||(ngpio>GPIO_MAX)){
		printf("GPIO_CLR: Error GPIO%hhu no existe. GPIO[%d-%d]\n",ngpio,GPIO_MIN,GPIO_MAX);
		return -1;
	}
	if(gpio_map==NULL){ return -1;}
	
	return (!GET_GPIO(ngpio))?0:1;
}

void GPIO_OFF(void)
{
	if(gpio_map!=NULL){
		GPIO_clr = 0x1FFFFFFC;
        close_io();
	}
}


int GPIO_CLR(unsigned char ngpio)
{
	if((ngpio<GPIO_MIN)||(ngpio>GPIO_MAX)){
		printf("GPIO_CLR: Error GPIO%hhu no existe. GPIO[%d-%d]\n",ngpio,GPIO_MIN,GPIO_MAX);
		return -1;
	}
	if(gpio_map==NULL){ return -1;}
	
	GPIO_clr = 1<<ngpio;
	
	return 0;
}

int IN_BOTON(unsigned int b) {
    if (b>=8) return 0;
    return (GPIO_GET(botones[b]));
}

int OUT_LED(unsigned int b, unsigned char val) {
    if (b>=8) return 0;
    return GPIO_SET(leds[b], val?ON:OFF);
}

void OUT_LEDS(unsigned int out) {
    int g = 0, mask = 1;
    int outmask = 0;
    // Set GPIO DISPLAY pins  to output
    for (g = 0; g <= 7; g++) {
        outmask = out&mask;
        GPIO_SET(leds[g], outmask);
        mask <<= 1;
    } // for
} // OUT_LEDS


void OUT_DISPLAYS(unsigned int out) {
    int g=0,mask = 1;
    int outmask = 0;

    for ( g = 0; g <= 7; g++) {
        outmask = out&mask;
         GPIO_SET(displays[g], outmask);
        mask <<= 1;
    } // for
} // OUT_DISPLAYS

void OUT_DISPLAY(unsigned int n, unsigned int valor, int punto) {
    unsigned int out = 0x00;
    const unsigned int dispcodes[] =  {6, 10, 2, 12, 4, 8, 0};
    if (n>=7) return;
    out= (dispcodes[n]);
    if (punto) out ++;
    out = (out <<4 );
    out |= valor;
    
    OUT_DISPLAYS(out);
     
} // OUT_DISPLAY
