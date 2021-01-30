//using separate port for data and control pins in 4-bit mode

#include "lcd.h"		  //LCD header file

void LCD_Init(void)
{
	LCD_Command_Dir = 0xFF;  //intialise command port as output
	LCD_Data_Dir = 0xF0;	 //intialise data port i.e D7-D4  as output	
//	LCD_Command_Port &= ~(1<<EN);
//	_delay_ms(20);			
	
	LCD_Command (0x33);		//initialize in 4-bit mode
	LCD_Command (0x32);		//initialize in 4-bit mode
	LCD_Command(0x28);     //2 lines and 5*7 matrix for 4bit mode
	LCD_Command (0x0c);    //display on cursor off
//	_delay_ms(1);
	LCD_Command (0x06);		//autoincrement cursor
	LCD_Command (0x01);		//clear
}

void LCD_Clear(void)		// LCD function to clear display
{
	LCD_Command (0x01);		// clear display 
	_delay_ms(2);
	LCD_Command (0x80);		// cursor at home position 
	_delay_ms(2);
}

void LCD_Command(unsigned char cmnd)	// LCD function to send command 
{
//	LCD_Data_Port= /*(LCD_Data_Port & 0xF0) |*/ (cmnd & 0xF0); //sending upper 4 nibble

	LCD_Data_Port= (cmnd & 0xF0); //sending upper 4 nibble
	LCD_Command_Port &= ~(1<<RS);	// RS=0 command reg. 
	_delay_ms(1);
	LCD_Command_Port &= ~(1<<RW);	// RW=0 Write operation 
	_delay_ms(1);
	LCD_Command_Port |= (1<<EN);	// Enable pulse 
	_delay_ms(1);
	LCD_Command_Port &= ~(1<<EN);  //disable pulse
	
	_delay_ms(1);
	
//	LCD_Data_Port= /*(LCD_Data_Port & 0xF0) |*/ (cmnd << 4);

	LCD_Data_Port= (cmnd << 4);
	LCD_Command_Port |= (1<<EN);	// Enable pulse 
	_delay_ms(1);
	LCD_Command_Port &= ~(1<<EN);
	_delay_ms(1);
}

void LCD_Char (unsigned char char_data)	// LCD data write function 
{
//	LCD_Data_Port= /*(LCD_Data_Port & 0XF0) |*/ (char_data & 0XF0); //SENDING UPPER NIBBLE;

	LCD_Data_Port= (char_data & 0XF0); //SENDING UPPER NIBBLE;
	LCD_Command_Port |= (1<<RS);	// RS=1 Data reg. 
	_delay_ms(1);
	LCD_Command_Port &= ~(1<<RW);	// RW=0 write operation 
	_delay_ms(1);
	LCD_Command_Port |= (1<<EN);	// Enable Pulse 
	_delay_ms(1);
	LCD_Command_Port &= ~(1<<EN);
	
	_delay_ms(1);
	
//	LCD_Data_Port= /*(LCD_Data_Port & 0XF0) |*/ (char_data << 4); //SENDING LOWER 4NIBBLES
	
	LCD_Data_Port= (char_data << 4); //SENDING LOWER 4NIBBLES
	LCD_Command_Port |= (1<<EN);
	_delay_ms(1);
	LCD_Command_Port &= ~ (1<<EN);
	_delay_ms(1);
	
}

void LCD_String (char *str)		// Send string to LCD function 
{
	int i;
	for(i=0;str[i]!=0;i++)		// Send each char of string till the NULL 
	{
		LCD_Char (str[i]);
	}
}

void LCD_String_xy (char row, char pos, char *str)// Send string to LCD with xy position 
{
	if (row == 0 && pos<16)
	LCD_Command((pos & 0x0F)|0x80);	// Command of first row and required position<16 
	else if (row == 1 && pos<16)
	LCD_Command((pos & 0x0F)|0xC0);	// Command of first row and required position<16 
	LCD_String(str);		// Call LCD string function *
}

int main()
{
	LCD_Init();
	_delay_ms(500);
	LCD_String_xy (0,3,"WELCOME TO");
	LCD_Command(0xC0);
	LCD_String_xy (1,6,"NETEL");	
	PORTE=0b01000000;
}