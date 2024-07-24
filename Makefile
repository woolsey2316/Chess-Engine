all:
	gcc -oFast bbc.c -o bbc
	gcc bbc.c -o bbc.exe
debug:
	gcc bbc.c -o bbc
	gcc bbc.c -o bbc.exe
