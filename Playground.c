//     $Date: 2018-03-26 08:32:18 +1100 (Mon, 26 Mar 2018) $
// $Revision: 1217 $
//   $Author: Peter $

#include "Ass-02.h"
#include <math.h>

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



//char* inputString = "0";
char* inputString;
char* textArray[] = {"7","8","9","+","-","4","5","6","/","*","1","2","3","sqrt","=-","0",".","clr","pow","="};

static int firstTime = 1;
void playground()
{
	if(firstTime == 1)
	{
		inputString = malloc(sizeof(char) * 1);
		strcpy(inputString, "0");  //init


		//printf("input string starts with %s", inputString);
		firstTime = 0;
	}

	int buttonId = atoi(array_of_words[0]);

	printf("Button id is: %d\n", buttonId);

	char* buttonText = textArray[buttonId];
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
			return;
		}
		if(strcmp(buttonText, "=") == 0)
		{
			printf("= was entered. Nothing to achieve on blank string\n");
			return;
		}
		if(strcmp(buttonText, "clr") == 0)
		{
			printf("clr was entered. Nothing to achieve on blank string\n");
			return;
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
			inputString = realloc(inputString, sizeof(inputString)+1);
			strcat(inputString, buttonText);
		}

	}






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
    printf("SERIAL: Got '%c'\n", c);
    HAL_GPIO_TogglePin(GPIOD, LD4_Pin); // Toggle LED4

    // STEPIEN: The following is some test code that can be removed
    //
    //todo remove later and replace with functioning stm code

    {
      int c;
      char si[]="1234";
      int i=111;
      char sf[]="r5b6c7d8";
      float f=2.22;

      printf("TEST: Float printf() test: %f\n", 1.234);
      sscanf(si, "%d", &i);
      c=sscanf(sf, "%f", &f);
      printf("TEST: Input string : '%s'\n", si);
      printf("TEST: Input int    : %d\n", i);
      printf("TEST: Input string : '%s'\n", sf);
      printf("TEST: Input float  : %f\n", f);
      printf("TEST: c=%d\n",c);
    }
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
		  //analyseKeywords();

		  playground();
		  printf("Current string %s\n", inputString);
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
		  //if(debugOn == 1) printArrayOfWords();
		  //analyseKeywords();
		  playground();
		  printf("Current string %s\n", inputString);
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

