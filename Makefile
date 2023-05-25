all:	run

run:
	cc main.c fialka.c inputCheck.c -lreadline -g -o run

timing:
	cc main_timing.c fialka.c inputCheck.c -lreadline -g -o timing

freq:
	cc main_freq.c fialka.c inputCheck.c -lreadline -g -o freq

flean:
	rm -rf file_*
