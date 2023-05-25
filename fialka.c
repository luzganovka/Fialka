#include "fialka.h"

void print_cyr (int id) {
	const char *alph = "АБВГДЕЖЗИКЛМНОПРСТУФХЦЧШЩЫЬЮЯЙ";
	printf ("%c%c", alph[2*id], alph[2*id + 1] );
}
/*
void write_cyr (int id, char **str, int *len) {
	const char *alph = "АБВГДЕЖЗИКЛМНОПРСТУФХЦЧШЩЫЬЮЯЙ";
	(*str)[2*len]	  = alph[2*id];
	(*str)[2*len + 1] = alph[2*id + 1];
	*len = *len + 1;
}
*/
/*
void print_str_cyr (char *str, int len) {
	for (int i = 0; i < ; i += ) {
		print_cyr ();
	}
}
*/
void read_cyr (int **ids, int *len) {
	char *msg = readline("input string -->");
	int num = 0;
	*len = strlen (msg) / 2;
	*ids = (int *) calloc (*len, sizeof(int));
	for (int i = 0; i < *len * 2; i += 2) {
 		num = 160 + (msg[i] + msg[i+1]);
		if		(10 <= num && num <= 25) {num -=1;}
		else if (27 <= num && num <= 28) {num -=2;}
		else if (30 <= num && num <= 31) {num -=3;}
		else if (9 == num)				 {num = 29;}
		(*ids)[i/2] = num;
	}
}
/*
// проход сигнала налево через один ротор. Проверка блокировки и вращение
int toleft (Rotor *rot, int *abs_num, int *block) {
	printf ("### toleft (rot, abs_num = %d, block: %d, %d)\n", *abs_num, block[0], block[1]);
	printf ("### ### toleft: abs_num1 = %d\n", *abs_num);
	printf ("### ### toleft: own_num = %d\n", *abs_num + rot->wiring_offset);
	for (int i=0; i<ALPH_LEN; i++) {
		printf ("wiring: %d\n", rot->wiring[i]);
	}
	(*abs_num) = rot->wiring[ (*abs_num) + rot->wiring_offset ];
	printf ("### ### toleft: abs_num2 = %d\n", *abs_num);
	if ( block[rot->odd] == 0) {
		rot->wiring_offset = (rot->wiring_offset++) % ALPH_LEN;
	}
	return 0;
}
*/

int contain (int *arr, int len, int val) {
	for (int i = 0; i < len; ++i) {
		if (arr[i] == val) {
			return 1;
		}
	}
	return 0;
}

int rotate (Rotor *rot, int slot_num, int *block) {
	int abs_num = -1;
	if (slot_num == 1 || (rot->odd == 0 && block[0] == 0)) {
		// вращаем чётные - от себя
		rot->wiring_offset = (rot->wiring_offset - 1) % ALPH_LEN;
		abs_num = (17 - rot->wiring_offset + ALPH_LEN) % ALPH_LEN;
		block[0] = contain (rot->bumps, rot->bumps_len, abs_num);
	} else if (slot_num == NUM_OF_ROT-1 || (rot->odd == 1 && block[1] == 0)){
		// вращаем нечётные - на себя
		rot->wiring_offset = (rot->wiring_offset + 1) % ALPH_LEN;
		abs_num = (20 - rot->wiring_offset + ALPH_LEN) % ALPH_LEN;
		block[1] = contain (rot->bumps, rot->bumps_len, abs_num);
	}
	return 0;	
}

//	проход сигнала справа налево
int toleft  (Rotor *rot, int *abs_num) {
	int own_num = (*abs_num + rot->wiring_offset + ALPH_LEN) % ALPH_LEN;
	own_num = rot->wiring[own_num];
	*abs_num 	= (own_num - rot->wiring_offset + ALPH_LEN) % ALPH_LEN;
	return *abs_num;
}

//	возврат сигнала направо
int toright (Rotor *rot, int *abs_num) {
	int i = 0;
	int own_num = (*abs_num + rot->wiring_offset + ALPH_LEN) % ALPH_LEN;
	while ( rot->wiring[i++] != own_num) {}
	own_num = i - 1;
	*abs_num    = (own_num - rot->wiring_offset + ALPH_LEN) % ALPH_LEN;
	return *abs_num;
}

