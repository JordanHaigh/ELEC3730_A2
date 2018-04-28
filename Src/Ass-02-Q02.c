//     $Date: 2018-03-26 08:32:18 +1100 (Mon, 26 Mar 2018) $
// $Revision: 1217 $
//   $Author: Peter $

#include "Ass-02.h"

typedef struct {
	  int startX;
	  int startY;
	  int width;
	  int height;
	  int buttonPressed;
	  char* text;
	  int id;
}Button;

static Button buttons[20];
char *textArray[] = {"7","8","9","+","-","4","5","6","/","x","1","2","3","sqrt","+-","0",".","clr","pow","="};


int buttonHere(int x, int y, Button button);
Button buildButton(int x, int y, int w, int h, int id);
void showButton(Button button);
void buttonToString(Button button);
void CalculatorInit(void);
void CalculatorProcess(void);
//void analyseTouch(Button currentButtonPressed);


//void analyseTouch(Button currentButtonPressed)
//{
//	//0,1,2,  ,  , 5,6,7,  ,  ,10,11,12,  ,  ,  15,  ,  ,  ,  ,
//
//	if(currentButtonPressed.id == 0 ||  //7
//			currentButtonPressed.id == 1 || //8
//			currentButtonPressed.id == 2 || //9
//			currentButtonPressed.id == 5 || //4
//			currentButtonPressed.id == 6 || //5
//			currentButtonPressed.id == 7 ||  //6
//			currentButtonPressed.id == 10 ||//1
//			currentButtonPressed.id == 11 ||//2
//			currentButtonPressed.id == 12 )//3
//	{
//		buildInputString(currentButtonPressed.text);
//	}
//	else if(currentButtonPressed.id == 15) //0 -- Check that 0 is not the first char in the input string. otherwise
//	{
//		buildInputString(currentButtonPressed.text);
//	}
//	else if(currentButtonPressed.id == 16) //decimal point
//	{
//		buildInputString(currentButtonPressed.text);
//	}
//
//
//
//
//	//Check if it is equals, div or times first
//	//display error message saying that there isnt any numbers yet
//
//	//if 0-9, clr, +-
//
//
//
//}
//
//static char *inputString = "0";
//static char *outputString;
//int stringLength = 1;
//void generateInputString(char *str)
//{
//	if(strcmp(str, inputString) == 0)
//	{
//		//when the input string contains only a zero and the string trying to be appended is also 0
//		//do nothing, its still zero
//		return;
//	}
//	if(strcmp(str, ".") == 0)
//	{
//		//append decimal point to next position of string array
//		inputString = (char*)realloc(inputString, sizeof(inputString)+1);
//		//
//	}
//
//
//}
//
//











void CalculatorInit(void)
{
  // STEPIEN: Assume horizontal display

  // Initialize and turn on LCD and calibrate the touch panel
  BSP_LCD_Init();
  BSP_LCD_DisplayOn();
  BSP_TP_Init();

  // Display welcome message
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  //BSP_LCD_DisplayStringAt(5,5, (uint8_t*)"ELEC3730 Assignment 2",LEFT_MODE);
  //BSP_LCD_DisplayStringAt(5,20, (uint8_t*)"Calculator Example",LEFT_MODE);

  printf("width = %d\n", (int)BSP_LCD_GetXSize());
  printf("height = %d\n", (int)BSP_LCD_GetYSize());

  //320 240
    //initialise buttons for screen
  //Button buttons[21]; //originally 20 buttons,extra NULL is added to array for termination
  for(int i = 0 ; i < 5; i++)
  {
	  for(int j = 0 ; j < 4 ;j++)
	  {
		  buttons[j * 5 + i] = buildButton(64* i, 80 + 40*j, 64,40,j* 5 + i);
		  showButton(buttons[j*5+i]);
		  buttonToString(buttons[j*5+i]);
	  }
  }


//  // Create colour choices
//  BSP_LCD_SetTextColor(LCD_COLOR_RED);
//  BSP_LCD_FillRect(5, 200, 30, 30);
//  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
//  BSP_LCD_SetFont(&Font24);
//  BSP_LCD_DisplayChar(290, 205, 'C');
//  BSP_LCD_DrawHLine(  0, 196, 320);
//  BSP_LCD_DrawVLine(  1, 198,  35);
//	BSP_LCD_SetTextColor(LCD_COLOR_ORANGE);


}



void CalculatorProcess(void)
{
  // STEPIEN: Assume horizontal display
  //uint16_t linenum = 0;

  static uint8_t fingerTouching = 0;
  static uint8_t bounce = 0;

  //Now determine which button was pressed on touch screen
  if(BSP_TP_GetDisplayPoint(&display) == 0 && fingerTouching == 0)//0 means valid point on LCD. 1 otherwise(bad coordinate or dodgy LCD)
  {
	  //printf("Bouncing..\n");
	  bounce++;
	  if(bounce == 1)
	  {
		  printf("Finger on..\n");
		  fingerTouching = 1;
		  bounce = 0;

		  Button currentButtonPressed;
		  //initialise text and id for now. gets around warnings thrown in the below printf.
		  //if an actual button is found, it will update the currentButtonPressed
		  currentButtonPressed.text = "OUTPUT BAR- NOT A BUTTON";
		  currentButtonPressed.id = 999;

		  for(int i = 0; i < 20;i++)
		  {
			  if(buttonHere(display.x, display.y, buttons[i])!= -1)
			  {
				  printf("Found button..\n");

				  currentButtonPressed = buttons[i];
			  }
		  }

		  printf("I am touching the '%s'. ID is %d \n" , currentButtonPressed.text, currentButtonPressed.id);
	      printf("TOUCH:  Got (%3d,%3d)\n", display.x, display.y);


	      //todo add in --> analyseTouch(currentButtonPressed);





	  }
  }
  else if(fingerTouching == 1)
  {
	  if(BSP_TP_GetDisplayPoint(&display) == 0)
	  {
		  //still touching
	  }
	  else
	  {
		  fingerTouching = 0;
		  printf("Finger off..\n");
	  }
  }

  //todo implement debouncing - busy waiting doesnt fucking work
}


int buttonHere(int x, int y, Button button)
{
	//returns the button id or -1
	if(button.startX  < x &&
			x < button.startX + button.width &&
			button.startY  < y &&
			y < button.startY + button.height)
		return button.id;
	else
		return -1;
}

Button buildButton(int x , int y, int w, int h, int id){
	Button b;
	b.startX = x;
	b.startY = y;
	b.width = w;
	b.height = h;
	b.id = id;

	b.text = textArray[id];

	return b;
}

void showButton(Button button){
	BSP_LCD_DrawRect(button.startX, button.startY, button.width, button.height);
	BSP_LCD_DisplayStringAt(button.startX + button.width/2,button.startY + button.height/2,button.text,LEFT_MODE);

}

void buttonToString(Button button)
{
	printf("Debug: \n");
	printf("Button startX: %d\n", button.startX);
	printf("Button startY: %d\n", button.startY);
	printf("Button width: %d\n", button.width);
	printf("Button height: %d\n", button.height);
	printf("Button text: %s\n", button.text);
	printf("Button id: %d\n", button.id);
}

