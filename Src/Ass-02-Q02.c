//     $Date: 2018-03-26 08:32:18 +1100 (Mon, 26 Mar 2018) $
// $Revision: 1217 $
//   $Author: Peter $

#include "Ass-02.h"
//#include "Ass-02-Q01.c"
#include <stdlib.h>

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
char *textArray[] = {"7","8","9","+","-","4","5","6","/","*","1","2","3","sqrt","+-","0",".","clr","pow","="};

char* inputString;
char* outputString;

static int inputStringIndex = 0;
static int decimalPointPlaced = 0;
static int firstTime = 1;


void CalculatorInit(void);
void CalculatorProcess(void);

Button buildButton(int x, int y, int w, int h, int id);
int buttonHere(int x, int y, Button button);
void showButton(Button button);
void buttonToString(Button button);

void analyseTouch(Button currentButtonPressed);
void concatenateButtonText(char* buttonText);
double doEquals();
int isOperator(char);
int maxSize = 20;

float compute(char operator,char* leftNum,char* rightNum);



/**************************Calculator Methods********************************/


void CalculatorInit(void)
{
  //Assume horizontal display

  // Initialize and turn on LCD and calibrate the touch panel
  BSP_LCD_Init();
  BSP_LCD_DisplayOn();
  BSP_TP_Init();

  // Display welcome message
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

  printf("width = %d\n", (int)BSP_LCD_GetXSize());
  printf("height = %d\n", (int)BSP_LCD_GetYSize());

  //320 240
  //initialise buttons for screen
  for(int i = 0 ; i < 5; i++)
  {
	  for(int j = 0 ; j < 4 ;j++)
	  {
		  buttons[j * 5 + i] = buildButton(64* i, 80 + 40*j, 64,40,j* 5 + i);
		  showButton(buttons[j*5+i]);
		  buttonToString(buttons[j*5+i]);
	  }
  }

  BSP_LCD_DisplayStringAt(20,40,"0" ,LEFT_MODE); //init calculator with 0

//  BSP_LCD_SetFont(&Font24);
}

void CalculatorProcess(void)
{
  //Assume horizontal display

  static uint8_t fingerTouching = 0;
  static uint8_t bounce = 0;
  static uint8_t offBounce = 0;

  //Now determine which button was pressed on touch screen
  if(BSP_TP_GetDisplayPoint(&display) == 0 && fingerTouching == 0)//0 means valid point on LCD. 1 otherwise(bad coordinate or dodgy LCD)
  {
	  //printf("Bouncing..\n");
	  bounce++;
	  if(bounce == 25)
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

	      if(currentButtonPressed.id != 999)
	    	  analyseTouch(currentButtonPressed);

	      	  BSP_LCD_DisplayStringAt(20,40,inputString ,LEFT_MODE);




	  }
  }
  else if(BSP_TP_GetDisplayPoint(&display) == 1)
  {
	  offBounce++;
	  if(offBounce == 25)
	  {
		  fingerTouching = 0;
		  //printf("Finger off..\n");

	  }

  }

}

/**************************Button Methods********************************/

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


/************************************************************************/

