/*
 * GccApplication1.c
 *
 * Created: 6/24/2017 11:50:58 AM
 * Author : Wicho
 */ 

#define F_CPU 16000000UL
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>






void LcdCommandWrite_UpperNibble(unsigned char cm){
	// Write upper nibble
	PORTC = (PORTC & 0xf0) | (cm >> 4);
	PORTC &= ~(1<<4); // RS = 0
	PORTC |= 1<<5; // E = 1
	_delay_ms(1);
	PORTC &= ~(1<<5); // E = 0
	_delay_ms(1);
}



void LcdCmdWrite(unsigned char cm){
	// Write upper nibble
	PORTC = (PORTC & 0xf0) | (cm >> 4);
	PORTC &= ~(1<<4); // RS = 0
	PORTC |= 1<<5; // E = 1
	_delay_ms(1);
	PORTC &= ~(1<<5); // E = 0
	_delay_ms(1);
	
	// Write lower nibble
	PORTC = (PORTC & 0xf0) | (cm & 0x0f);
	PORTC &= ~(1<<4); // RS = 0
	PORTC |= 1<<5; // E = 1
	_delay_ms(1);
	PORTC &= ~(1<<5); // E = 0
	_delay_ms(1);
}

void LcdDataWrite(unsigned char cm){
	// Write upper nibble
	PORTC = (PORTC & 0xf0) | (cm >> 4);
	PORTC |= 1<<4; // RS = 1
	PORTC |= 1<<5; // E = 1
	_delay_ms(1);
	PORTC &= ~(1<<5); // E = 0
	_delay_ms(1);
	
	// Write lower nibble
	PORTC = (PORTC & 0xf0) | (cm & 0x0f);
	PORTC |= 1<<4; // RS = 1
	PORTC |= 1<<5; // E = 1
	_delay_ms(1);
	PORTC &= ~(1<<5); // E = 0
	_delay_ms(1);
}

int flag = 0; 
int state = 0;

int main (void){


	//This is to initialize the LCD
	DDRC |= 0x0f; //sets 1's to the pins PC0,PC1,PC2,PC3
	DDRC |= ((1<<4)|(1<<5)); //sets 1's to PC4 or PC5
	LcdCommandWrite_UpperNibble(0x30);
	_delay_ms(4.1);
	LcdCommandWrite_UpperNibble(0x30);
	_delay_us(100);
	LcdCommandWrite_UpperNibble(0x30);
	LcdCommandWrite_UpperNibble(0x20);
	LcdCmdWrite(0x28);
	LcdCmdWrite(0x08);
	LcdCmdWrite(0x01);
	LcdCmdWrite(0x06);
	LcdCmdWrite(0x0f);
	_delay_ms(120);
	
	//We want to set reg
	DDRD = 0x00;
	PORTD = 0xff;
	
	
	EIMSK |= (1<<INT1);
	EICRA |= (1<<ISC11);
	

	while(1){
		if (flag == 0){
			//ENABLE GLOBAL INTERUPTS
			sei();
			flag = 1;
		}
		if (flag == 1){
			flag = 0;
			cli();
			_delay_ms(900);
					
		}
		
			
	}
}


ISR(INT1_vect)
{

	LcdCmdWrite(0x01);
	LcdCmdWrite(0x80);
	LcdDataWrite('0');
	flag = 1;

}

