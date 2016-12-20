#include <stdint.h>
#include "ST7735.h"
#include "Nokia5110.h"

// 16 rows (0 to 15) and 21 characters (0 to 20)
// Requires (11 + size*size*6*8) bytes of transmission for each character
uint32_t StX=0; // position along the horizonal axis 0 to 20
uint32_t StY=0; // position along the vertical axis 0 to 15
uint16_t StTextColor = ST7735_YELLOW;

enum initRFlags TabColor = INITR_GREENTAB;

void delay1ms (uint32_t ms)
{
	uint32_t volatile time;
	while(ms){
		time = 72724*2/91*5/8;
		while(time){
			time --;
		}
		ms--;
	}
}

static void commandlist(const uint8_t *addr){
	
	uint16_t ms;
	uint8_t numCommand, number_Arg;
	
	numCommand = *(addr ++);
	
	while(numCommand--){
		writeDC(COMMAND, *(addr++));
		number_Arg = *addr;
		ms = *(addr++) & DELAY;
		number_Arg &= ~DELAY;
		while(number_Arg){
			writeDC(DATA, *(addr++));
			number_Arg--;
		}
		if(ms){
			ms = *(addr++);
			if(ms == 255) {ms = 500;}
			delay1ms(ms);
		}
	}
}

// Set the region of the screen RAM to be modified
// Pixel colors are sent left to right, top to bottom
// (same as Font table is encoded; different from regular bitmap)
// Requires 11 bytes of transmission
void static setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
	writeDC(COMMAND, ST7735_CASET); //Column addr set
	writeDC(DATA, 0x00);
	writeDC(DATA, x0 + ColStart);
	writeDC(DATA, 0x00);
	writeDC(DATA, x1 + ColStart);	
	
	writeDC(COMMAND, ST7735_RASET); //Row addr set
	writeDC(DATA, 0x00);
	writeDC(DATA, y0 + RowStart);
	writeDC(DATA, 0x00);
	writeDC(DATA, y1 + RowStart);	
	
	writeDC(COMMAND, ST7735_RAMWR);
}

//------------ST7735_Color565------------
// Pass 8-bit (each) R,G,B and get back 16-bit packed color. see datasheet page 52.
// ?RGB 5 6 5 bit input
// Input: r red value
//        g green value
//        b blue value
// Output: 16-bit color
uint16_t ST7735_Color565(uint8_t r, uint8_t g, uint8_t b)
{
	return ((b & 0xF8) << 8) | ((g & 0xFC) << 3) | (r >> 3); //don't understand the format
}

//------------ST7735_FillRect------------
// Draw a filled rectangle at the given coordinates with the given width, height, and color.
// Requires (11 + 2*w*h) bytes of transmission (assuming image fully on screen)
// Input: x     horizontal position of the top left corner of the rectangle, columns from the left edge
//        y     vertical position of the top left corner of the rectangle, rows from the top edge
//        w     horizontal width of the rectangle
//        h     vertical height of the rectangle
//        color 16-bit color, which can be produced by ST7735_Color565()
// Output: none

void Fill_Full_Screen(uint16_t color)
{
	FillRect(0x00, 0x00, 128, 160, color );
}

void ST7735_init(const uint8_t *cmdList)
{
	
	volatile uint32_t delay;
	volatile uint32_t lock_status;
	ColStart = 0;
	RowStart = 0;
	SYSCTL_RCGCGPIO_R |= 0x20;
	
	while((SYSCTL_PRGPIO_R & 0x00000020) == 0){}; 

	GPIO_PORTF_LOCK_R = 0x4C4F434B;
	lock_status = GPIO_PORTF_LOCK_R;

	GPIO_PORTF_DIR_R |= 0x11; //PF0, PF4 are output  
	DebugPort("Finish");
	GPIO_PORTF_AFSEL_R |= 0x0E;
	GPIO_PORTF_AFSEL_R &= ~0x11; //PF4 and PF0 are GPIO
	GPIO_PORTF_DEN_R |= 0x1F;       
	GPIO_PORTF_AMSEL_R &= ~0x1F;     
	GPIO_PORTF_PCTL_R  = (GPIO_PORTF_PCTL_R & ~0xFFFFF) + 0x02220;

	GPIO_PORTF_PUR_R |= 0x08;	
		
	TFT_CS = TFT_CS_LOW;
	RESET = RESET_HIGH;
	delay1ms(500);
	RESET = RESET_LOW;
	delay1ms(500);
	RESET = RESET_HIGH;
	delay1ms(500);
		
	SYSCTL_RCGCSSI_R |= 0x02; 
	delay = SYSCTL_RCGCSSI_R;
	SSI1_CR1_R &= ~0x02;	
	SSI1_CR0_R = (SSI1_CR0_R & ~0xFFFF) + 0x0907;
	SSI1_CPSR_R = 0xFA;
	/* Pll 50MHZ, 20K clock. 
		BR = sysclk/(CPSDVSR * (1+SCR))
		SCR = 9;
	*/
	SSI1_CR1_R |= 0x02;
		
	if(cmdList)
	{
		commandlist(cmdList);
	}
}
 