void analyseTouch(Button currentButtonPressed)
{
	//https://stackoverflow.com/a/6161123
	if(firstTime == 1)
	{
		inputString = (char*)malloc(sizeof(char) * maxSize);
		strcpy(inputString, "0");  //init


		//printf("input string starts with %s", inputString);
		firstTime = 0;
	}

	int buttonId = currentButtonPressed.id;

	printf("Button id is: %d\n", buttonId);

	char* buttonText = currentButtonPressed.text;
	printf("Button to text is %s\n\n", buttonText);
//
//	printf("sizeof inputstring %d\n", sizeof(inputString));
//	printf("strlen inputString %d\n\n", strlen(inputString));

	if(strlen(inputString) == 1 && inputString[0] == '0') //Length 1 and only "0"
	{
		//blank string
		printf("Recognised strlen of 1 and only a 0\n");

		if(strcmp(buttonText,"0") == 0)
		{
			printf("0 was entered. Nothing to achieve on blank string\n");
			//return;
		}
		if(strcmp(buttonText, "=") == 0)
		{
			printf("= was entered. Nothing to achieve on blank string\n");
			//return;
		}
		if(strcmp(buttonText, "clr") == 0)
		{
			printf("clr was entered. Nothing to achieve on blank string\n");
			//return;
		}
		if(strcmp(buttonText, "1") == 0 ||strcmp(buttonText, "2") == 0 || strcmp(buttonText, "3") == 0 ||
				strcmp(buttonText, "4") == 0 ||strcmp(buttonText, "5") == 0 || strcmp(buttonText, "6") == 0 ||
				strcmp(buttonText, "7") == 0 || strcmp(buttonText, "8") == 0 || strcmp(buttonText, "9") == 0)
		{
			//replace current "0" with another number
			printf("found %s. replacing current 0\n", buttonText);
			strcpy(inputString, buttonText);
		}

		if(strcmp(buttonText, ".") == 0)
		{
			concatenateButtonText(buttonText);
			decimalPointPlaced = 1;


		}
		if(strcmp(buttonText, "+") == 0 || strcmp(buttonText, "-") == 0 ||
				strcmp(buttonText, "/") == 0 || strcmp(buttonText, "*") == 0)
		{
			concatenateButtonText(buttonText);
		}

	}
	else if(strcmp(buttonText, "0") == 0 || strcmp(buttonText, "1") == 0 || strcmp(buttonText, "2") == 0 ||
			strcmp(buttonText, "3") == 0 || strcmp(buttonText, "4") == 0 || strcmp(buttonText, "5") == 0 ||
			strcmp(buttonText, "6") == 0 || strcmp(buttonText, "7") == 0 || strcmp(buttonText, "8") == 0 ||
			strcmp(buttonText, "9") == 0)
	{
		//append to end of string
		concatenateButtonText(buttonText);
	}
	else if(strcmp(buttonText, ".") == 0)
	{
		//make sure there isnt a decimal point already placed
		if(decimalPointPlaced == 0)
		{
			//go for it
			concatenateButtonText(buttonText);
			decimalPointPlaced = 1;
		}
		else
		{
			printf("error. already placed decimal point...\n");
			//return;
		}
	}
	else if(strcmp(buttonText, "+") == 0 || strcmp(buttonText, "-") == 0 ||
			strcmp(buttonText, "/") == 0 || strcmp(buttonText, "*") == 0)
	{
		printf("found +,-,/,*\n");

		char previousChar = inputString[inputStringIndex];
		printf("comparing char=> %c",previousChar);
		//check that operator is not placed twice in a row
		if((previousChar == '+' && strcmp(buttonText, "+") == 0) ||
				(previousChar == '-' && strcmp(buttonText, "-") == 0) ||
				(previousChar == '/' && strcmp(buttonText, "/") == 0) ||
				(previousChar == '*' && strcmp(buttonText, "*") == 0))
		{
			//error cant do that mate
			printf("Error. cannot have 2 operators in a row\n");
			return;
		}
		//if previous char is operator and new char is operator
		//replace old operator with new operator
		else if((previousChar == '-' || previousChar == '/' || previousChar == '*') && strcmp(buttonText,"+") == 0)
		{
			inputString[inputStringIndex] = '+';
			decimalPointPlaced = 0;

		}
		else if((previousChar == '+' || previousChar == '/' || previousChar == '*') && strcmp(buttonText,"-") == 0)
		{
			inputString[inputStringIndex] = '-';
			decimalPointPlaced = 0;

		}
		else if((previousChar == '+' || previousChar == '-' || previousChar == '*') && strcmp(buttonText,"/") == 0)
		{
			inputString[inputStringIndex] = '/';
			decimalPointPlaced = 0;

		}
		else if((previousChar == '+' || previousChar == '-' || previousChar == '/') && strcmp(buttonText,"*") == 0)
		{
			inputString[inputStringIndex] = '*';
			decimalPointPlaced = 0;

		}
		//todo missing functionality for pow, mod sqrt etc.
		else //add like normal
		{
			//last char was not an operator
			//append new operator to string
			//go for it mate
			concatenateButtonText(buttonText);
			decimalPointPlaced = 0;

		}
	}
	else if(strcmp(buttonText, "=") == 0)
	{
		//cannot parse string if there is an operator at the end of string
		char lastChar = inputString[inputStringIndex];
		if(lastChar == '+' || lastChar ==  '-' || lastChar ==  '/' ||  lastChar == '*')
		{
			printf("Error. last character is an operator. cannot do equals yet\n");
		}
		else
		{
			double result = doEquals(); //todo get iplementation for double to string for output
			result ++; //todo not relevant, must be removed before submission
			strcpy(inputString,"0");

			//reset variables

			firstTime = 1;
			inputStringIndex = 0;
			decimalPointPlaced = 0;
			BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
			BSP_LCD_FillRect(1, 1, 318, 78);
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		}
	}
	else if(strcmp(buttonText, "clr") == 0)
	{
		printf("in clr with non empty string\n");
		//reset variables
		strcpy(inputString,"0");
		//free(inputString);
		firstTime = 1;
		inputStringIndex = 0;
		decimalPointPlaced = 0;
		//make white rectangle
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		BSP_LCD_FillRect(1, 1, 318, 78);
		//revert to black text
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

	}
	else
	{
		printf("Hey you pressed a button that doesnt have implementation yet\n");
	}

	printf("Current String=> %s\n", inputString);
}