void state_print (Rotor **rotors) {
	for (int i = 0; i < NUM_OF_ROT; i++) {
		printf (" | ");
		print_cyr ( (rotors[i]->wiring_offset + rotors[i]->alphabet_offset + ALPH_LEN) % ALPH_LEN);
	}
	printf (" |\n");
}

int init_slots (int *order, Rotor **slots, int ***reflectors) {
//	printf ("### init_slots (order, slots)\n");
	int rnum = 0;
	int **wiring_arr = (int **) calloc (NUM_OF_ROT, sizeof(int *));
	int **bump_arr = (int **) calloc (NUM_OF_ROT, sizeof(int *));
	int **ref_arr = (int **) calloc (3, sizeof(int*));
	for (int i = 0; i < NUM_OF_ROT; i++) {
		wiring_arr[i] = (int *) calloc (ALPH_LEN, sizeof(int));
	}


//           4              9             14            19             24
	int wiring_0[ALPH_LEN] = {
13,	22,	8,	18,	20,	12,	28,	4,	15,	27,	3,	5,	16,	14,	23,	26,	1,	25,	17,	11,	30,	10,	24,	7,	6,	21,	29,	2,	9,	19	};
	int wiring_1[ALPH_LEN] = {
20,	8,	5,	15,	4,	28,	21,	1,	24,	13,	29,	12,	14,	23,	25,	7,	9,	30,	27,	3,	11,	18,	17,	19,	22,	10,	2,	26,	6,	16	};
	int wiring_2[ALPH_LEN] = {
29,	11,	4,	22,	24,	16,	18,	2,	23,	3,	17,	8,	20,	5,	28,	12,	15,	26,	30,	7,	21,	19,	13,	10,	27,	25,	9,	1,	14,	6	};
	int wiring_3[ALPH_LEN] = {
4,	12,	19,	29,	24,	23,	7,	30,	15,	1,	20,	14,	18,	2,	16,	27,	10,	25,	17,	28,	6,	21,	11,	8,	22,	5,	9,	3,	26,	13	};
	int wiring_4[ALPH_LEN] = {
18,	2,	15,	7,	20,	28,	8,	13,	23,	12,	19,	27,	4,	24,	10,	14,	11,	6,	30,	3,	17,	26,	22,	1,	29,	25,	16,	21,	5,	9	};
	int wiring_5[ALPH_LEN] = {
16,	4,	14,	24,	23,	19,	30,	3,	1,	8,	27,	13,	9,	5,	29,	10,	15,	26,	22,	7,	25,	17,	20,	11,	2,	6,	21,	28,	18,	12	};
	int wiring_6[ALPH_LEN] = {
26,	23,	7,	5,	13,	8,	24,	30,	29,	20,	22,	9,	12,	10,	25,	16,	3,	21,	19,	18,	4,	1,	28,	27,	6,	2,	15,	17,	11,	14	};
	int wiring_7[ALPH_LEN] = {
16,	22,	14,	30,	24,	15,	17,	20,	4,	7,	27,	12,	6,	13,	25,	21,	1,	5,	26,	8,	11,	23,	29,	28,	3,	18,	10,	19,	2,	9	};
	int wiring_8[ALPH_LEN] = {
12,	1,	17,	29,	6,	4,	7,	11,	15,	3,	21,	25,	9,	26,	30,	13,	22,	20,	10,	24,	27,	14,	28,	23,	2,	5,	19,	18,	16,	8	};
	int wiring_9[ALPH_LEN] = {
9,	21,	8,	16,	25,	5,	20,	22,	4,	27,	14,	19,	1,	15,	30,	2,	10,	17,	7,	24,	12,	18,	29,	3,	23,	6,	13,	28,	26,	11	};



// ШТИФТЫ. Первое число - количество штифтов
	int bump_0[] = {19,	13,22,8,20,28,3,16,14,26,1,17,11,10,24,7,6,29,9,19};
	int bump_1[] = {11,	28,1,14,7,30,3,18,19,22,2,6};
	int bump_2[] = {7,	22,2,8,26,21,9,14};
	int bump_3[] = {19,	4,19,23,7,30,1,20,14,18,2,25,7,28,6,11,22,5,9,26};
	int bump_4[] = {7,	12,4,6,26,1,5,9};
	int bump_5[] = {17,	14,19,30,3,13,9,5,26,22,7,25,17,11,2,6,28,12};
	int bump_6[] = {17,	26,23,7,24,30,29,25,16,3,19,4,1,28,27,6,2,11};
	int bump_7[] = {11,	22,14,17,20,8,28,18,10,19,2,9};
	int bump_8[] = {7,	1,6,4,21,20,14,2};
	int bump_9[] = {23,	9,21,8,25,5,22,4,27,19,1,15,30,2,17,7,24,18,29,3,6,13,26,11};

// РЕФЛЕКТОРЫ
	int reflector_0[] = {
23,	6,	20,	28,	14,	2,	12,	17,	22,	11,	10,	7,	13,	5,	29,	18,	8,	24,	27,	3,	25,	9,	1,	16,	21,	30,	19,	4,	15,	26	};
	int reflector_1[] = {
23,	6,	20,	28,	14,	2,	12,	17,	22,	11,	10,	7,	13,	5,	29,	24,	8,	16,	27,	3,	25,	9,	1,	18,	21,	30,	19,	4,	15,	26	};
	int reflector_2[] = {
1,	2,	3,	4,	5,	6,	7,	8,	9,	10,	11,	12,	13,	14,	15,	16,	17,	18,	19,	20,	21,	22,	23,	24,	25,	26,	27,	28,	29,	30	};
	

	wiring_arr[0] = wiring_0;
	wiring_arr[1] = wiring_1;
	wiring_arr[2] = wiring_1;
	wiring_arr[3] = wiring_1;
	wiring_arr[4] = wiring_1;
	wiring_arr[5] = wiring_1;
	wiring_arr[6] = wiring_1;
	wiring_arr[7] = wiring_1;
	wiring_arr[8] = wiring_1;
	wiring_arr[9] = wiring_1;

	bump_arr[0] = bump_0;
	bump_arr[1] = bump_1;
	bump_arr[2] = bump_2;
	bump_arr[3] = bump_3;
	bump_arr[4] = bump_4;
	bump_arr[5] = bump_5;
	bump_arr[6] = bump_6;
	bump_arr[7] = bump_7;
	bump_arr[8] = bump_8;
	bump_arr[9] = bump_9;

	ref_arr[0] = reflector_0;	// coding
	ref_arr[1] = reflector_1;	// encoding
	ref_arr[2] = reflector_2;	// plain text

	for (int i = 0; i < NUM_OF_ROT; i++) {
		rnum = order[i];
		slots[i] = (Rotor *) calloc (1, sizeof(Rotor));

		slots[i]->wiring = (int *) calloc (ALPH_LEN, sizeof(int));
		for (int j = 0; j < ALPH_LEN; j++) {
			slots[i]->wiring[j] = wiring_arr[rnum][j] - 1;
		}
		slots[i]->bumps_len = bump_arr[rnum][0];
		slots[i]->bumps = (int *) calloc (bump_arr[rnum][0], sizeof(int));
		for (int j = 1; j < bump_arr[rnum][0] + 1; ++j) {
			slots[i]->bumps[j-1]  = bump_arr[rnum][j] - 1;
		}

		slots[i]->odd	= 1 - (i % 2);	// нумерация роторов начинается с 1
										// 0,2,4,6,8 - нечёт, odd = 1
										// 1,3,5,7,9 - чёт, odd = 0
		slots[i]->alphabet_offset = i;	// АБВГДЕЖЗИК - константа
		slots[i]->wiring_offset	 = 0;	// учтановим позже и будем менять
	}

	*reflectors = (int **) calloc (3, sizeof(int *));
// Кажется, третий уровень указателя не нужен, но он был в fialka.h после потери.
	for (int i = 0; i < 3; ++i) {
		(*reflectors)[i] = (int *) calloc (ALPH_LEN, sizeof(int));
		for (int j = 0; j < ALPH_LEN; ++j) {
			(*reflectors)[i][j] = ref_arr[i][j] - 1;
		}
	}
//	printf ("### ### init_slots: OK, end\n");
}
