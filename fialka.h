#ifndef FIALKA_H
#define FIALKA_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#define ALPHABET "АБВГДЕЖЗИКЛМНОПРСТУФХЦЧШЩЫЬЮЯЙ" // ??
#define ALPH_LEN   30
#define NUM_OF_ROT 10

typedef struct Rotor {
	int *wiring; // проводка: индекс-абс номер правого пина, значение - левого
	int *bumps;				// соб номера пинов со штифтами
	int bumps_len;			// колисество пинов со штифтами
	int odd;				// нечётный? - 1 или 0
	int alphabet_offset;	// константа, смещение буквы А отн начала проводки
	int wiring_offset;		// текущий поворот проводки
} Rotor;

int rotate	(Rotor *rot, int slot_num, int *block),
	toleft	(Rotor *rot, int *abs_num),
	toright (Rotor *rot, int *abs_num),
	init_slots (int *order, Rotor **slots, int ***reflectors),
	contain	(int *arr, int len, int val);
void state_print (Rotor **rotors),
	print_cyr (int id),
	read_cyr  (int **ids, int *msg_len);
#endif
