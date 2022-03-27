all:

text: compall runtext

hamlet: compall runhamlet

compall:
	gcc List.c HTab.c problem.c main.c -Wall -Wextra -o htab.out

runhamlet:
	./htab.out hamlet.txt

valrunhamlet:
	valgrind -s --leak-check=full --undef-value-errors=yes ./htab.out hamlet.txt

runtext:
	./htab.out --graph-dump=yes Text.txt

valruntext:
	valgrind -s --leak-check=full --undef-value-errors=yes ./htab.out --graph-dump=yes Text.txt
