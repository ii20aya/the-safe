/*
 * butt.c
 *
 * Created: 10/13/2023 12:42:58 AM
 *  Author: HD STORE
 */ 
#include "DIO.h"



void BUTTON_init(char port , char bin){

	
	
	DIO_SET_BINDIR(port, bin , 0); //input device
	

}
/////////////////////////////////////////

unsigned char BUTTON_read(unsigned char port ,unsigned char bin){
	unsigned char x;
	x=DIO_READ(port , bin );
	return x;
}