void FillRect(uint8_t x, uint8_t y, uint16_t w, uint16_t h, uint16_t color)
{
	uint8_t hi = color >>8;
	uint8_t lo = color & 0xFF;
	if((x>= _width)||(y >= _height)) return;
	if((x + w) > _width) { w = _width - x;}
	if((y + h) > _height) { h = _height - y;}
	setAddrWindow(x, y, x+w, y+h);  //?
	
	for(y=h; y>0; y--){
		for(x=w; x>0; x--){
			writeDC(DATA, hi);	
			writeDC(DATA, lo);	
		}
	}
}


//------------ST7735_BInit------------
// Initialization for ST7735B screens.
// Input: none
// Output: none

void ST7735_BInit(void){
	ST7735_init(Bcmd);
	//SetCursor(0x00, 0x00);
	StTextColor = ST7735_YELLOW;
	Fill_Full_Screen(0);   
}

//------------ST7735_RInit------------
// Initialization for ST7735R screens (green or red tabs).
// Input: option one of the enumerated options depending on tabs
// Output: none
void ST7735_RInit(enum initRFlags option){
	ST7735_init(Rcmd1);

	if(option == INITR_GREENTAB)
	{
		DebugPort("Green");
		ST7735_init(Rcmd2green);
		ColStart = 2;
    RowStart = 1;
	}
	else{
		ST7735_init(Rcmd2red);
	}
	DebugPort("3");
	ST7735_init(Rcmd3);
	
	// if black, change MADCTL color filter
  if (option == INITR_BLACKTAB) {
    writeDC(COMMAND, ST7735_MADCTL);
    writeDC(DATA, 0xC0);
  }
	TabColor = option;
	ST7735_SetCursor(0x00, 0x00);
	StTextColor = ST7735_YELLOW;
	DebugPort("ST7735R Init");
	Fill_Full_Screen(0xFFFF); 
}
// The Data/Command pin must be valid when the eighth bit is
// sent.  The SSI module has hardware input and output FIFOs
// that are 8 locations deep.  Based on the observation that
// the LCD interface tends to send a few commands and then a
// lot of data, the FIFOs are not used when writing
// commands, and they are used when writing data.  This
// ensures that the Data/Command pin status matches the byte
// that is actually being transmitted.
// The write command operation waits until all data has been
// sent, configures the Data/Command pin for commands, sends
// the command, and then waits for the transmission to
// finish.
// The write data operation waits until there is room in the
// transmit FIFO, configures the Data/Command pin for data,
// and then adds the data to the transmit FIFO.
// NOTE: These functions will crash or stall indefinitely if
// the SSI0 module is not initialized and enabled.
void static writeDC(uint8_t type, uint8_t data) {
	if(type == 0){
		while((SSI1_SR_R & 0x10) == 0x10){};
		DC = DC_COMMAND;
		SSI1_DR_R = data;
		while((SSI1_SR_R & 0x10) == 0x10){};
	}else{
		while((SSI1_SR_R & 0x02) == 0x00){}; //when TX FIFO is full, wait
		DC = DC_DATA;
	  SSI1_DR_R = data;
	}
}


