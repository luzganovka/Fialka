#include "inputCheck.h"

int input_int(int* x, int min, int max, char* message){
	printf("%s", message);
	int scf_res;
	while(1){
		scf_res = scanf("%d", x);
		scanf("%*[^\n]");
		if (scf_res == EOF) {
			printf("End of file!\n");
			return -1;
		}
		if (!scf_res) {
			printf("Input error!\n");
			scanf("%*[^\n]");
		}
		else if (*x < min || *x > max) {
			printf("Out of range!\n");
		}
		else {
			break;
		}
		printf("%s", message);
	}
	return 0;
}
/*
int input_int_wrap (int* x, int min, int max) {
	scf_res = input_int (x, min, max);
	switch (scf_res) {
	case 0:
		return 0;
	}
}
*/

int input_str() {
    char *input = readline(PROMPT);
    while (input != NULL) {
        printf("\"%s\"\n", input);
        char *output = parcing(input);
        printf("\"%s\"\n", output);
        free (input);
        free (output);
        input = readline(PROMPT);
    }
    return 0;
}

char *parcing(const char *str) {
    char *s = strdup(str);
    int s_len = strlen(s);
    char *res = (char*)calloc(s_len + 1, sizeof(char));
    int len = 0;
    int w_len = 0;
    char* word = strtok(s, DELIM);
    while (word != NULL) {
        w_len = strlen(word);
//      printf("word: \"%s\"\n", word);

		memcpy (res + len, word, w_len * sizeof(char));
        len += w_len;
        res[len] = ' ';
        len++;

//      printf("result: \"%s\"\n", res);
        word = strtok(NULL, DELIM);
    }
    if (len > 0){
        len--;
    }
    res[len] = '\0';
    res = realloc(res, (len+1) * sizeof(char));
    return res;
}

int strToInt (char *str){
	int res = 0, digit = 0;
	int len = strlen(str);
	for (int i = 0; i < len; ++i) {
		digit = (int)(str[i] - 48);
		res = res * 10 + digit;
	}
	return res;
}
