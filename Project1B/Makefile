# NAME: Kanisha Shah
# EMAIL: kanishapshah@gmail.com
# ID: 504958165

default:
	gcc -g -Wall -Wextra -lmcrypt -o lab1b-client lab1b-client.c
	gcc -g -Wall -Wextra -lmcrypt -o lab1b-server lab1b-server.c

clean:
	rm -rf lab1b-client lab1b-server *.tar.gz *.dSYM

dist:
	tar -cvzf lab1b-504958165.tar.gz lab1b-client.c lab1b-server.c my.key Makefile README
