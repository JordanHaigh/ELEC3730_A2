/**
 * ELEC3730 ASSIGNMENT 2
 * QUESTION 2 - CALCULATOR INTERFACE
 * JORDAN HAIGH AND EVAN GRESHAM
 *
 * File takes in user input either from STM32 (Putty) or command line
 * Utilises keywords to perform mathematical functions
 * */
#include "Ass-02.h"

#include "Ass-02-Q01.h"

#include <stdlib.h>

//Button struct necessary for multiple buttons - Form of encapsulating
typedef struct {
	  int startX;
	  int startY;
	  int width;
	  int height;
	  char* text;
	  int id;
}Button;


static Button buttons[20]; //Needed for many loops of calculator
//Text array used for buttons - determined by button id
char *textArray[] = {"7","8","9","+","-","4","5","6","/","x","1","2","3","del","+-","0",".","clr","ans","="};

char* inputString; //input string from user
char* outputString; //output displayed to interface

static int inputStringIndex = 0; //current position of index of string
static int decimalPointPlaced = 0; //limiter for decimal point
static int firstTime = 1; //Used in analyse touch
static int equalsPressed = 0; //Used when outputting to screen that will result will stay till user input again


void CalculatorInit(void);
void CalculatorProcess(void);

Button buildButton(int x, int y, int w, int h, int id);
int buttonHere(int x, int y, Button button);
void showButton(Button button);
void buttonToString(Button button);

void analyseTouch(Button currentButtonPressed);
void concatenateButtonText(char* buttonText);
//double doEquals();
double doEquals2();

int isOperator(char);
int maxSize = 20;
#define MAXINPUTLENGTH 26
static int currentInputLength = 0;

double compute(char operator,char* leftNum,char* rightNum);
char answer[50]; //Getting around malloc bullshit - Other demonstrator (He took over from Lyall for Fri 2-4pm Week 8)
//instructed us to use static arrays rather than malloc for our answers


/**************************Calculator Methods********************************/

/*
 * Initialise calculator methods
 * Set up touch display and begin calibration
 * build buttons and display to screen
 * */
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

  if(debugOn ==1)printf("width = %d\n", (int)BSP_LCD_GetXSize());
  if(debugOn ==1)printf("height = %d\n", (int)BSP_LCD_GetYSize());

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
  //Display outputstring in bar at top
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_DisplayStringAt(20,40,"0" ,LEFT_MODE); //init calculator with 0
  strcpy(answer,"0");
}


/*
 * Constant looping method to gather information from user touch
 * */
void CalculatorProcess(void)
{
  //Assume horizontal display

  //Variables used for debouncing issue
	//Debouncing is not perfect, but good enough for purpose of assignment
  static uint8_t fingerTouching = 0;
  static uint8_t bounce = 0;
  static uint8_t offBounce = 0;

  //Now determine which button was pressed on touch screen
  if(BSP_TP_GetDisplayPoint(&display) == 0 && fingerTouching == 0)//0 means valid point on LCD. 1 otherwise(bad coordinate or dodgy LCD)
  {
	  bounce++;
	  if(bounce == 40)
	  {
		  //Once bounced for 40, NOW it realises there is a finer press
		  if(debugOn ==1)printf("Finger on..\n");
		  fingerTouching = 1;
		  bounce = 0;

		  Button currentButtonPressed;
		  //initialise text and id for now. gets around warnings thrown in the below printf.
		  //if an actual button is found, it will update the currentButtonPressed
		  currentButtonPressed.text = "OUTPUT BAR- NOT A BUTTON";
		  currentButtonPressed.id = 999;

		  //go through button array and figure out which button was being pressed
		  for(int i = 0; i < 20;i++)
		  {
			  if(buttonHere(display.x, display.y, buttons[i])!= -1)
			  {
				  if(debugOn ==1)printf("Found button..\n");

				  currentButtonPressed = buttons[i];
			  }
		  }

		  if(debugOn ==1)printf("I am touching the '%s'. ID is %d \n" , currentButtonPressed.text, currentButtonPressed.id);
	      if(debugOn ==1)printf("TOUCH:  Got (%3d,%3d)\n", display.x, display.y);

	      if(currentButtonPressed.id != 999)
	      {
	    	  //Button pressed was a valid button and not the output bar
	    	  analyseTouch(currentButtonPressed);

	    	  //Reset the font and display the answer at the top of the screen
	    	  BSP_LCD_SetFont(&Font16);
	      	  BSP_LCD_DisplayStringAt(20,40,inputString ,LEFT_MODE);
	      }
	  }
  }
  else if(BSP_TP_GetDisplayPoint(&display) == 1)
  {
	  //Finger is not touching the screen

	  offBounce++; //new bouncing variable
	  if(offBounce == 25)
	  {
		  fingerTouching = 0;
		  //printf("Finger off..\n");
	  }
  }
}

