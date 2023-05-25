//	cc main.c fialka.c inputCheck.c -lreadline -g -o run
//
//	АБВГДЕЖЗИКЛМНОПРСТУФХЦЧШЩЫЬЮЯЙ
//////////////////////////////////////////////////////////////

#include "fialka.h"
#include "inputCheck.h"
#include <time.h>
	
void check_rot (Rotor *rot) {
	printf ("bumps_len =\t\t%d\nodd =\t\t\t%d\nalphabet_offset =\t%d\nwiring_offset =\t\t%d\n", rot->bumps_len, rot->odd, rot->alphabet_offset, rot->wiring_offset);
	printf ("wiring:\n");
	for (int i = 0; i < ALPH_LEN; ++i) {
		printf ("%d, ",rot->wiring[i]);
	}
	printf ("\nbumps:\n");
	for (int i = 0; i < rot->bumps_len; ++i) {
		printf ("%d, ",rot->bumps[i]);
	}
	printf ("\n");
}

void print_msg (int *ids, int len) {
	printf ("\nENCRYPTED MSG: ");
	for (int i = 0; i < len; ++i) {
		print_cyr (ids[i]);
	}
	printf ("\n");
}

int encryption (int *ids, int *encr_ids, int msg_len, int mode, int step_mode, int *order) {
	int *block = (int *) calloc (2, sizeof(int));
	Rotor **slots = (Rotor **) calloc (NUM_OF_ROT, sizeof(Rotor *));
	int **reflectors = NULL;	// reflectors
	int encr_len = 0, abs_num = 0, i = 0, prev_abs_num = 0;

//	THIS GUY SHOULD BE INPUTTED (!)

	init_slots (order, slots, &reflectors);
	block[0] = 0;
	block[1] = 0;
//	state_print (slots);
//	read_cyr (&ids, &msg_len);

	for (int j = 0; j < msg_len; j++){	// untill the text ends
		abs_num = ids[j];
// START CODING - GOING ON THE LEFT

		for (i = NUM_OF_ROT - 1; i >= 0; i--) {
			toleft (slots[i], &abs_num);
		}
// OK. REFLECTOR IS REACHED

		prev_abs_num = abs_num;
		abs_num = reflectors[mode][abs_num];	// reflector is static
		if (prev_abs_num == abs_num) {	// letter is enciphered as itself
			// (?)
		}
// OK. WE'LL GO BACK NOW

		for (i = 0; i < NUM_OF_ROT; ++i) {
			toright (slots[i], &abs_num);
		}

// один поворот всех роторов
// <-- нечётные		8,6,4,2,0
//		чётные -->	1,3,5,7,9
		for (i = 1; i < NUM_OF_ROT; i+=2) {
			rotate (slots[i], i,  block);		// чётные:		1,3,5,7,9
			rotate (slots[NUM_OF_ROT - 1 - i], NUM_OF_ROT - 1 - i, block);
												// нечётные:	8,6,4,2,0
//			printf ("wiring_offset_%d = %d\n", i, slots[i  ]->wiring_offset);
//			printf ("wiring_offset_%d = %d\n", i+1,slots[i+1]->wiring_offset);
		}

		encr_ids[encr_len] = abs_num;		
		encr_len ++;
//		printf ("RETURNED ABS_NUM = %d\n\n\n", abs_num);
//		print_cyr (abs_num);
		if (step_mode) {
			print_msg (encr_ids, encr_len);
			state_print (slots);
		}
	}
//	print_msg (encr_ids, encr_len);
}

int d_order (int *order) {
	int user = 0, i = 0;
	input_int (&user, 0, 1, "Do you want to use default rotor order (0) or change it (1)? --> ");
	
	if (user == 1) {
		for (i = 0; i < NUM_OF_ROT; ++i) {
			printf ("enter number of rotor, inserted into slot %d", i+1);
			input_int (&(order[i]), 1, 10, " --> ");
			order[i] --;
		}
	} else {
		for (i = 0; i < NUM_OF_ROT; ++i) {
			order[i] = i;
		}
	}
}

int main() {
	int msg_len = 0, mode = 0, step_mode = 0;
	int *ids	  = (int *) calloc (msg_len, sizeof(int));
	int *order = (int *) calloc (NUM_OF_ROT, sizeof(int));
	d_order (order);

	input_int (&mode, 0, 2, "Choose mode:\n0. encryption\n1. decryption\n2. plain text\n--> ");
	input_int (&step_mode, 0, 1, "Do you want to use step-by-step mode? --> ");

	read_cyr (&ids, &msg_len); 
	int *encr_ids = (int *) calloc (msg_len, sizeof(int));
	encryption (ids, encr_ids, msg_len, mode, step_mode, order);
	print_msg (encr_ids, msg_len);
			
	free(ids);
		
}

