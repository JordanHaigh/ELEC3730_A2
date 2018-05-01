//     $Date: 2018-03-26 08:32:18 +1100 (Mon, 26 Mar 2018) $
// $Revision: 1217 $
//   $Author: Peter $

#include "Ass-02.h"
//#include "Ass-02-Q01.h"
#include <math.h>
#include <stdlib.h>


#ifdef STM32F407xx
#include "usart.h"
#endif



uint8_t debugOn = 0;
int wordCount = 0;
char **array_of_words;
char* newString;
int length = 10;
int stringIndex = 0;
uint8_t buildInputStringFirstTime = 1;


void buildInputString(uint8_t c);
int string_parser(char *inp, char **array_of_words_p[]);
void analyseKeywords(uint8_t argNum, char* argStrings[]);

//uint8_t checkForNumericArgument(uint8_t processingIntegerFlag);
//uint8_t checkArgumentLength(uint8_t flag, uint8_t expectedWordCount);

uint8_t checkForNumericArgument2(uint8_t processingIntegerFlag, uint8_t argNum, char* argStrings[]);
uint8_t checkArgumentLength2(uint8_t flag, uint8_t expectedWordCount, uint8_t argNum);

float validateAddition(uint8_t argNum, char* argStrings[]);
float addNumbers(uint8_t argNum, char* argStrings[]);
float validateSubtraction(uint8_t argNum, char* argStrings[]);
float subNumbers(uint8_t argNum, char* argStrings[]);
float validateMultiplication(uint8_t argNum, char* argStrings[]);
float mulNumbers(uint8_t argNum, char* argStrings[]);
float validateDivision(uint8_t argNum, char* argStrings[]);
float divNumbers(uint8_t argNum, char* argStrings[]);

void debugMode(uint8_t argNum, char* argStrings[]);
void helpDesk(uint8_t argNum, char* argStrings[]);

float validateAndRunRoot(uint8_t flag, uint8_t argNum, char* argStrings[]);
float validateSquareRoot(uint8_t argNum, char* argStrings[]);
float validateCubeRoot(uint8_t argNum, char* argStrings[]);
float squareRoot(char* argStrings[]);
float cubeRoot(char* argStrings[]);
float validatePower(uint8_t argNum, char* argStrings[]);
float power(char* argStrings[]);
float validateModulo(uint8_t argNum, char* argStrings[]);
int modulo(char* argStrings[]);



void printArrayOfWords()
{
	if (wordCount != 0) {
		printf("count = %d\n", wordCount);
		for (int j=0;j<wordCount;j++) {
		  printf("Word(%d): '%s'\n", j+1, (array_of_words)[j]);
		}
	  }
	else
		printf("No words found\n");
}


void freeEverything()
{
	free(newString);
	free(array_of_words);
}



typedef struct
{
	char* nameString; //command string
	float (*function_p)(uint8_t argNum, char* argStrings[]); //function pointer. todo make sure this is right for evan to use (is it necessary?)
	char* helpString; //help message
} command_s;

const command_s commandList[] = {
		{"add",  &validateAddition, "add <num 1> .. <num N>\n"},
		{"sub",  &validateSubtraction, "sub <num 1> <num 2>\n"},
		{"mul",  &validateMultiplication, "mul <num 1> .. <num N>\n"},
		{"div",  &validateDivision, "div <num 1> <num 2>\n"},
//		{"sqrt", &validateSquareRoot, "sqrt <num1> : Finds the square root of a floating point number.\n"},
//		{"cbrt", &validateCubeRoot,"cbrt <num1> : Finds the cube root of a floating point number.\n"},
//		{"pow",  &validatePower,"pow <num1> <num2> : Finds the power of the num1 to the power of num2.\n"},
//		{"mod",  &validateModulo,"mod <num1> <num2>: Finds the modulo of two INTEGERS.\n"},
//		{"debug",&debugMode,"debug <on|off> : Turn debug messages on or off.\n"},
//		{"help", &helpDesk,"help [command] : Prints help information for a command\n"},
		{NULL, NULL, NULL}

};

