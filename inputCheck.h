#ifndef INPUT_CHECK
#define INPUT_CHECK

#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <limits.h>
#define DELIM " \t/"
#define PROMPT "> "


int input_int (int *x, int min, int max, char* message);
int input_str ();
char *parcing (const char *str);
int strToInt (char *str);

#endif
