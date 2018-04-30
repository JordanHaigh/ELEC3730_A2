//     $Date: 2018-03-26 08:32:18 +1100 (Mon, 26 Mar 2018) $
// $Revision: 1217 $
//   $Author: Peter $

#include "Ass-02.h"
#include <math.h>
#include <stdlib.h>


#ifdef STM32F407xx
#include "usart.h"
#endif

char* newString;
char **array_of_words;
int wordCount;


int stringIndex = 0;
uint8_t buildInputStringFirstTime = 1;

uint8_t debugOn = 0;

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

void buildInputString(uint8_t c);
int string_parser(char *inp, char **array_of_words_p[]);
void analyseKeywords();
void freeEverything()
{
	free(newString);
	free(array_of_words);
}
uint8_t checkForNumericArgument(uint8_t processingIntegerFlag);
uint8_t checkArgumentLength(uint8_t flag, uint8_t expectedWordCount);

void validateAddition();
float addNumbers();
void validateSubtraction();
float subNumbers();
void validateMultiplication();
float mulNumbers();
void validateDivision();
float divNumbers();

void debugMode();
void helpDesk();

void validateAndRunRoot(uint8_t flag);
float squareRoot();
float cubeRoot();
void validatePower();
float power();
void validateModulo();
int modulo();




void analyseKeywords()
{

	if(wordCount == 0)
	{
		printf("Error. No input. Seek help.\n");
		return;
	}

	char* firstKeyword  = (array_of_words)[0];

	//must be either add,sub,mul,div
	if(firstKeyword[0] == 'a' && firstKeyword[1] == 'd' && firstKeyword[2] == 'd' && firstKeyword[3] == '\0')
		validateAddition();
	else if(firstKeyword[0] == 's' && firstKeyword[1] == 'u' && firstKeyword[2] == 'b' && firstKeyword[3] == '\0')
		validateSubtraction();
	else if(firstKeyword[0] == 'm' && firstKeyword[1] == 'u' && firstKeyword[2] == 'l' && firstKeyword[3] == '\0')
		validateMultiplication();
	else if(firstKeyword[0] == 'd' && firstKeyword[1] == 'i' && firstKeyword[2] == 'v' && firstKeyword[3] == '\0')
		validateDivision();
	else if(firstKeyword[0] == 'h' && firstKeyword[1] == 'e' && firstKeyword[2] == 'l' && firstKeyword[3] == 'p' && firstKeyword[4] == '\0')
			helpDesk();
	else if(firstKeyword[0] == 'd' && firstKeyword[1] == 'e' && firstKeyword[2] == 'b' && firstKeyword[3] == 'u' && firstKeyword[4] == 'g' && firstKeyword[5] == '\0')
		debugMode();



	//EXTRA FUNCTIONALITY
	else if(firstKeyword[0] == 's' && firstKeyword[1] == 'q' && firstKeyword[2] == 'r' && firstKeyword[3] == 't' && firstKeyword[4] == '\0')
		validateAndRunRoot(0);
	else if(firstKeyword[0] == 'c' && firstKeyword[1] == 'b' && firstKeyword[2] == 'r' && firstKeyword[3] == 't' && firstKeyword[4] == '\0')
		validateAndRunRoot(1);
	else if(firstKeyword[0] == 'p' && firstKeyword[1] == 'o' && firstKeyword[2] == 'w' && firstKeyword[3] == '\0')
		validatePower();
	else if(firstKeyword[0] == 'm' && firstKeyword[1] == 'o' && firstKeyword[2] == 'd' && firstKeyword[3] == '\0')
		validateModulo();


	else
		printf("Error. Unrecognised command. Seek help.\n");

}

void validateAndRunRoot(uint8_t flag)
{
	if(checkArgumentLength(1,2) == 0)
		printf("Error. Must contain only one number for Root.\n");
	else
	{
		if(checkForNumericArgument(0) == 1)
		{
			float result;
			if(flag == 0) //sqrt
				result = squareRoot();
			else //cbrt
				result = cubeRoot();

			printf("Result: %.2f\n", result);
		}
	}
}

float squareRoot()
{
	float result = atof(array_of_words[1]);
	return sqrtf(result);
}

float cubeRoot()
{
	return cbrtf(atof(array_of_words[1]));
}

void validatePower()
{
 	if(checkArgumentLength(1,3) == 0)
	{
			printf("Error. Must contain only two numbers for Powers. ");
			(wordCount > 3) ? printf("No one man should have all that power.\n") : printf("\n");
	}
	else
	{
		if(checkForNumericArgument(0) == 1)
		{

			float result = power();
			printf("Result: %.2f\n", result);
		}
	}
}

float power()
{
	return powf(atof(array_of_words[1]),atof(array_of_words[2]));
}

void validateModulo()
{
	if(checkArgumentLength(1,3) == 0)
		printf("Error. Must contain only two integers for Modulo.\n");
	else
	{
		if(checkForNumericArgument(1) == 1)
		{
			float result = modulo();
			printf("Result: %.2f\n", result);
		}
	}
}

int modulo()
{
	return (int)atof(array_of_words[1]) % (int)atof(array_of_words[2]);
}