void analyseKeywords(uint8_t argNum, char* argStrings[])
{

	if(argNum == 0)
	{
		printf("Error. No input. Seek help.\n");
		return;
	}

	char* firstKeyword  = (argStrings)[0];

	//must be either add,sub,mul,div
	if(firstKeyword[0] == 'a' && firstKeyword[1] == 'd' && firstKeyword[2] == 'd' && firstKeyword[3] == '\0')
		validateAddition(argNum, argStrings);
	else if(firstKeyword[0] == 's' && firstKeyword[1] == 'u' && firstKeyword[2] == 'b' && firstKeyword[3] == '\0')
		validateSubtraction(argNum, argStrings);
	else if(firstKeyword[0] == 'm' && firstKeyword[1] == 'u' && firstKeyword[2] == 'l' && firstKeyword[3] == '\0')
		validateMultiplication(argNum, argStrings);
	else if(firstKeyword[0] == 'd' && firstKeyword[1] == 'i' && firstKeyword[2] == 'v' && firstKeyword[3] == '\0')
		validateDivision(argNum, argStrings);
	else if(firstKeyword[0] == 'h' && firstKeyword[1] == 'e' && firstKeyword[2] == 'l' && firstKeyword[3] == 'p' && firstKeyword[4] == '\0')
			helpDesk(argNum,argStrings);
	else if(firstKeyword[0] == 'd' && firstKeyword[1] == 'e' && firstKeyword[2] == 'b' && firstKeyword[3] == 'u' && firstKeyword[4] == 'g' && firstKeyword[5] == '\0')
		debugMode(argNum,argStrings);



	//EXTRA FUNCTIONALITY
	else if(firstKeyword[0] == 's' && firstKeyword[1] == 'q' && firstKeyword[2] == 'r' && firstKeyword[3] == 't' && firstKeyword[4] == '\0')
		validateAndRunRoot(0, argNum, argStrings);
	else if(firstKeyword[0] == 'c' && firstKeyword[1] == 'b' && firstKeyword[2] == 'r' && firstKeyword[3] == 't' && firstKeyword[4] == '\0')
		validateAndRunRoot(1,argNum, argStrings);
	else if(firstKeyword[0] == 'p' && firstKeyword[1] == 'o' && firstKeyword[2] == 'w' && firstKeyword[3] == '\0')
		validatePower(argNum, argStrings);
	else if(firstKeyword[0] == 'm' && firstKeyword[1] == 'o' && firstKeyword[2] == 'd' && firstKeyword[3] == '\0')
		validateModulo(argNum, argStrings);


	else
		printf("Error. Unrecognised command. Seek help.\n");

}


float validateSquareRoot(uint8_t argNum, char* argStrings[])
{
	return validateAndRunRoot(0,argNum,argStrings);
}

float validateCubeRoot(uint8_t argNum, char* argStrings[])
{
	return validateAndRunRoot(1,argNum,argStrings);
}

float validateAndRunRoot(uint8_t flag, uint8_t argNum, char* argStrings[])
{
	float result = 0; //sentinel
	if(checkArgumentLength2(1,2, argNum) == 0)
		printf("Error. Must contain only one number for Root.\n");
	else
	{
		if(checkForNumericArgument2(0,argNum, argStrings) == 1)
		{
			//float result;
			if(flag == 0) //sqrt
				result = squareRoot(argStrings);
			else //cbrt
				result = cubeRoot(argStrings);

			printf("Result: %.2f\n", result);
		}
	}

	return result;
}

float squareRoot(char* argStrings[])
{
	float result = atof(argStrings[1]);
	return sqrtf(result);
}

float cubeRoot(char* argStrings[])
{
	return cbrtf(atof(argStrings[1]));
}

float validatePower(uint8_t argNum, char* argStrings[])
{
	float result = 0;
 	if(checkArgumentLength2(1,3, argNum) == 0)
	{
			printf("Error. Must contain only two numbers for Powers. ");
			(argNum > 3) ? printf("No one man should have all that power.\n") : printf("\n");
	}
	else
	{
		if(checkForNumericArgument2(0,argNum, argStrings) == 1)
		{

			result = power(argStrings);
			printf("Result: %.2f\n", result);
		}
	}
 	return result;
}

float power(char* argStrings[])
{
	return powf(atof(argStrings[1]),atof(argStrings[2]));
}

float validateModulo(uint8_t argNum, char* argStrings[])
{
	float result = 0;
	if(checkArgumentLength2(1,3,argNum) == 0)
		printf("Error. Must contain only two integers for Modulo.\n");
	else
	{
		if(checkForNumericArgument2(1,argNum,argStrings) == 1)
		{
			result = modulo(argStrings);
			printf("Result: %.2f\n", result);
		}
	}
	return result;
}