//------------ST7735_DrawChar------------
// Advanced character draw function.  This is similar to the function
// from Adafruit_GFX.c but adapted for this processor.  However, this
// function only uses one call to setAddrWindow(), which allows it to
// run at least twice as fast.
// Requires (11 + size*size*6*8) bytes of transmission (assuming image fully on screen)
// Input: x         horizontal position of the top left corner of the character, columns from the left edge
//        y         vertical position of the top left corner of the character, rows from the top edge
//        c         character to be printed
//        textColor 16-bit color of the character
//        bgColor   16-bit color of the background
//        size      number of pixels per character pixel (e.g. size==2 prints each pixel of font as 2x2 square)
// Output: none
void DrawChar(uint8_t x, uint8_t y, char c, uint16_t textcolor, uint16_t bgcolor, uint8_t size) 
{
	int8_t m, n, i;
	uint8_t shape = 0x14;
	uint16_t newshape = 0x0000;
	uint8_t text_hi, text_low, bg_hi, bg_low;
	for(i = 0; i < 8; i++)
	{
		newshape += (shape << i) & (0x0001 << 2*i) ;
		newshape += (shape << i+1) & (0x0001 << 2*i+1);
	}

	text_hi = textcolor >> 8;
	text_low = textcolor & 0xFF;
	bg_hi = bgcolor >> 8;
	bg_low = bgcolor & 0xFF;
	if((x>= _width)||(y >= _height)) return;

	for(m = 0; m < 5; m++){
		setAddrWindow(x+m*size, y, x+(m+1)*size, y+16*size);
		
		for(n = 0; n < 8 ; n++){
			//if(((newshape >> n) & 0x01) == 0x01)
			if(((Font[c][m] >> n) & 0x01) == 0x01)
			//if(((shape >> n) & 0x01) == 0x01)
			//if((n == 5)||(n == 8)||(n == 9))
			{
				for(i = 0; i < 2*size*size; i++){
					writeDC(DATA, text_hi);
					writeDC(DATA, text_low);
				}
			}
			else{
				for(i = 0; i < 2*size*size; i++){
					writeDC(DATA, bg_hi);
					writeDC(DATA, bg_low);
				}
			}
		}
		
	}
	
	setAddrWindow(x+5*size, y, x+6*size, y+16*size);
	for(i = 0; i < 16*size*size; i++){
		writeDC(DATA, bg_hi);
		writeDC(DATA, bg_low);
	}

}

//------------ST7735_DrawCharS------------
// Simple character draw function.  This is the same function from
// Adafruit_GFX.c but adapted for this processor.  However, each call
// to ST7735_DrawPixel() calls setAddrWindow(), which needs to send
// many extra data and commands.  If the background color is the same
// as the text color, no background will be printed, and text can be
// drawn right over existing images without covering them with a box.
// Requires (11 + 2*size*size)*6*8 (image fully on screen; textcolor != bgColor)
// Input: x         horizontal position of the top left corner of the character, columns from the left edge
//        y         vertical position of the top left corner of the character, rows from the top edge
//        c         character to be printed
//        textColor 16-bit color of the character
//        bgColor   16-bit color of the background
//        size      number of pixels per character pixel (e.g. size==2 prints each pixel of font as 2x2 square)
// Output: none


//------------ST7735_DrawString------------
// String draw function.
// 16 rows (0 to 15) and 21 characters (0 to 20)
// Requires (11 + size*size*6*8) bytes of transmission for each character
// Input: x         columns from the left edge (0 to 20)
//        y         rows from the top edge (0 to 15)
//        pt        pointer to a null terminated string to be printed
//        textColor 16-bit color of the characters
// bgColor is Black and size is 1
// Output: number of characters printed
void DrawString(uint8_t x, uint8_t y, char *ptr, uint16_t textcolor, uint16_t bgcolor, uint8_t size) {
	while(*ptr){
		DrawChar(x, y, *ptr++, textcolor, bgcolor, size);  
		x = x + 6;
	}
}



//********ST7735_SetCursor*****************
// Move the cursor to the desired X- and Y-position.  The
// next character will be printed here.  X=0 is the leftmost
// column.  Y=0 is the top row.
// inputs: newX  new X-position of the cursor (0<=newX<=20)
//         newY  new Y-position of the cursor (0<=newY<=15)
// outputs: none
void ST7735_SetCursor(uint8_t x, uint8_t y)
{
	if(!((x<=20)&(y<=15)))
	{return;}
	StX = x;
	StY = y;
}


