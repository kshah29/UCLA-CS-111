# NAME: Kanisha Shah
# EMAIL: kanishapshah@gmail.com
# ID: 504958165

default:
	gcc -lmraa -lm -g -Wall -Wextra -o lab4b lab4b.c

clean:
	rm -rf lab4b *.tar.gz *.dSYM

dist:
	tar -cvzf lab4b-504958165.tar.gz lab4b.c Makefile test.sh README

check: default
		./test.sh