int modulo(char* argStrings[])
{
	return (int)atof(argStrings[1]) % (int)atof(argStrings[2]);
}

uint8_t checkArgumentLength2(uint8_t flag, uint8_t expectedWordCount, uint8_t argNum)
{
	if(argNum == 1)
		return 0;

	if(flag == 0) //used for addition and subtraction. N number of arguments supported
		return 1;
	else //flag must be 1. flag is used in the operations where a number of arguments is specified, i.e sub, div, sqrt, sbrt, mod, pow
	{
		if(argNum < expectedWordCount || argNum > expectedWordCount)
			return 0;
		else
			return 1;

	}
}



uint8_t checkForNumericArgument2(uint8_t processingIntegerFlag,uint8_t argNum, char* argStrings[])
{
	uint8_t foundDecimalPoint = 0;

	for(int i = 1; i < argNum; i++)
	{

		for(int j = 0; argStrings[i][j] != '\0'; j++) //check each letter in the word
		{
			if(argStrings[i][j] == '.')
			{ //searching for decimal point

				if(foundDecimalPoint == 0)
					foundDecimalPoint = 1;
				else
				{
					printf("Error. Not a valid input\n");
					return 0;
				}
			}

			else if(!(argStrings[i][j] >= 48 && argStrings[i][j] <= 57)) //if the number is not within 0-9 (non numeric)
			{
				printf("Error. Not a valid input\n");
				return 0;
			}

			else if(processingIntegerFlag == 1 && foundDecimalPoint == 1 && (argStrings[i][j] >= 48 && argStrings[i][j] <= 57)) //if a decimal point is found and numerals trail it, it must be a decimal
			{
				printf("Error. Not a valid Input\n");
				return 0;
			}

		}
		//new word, reset decimal point
		foundDecimalPoint = 0;
	}

	return 1;
}

float validateAddition(uint8_t argNum, char* argStrings[])
{
	float result = 0;
	if(checkArgumentLength2(0,99, argNum) == 0)
		printf("Error. Must contain one or more numbers for addition.\n");
	else
	{
		if(checkForNumericArgument2(0,argNum,argStrings) == 1)
		{
			result = addNumbers(argNum, argStrings);
			printf("Result: %.2f\n", result);
		}
	}
	return result;
}


float addNumbers(uint8_t argNum, char* argStrings[])
{

	float currentSum = 0;

	for(int i = 1; i < argNum; i++)
	{
		float stringToFloat = atof(argStrings[i]);
		currentSum += stringToFloat;
	}

	return currentSum;
}

float validateSubtraction(uint8_t argNum, char* argStrings[])
{
	float result = 0;
	if(checkArgumentLength2(1,3,argNum) == 0)
		printf("Error. Must contain exactly two numbers for subtraction\n");
	else
	{
		if(checkForNumericArgument2(0,argNum, argStrings) == 1)
		{
			result = subNumbers(argNum, argStrings);
			printf("Result: %.2f\n", result);
		}
	}
	return result;
}

float subNumbers(uint8_t argNum, char* argStrings[])
{
	float currentSub = 0;
	uint8_t firstRun = 1;
	for(int i = 1; i < argNum; i++)
	{
		float stringToFloat = atof((argStrings)[i]);

		if(firstRun == 1)
		{
			currentSub = stringToFloat;
			firstRun = 0;
		}
		else
			currentSub -= stringToFloat;
	}

	return currentSub;

}

float validateMultiplication(uint8_t argNum, char* argStrings[])
{
	float result = 0;
	if(checkArgumentLength2(0,99,argNum) == 0)
		printf("Error. Must contain one or more numbers for multiplication.\n");
	else
	{
		if(checkForNumericArgument2(0,argNum,argStrings) == 1)
		{
			result = mulNumbers(argNum, argStrings);
			printf("Result: %.2f\n", result);
		}

	}
	return result;
}

float mulNumbers(uint8_t argNum, char* argStrings[])
{
	float currentMul = 0;
	uint8_t firstTime = 1;
	for(int i = 1; i < argNum; i++)
	{
		float stringToFloat = atof((argStrings)[i]);

		if(firstTime)
		{
			currentMul = stringToFloat;
			firstTime = 0;

		}
		else
			currentMul *= stringToFloat;
	}

	return currentMul;
}