//------------ST7735_DrawBitmap------------
// Displays a 16-bit color BMP image.  A bitmap file that is created
// by a PC image processing program has a header and may be padded
// with dummy columns so the data have four byte alignment.  This
// function assumes that all of that has been stripped out, and the
// array image[] has one 16-bit halfword for each pixel to be
// displayed on the screen (encoded in reverse order, which is
// standard for bitmap files).  An array can be created in this
// format from a 24-bit-per-pixel .bmp file using the associated
// converter program.
// (x,y) is the screen location of the lower left corner of BMP image
// Requires (11 + 2*w*h) bytes of transmission (assuming image fully on screen)
// Input: x     horizontal position of the bottom left corner of the image, columns from the left edge
//        y     vertical position of the bottom left corner of the image, rows from the top edge
//        image pointer to a 16-bit color BMP image
//        w     number of pixels wide
//        h     number of pixels tall
// Output: none
// Must be less than or equal to 128 pixels wide by 160 pixels high




//-----------------------ST7735_OutUDec-----------------------
// Output a 32-bit number in unsigned decimal format
// Position determined by ST7735_SetCursor command
// Color set by ST7735_SetTextColor
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1-10 digits with no space before or after




//------------ST7735_SetRotation------------
// Change the image rotation.
// Requires 2 bytes of transmission
// Input: m new rotation value (0 to 3)
// Output: none



// *************** ST7735_PlotClear ********************
// Clear the graphics buffer, set X coordinate to 0
// This routine clears the display
// Inputs: ymin and ymax are range of the plot
// Outputs: none
void ST7735_PlotClear(uint8_t ymin, uint8_t ymax)
{
	FillRect(0, ymin, _width, ymax, 0xFFFF);
	ST7735_SetCursor(0, ymin);
	
}



// *************** ST7735_PlotPoint ********************
// Used in the voltage versus time plot, plot one point at y
// It does output to display
// Inputs: y is the y coordinate of the point plotted
// Outputs: none





// *************** ST7735_PlotLine ********************
// Used in the voltage versus time plot, plot line to new point
// It does output to display
// Inputs: y is the y coordinate of the point plotted
// Outputs: none




// *************** ST7735_PlotPoints ********************
// Used in the voltage versus time plot, plot two points at y1, y2
// It does output to display
// Inputs: y1 is the y coordinate of the first point plotted
//         y2 is the y coordinate of the second point plotted
// Outputs: none




// *************** ST7735_PlotBar ********************
// Used in the voltage versus time bar, plot one bar at y
// It does not output to display until RIT128x96x4ShowPlot called
// Inputs: y is the y coordinate of the bar plotted
// Outputs: none


// *************** ST7735_PlotdBfs ********************
// Used in the amplitude versus frequency plot, plot bar point at y
// 0 to 0.625V scaled on a log plot from min to max
// It does output to display
// Inputs: y is the y ADC value of the bar plotted
// Outputs: none



// *************** ST7735_PlotNext ********************
// Used in all the plots to step the X coordinate one pixel
// X steps from 0 to 127, then back to 0 again
// It does not output to display
// Inputs: none
// Outputs: none




// *************** ST7735_PlotNextErase ********************
// Used in all the plots to step the X coordinate one pixel
// X steps from 0 to 127, then back to 0 again
// It clears the vertical space into which the next pixel will be drawn
// Inputs: none
// Outputs: none


// *************** ST7735_OutChar ********************
// Output one character to the LCD
// Position determined by ST7735_SetCursor command
// Color set by ST7735_SetTextColor
// Inputs: 8-bit ASCII character
// Outputs: none



//********ST7735_OutString*****************
// Print a string of characters to the ST7735 LCD.
// Position determined by ST7735_SetCursor command
// Color set by ST7735_SetTextColor
// The string will not automatically wrap.
// inputs: ptr  pointer to NULL-terminated ASCII string
// outputs: none


// ************** ST7735_SetTextColor ************************
// Sets the color in which the characters will be printed
// Background color is fixed at black
// Input:  16-bit packed color
// Output: none
// ********************************************************

