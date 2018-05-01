

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

#ifndef MESTRUCT
#define MESTRUCT
typedef struct
{
	char* nameString; //command string
	float (*function_p)(uint8_t argNum, char* argStrings[]); //function pointer. todo make sure this is right for evan to use (is it necessary?)
	char* helpString; //help message
} command_s;
#endif

const command_s commandList[11]; //todo alter later?
