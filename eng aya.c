#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <stdbool.h>
#include "macro.h"
#include "DIO.h"
#include "lcd.h"
#include "keypad.h"
#include "buzzer.h"
#include "eeprom.h"
#include "led.h"
#include "button.h"
#include <avr/interrupt.h>//to use bilt in functions in interrupt
#define cc   //common cathode
#include <stdio.h>
#include <math.h>
#include <float.h>
#define STATUS 0x70 // to know if this 1st time to set pass or not to check directly
#define LOCATION1 0x71
#define LOCATION2 0x72
#define LOCATION3 0x73
#define LOCATION4 0x74
#define  NOTPRESSED 0xff
int main(void)
{	keypad_inti();
	LCD_inti();
	LED_init('B' , 3);
	LED_init('B', 4);
	BUTTON_init('c',0);
	LCD_send_command(64); //to draw right
	LCD_send_data(0x00);
	LCD_send_data(0x00);
	LCD_send_data(0x01);
	LCD_send_data(0x03);
	LCD_send_data(0x16);
	LCD_send_data(0x1C);
	LCD_send_data(0x08);
	LCD_send_data(0x00);
	
	LCD_send_data(0x00); //indicate openn
	LCD_send_data(0x00);
	LCD_send_data(0x0C);
	LCD_send_data(0x12);
	LCD_send_data(0x10);
	LCD_send_data(0x1F);
	LCD_send_data(0x0F);
	LCD_send_data(0x0F);
	
	LCD_send_data(0x00); //to draw x
	LCD_send_data(0x11);
	LCD_send_data(0x0A);
	LCD_send_data(0x04);
	LCD_send_data(0x0A);
	LCD_send_data(0x11);
	LCD_send_data(0x00);
	LCD_send_data(0x00);
	
	LCD_send_data(0x00); // not opened safe
	LCD_send_data(0x00);
	LCD_send_data(0x0E);
	LCD_send_data(0x11);
	LCD_send_data(0x11);
	LCD_send_data(0x1F);
	LCD_send_data(0x1F);
	LCD_send_data(0x1F);
	 
	char val=NOTPRESSED , arr[4];
	int i=0,trial;
	bool flag=false, flag2=false , enter=false;
	while(1){
		if(BUTTON_read('c',0)==1){ //external pull down
		EEPROM_write(STATUS,0xff);
	}
		trial=2;
if(EPROM_read(STATUS)==NOTPRESSED){ //this mean this is 1st time and we will ask user to set new pass
	LCD_move_cursore(1 , 0);
	LCD_send_string("Set Pass : ");
	for(i=0 ; i<=3;i++){
	do
	{
		val=keypad_check_press();
	}while (val==NOTPRESSED);
	LCD_send_data(val);
	_delay_ms(600);
	LCD_move_cursore(1,12+i);
	LCD_send_data('*');
	EEPROM_write(LOCATION1+i,val); // store new pass in eeprom "non voltaile memory"
	}//end of for	
	_delay_ms(2000);
	LCD_clear_screen();
	enter=true;// to enter else to able user to c
	EEPROM_write(STATUS,0x00); 
}//end of if
 
else if((enter==true) || (EPROM_read(STATUS)!=NOTPRESSED) ){ //mean not 1st time to run code << ak to check pass not set
	 LCD_move_cursore(1 , 0);
	 LCD_send_string("Check Pass: ");
	for(i=0 ; i<=3;i++){
		do
		{
			val=keypad_check_press();
		}while (val==NOTPRESSED);
		LCD_send_data(val);
		_delay_ms(600);
		LCD_move_cursore(1,13+i);
		LCD_send_data('*');
		arr[i]=val;
	}//end of for
	_delay_ms(2000);
	LCD_clear_screen();
	flag=true;
	flag2=true;
	while(flag && flag2){ // flag=true<< indicate numbrs user entered!= which stored in eeprom
		                  // flag2=true << indicate loop ended and numbrs user entered== which stored in eeprom
		
		for(i=0 ; i<=3;i++){
			if(arr[i]!=EPROM_read(LOCATION1+i))
				flag=false;
				}
				flag2=false; //just to out from this while when comparing is finish
	}//end of while
	
	if(flag==true){ //safe will opened
		LCD_move_cursore(1,8);
		LCD_send_data(0);//send correct
		_delay_ms(500);
		LCD_move_cursore(2,1);
		LCD_send_string("Safe OPENED  ");
		LED_ON('B',4);
		LCD_send_data(1); //send safe opened logo
		_delay_ms(1500);
		LED_OFF('B',4);
		LCD_clear_screen();
	}
	if(flag==false){ // safe still closed
		flag=true;
		LCD_move_cursore(1,8);
		LCD_send_data(2);//send wrong
		_delay_ms(500);
		LCD_move_cursore(2,1);
		LCD_send_string("Still CLOSED  ");
			LED_ON('B',3);
		LCD_send_data(3);
		_delay_ms(1500);
		LED_OFF('B',3);
		LCD_clear_screen();
		LCD_move_cursore(1,1);
		LCD_send_string("ONE Left Trial ..");
		_delay_ms(800);
		LCD_clear_screen();
		// to try again
		 LCD_move_cursore(1 , 0);
		 LCD_send_string("Check Pass: ");
		 for(i=0 ; i<=3;i++){
			 do
			 {
				 val=keypad_check_press();
			 }while (val==NOTPRESSED);
			 LCD_send_data(val);
			 _delay_ms(600);
			 LCD_move_cursore(1,13+i);
			 LCD_send_data('*');
			 arr[i]=val;
		 }//end of for
		 _delay_ms(2000);
		 LCD_clear_screen();
		 // to check again
		for(i=0 ; i<=3;i++){ 
			if(arr[i]!=EPROM_read(LOCATION1+i))
			flag=false;
		}
		if(flag){
			LCD_move_cursore(1,8);
			LCD_send_data(0);//send correct
			_delay_ms(500);
			LCD_move_cursore(2,1);
			LCD_send_string("Safe OPENED  ");
			LED_ON('B',4);
			LCD_send_data(1); //send safe opened logo
			_delay_ms(1500);
			LED_OFF('B',4);
			LCD_clear_screen();
		}
		else{
			LCD_move_cursore(1,1);
			LCD_send_string("Exceed Limit !");
			LED_ON('B',3);
			_delay_ms(1000);
			LED_OFF('B',3);
			LCD_clear_screen();
			LCD_move_cursore(1,1);
			LCD_send_string("Wait 10 Sec to");
			_delay_ms(500);
			LCD_move_cursore(2,1);
			LCD_send_string("TRY Again ");
			_delay_ms(1100);
			LCD_clear_screen();
			LCD_move_cursore(1,1);
			LCD_send_string("LEFT ");
			LCD_move_cursore(1,11);
			LCD_send_string("Sec");
			for(i=8 ; i<=17 ; i++){
				LCD_move_cursore(1,7);
				LCD_send_data(65-i);
				_delay_ms(1000);
			}
			LCD_clear_screen();
		}
		
	}
}//end of else

	
	} //end of main while

}//end of main5


	

	