float validateDivision(uint8_t argNum, char* argStrings[])
{
	float result = 0;
	if(checkArgumentLength2(1,3,argNum) == 0)
		printf("Error. Must contain exactly two numbers for division\n");
	else
	{
		if(checkForNumericArgument2(0,argNum,argStrings) == 1)
		{
			result = divNumbers(argNum,argStrings);
			printf("Result: %.2f\n", result);
		}
	}
	return result;
}

float divNumbers(uint8_t argNum, char* argStrings[])
{
	float currentDiv = 0;
	uint8_t firstRun = 1;
	for(int i = 1; i < argNum; i++)
	{
		float stringToFloat = atof((argStrings)[i]);
		if(stringToFloat == 0.0)
		{
			return 0.0; //it was always going to be a 0.0 result regardless
		}

		if(firstRun == 1)
		{
			currentDiv = stringToFloat;
			firstRun = 0;
		}
		else
			currentDiv /= stringToFloat;
	}

	return currentDiv;

}


void debugMode(uint8_t argNum, char* argStrings[])
{
	if(argNum < 2)
	{
		printf("##Extended Functionality## \t\t Debug status => %d\n", debugOn);
	}
	else if(argNum > 2)
	{
		printf("Error. Debug command must take zero or one argument. Seek help.\n");
		return;
	}
	else
	{
		char * secondKeyword =  (argStrings)[1];
		if(secondKeyword[0] == 'o' && secondKeyword[1] == 'n' && secondKeyword[2] == '\0')
		{
			if(debugOn == 1)
				printf("Debug is already on.\n");
			else
			{
				debugOn = 1;
				printf("Debug message will be displayed\n");
			}
		}
		else if(secondKeyword[0] == 'o' && secondKeyword[1] == 'f' && secondKeyword[2] == 'f' && secondKeyword[3] == '\0')
		{
			if(debugOn == 0)
				printf("Debug is already off.\n");
			else
			{
				debugOn = 0;
				printf("Debug message will not be displayed");
			}
		}
		else
			printf("Error. Not a valid input for debug keyword. Seek help.");
	}


}

void helpDesk(uint8_t argNum, char* argStrings[])
{
	if(argNum < 2) {
		printf("add <num1> ... <num N> : Sum one or more numbers.\n");
		printf("sub <num1> <num 2> : Subtract two numbers.\n");
		printf("mul <num1> ... <num N> : Multiply one or more numbers.\n");
		printf("div <num1> <num2> : Divide two numbers.\n");
		printf("debug <on|off> : Turn debug messages on or off.\n");
		printf("help [command] : Prints help information for a command\n");

		printf("\n\n##### EXTRA FUNCTIONS #####\n");
		printf("sqrt <num1> : Finds the square root of a floating point number.\n");
		printf("cbrt <num1> : Finds the cube root of a floating point number.\n");
		printf("pow <num1> <num2> : Finds the power of the num1 to the power of num2.\n");
		printf("mod <num1> <num2>: Finds the modulo of two INTEGERS.\n");

	}
	else if(argNum == 2)	{
		char * secondKeyword =  (argStrings)[1];

			if(secondKeyword[0] == 'a' && secondKeyword[1] == 'd' && secondKeyword[2] == 'd' && secondKeyword[3] == '\0')
				printf("add <num1> ... <num N> : Sum one or more numbers.\n");

			else if(secondKeyword[0] == 's' && secondKeyword[1] == 'u' && secondKeyword[2] == 'b' && secondKeyword[3] == '\0')
				printf("sub <num1> <num 2> : Subtract two numbers.\n");

			else if(secondKeyword[0] == 'm' && secondKeyword[1] == 'u' && secondKeyword[2] == 'l' && secondKeyword[3] == '\0')
				printf("mul <num1> ... <num N> : Multiply one or more numbers.\n");

			else if(secondKeyword[0] == 'd' && secondKeyword[1] == 'i' && secondKeyword[2] == 'v' && secondKeyword[3] == '\0')
				printf("div <num1> <num2> : Divide two numbers.\n");
			else if(secondKeyword[0] == 'h' && secondKeyword[1] == 'e' && secondKeyword[2] == 'l' && secondKeyword[3] == 'p' && secondKeyword[4] == '\0')
				printf("help [command] : Prints help information for a command\n");
			else if(secondKeyword[0] == 'd' && secondKeyword[1] == 'e' && secondKeyword[2] == 'b' && secondKeyword[3] == 'u' && secondKeyword[4] == 'g' && secondKeyword[5] == '\0')
				printf("debug <on|off> : Turn debug messages on or off.\n");


			else if(secondKeyword[0] == 's' && secondKeyword[1] == 'q' && secondKeyword[2] == 'r' && secondKeyword[3] == 't' && secondKeyword[4] == '\0')
				printf("sqrt <num1> : Finds the square root of a floating point number.\n");
			else if(secondKeyword[0] == 'c' && secondKeyword[1] == 'b' && secondKeyword[2] == 'r' && secondKeyword[3] == 't' && secondKeyword[4] == '\0')
				printf("cbrt <num1> : Finds the cube root of a floating point number.\n");
			else if(secondKeyword[0] == 'p' && secondKeyword[1] == 'o' && secondKeyword[2] == 'w' && secondKeyword[3] == '\0')
				printf("pow <num1> <num2> : Finds the power of the num1 to the power of num2.\n");
			else if(secondKeyword[0] == 'm' && secondKeyword[1] == 'o' && secondKeyword[2] == 'd' && secondKeyword[3] == '\0')
				printf("mod <num1> <num2>: Finds the modulo of two INTEGERS.\n");


			else
				printf("Error. Unrecognised command. Seek help.\n");
	}
	else
		printf("Error. Unrecognised command. Seek help\n");
}