uint8_t checkArgumentLength(uint8_t flag, uint8_t expectedWordCount)
{
	if(wordCount == 1)
		return 0;

	if(flag == 0) //used for addition and subtraction. N number of arguments supported
		return 1;
	else //flag must be 1. flag is used in the operations where a number of arguments is specified, i.e sub, div, sqrt, sbrt, mod, pow
	{
		if(wordCount < expectedWordCount || wordCount > expectedWordCount)
			return 0;
		else
			return 1;

	}
}




uint8_t checkForNumericArgument(uint8_t processingIntegerFlag)
{
	uint8_t foundDecimalPoint = 0;

	for(int i = 1; i < wordCount; i++)
	{

		for(int j = 0; array_of_words[i][j] != '\0'; j++) //check each letter in the word
		{
			if(array_of_words[i][j] == '.')
			{ //searching for decimal point

				if(foundDecimalPoint == 0)
					foundDecimalPoint = 1;
				else
				{
					printf("Error. Not a valid input\n");
					return 0;
				}
			}

			else if(!(array_of_words[i][j] >= 48 && array_of_words[i][j] <= 57)) //if the number is not within 0-9 (non numeric)
			{
				printf("Error. Not a valid input\n");
				return 0;
			}

			else if(processingIntegerFlag == 1 && foundDecimalPoint == 1 && (array_of_words[i][j] >= 48 && array_of_words[i][j] <= 57)) //if a decimal point is found and numerals trail it, it must be a decimal
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

void validateAddition()
{
	if(checkArgumentLength(0,99) == 0)
		printf("Error. Must contain one or more numbers for addition.\n");
	else
	{
		if(checkForNumericArgument(0) == 1)
		{
			float result = addNumbers();
			printf("Result: %.2f\n", result);
		}
	}
}

float addNumbers()
{

	float currentSum = 0;

	for(int i = 1; i < wordCount; i++)
	{
		float stringToFloat = atof(array_of_words[i]);
		currentSum += stringToFloat;
	}

	return currentSum;
}

void validateSubtraction()
{
	if(checkArgumentLength(1,3) == 0)
		printf("Error. Must contain exactly two numbers for subtraction\n");
	else
	{
		if(checkForNumericArgument(0) == 1)
		{
			float result = subNumbers();
			printf("Result: %.2f\n", result);
		}
	}
}

float subNumbers()
{
	float currentSub = 0;
	uint8_t firstRun = 1;
	for(int i = 1; i < wordCount; i++)
	{
		float stringToFloat = atof((array_of_words)[i]);

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

void validateMultiplication()
{
	if(checkArgumentLength(0,99) == 0)
		printf("Error. Must contain one or more numbers for multiplication.\n");
	else
	{
		if(checkForNumericArgument(0) == 1)
		{
			float result = mulNumbers();
			printf("Result: %.2f\n", result);
		}

	}
}

float mulNumbers()
{
	float currentMul = 0;
	uint8_t firstTime = 1;
	for(int i = 1; i < wordCount; i++)
	{
		float stringToFloat = atof((array_of_words)[i]);

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

void validateDivision()
{
	if(checkArgumentLength(1,3) == 0)
		printf("Error. Must contain exactly two numbers for division\n");
	else
	{
		if(checkForNumericArgument(0) == 1)
		{
			float result = divNumbers();
			printf("Result: %.2f\n", result);
		}
	}
}

float divNumbers()
{
	float currentDiv = 0;
	uint8_t firstRun = 1;
	for(int i = 1; i < wordCount; i++)
	{
		float stringToFloat = atof((array_of_words)[i]);
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


void debugMode()
{
	if(wordCount < 2)
	{
		printf("##Extended Functionality## \t\t Debug status => %d\n", debugOn);
	}
	else if(wordCount > 2)
	{
		printf("Error. Debug command must take zero or one argument. Seek help.\n");
		return;
	}
	else
	{
		char * secondKeyword =  (array_of_words)[1];
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
				printf("Debug message will not be displayed\n");
			}
		}
		else
			printf("Error. Not a valid input for debug keyword. Seek help.\n");
	}


}

void helpDesk()
{
	if(wordCount < 2) {
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
	else if(wordCount == 2)	{
		char * secondKeyword =  (array_of_words)[1];

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
      	analyseKeywords();
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
	  newString = (char*)malloc(sizeof(char)); //start of string. will have reallocation of memory if string is longer
	  buildInputStringFirstTime = 0; //now not the first run through
	  if(c == '\0' || c == 10) {
		  newString[stringIndex] = '\0';

		  wordCount = string_parser(newString, &array_of_words);

		  //printArrayOfWords();
		  //printf("DEBUG: word count -> %d", wordCount);
		  analyseKeywords();
		  freeEverything();

		  //printNewString();
		  buildInputStringFirstTime = 1; //reset
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
		  analyseKeywords();
		  freeEverything();


		  buildInputStringFirstTime = 1; //reset
		  stringIndex = 0; //reset

		}
		else{
		  newString[stringIndex] = c;
		  stringIndex++;
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
