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

int encryption (int *ids, int msg_len, int **encr_ids) {
	int *block = (int *) calloc (2, sizeof(int));
	Rotor **slots = (Rotor **) calloc (NUM_OF_ROT, sizeof(Rotor *));
	int **reflectors = NULL;	// reflectors
	int encr_len = 0, abs_num = 0, i = 0, prev_abs_num = 0, mode = 0, step_mode = 0;

//	THIS GUY SHOULD BE INPUTTED (!)
	int order[] = {0,1,2,3,4,5,6,7,8,9,2,3,4,5,6,7,8,9};

	mode = 0;
	step_mode = 0;

	init_slots (order, slots, &reflectors);
	block[0] = 0;
	block[1] = 0;
//	state_print (slots);
//	read_cyr (&ids, &msg_len);
	*encr_ids = (int *) calloc (msg_len, sizeof(int));

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
//		state_print (slots);


		(*encr_ids)[encr_len] = abs_num;		
		encr_len ++;
//		printf ("RETURNED ABS_NUM = %d\n\n\n", abs_num);
//		print_cyr (abs_num);
		if (step_mode) {print_msg (*encr_ids, encr_len);}
	}
//	print_msg (*encr_ids, encr_len);
}

int main() {
	int msg_len = 0, *ids = NULL, *encr_ids = NULL, i = 0;
	double *freq_pl = NULL, *freq_en = NULL;
	FILE *file_output = NULL;

	freq_pl = (double *) calloc (ALPH_LEN, sizeof (double));
	freq_en = (double *) calloc (ALPH_LEN, sizeof (double));
	file_output = fopen("file_output_freq_5", "w");

	read_cyr (&ids, &msg_len);
	for (i = 0; i < msg_len; ++i) {
		freq_pl[ids[i]] += 1;
	}

	encryption (ids, msg_len, &encr_ids);
	free(ids);

	printf ("\nencrypted message: ");
	for (i = 0; i < msg_len; ++i) {
		freq_en[encr_ids[i]] += 1;
		print_cyr (encr_ids[i]);
	}
	printf ("\n");
	for (i = 0; i < ALPH_LEN; ++i) {
		freq_pl[i] /= msg_len;
		freq_en[i] /= msg_len;
		fprintf (file_output, "%d\t%lf\t%lf\n", i, freq_pl[i], freq_en[i]);
	}
	

	fprintf (file_output, "len: %d,\n", msg_len);

	fclose (file_output);
}