/**************************Button Methods********************************/

/*
 * Initialise buttons with startx, starty, width and height. allocate text depending on id that is given
 * */
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

/*
 * Determine if a button is being pressed at the x y input coordinates
 * If it is able to determine the button that is being pressed, it will return its id
 * Otherwise return -1
 * */
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

/*
 * Show button on LCD Screen
 * */
void showButton(Button button){

	BSP_LCD_DrawRect(button.startX, button.startY, button.width, button.height);
	BSP_LCD_SetFont(&Font20);
	BSP_LCD_DisplayStringAt(button.startX + button.width/2,button.startY + button.height/2 -8,button.text,CENTER_MODE);
}


/*
 * Print all button information to string
 * */
void buttonToString(Button button)
{
	if(debugOn == 1)
	{
		printf("Debug: \n");
		printf("Button startX: %d\n", button.startX);
		printf("Button startY: %d\n", button.startY);
		printf("Button width: %d\n", button.width);
		printf("Button height: %d\n", button.height);
		printf("Button text: %s\n", button.text);
		printf("Button id: %d\n", button.id);
	}

}


/************************************************************************/

/*
 * Main analysis method
 * Does plethora of checks
 * If input string is blank -
 * 		If decimal point inputted - extends string
* 		If 0-9 inputted - replaced current string with number
* 		If +,-,/,* inputted, extends string
* 		If +- inputted, does nothing
* 		If clr, equals, del inputted - do nothing
* 	If input string not blank
* 		- If decimal point - make sure previous char is not operator
* 		 - if 0-9 extend string
* 		 if  +,-/,* inputted - make sure that previous char is number. if previous char is oeprator, update that operator
 * */
