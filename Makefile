all:
	gcc -D C99 -Wall -Werror mymalloc.c tests.c -lcunit -rdynamic -o tests
	./tests
comp:
	gcc -D C99 -Wall -Werror mymalloc.c tests.c -lcunit -rdynamic -o tests
run:
	./tests

.PHONY: run
