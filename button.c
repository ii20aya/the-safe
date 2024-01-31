/*
 * button.c
 *
 * Created: 10/12/2023 2:15:14 PM
 *  Author: HD STORE
 */

 #include "DIO.h"

 void BUTTON_inti(char port, char pin) {
	 DIO_SET_BINDIR(port, pin, 0);
 }

 unsigned char BUTTON_read(char port, char pin) {
	 unsigned char x=0;
	 x= DIO_READ(port, pin);
	 return x;
 }
