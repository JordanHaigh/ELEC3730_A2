/**
 * ELEC3730 ASSIGNMENT 2
 * QUESTION 1 - COMMAND LINE PARSER
 * JORDAN HAIGH AND EVAN GRESHAM
 *
 * File takes in user input either from STM32 (Putty) or command line
 * Utilises keywords to perform mathematical functions
 * */

#include "Ass-02.h"
#include "Ass-02-Q01.h"
#include <math.h>
#include <stdlib.h>


#ifdef STM32F407xx
#include "usart.h"
#endif

//Global constants necessary for consistent looping
uint8_t debugOn = 0;
int wordCount = 0;
char **array_of_words; //array of words found from command line
char* newString; //building string from command line
int length = 10; //used for memory allocation
int stringIndex = 0;
uint8_t buildInputStringFirstTime = 1;



//Structure defined inside C file, though defined in the .h file
//Allows for structure to be used in Question 2
#ifndef MECOMMANDLIST
#define MECOMMANDLIST
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
#endif

/*
 * If the program has found more than 1 word, print out each word found
 * */
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

/*
 * Used to free global char arrays - newString(created from newString), array_of_words
 * */
void freeEverything()
{
	free(newString);
	free(array_of_words);
}


/*
 * Main area where the keywords found from command line are parsed
 * Gathers the first keyword and checks char by char if it is a valid keyword for mathematical operation
 * Needs to check first if the string parser was able to find any words - otherwise crash.
 * */
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
/*
 * Performs error checking on the argStrings to make sure its valid input before calculating the square root
 * */
double validateSquareRoot(uint8_t argNum, char* argStrings[])
{
	return validateAndRunRoot(0,argNum,argStrings);
}

/*
 * Performs error checking on the argStrings to make sure its valid input before calculating cube root
 * */
double validateCubeRoot(uint8_t argNum, char* argStrings[])
{
	return validateAndRunRoot(1,argNum,argStrings);
}

/*
 * Checks that the argument length of string is correct. If correct,
 * */
double validateAndRunRoot(uint8_t flag, uint8_t argNum, char* argStrings[])
{
	double result = 0; //Start at zero. Will be changed later
	if(checkArgumentLength2(1,2, argNum) == 0) //If Fail
		printf("Error. Must contain only one number for Root.\n");
	else
	{
		//Argument length did not fail, make sure we are working with numerical arguments
		if(checkForNumericArgument2(0,argNum, argStrings) == 1)
		{
			//double result;
			if(flag == 0) //Flag for calculating square root
				result = squareRoot(argStrings);
			else //Flag for calculating cube root
				result = cubeRoot(argStrings);

			printf("Result: %lg\n", result); //Print result to putty/command line
		}
	}

	return result;
}

/*
 * Calculate the square root of the second inputted argument
 * */
double squareRoot(char* argStrings[])
{
	double result = strtof(argStrings[1],NULL);
	return sqrtf(result);
}
/*
 * Calculate the cube root of the second inputted argument
 * */
double cubeRoot(char* argStrings[])
{
	return cbrtf(strtof(argStrings[1],NULL));
}

/*
 * Validate the arguments of the string to make sure they can be used for calculating the power
 * If all arguments are valid and or numerals, we can calculate the power of the number
 * Bonus easter egg if three arguments are entered
 * */
double validatePower(uint8_t argNum, char* argStrings[])
{
	double result = 0;
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
			printf("Result: %lg\n", result);
		}
	}
 	return result;
}
/*
 * Calculate the first argument to the power of the second argument
 * */
double power(char* argStrings[])
{
	return powf(strtof(argStrings[1],NULL),strtof(argStrings[2],NULL));
}

/*
 * Validate the arguments of the string to make sure they can be used for calculating the modulo
 * If all arguments are valid and or numerals, we can calculate the modulo of the two numbers
 * Only accepted two INTEGERS as arguments
 */
double validateModulo(uint8_t argNum, char* argStrings[])
{
	double result = 0;
	if(checkArgumentLength2(1,3,argNum) == 0)
		printf("Error. Must contain only two integers for Modulo.\n");
	else
	{
		if(checkForNumericArgument2(1,argNum,argStrings) == 1)
		{
			result = modulo(argStrings);
			printf("Result: %lg\n", result);
		}
	}
	return result;
}

/*
 * Calculate and return the modulo of the first number mod second number
 * */
int modulo(char* argStrings[])
{
	return (int)strtof(argStrings[1],NULL) % (int)strtof(argStrings[2],NULL);
}

