/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: jpsilva17_ESEI
 *
 * Created on 16 de noviembre de 2018, 8:21
 */

#include <stdio.h>
#include <stdlib.h>
#include "gpio.h"
#include <unistd.h>

/*
 * 
 */
int main(int argc, char** argv) {
    GPIO_CONFIG();
    /*
    int valores [4]={8,6,4,2};
    int i;
    while(IN_BOTON(7)==0){
     
        for(i=0;i<4;i++){
        
            OUT_DISPLAY(i,valores[i],1);
            usleep(2500);
        }
    
    
    }
     */

    int valores [4] = {9, 9, 9, 2};
    int i;
    while (IN_BOTON(7) == 0) {

        for (i = 0; i < 4; i++) {

            OUT_DISPLAY(i, valores[i], 1);
            usleep(2500);
        }
        valores[0] = valores[0] - 1;
        int boton1=IN_BOTON(0);
        int boton2=IN_BOTON(1);
        int boton3=IN_BOTON(2);
        if (!(boton1 == 1 && boton2==0 && boton3 == 1)){
            if (valores[0] == 0) {
                valores[0] = 9;
                valores[1] = valores[1] - 1;

            }

            if (valores[1] == 0) {
                valores[1] = 9;
                valores[2] = valores[2] - 1;

            }

            if (valores[2] == 0) {
                valores[2] = 9;
                valores[3] = valores[3] - 1;

            }

        }else{
        
            valores[0]=0;
            valores[1]=0;
            valores[2]=0;
            valores[3]=8;
        
        
        
        }
    }





    GPIO_OFF();
    return (EXIT_SUCCESS);
}

