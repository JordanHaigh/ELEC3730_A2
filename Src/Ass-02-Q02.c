//     $Date: 2018-03-26 08:32:18 +1100 (Mon, 26 Mar 2018) $
// $Revision: 1217 $
//   $Author: Peter $

#include "Ass-02.h"

#include "Ass-02-Q01.h"

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
char *textArray[] = {"7","8","9","+","-","4","5","6","/","*","1","2","3","sqrt","+-","0",".","clr","^","="};

char* inputString;
char* outputString;

static int inputStringIndex = 0;
static int decimalPointPlaced = 0;
static int firstTime = 1;
static int equalsPressed = 0;


void CalculatorInit(void);
void CalculatorProcess(void);

Button buildButton(int x, int y, int w, int h, int id);
int buttonHere(int x, int y, Button button);
void showButton(Button button);
void buttonToString(Button button);

void analyseTouch(Button currentButtonPressed);
void concatenateButtonText(char* buttonText);
float doEquals();
float doEquals2();

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
	  if(bounce == 40)
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
	      {
	    	  analyseTouch(currentButtonPressed);


	      	  BSP_LCD_DisplayStringAt(20,40,inputString ,LEFT_MODE);



	      }

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

	if(strcmp(button.text, "sqrt") == 0)
		BSP_LCD_SetFont(&Font12);
	else
		BSP_LCD_SetFont(&Font16);



	BSP_LCD_DisplayStringAt(button.startX + button.width/2,button.startY + button.height/2,button.text,CENTER_MODE);

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
	printf("enter analyse touch\n");
	//https://stackoverflow.com/a/6161123
	if(firstTime == 1)
	{
		printf("1\n");
		inputString = (char*)realloc(inputString, sizeof(char) * maxSize);
		printf("1\n");
		strcpy(inputString, "0");  //init
		printf("1\n");
		//printf("input string starts with %s", inputString);
		firstTime = 0;
		printf("1\n");

	}

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
				strcmp(buttonText, "/") == 0 || strcmp(buttonText, "*") == 0 || strcmp(buttonText, "^")==0)
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
			float result = doEquals2();
//			float result = doEquals(); //todo get iplementation for float to string for output
//			result ++; //todo not relevant, must be removed before submission
//			strcpy(inputString,"0");
			char resultString [64];
			snprintf(resultString, sizeof(resultString), "%f",result);
			strcpy(inputString, resultString);
			//reset variables

			firstTime = 1;
			inputStringIndex = 0;
			decimalPointPlaced = 0;
			BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
			BSP_LCD_FillRect(1, 1, 318, 78);
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
			equalsPressed = 1;