/*
 * Validity check used by mathematical functions. Depending on the flag and expected word count, it will
 * return 1 for true and 0 for false.
 * Add and mul can accept any number of arguments
 * Sub, div, and other functions require two arguments
 * */
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


/*
 * Validity check used by mathematical functions. It will return true or false depending whether the argument is all
 * numerals or not. Also takes into account the positioning of the decimal point (decimal point cant be placed twice for same number)
 * */
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

			else if(!(argStrings[i][j] >= 48 && argStrings[i][j] <= 57) && !argStrings[i][j] == '-')
			{
				//if the number is not within 0-9 (non numeric)

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

/*
 * Validity check used before actually completing addition. Number of arguments must be greater than or equal to 1
 * Needs to check that the arguments are numeric as well before adding numbers
 * */
double validateAddition(uint8_t argNum, char* argStrings[])
{
	double result = 0;
	if(checkArgumentLength2(0,99, argNum) == 0)
		printf("Error. Must contain one or more numbers for addition.\n");
	else
	{
		if(checkForNumericArgument2(0,argNum,argStrings) == 1)
		{
			result = addNumbers(argNum, argStrings);
			printf("Result: %lg\n", result);
		}
	}
	return result;
}

/*
 * Adds numbers together and returns as a double
 * */
double addNumbers(uint8_t argNum, char* argStrings[])
{

	double currentSum = 0;

	for(int i = 1; i < argNum; i++)
	{
		double stringToDouble = strtof(argStrings[i],NULL);
		currentSum += stringToDouble;
	}

	return currentSum;
}

/*
 * 	Validity check before subtracting arguments. Number of arguments must only be 2.
 * 	Needs to check that all arguments are numeric before subtracting
 * */
double validateSubtraction(uint8_t argNum, char* argStrings[])
{
	double result = 0;
	if(checkArgumentLength2(1,3,argNum) == 0)
		printf("Error. Must contain exactly two numbers for subtraction\n");
	else
	{
		if(checkForNumericArgument2(0,argNum, argStrings) == 1)
		{
			result = subNumbers(argNum, argStrings);
			printf("Result: %lg\n", result);
		}
	}
	return result;
}

/*
 * 	Subtract second argument from first argument
 * 	Flag used to determine first argument
 * */
double subNumbers(uint8_t argNum, char* argStrings[])
{
	double currentSub = 0;
	uint8_t firstRun = 1;
	for(int i = 1; i < argNum; i++)
	{
		double stringToDouble = strtof(argStrings[i],NULL);

		if(firstRun == 1)
		{
			currentSub = stringToDouble;
			firstRun = 0;
		}
		else
			currentSub -= stringToDouble;
	}

	return currentSub;

}

/*
 * 	Validity check before calculating multiplication. Argument length can be infinite
 * 	Must check that arguments are numerical before starting multiplication.
 * 	Once valid, we can compute multiplication
 * */
double validateMultiplication(uint8_t argNum, char* argStrings[])
{
	double result = 0;
	if(checkArgumentLength2(0,99,argNum) == 0)
		printf("Error. Must contain one or more numbers for multiplication.\n");
	else
	{
		if(checkForNumericArgument2(0,argNum,argStrings) == 1)
		{
			result = mulNumbers(argNum, argStrings);
			printf("Result: %lg\n", result);
		}

	}
	return result;
}

/*
 * Multiply N numbers together.
 * Uses a flag to start the first argument as the current multiple.
 * That way it isnt always going to be returning 0;
 * */
double mulNumbers(uint8_t argNum, char* argStrings[])
{
	double currentMul = 0;
	uint8_t firstTime = 1;
	for(int i = 1; i < argNum; i++)
	{
		double stringToDouble = 0;
		sscanf(argStrings[i],"%lg",&stringToDouble);
		//printf("In mul - number to mul is %lg\n", stringToDouble);


		//double stringToDouble = strtof((argStrings)[i]);

		if(firstTime)
		{
			currentMul = stringToDouble;
			firstTime = 0;

		}
		else
			currentMul *= stringToDouble;
	}
	//printf("Current myl is %lg\n", currentMul);

	return currentMul;
}

/*
 * Validity check before calculating the division of two numbers. Must be only two arguments
 * Validity check of numerals conducted before division can start
 * */
double validateDivision(uint8_t argNum, char* argStrings[])
{
	double result = 0;
	if(checkArgumentLength2(1,3,argNum) == 0)
		printf("Error. Must contain exactly two numbers for division\n");
	else
	{
		if(checkForNumericArgument2(0,argNum,argStrings) == 1)
		{
			result = divNumbers(argNum,argStrings);
			printf("Result: %lg\n", result);
		}
	}
	return result;
}

/*
 * Divide number 1 by number 2
 * Uses flag to allocate first number as current divider
 * If any of the inputs is a zero, straight away returns zero. (Always going to be zero and gets around DivideByZeroException)
 * */
double divNumbers(uint8_t argNum, char* argStrings[])
{
	double currentDiv = 0;
	uint8_t firstRun = 1;
	if(strcmp(argStrings[2],"0") == 0){
		printf("Error cannot divide by 0\n");
		return 0;

	}
	for(int i = 1; i < argNum; i++)
	{
		double stringToDouble = strtof((argStrings)[i],NULL);
		if(stringToDouble == 0.0)
		{

			return 0.0; //it was always going to be a 0.0 result regardless - smart boy
			//gets around the divide by zero exception
		}

		if(firstRun == 1)
		{
			currentDiv = stringToDouble;
			firstRun = 0;
		}
		else
			currentDiv /= stringToDouble;
	}

	return currentDiv;

}

/*
 * Allows for debug messages to be turned on or off
 * If no arguments supplied, it will print the current status of debug
 * If one argument supplied <on | off> it will toggle the current status
 * If more than one argument, seek help
 * */
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

/*
 * Help desk supplies all help information about math functions
 * If no arguments supplied, prints all information about functions
 * If one SPECIFIC math function supplied, prints information about THAT function
 * */
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
		printf("sqrt <num1> : Finds the square root of a double precision floating point number.\n");
		printf("cbrt <num1> : Finds the cube root of a double precision floating point number.\n");
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

/*Init method for Command Line - Does nothing important*/
void CommandLineParserInit(void)
{
  // Print welcome message
//  printf("\014"); //this is a form feed
  printf("ELEC3730 Assignment 2\n");
//  printf("Command Line Parser Example\n");
}

/*
 * Continuous looping method
 * If the project is running through WINNT - Builds input string until 'enter' key is inputted
 * If project is running through STM32 (PUTTY) - Builds input string in putty until entered
 *
 * ONE MUST FINISH BEFORE USING THE OTHER AGAIN
 * CANNOT MULTI TYPE AT SAME TIME
 * */

int i = 0;
char command_line[101];
void CommandLineParserProcess(void)
{
	//printf("hello\n");
  uint8_t c;


  // Check for input and echo back
#ifdef STM32F407xx
  if (HAL_UART_Receive(&huart2, &c, 1, 0x0) == HAL_OK) //Get input from putty
  {
		printf("%c",c); //print to putty and allocate to array
		command_line[i]=c;
		i++;

    	//printf("\n");
    	if(c == 13 || i+1 == 100) //If input is enter character or run out of chars
    	{
    		printf("\n");

    		command_line[i-1]='\0'; //Need to replace enter character with null terminator

			// Parse the input and print result
			wordCount = string_parser(command_line, &array_of_words);
			if(debugOn == 1) printArrayOfWords();
			analyseKeywords(wordCount, array_of_words);
			freeEverything();

			//reset command line and i variable
			for(int j = 0; command_line[j] != '\0';j++)
			{
				command_line[j] = ' ';
			}
			i = 0;
    	}


  }
#else
  c = getchar();
  //printf("SERIAL: Got '%c' or in ascii '%d'\n", c,c);
  buildInputString(c);

#endif
}





/*
 * Builds input string from command line
 * gets char by char and keeps reallocating till finished with input string
 * Once enter is hit, it will begin to parse the string into keywords and begin analysing
 * */
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

		if(c == '\0' || c == 10){ //if enter
		  newString[stringIndex] = '\0'; //null terminate string
		  //begin parse
		  wordCount = string_parser(newString, &array_of_words);
		  if(debugOn == 1) printArrayOfWords();
		  //begin analysis of keywords
		  analyseKeywords(wordCount, array_of_words);
		  freeEverything();


		  buildInputStringFirstTime = 1; //reset
		  stringIndex = 0; //reset

		}
		else{
		  newString[stringIndex] = c; //keep appending
		  stringIndex++;
		  if(stringIndex == length) //if string length is maxed out
		  {
			  length *=2; //allocate more memory
			  newString = (char*)realloc(newString,sizeof(char*) * length);
		  }
		}
	}
}

/*
 * String parser from Assignment 1
 * Same as before, goes through string and determines new string without backspaces
 * Loops to determine number of words, number of characters in each word
 * Allocates accordingly
 * */
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