void CommandLineParserInit(void)
{
  // Print welcome message
//  printf("\014"); //this is a form feed
  printf("ELEC3730 Assignment 2\n");
//  printf("Command Line Parser Example\n");
}


void CommandLineParserProcess(void)
{
  uint8_t c;


  // Check for input and echo back
#ifdef STM32F407xx
  if (HAL_UART_Receive(&huart2, &c, 1, 0x0) == HAL_OK) //code for running through stm board
  {
    HAL_GPIO_TogglePin(GPIOD, LD4_Pin); // Toggle LED4

    	char c;
    	int i;
        char command_line[101];

    	// Get one line of input
    	printf("--> Question 1 - Enter text:\n");
    	i=0;
    	c=getchar();
    	while (c != 13 && i < 100)
    	{
    		printf("%c",c);
    		command_line[i]=c;
    		i++;
    	    c=getchar();
    	}
    	printf("\n");
    	command_line[i]=0;

    	// Parse the input and print result
      	wordCount = string_parser(command_line, &array_of_words);
      	if(debugOn == 1) printArrayOfWords();
      	analyseKeywords(wordCount, array_of_words);
      	freeEverything();

  }
#else
  c = getchar();
  //printf("SERIAL: Got '%c' or in ascii '%d'\n", c,c);
  buildInputString(c);

#endif
}






void buildInputString(uint8_t c){
	if(buildInputStringFirstTime == 1)	  {
	  newString = (char*)malloc(sizeof(char*) * length); //start of string. will have reallocation of memory if string is longer
	  buildInputStringFirstTime = 0; //now not the first run through
	  if(c == '\0' || c == 10) {
		  newString[stringIndex] = '\0';

		  wordCount = string_parser(newString, &array_of_words);

		  //printArrayOfWords();
		  //printf("DEBUG: word count -> %d", wordCount);
		  analyseKeywords(wordCount, array_of_words);
		  freeEverything();

		  //printNewString();
		  buildInputStringFirstTime = 1; //reset
		  length = 10;
	  }
	  else{
		  newString[stringIndex] = c;
		  stringIndex++;

	  }

	}
	else{
		newString = (char*)realloc(newString, sizeof(newString)+1);

		if(c == '\0' || c == 10){
		  newString[stringIndex] = '\0';

		  wordCount = string_parser(newString, &array_of_words);
		  if(debugOn == 1) printArrayOfWords();
		  analyseKeywords(wordCount, array_of_words);
		  freeEverything();


		  buildInputStringFirstTime = 1; //reset
		  stringIndex = 0; //reset

		}
		else{
		  newString[stringIndex] = c;
		  stringIndex++;
		  if(stringIndex == length)
		  {
			  length *=2;
			  newString = (char*)realloc(newString,sizeof(char*) * length);
		  }
		}
	}
}

