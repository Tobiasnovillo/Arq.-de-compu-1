#include "EasyPIO.h"
#include <stdio.h>
int menu (void);
void autof(void);
void disp_binary(int);
void no_yet (void);
//
// La enumeraci�n de Pines es de acuerdo a la convenci�n de BroadCom.
// Pines 14,15,18,23,24,25,8,7 deben conectarse a los 8 LEDs
const char led[] = {14,15,18,23,24,25,8,7}; //variable global

void main (void){
    pioInit();
    int i;
    for(i=0; i<8; i++){
        pinMode(led[i], OUTPUT); // Configure los 8 pines para los LEDs como salidas en main
    }
    leds(0xFF); //Turn off leds active low
    //
    int choice;
    for(;;){
        choice=menu();
        switch (choice){
            case 1: autof();
            break;
            case 2: choquef();
            break;
            case 3: no_yet();
            break;
            case 4: no_yet();
            break;
            case 5: exit(0);
        }
    }
}
void no_yet (void){
    int num;
    printf("No implementada\n");
}
/*void autof (void){
  int num;
  printf("Auto fantastico No implementado\n");
}*/
int menu (void){
    int s;
    do{
        //  clrscr();
        printf("\n   MENU  \n");
        printf("1) autof\n");
        printf("2) choquef\n");
        printf("3) opcion3\n");
        printf("4) opcion4\n");
        printf("5) salir\n");
        printf("Elija su opcion: ");
        scanf("%d", &s);
    } while (s<1 || s>5);
    return s;
}
void disp_binary (int i){
    int t;
    for (t=128; t>0; t=t/2)
        if(i&t) printf ("*");
        else printf("_");
    printf("\n");
}
int leds(int num){
    int i, numval;

    for(i=0; i<8; i++){
        // Write the number to the 8 LEDs
        numval = (num >> i) & 0x01;
        digitalWrite(led[i], numval);
    }
    //  delayMillis(3000); // Espera 3 segundos

    return 0;
}