//     $Date: 2018-03-26 08:32:18 +1100 (Mon, 26 Mar 2018) $
// $Revision: 1217 $
//   $Author: Peter $

#include "Ass-02.h"
#ifdef STM32F407xx
#include "usart.h"
#endif

//
// REPLACE THE EXAMPLE CODE WITH YOUR CODE
//

void CommandLineParserInit(void)
{
  // Print welcome message
  printf("\014");
  printf("ELEC3730 Assignment 2\n");
  printf("Command Line Parser Example\n");
}

void CommandLineParserProcess(void)
{
  uint8_t c;

  // Check for input and echo back
#ifdef STM32F407xx
  if (HAL_UART_Receive(&huart2, &c, 1, 0x0) == HAL_OK)
  {
    printf("SERIAL: Got '%c'\n", c);
    HAL_GPIO_TogglePin(GPIOD, LD4_Pin); // Toggle LED4

    // STEPIEN: The following is some test code that can be removed
    //
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
  printf("SERIAL: Got '%c'\n", c);
#endif
}