void concatenateButtonText(char* buttonText)
{


	if(inputStringIndex + 3 >= maxSize){
		maxSize *= 2;
		inputString = (char*)realloc(inputString, maxSize*sizeof(char));
	}
//	inputString = realloc(inputString, (inputStringIndex + 2) * sizeof(char));
	strcat(inputString, buttonText);
	inputStringIndex += strlen(buttonText);
	printf("inputstringindex => %d\n", inputStringIndex);

}

double doEquals()
{
//	printf("input string at equals %s\n", inputString);
	char* newString;
//	printf("input string at equals 2 %s\n", inputString);

	newString = malloc(sizeof(char) * 1);
//	printf("input string at equals 3 %s\n", inputString);

	strcpy(newString, "");  //init
//	printf("input string at equals 4 %s\n", inputString);


	printf("Parsing is kinda implemented\n");
//	printf("input string at equals 5 %s\n", inputString);

	char operators[2][2]= {{'*', '/'},{'+', '-'}};
//	printf("input string at equals 1 %s\n", inputString);

	for(int i = 0 ; i < 2; i++)
	{
		for(int j = 0 ; j<inputStringIndex + 1 ; j++ )
		{
			for(int k = 0 ; k< 2;k++)
			{
				if(operators[i][k] == inputString[j]){
//					printf("input string at 1 %s\n", inputString);

					printf("found operator %c\n" , inputString[j]);
					int leftCounter = 1;
					while(j-leftCounter >= 0 && !isOperator(inputString[j-leftCounter]))
					{
						leftCounter+=1;
					}
//					printf("input string at 2 %s", inputString);

					int rightCounter = 1;
					while(j+rightCounter < inputStringIndex + 1 && !isOperator(inputString[j+rightCounter]))
					{
						rightCounter+=1;
					}
//					printf("input string at 3 %s", inputString);

					printf("Left%d, Right %d, j %d\n", leftCounter, rightCounter , j);


//					printf("inputString before %s\n", inputString);



					char* leftNum = malloc(sizeof(char) * (leftCounter-1));
					strcpy(leftNum, "");  //init
					strncat(leftNum, &inputString[j-leftCounter+1], leftCounter -1);
					char* rightNum = malloc(sizeof(char) * (rightCounter-1));
					strcpy(rightNum, "");  //init
					strncat(rightNum, &inputString[j +1], rightCounter -1);
					printf("inputString %s\n", inputString);
					printf("rightNum %s\n", rightNum);
					printf("leftNum %s\n\n", leftNum);

					float result = compute(operators[i][k],leftNum,rightNum);
					char* resultString;
					strcpy(resultString,"");
					snprintf(resultString, sizeof(float), "%f", result);
					printf("result %f", result);
//
//					char* tempString = (char*)malloc();
//
//
//					strncpy(tempString, &inputString[0], j-leftCounter+1);
//					strcat(tempString, result);
//					strcat(tempString,&inputString[j+rightCounter]);
//					printf("after computation %s",tempString);
//
//
//					strcpy(inputString, tempString);
					free(resultString);
					free(leftNum);
					free(rightNum);



//					char* charToConcat = malloc(sizeof(char)); //free later
//					strcpy(charToConcat,"");
//
//					for(int posCounter =j-leftCounter+1 ; posCounter < j+rightCounter; posCounter++ )
//					{
//						//make temp char array of size 1
//						//todo may need to extend temp char array for items with length > 1
//						//todo or it'll be fine? because we are concatenating single chars to build the new string
//						//todo then from the new string we can derive what operation is being applied.
//						char temp[1];
//						temp[0] = inputString[posCounter];
//						strcat(charToConcat,temp);
//						printf("charTOConcet ==%s \n",charToConcat );
//					}
//
//					newString = realloc(newString, sizeof(newString)+ sizeof(charToConcat));
//					strcat(newString, charToConcat);
//					free(charToConcat);
//
//					printf("newString == %s\n", newString);
				}
			}
		}
	}

	return 1.0; //todo wrong. replace with correct result
}