void analyseTouch(Button currentButtonPressed)
{
	if(debugOn ==1)printf("enter analyse touch\n");
	//https://stackoverflow.com/a/6161123
	if(firstTime == 1)
	{
		//printf("1\n");
		inputString = (char*)realloc(inputString, sizeof(char) * maxSize);
		//printf("1\n");
		strcpy(inputString, "0");  //init
		//printf("1\n");
		//printf("input string starts with %s", inputString);
		firstTime = 0;
		//printf("1\n");

	}

	//In the event that we have pressed equals and keep displaying answer until next button press
    if(equalsPressed == 1)
    {
		equalsPressed = 0;
		strcpy(inputString,"0"); //reset input string
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		BSP_LCD_FillRect(1, 1, 318, 78);
		//revert to black text
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    }

	int buttonId = currentButtonPressed.id;

	if(debugOn ==1)printf("Button id is: %d\n", buttonId);

	char* buttonText = currentButtonPressed.text;
	if(debugOn ==1)printf("Button to text is %s\n\n", buttonText);
//
//	printf("sizeof inputstring %d\n", sizeof(inputString));
//	printf("strlen inputString %d\n\n", strlen(inputString));


	///If working with a blank string
	if(strlen(inputString) == 1 && inputString[0] == '0') //Length 1 and only "0"
	{
		//blank string
		if(debugOn ==1)printf("Recognised strlen of 1 and only a 0\n");

		if(strcmp(buttonText,"0") == 0)
		{
			if(debugOn ==1)printf("0 was entered. Nothing to achieve on blank string\n");
			//return;
		}
		if(strcmp(buttonText, "=") == 0)
		{
			if(debugOn ==1)printf("= was entered. Nothing to achieve on blank string\n");
			//return;
		}
		if(strcmp(buttonText, "clr") == 0)
		{
			if(debugOn ==1)printf("clr was entered. Nothing to achieve on blank string\n");
			//return;
		}
		if(strcmp(buttonText, "1") == 0 ||strcmp(buttonText, "2") == 0 || strcmp(buttonText, "3") == 0 ||
				strcmp(buttonText, "4") == 0 ||strcmp(buttonText, "5") == 0 || strcmp(buttonText, "6") == 0 ||
				strcmp(buttonText, "7") == 0 || strcmp(buttonText, "8") == 0 || strcmp(buttonText, "9") == 0)
		{
			//replace current "0" with another number
			if(debugOn ==1)printf("found %s. replacing current 0\n", buttonText);
			strcpy(inputString, buttonText);
		}

		if(strcmp(buttonText, ".") == 0)
		{
			concatenateButtonText(buttonText);
			decimalPointPlaced = 1;


		}
		if(strcmp(buttonText, "+") == 0 || strcmp(buttonText, "-") == 0 ||
				strcmp(buttonText, "/") == 0 || strcmp(buttonText, "x") == 0 || strcmp(buttonText, "^")==0)
		{
			concatenateButtonText(buttonText);
		}

	}

	//Not a blank string and we enter in a number
	else if(strcmp(buttonText, "0") == 0 || strcmp(buttonText, "1") == 0 || strcmp(buttonText, "2") == 0 ||
			strcmp(buttonText, "3") == 0 || strcmp(buttonText, "4") == 0 || strcmp(buttonText, "5") == 0 ||
			strcmp(buttonText, "6") == 0 || strcmp(buttonText, "7") == 0 || strcmp(buttonText, "8") == 0 ||
			strcmp(buttonText, "9") == 0)
	{
		//append to end of string
		concatenateButtonText(buttonText);
	}
	//If decimal point entered
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
			if(debugOn ==1)printf("error. already placed decimal point...\n");
			//return;
		}
	}
	//If operation entered
	else if(strcmp(buttonText, "+") == 0 || strcmp(buttonText, "-") == 0 ||
			strcmp(buttonText, "/") == 0 || strcmp(buttonText, "x") == 0)
	{

		//need to check if last char was operator ornumber
		if(debugOn ==1)printf("found +,-,/,*\n");

		char previousChar = inputString[inputStringIndex];
		if(debugOn ==1)printf("comparing char=> %c",previousChar);
		//check that operator is not placed twice in a row
		if((previousChar == '+' && strcmp(buttonText, "+") == 0) ||
				(previousChar == '-' && strcmp(buttonText, "-") == 0) ||
				(previousChar == '/' && strcmp(buttonText, "/") == 0) ||
				(previousChar == 'x' && strcmp(buttonText, "x") == 0))
		{
			//error cant do that mate
			if(debugOn ==1)printf("Error. cannot have 2 operators in a row\n");
			return;
		}
		//if previous char is operator and new char is operator
		//replace old operator with new operator
		else if((previousChar == '-' || previousChar == '/' || previousChar == 'x') && strcmp(buttonText,"+") == 0)
		{
			inputString[inputStringIndex] = '+';
			decimalPointPlaced = 0;

		}
		else if((previousChar == '+' || previousChar == '/' || previousChar == 'x') && strcmp(buttonText,"-") == 0)
		{
			inputString[inputStringIndex] = '-';
			decimalPointPlaced = 0;

		}
		else if((previousChar == '+' || previousChar == '-' || previousChar == 'x') && strcmp(buttonText,"/") == 0)
		{
			inputString[inputStringIndex] = '/';
			decimalPointPlaced = 0;

		}
		else if((previousChar == '+' || previousChar == '-' || previousChar == '/') && strcmp(buttonText,"x") == 0)
		{
			inputString[inputStringIndex] = 'x';
			decimalPointPlaced = 0;

		}
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
		if(lastChar == '+' || lastChar ==  '-' || lastChar ==  '/' ||  lastChar == 'x')
		{
			if(debugOn ==1)printf("Error. last character is an operator. cannot do equals yet\n");
		}
		else
		{

			double result = doEquals2();

//			strcpy(inputString,"0");
			char resultString [64];
			snprintf(resultString, sizeof(resultString), "%.9lg",result);
			strcpy(inputString, resultString);
			strcpy(answer,resultString);
			//reset variables

			firstTime = 1;
			inputStringIndex = 0;
			decimalPointPlaced = 0;
			BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
			BSP_LCD_FillRect(1, 1, 318, 78);
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
			equalsPressed = 1;
			currentInputLength = 0;
//	      	BSP_LCD_DisplayStringAt(20,40,resultString ,LEFT_MODE);

		}
	}
	else if(strcmp(buttonText, "clr") == 0)
	{
		if(debugOn ==1)printf("in clr with non empty string\n");
		//reset variables
		strcpy(inputString,"0");
		//free(inputString);
		firstTime = 1;
		inputStringIndex = 0;
		decimalPointPlaced = 0;
		currentInputLength = 0;
		//make white rectangle
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		BSP_LCD_FillRect(1, 1, 318, 78);
		//revert to black text
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

	}
	else if(strcmp(buttonText , "+-") == 0)
	{
		if(inputString[inputStringIndex] == '-' && (inputStringIndex ==0  || isOperator(inputString[inputStringIndex-1]))){
			//the minus we are looking at is for negating a number
			//so we need to remove it
			inputString[inputStringIndex] = '\0';
			inputStringIndex -=1;
			BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
			BSP_LCD_FillRect(1, 1, 318, 78);
			//revert to black text
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);


		}else if(!isOperator(inputString[inputStringIndex])){
			//the minus needs to be before a number, therefore it needs to be after an operator(or at the start of the string)
			//so error
			//instead of error we need to continue back until the start of the number then insert it there
			int tempPos = inputStringIndex-1;
			while(tempPos >=0 && !isOperator(inputString[tempPos])){
				if(debugOn ==1)printf("tempPos %d\n", tempPos);
				tempPos-=1;
			}
			if(debugOn ==1)printf("final tempPos %d\n", tempPos);

			if(tempPos >=0 && (tempPos ==0 || isOperator(inputString[tempPos-1]))){//if the number is negative
				//take out minus
				for(int i = tempPos ; i< inputStringIndex; i++){
					inputString[i] = inputString[i+1];
				}
				inputString[inputStringIndex] = '\0';
				inputStringIndex -=1;
				BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
				BSP_LCD_FillRect(1, 1, 318, 78);
				//revert to black text
				BSP_LCD_SetTextColor(LCD_COLOR_BLACK);



			}else{
				//now temp pos is pointing to the position in the array before the current number (hopefully)
				//todo check for size
				//shift everything forward
				for(int i = inputStringIndex; i>tempPos; i--){
					inputString[i+1] = inputString[i];
				}
				inputString[tempPos+1] = '-';
				inputStringIndex +=1;

				inputString[inputStringIndex+1] = '\0';

			}



//			printf("inputStringIndex %d\n", inputStringIndex);
//			printf("previous %c\n", inputString[inputStringIndex]);
//			printf("Error. negative sign needs to be placed before a number\n");
//			return;

		}else{//in every other case we good

			concatenateButtonText("-");


		}

	}
	else if(strcmp(buttonText , "del") == 0){
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		BSP_LCD_FillRect(1, 1, 318, 78);
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

		if(inputStringIndex ==0){
			strcpy(inputString,"0");
			firstTime = 1;
			inputStringIndex = 0;
			decimalPointPlaced = 0;

		}else{
			if(isOperator(inputString[inputStringIndex])){
				//printf("1\n");
				//if an operator is deleted then check if the previous number has a decimal
				int tempPos = inputStringIndex -1;
				while(tempPos >=0 && !isOperator(inputString[tempPos])){
					if(inputString[tempPos] == '.'){
						//printf("2\n");
						decimalPointPlaced = 1; //todo should this be 0?
						break;
					}
					tempPos -=1;

				}

			}

			inputString[inputStringIndex] = '\0';
			inputStringIndex -=1;
			currentInputLength -=1;

		}


	}
	else if(strcmp(buttonText , "ans") == 0){
		concatenateButtonText(buttonText); //todo need to fix this
	}
	else
	{
		if(debugOn ==1)printf("Hey you pressed a button that doesnt have implementation yet\n");
	}

	if(debugOn ==1)printf("Current String=> %s\n", inputString);
}