//	      	BSP_LCD_DisplayStringAt(20,40,resultString ,LEFT_MODE);

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
	else if(strcmp(buttonText , "+-") == 0)
	{
		if(inputString[inputStringIndex] == '-' && (inputStringIndex ==0  || isOperator(inputString[inputStringIndex-1]))){
			//the minus we are looking at is for negating a number\
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
				printf("tempPos %d\n", tempPos);
				tempPos-=1;
			}
			printf("final tempPos %d\n", tempPos);

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


float doEquals2(){

	printf("Entering doEquals2()   inputString %s\n", inputString);
	int numberOfNumbers = 0;
	int numberOfOperators = 0;
	int lookingAtNumber = 0;
	for(int i = 0; i< (int)strlen(inputString); i++){
		if(isOperator(inputString[i])){
			numberOfOperators +=1;
			lookingAtNumber =0;
		}else{
			if(lookingAtNumber == 0){
				numberOfNumbers +=1;
				lookingAtNumber = 1;
			}
		}
	}

	printf("before");

	char numbers[numberOfNumbers][50];
//	free(numbers[0]);
//	free(numbers);

	printf("before2");

//	numbers = (char*[])calloc(numberOfNumbers,sizeof(char*));


	printf("between");
	if(numbers == NULL){
		printf("malloc failedd\n");
		return 0;

	}
	char operators[numberOfOperators];

//	char* operators = (char*)malloc(sizeof(char) * numberOfOperators);
	printf("after");

	int numbersIndex = 0;
	int operatorIndex = 0;
	int startI = 0;
	for(int i = 0 ;i< (int)strlen(inputString); i++){
		if(isOperator(inputString[i])){
			if(startI <= i-1){
//				printf("before3\n");
				char tempNumber[50];
//				char* tempNumber = (char*)malloc(sizeof(char) * (i-startI+1));
//				printf("after3\n");

				strncpy(numbers[numbersIndex], &inputString[startI],i-startI);
				numbers[numbersIndex][i-startI] = '\0';
//				float d;
//				sscanf(tempNumber,"%f",&d);
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
	printf("after3\n");

	strncpy(numbers[numbersIndex], &inputString[startI],i-startI);
	numbers[numbersIndex][i-startI] = '\0';
//	tempNumber[i-startI] ='\0';
//				float d;
//				sscanf(tempNumber,"%f",&d);
//	numbers[numbersIndex] = tempNumber;
	numbersIndex +=1;

//	free(tempNumber);


	//print stuff
	printf("numberOfNumbers %d  numberOfOperators %d\n",numberOfNumbers, numberOfOperators);
	for(int i = 0 ; i< numberOfNumbers;i++){
		printf("numbers %d : %s\n", i,numbers[i]);

	}

	for(int i = 0 ; i< numberOfOperators;i++){
		printf("operators %d : %c\n", i,operators[i]);

	}



	char operatorOrder[2][2]= {{'*', '/'},{'+', '-'}};

	for(int operatorGroup = 0; operatorGroup < 2; operatorGroup++){//for each operator grouping
		for(int i = 0; i< numberOfOperators;i++){//for each operator in operator list from input string
			for(int specificOperator = 0; specificOperator < 2; specificOperator++ ){//for each operator in the group
				if(operators[i] == operatorOrder[operatorGroup][specificOperator]){
					//if operator found which matches the one we are looking at
					float result = 0;
					char resultString[64];
					result = compute(operators[i],numbers[i], numbers[i+1]);
					snprintf(resultString, sizeof(resultString), "%f",result);
					printf("left Number: %s \t operator: %c \t right Number: %s \t result: %s \n", numbers[i], operators[i], numbers[i+1], resultString);


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
						float finalResult =0;
						sscanf(numbers[0],"%f",&finalResult);
						printf("finalResult %f\n", finalResult );

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


	float finalResult =0;
	sscanf(numbers[0],"%f",&finalResult);
//	for(int n = 0; n< originalNumberOfNumbers ; n++){
//		free(numbers[n]);
//	}

//	free(numbers[0]);
//	free(numbers);
//	free(operators);
	return finalResult;



}


float doEquals()
{
	int done = 0;
	int stringIsAllNumbers = 1;
	int memorySize = 20;
	char* newString;
	newString = malloc(sizeof(char) * 1);
	strcpy(newString, "");  //init
	float result = 0;
	printf("Parsing is kinda implemented\n");

	char operators[2][2]= {{'*', '/'},{'+', '-'}};
	char resultString[64];
//	while(done == 0)
//	{
//		printf("looping...\n");
		for(int i = 0 ; i < 2; i++)
		{
			//for(int j = 0 ; j<inputStringIndex + 1 ; j++ )
			for(int j = 0; j <= (int)strlen(inputString);j++)
			{
				for(int k = 0 ; k< 2;k++)
				{
					if(operators[i][k] == inputString[j])
					{
	//					printf("input string at 1 %s\n", inputString);

						printf("found operator %c\n" , inputString[j]);
						int leftCounter = 1;
						while(j-leftCounter >= 0 && !isOperator(inputString[j-leftCounter]))
						{
							leftCounter+=1;
						}
						if(leftCounter == 1){
							for(int z = j+1; z < (int)strlen(inputString); z++){
								if(isOperator(inputString[z])){
									j = z;
									int leftCounter = 1;
									while(j-leftCounter >= 0 && !isOperator(inputString[j-leftCounter]))
									{
										leftCounter+=1;
									}
									break;


								}

							}

						}
						if(leftCounter ==1){
							return result;

						}

						int rightCounter = 1;

	//					while(j+rightCounter < inputStringIndex + 1 && !isOperator(inputString[j+rightCounter]))
						while(j+rightCounter < (int)strlen(inputString) && !(isOperator(inputString[j+rightCounter]) && rightCounter != 1))
						{
							rightCounter+=1;
						}




	//					printf("input string at 3 %s", inputString);

						printf("Left %d, Right %d, j %d\n", leftCounter, rightCounter , j);


	//					printf("inputString before %s\n", inputString);



						char* leftNum = (char*)malloc(sizeof(char) * (leftCounter-1));
						printf("1");
						strcpy(leftNum, "");  //init
						printf("1");
						strncat(leftNum, &inputString[j-leftCounter+1], leftCounter -1);
						printf("1");
						char* rightNum = (char*)malloc(sizeof(char) * (rightCounter-1));
						printf("1");
						strcpy(rightNum, "");  //init
						printf("1");
						strncat(rightNum, &inputString[j +1], rightCounter -1);

						printf("inputString %s\n", inputString);
						printf("rightNum %s\n", rightNum);
						printf("leftNum %s\n\n", leftNum);

						result = compute(operators[i][k],leftNum,rightNum);
						snprintf(resultString, sizeof(resultString), "%f",result);

						char* fixedString = (char*)malloc(sizeof(char));
						strcpy(fixedString, "");
						int fixedStringIndex = 0;

						for(int z = 0; z <= j-leftCounter;z++)
						{
							fixedString[fixedStringIndex] = inputString[z];
							fixedStringIndex++;
							if(fixedStringIndex == memorySize)
							{
								memorySize *= 2;
								fixedString = (char*)realloc(fixedString,sizeof(char) * memorySize);

							}
						}
						snprintf(resultString, sizeof(resultString), "%f", result);

						for(int z = 0; resultString[z] != '\0';z++)
						{
							fixedString[fixedStringIndex] = resultString[z];
							fixedStringIndex++;
							if(fixedStringIndex == memorySize)
							{
								memorySize *= 2;
								fixedString = (char*)realloc(fixedString,sizeof(char) * memorySize);

							}
						}

						for(int z = j + rightCounter; z < (int)strlen(inputString);z++)
						{
							fixedString[fixedStringIndex] = inputString[z];
							fixedStringIndex++;
							if(fixedStringIndex == memorySize)
							{
								memorySize *= 2;
								fixedString = (char*)realloc(fixedString,sizeof(char) * memorySize);

							}
						}
						if(fixedStringIndex +1 == memorySize){
							fixedString = (char*)realloc(fixedString,sizeof(char)*(memorySize +1));
						}

						fixedString[fixedStringIndex] = '\0';
						fixedStringIndex++;

						printf("Current fixed stirng %s\n", fixedString);
						//string is now fixed
						free(inputString);
						inputString = (char*)malloc(sizeof(char) *(int)strlen(fixedString));
						strcpy(inputString, fixedString);
						free(fixedString);
						free(leftNum);
						free(rightNum);


						int foundOperator = 0;
						for(int z = 0; z < (int)strlen(inputString);z++)
						{
							printf("Current char in inputstring =>%c\n", inputString[z]);
							if(isOperator(inputString[z]) == 1)
							{
								foundOperator = 1; //not done
								break;
							}
						}
						if(foundOperator == 0)//no more operators
						{
							done = 1; //finished
						}
					}
					else
					{
						//printf("found number");
						//need to check if there are any operators in the string
						for(int z = j; z <= (int)strlen(inputString);z++)
						{
							//printf("Current char in inputstring =>%c\n", inputString[z]);
							stringIsAllNumbers = stringIsAllNumbers && (isOperator(inputString[z]) == 0);
						}
						if(stringIsAllNumbers == 1)
						{
							printf("string was all numbers\n");
							done = 1;
							return atof(inputString);

						}
					}
				}
			}
		}
//	}


	printf("Final result is %s", resultString);


	return result;
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
		numbers[0] = commandList[2].nameString;
		result = commandList[2].function_p(3,numbers);
		//result = mulNumbers(3,numbers);//mul [left] [right].
		break;
	case '/':
		numbers[0] = commandList[3].nameString;
		result = commandList[3].function_p(3,numbers);
		//result = divNumbers(3,numbers);
		break;
	case '+':
		numbers[0] = commandList[0].nameString; //printf("about to add\n");
		result = commandList[0].function_p(3,numbers);
		//printf("finished add\n");

//		result = atof(leftNum);
//		printf("leftNum %f\n",atof(leftNum));
//		printf("result -> %f\n", result);
//
//		result += atof(rightNum);
//		printf("result -> %f\n", result);
////		result = addNumbers(2,numbers);
		break;
	case '-':
		numbers[0] = commandList[1].nameString;
		result = commandList[1].function_p(3,numbers);
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