float compute(char operator,char* leftNum,char* rightNum){
	/**
	*	Used as reference to commandList[]
	*
	*   {"add",  &validateAddition, "add <num 1> .. <num N>\n"},
	*	{"sub",  &validateSubtraction, "sub <num 1> <num 2>\n"},
	*	{"mul",  &validateMultiplication, "mul <num 1> .. <num N>\n"},
	*	{"div",  &validateDivision, "div <num 1> <num 2>\n"},
	*	{"sqrt", &validateSquareRoot, "sqrt <num1> : Finds the square root of a floating point number.\n"},
	*	{"cbrt", &validateCubeRoot,"cbrt <num1> : Finds the cube root of a floating point number.\n"},
	*	{"pow",  &validatePower,"pow <num1> <num2> : Finds the power of the num1 to the power of num2.\n"},
	*	{"mod",  &validateModulo,"mod <num1> <num2>: Finds the modulo of two INTEGERS.\n"},
	*	{"debug",&debugMode,"debug <on|off> : Turn debug messages on or off.\n"},
	*	{"help", &helpDesk,"help [command] : Prints help information for a command\n"},
	*	{NULL, NULL, NULL}
	**/




	//todo use correct names for function
	float result = 0;
	char* numbers[3] = {NULL,leftNum,rightNum};
	switch(operator){
	case '*':
		numbers[0] = "mul";
		//result = mulNumbers(3,numbers);//mul [left] [right].
		break;
	case '/':
		numbers[0] = "div";
		//result = divNumbers(3,numbers);
		break;
	case '+':
		numbers[0] = "add";
		//result = commandList[0].function_p(3,numbers);

//		result = atof(leftNum);
//		printf("leftNum %f\n",atof(leftNum));
//		printf("result -> %f\n", result);
//
//		result += atof(rightNum);
//		printf("result -> %f\n", result);
////		result = addNumbers(2,numbers);
		break;
	case '-':
		//result = subNumbers(2,numbers);
		break;
	}


	return result;


}


int isOperator(char input ){
	if(input == '*' || input == '/' || input == '+' || input == '-'){
		return 1;
	}
	return 0;
}