int string_parser(char *inp, char **array_of_words_p[])
{
	/**********************************SETUP**********************************/

	int numberOfWords = 0;
	int currentWordLength = 0;

	//If input is an empty string, returns straight away
	if(inp[0] == '\0')
		return 0;

	int lengthOfInput = 1; //start from 1 for null terminator
	for(int i = 0; inp[i] != '\0';i++)
		lengthOfInput++;

	//Allocate memory, take into account backspaces for now
	char* convertedInput = (char*)malloc((size_t)lengthOfInput * sizeof(char)); //null terminator

	/**********************************REMOVING BACKSPACES**********************************/
	int reader = 0; //Reading inp variable
	int writer = 0; //Writing to converted Input
	while(inp[reader] != '\0') {
		if(inp[reader] == 127 || inp[reader] == 8) { //backspace char or delete char
			if(writer > 0) //taken into account so the writer isn't being subtracted to a negative index
				writer--;
		}
		else {
			convertedInput[writer] = inp[reader]; //store char from reader into writer
			writer++;
		}
		reader++; //always incrementing writer
	}
	convertedInput[writer] = '\0'; //add null terminator


	/**********************************FIND NUMBER OF WORDS**********************************/
	for(int i = 0; convertedInput[i] != '\0'; i++) { //Iterate until end of the input string
		if(convertedInput[i] != ' ') {
			currentWordLength++;

			//Found a valid character
			if(convertedInput[i+1] == '\0') { //If the next char is the end of the file
				//This gets around the edge case of the for loop stopping when it reaches '\0'
				if(currentWordLength > 0)
					numberOfWords++;
				currentWordLength = 0; //Reset
				continue;
			}

		}
		else {
			//Invalid Character
			if(convertedInput[i-1] == ' ' || convertedInput[i-1] == 127 || convertedInput[i-1] == 8)
				continue;
			else {
				//word is finished
				if(currentWordLength > 0)
					numberOfWords++;
				currentWordLength = 0;
			}
		}
	}

	//allocate single char pointers for number of words
	(*array_of_words_p) = (char**)malloc(sizeof(char*) * (size_t)numberOfWords);


	/**********************************FIND NUMBER OF WORDS**********************************/
	//Reset variables
	currentWordLength = 0;
	int wordIndex = 0; //Used in array indexes
	for(int i = 0; convertedInput[i] != '\0'; i++) {
		if(convertedInput[i] != ' ') { 	//not a space - valid character
			currentWordLength++;

			if(convertedInput[i+1] == '\0' && currentWordLength > 0) {
				//next character is the end, it would not pick up the end of the word
				(*array_of_words_p)[wordIndex] = (char*)malloc(sizeof(char) * (size_t)(currentWordLength + 1)); //Plus 1 is for null terminator
				currentWordLength = 0;
			}
		}
		else {
			//Invalid Character
			if(convertedInput[i-1] == ' ' || convertedInput[i-1] == 127 || convertedInput[i-1] == 8)
				continue;
			else {
				//End of word
				(*array_of_words_p)[wordIndex] = (char*)malloc(sizeof(char) * (size_t)(currentWordLength)+1); //Plus 1 is for null terminator
				wordIndex++;
				currentWordLength = 0;
			}
		}
	}

	/**********************************FIND NUMBER OF LETTERS IN EACH WORD**********************************/
	currentWordLength = 0;
	wordIndex = 0;
	int letterIndex = 0;

	for(int i = 0; convertedInput[i] != '\0'; i++) {
		if(convertedInput[i] != ' ') {
			//valid character
			(*array_of_words_p)[wordIndex][letterIndex] = convertedInput[i];

			letterIndex++;
			if(convertedInput[i+1] == '\0') { //End of string
				(*array_of_words_p)[wordIndex][letterIndex] = '\0';
				wordIndex++;
				letterIndex = 0;
			}
		}
		else {
			//Invalid Character
			if(convertedInput[i-1] == ' ' || convertedInput[i-1] == 127 || convertedInput[i-1] == 8)
				continue;
			else
			{
				//reached end of word
				(*array_of_words_p)[wordIndex][letterIndex] = '\0';
				wordIndex++;
				letterIndex = 0;
			}
		}
	}

	//free the converted input as we do not need it anymore
	free(convertedInput);

	return numberOfWords;
}
