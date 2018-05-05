/**
 * ELEC3730 ASSIGNMENT 2
 * QUESTION 1 - COMMAND LINE PARSER
 * JORDAN HAIGH AND EVAN GRESHAM
 * HEADER FILE
 *
 * File takes in user input either from STM32 (Putty) or command line
 * Utilises keywords to perform mathematical functions
 * */





void buildInputString(uint8_t c);
int string_parser(char *inp, char **array_of_words_p[]);
void analyseKeywords(uint8_t argNum, char* argStrings[]);


uint8_t checkForNumericArgument2(uint8_t processingIntegerFlag, uint8_t argNum, char* argStrings[]);
uint8_t checkArgumentLength2(uint8_t flag, uint8_t expectedWordCount, uint8_t argNum);

double validateAddition(uint8_t argNum, char* argStrings[]);
double addNumbers(uint8_t argNum, char* argStrings[]);
double validateSubtraction(uint8_t argNum, char* argStrings[]);
double subNumbers(uint8_t argNum, char* argStrings[]);
double validateMultiplication(uint8_t argNum, char* argStrings[]);
double mulNumbers(uint8_t argNum, char* argStrings[]);
double validateDivision(uint8_t argNum, char* argStrings[]);
double divNumbers(uint8_t argNum, char* argStrings[]);

void debugMode(uint8_t argNum, char* argStrings[]);
void helpDesk(uint8_t argNum, char* argStrings[]);

double validateAndRunRoot(uint8_t flag, uint8_t argNum, char* argStrings[]);
double validateSquareRoot(uint8_t argNum, char* argStrings[]);
double validateCubeRoot(uint8_t argNum, char* argStrings[]);
double squareRoot(char* argStrings[]);
double cubeRoot(char* argStrings[]);
double validatePower(uint8_t argNum, char* argStrings[]);
double power(char* argStrings[]);
double validateModulo(uint8_t argNum, char* argStrings[]);
int modulo(char* argStrings[]);

#ifndef MESTRUCT
#define MESTRUCT
typedef struct
{
	char* nameString; //command string
	double (*function_p)(uint8_t argNum, char* argStrings[]); //function pointer
	char* helpString; //help message
} command_s;
#endif

//Global variables to be used in Q1 and in Q2
const command_s commandList[11];
uint8_t debugOn;