/*
 * Adds button text to the input string
 * updates size of inputstring if necessary and input string index
 * */
void concatenateButtonText(char* buttonText)
{
	currentInputLength += (int)strlen(buttonText);
	if(currentInputLength > MAXINPUTLENGTH)
	{
		printf("stop. you can't enter any more...\n");
		currentInputLength -=(int)strlen(buttonText);
	}
	else
	{
		if(inputStringIndex + 3 >= maxSize){
			maxSize *= 2;
			inputString = (char*)realloc(inputString, maxSize*sizeof(char));
		}
	//	inputString = realloc(inputString, (inputStringIndex + 2) * sizeof(char));
		strcat(inputString, buttonText);
		inputStringIndex += strlen(buttonText);
		if(debugOn ==1)printf("inputstringindex => %d\n", inputStringIndex);
	}


}

/*
 * Parseing of equation
 *
 * */
double doEquals2(){

	if(debugOn ==1)printf("Entering doEquals2()   inputString %s\n", inputString);
	int numberOfNumbers = 0;
	int numberOfOperators = 0;
	int lookingAtNumber = 0;

	//iterate through entire string and find number of operators
	for(int i = 0; i< (int)strlen(inputString); i++){
		if(isOperator(inputString[i]) && lookingAtNumber!= 0){
			numberOfOperators +=1;
			lookingAtNumber =0;
		}else{
			if(lookingAtNumber == 0){
				numberOfNumbers +=1;
				lookingAtNumber = 1;
			}
		}
	}

	//printf("before");

	//now we can allocate how many numbers we found in the input string
	char numbers[numberOfNumbers][50];
//	free(numbers[0]);
//	free(numbers);

	//printf("before2");

//	numbers = (char*[])calloc(numberOfNumbers,sizeof(char*));


	//printf("between");
	if(numbers == NULL){
		if(debugOn ==1)printf("malloc failedd\n");
		return 0;

	}

	//allocate how many operators we found
	char operators[numberOfOperators];

//	char* operators = (char*)malloc(sizeof(char) * numberOfOperators);
	//printf("after");

	//iterate through input string again, this time determining the indexes of number and operator
	int numbersIndex = 0;
	int operatorIndex = 0;
	int startI = 0;

	for(int i = 0 ;i< (int)strlen(inputString); i++){
		if(isOperator(inputString[i])){
			if(startI <= i-1){
//				printf("before3\n");
//				char tempNumber[50];
//				char* tempNumber = (char*)malloc(sizeof(char) * (i-startI+1));
//				printf("after3\n");

				strncpy(numbers[numbersIndex], &inputString[startI],i-startI);
				numbers[numbersIndex][i-startI] = '\0';
				if(strcmp("ans", numbers[numbersIndex])==0){
					strcpy(numbers[numbersIndex], answer);
				}
				if(strcmp("-ans", numbers[numbersIndex])==0){

					strcpy(numbers[numbersIndex], "-");
					strcat(numbers[numbersIndex], answer);
					if(debugOn ==1)printf("balh %s", numbers[numbersIndex]);
					if(numbers[numbersIndex][1] =='-'){
						if(debugOn ==1)printf("answer %s\n", &answer[1]);
						strcpy(numbers[numbersIndex], &answer[1]);
					}
				}

//				double d;
//				sscanf(tempNumber,"%.9lg",&d);
//				numbers[numbersIndex] = tempNumber;
				numbersIndex +=1;

//				free(tempNumber);
				startI = i+1;

				operators[operatorIndex] = inputString[i];
				operatorIndex +=1;


			}else{//if previous index was an operator as well or this is the first position then this is a minus
			}



		}
	}

	//add the last number
	int i = (int)strlen(inputString);
//	printf("before3\n");
//	char tempNumber[50];
//				char* tempNumber = (char*)malloc(sizeof(char) * (i-startI+1));
//	printf("after3\n");


	//add the current number from the input string to the numbers array
	strncpy(numbers[numbersIndex], &inputString[startI],i-startI);
	numbers[numbersIndex][i-startI] = '\0';
	if(strcmp("ans", numbers[numbersIndex])==0){
		strcpy(numbers[numbersIndex], answer);
	}
	if(strcmp("-ans", numbers[numbersIndex])==0){

		strcpy(numbers[numbersIndex], "-");
		strcat(numbers[numbersIndex], answer);
		if(numbers[numbersIndex][1] =='-'){
			strcpy(numbers[numbersIndex], &answer[1]);
		}
	}

//	tempNumber[i-startI] ='\0';
//				double d;
//				sscanf(tempNumber,"%.9lg",&d);
//	numbers[numbersIndex] = tempNumber;
	numbersIndex +=1;

//	free(tempNumber);


	//print information
	if(debugOn == 1)printf("numberOfNumbers %d  numberOfOperators %d\n",numberOfNumbers, numberOfOperators);
	for(int i = 0 ; i< numberOfNumbers;i++){
		if(debugOn ==1)printf("numbers %d : %s\n", i,numbers[i]);

	}

	for(int i = 0 ; i< numberOfOperators;i++){
		if(debugOn ==1)printf("operators %d : %c\n", i,operators[i]);

	}



	//order of operations. we're not doing brackets
	char operatorOrder[2][2]= {{'x', '/'},{'+', '-'}};

	for(int operatorGroup = 0; operatorGroup < 2; operatorGroup++){//for each operator grouping
		for(int i = 0; i< numberOfOperators;i++){//for each operator in operator list from input string
			for(int specificOperator = 0; specificOperator < 2; specificOperator++ ){//for each operator in the group
				if(operators[i] == operatorOrder[operatorGroup][specificOperator]){
					//if operator found which matches the one we are looking at
					double result = 0;
					char resultString[64]; //better than using malloc. fuck malloc
					result = compute(operators[i],numbers[i], numbers[i+1]);
					snprintf(resultString, sizeof(resultString), "%.9lg",result);
					if(debugOn ==1)printf("left Number: %s \t operator: %c \t right Number: %s \t result: %s \n", numbers[i], operators[i], numbers[i+1], resultString);


					//now we need to repopulate the numbers and operators array
					for(int j = 0; j< numberOfNumbers-1;j++){
						if(j<i){//just copy it across
//							strcpy(newNumbers[j], numbers[j]);

						}else if(j==i){
//							free(numbers[j]);
							strcpy(numbers[j], resultString);
	//							newNumbers[j] = resultString;

						}else{

//							free(numbers[j]);
							strcpy(numbers[j], numbers[j+1]);

//							newNumbers[j] = numbers[j+1];
						}
					}
					numberOfNumbers-=1;
//					numbers = (char**)realloc(sizeof(char*) * numberOfNumbers);
					for(int j = 0; j< numberOfOperators-1;j++){
						if(j<i){//just copy it across
//							strcpy(newNumbers[j], numbers[j]);

						}else if(j>=i){
							operators[j] = operators[j+1];

//							newNumbers[j] = numbers[j+1];
						}
					}

					numberOfOperators -=1;
//					operators = (char*)realloc(sizeof(char) * numberOfOperators);




//
//					char** newNumbers = (char**)malloc(sizeof(char*) * numberOfNumbers-1);
//					char* newOperators = (char*)malloc(sizeof(char) * numberOfOperators-1);
//
//
//					for(int j = 0; j< numberOfNumbers-1;j++){
//						if(j<i){//just copy it across
//							strcpy(newNumbers[j], numbers[j]);
//
//						}else if(j==i){
//							strcpy(newNumbers[j], resultString);
////							newNumbers[j] = resultString;
//
//						}else{
//							newNumbers[j] = numbers[j+1];
//						}
//					}
//
//					numbers = newNumbers;

					if(numberOfOperators ==0){
						double finalResult =0;
						sscanf(numbers[0],"%lg",&finalResult);
						if(debugOn ==1)printf("finalResult %.9lg\n", finalResult );

//						for(int n = 0; n< originalNumberOfNumbers ; n++){
//							free(numbers[n]);
//						}

//						free(numbers[0]);


//						free(numbers);

//						free(operators);
						return finalResult;
					}


					i--;
					break;


				}
			}
		}
	}


	double finalResult =0;
	sscanf(numbers[0],"%lg",&finalResult);
//	for(int n = 0; n< originalNumberOfNumbers ; n++){
//		free(numbers[n]);
//	}

//	free(numbers[0]);
//	free(numbers);
//	free(operators);
	return finalResult;



}

/*
 * Determines which operation to apply to two numbers
 * */
double compute(char operator,char* leftNum,char* rightNum){
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

	double result = 0;
	char* numbers[3] = {NULL,leftNum,rightNum};
	switch(operator){
	case 'x':
		numbers[0] = commandList[2].nameString; //mul [left] [right].
		result = commandList[2].function_p(3,numbers);
		break;
	case '/':
		numbers[0] = commandList[3].nameString;
		result = commandList[3].function_p(3,numbers);
		break;
	case '+':
		numbers[0] = commandList[0].nameString; //printf("about to add\n");
		result = commandList[0].function_p(3,numbers);
		break;
	case '-':
		numbers[0] = commandList[1].nameString;
		result = commandList[1].function_p(3,numbers);
		break;
	}

	return result;
}

/*
 * Determines if char input is an operator or not
 * Return 1 if true
 * 0 if false
 * */
int isOperator(char input ){
	if(input == 'x' || input == '/' || input == '+' || input == '-'){
		return 1;
	}
	return 0;
}
