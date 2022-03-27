all:

text: compall runtext

hamlet: compall runhamlet

compall:
	gcc List/List.c Htab/HTab.c problem/problem.c problem/main.c -Wall -Wextra -o htab.out

runhamlet:
	./htab.out Texts/hamlet.txt

valrunhamlet:
	valgrind -s --leak-check=full --undef-value-errors=yes ./htab.out Texts/hamlet.txt

runtext:
	./htab.out --graph-dump=yes Texts/Text.txt

valruntext:
	valgrind -s --leak-check=full --undef-value-errors=yes ./htab.out --graph-dump=yes Texts/Text.txt
