# NAME: Kanisha Shah
# EMAIL: kanishapshah@gmail.com
# ID: 504958165

default:
	gcc -lmraa -lm -g -Wall -Wextra -o lab4c_tcp lab4c_tcp.c
	gcc -lmraa -lssl -lcrypto -lm -g -Wall -Wextra -o lab4c_tls lab4c_tls.c

clean:
	rm -rf lab4c_tcp lab4c_tls *.tar.gz *.dSYM

dist:
	tar -cvzf lab4c-504958165.tar.gz lab4c_tcp.c lab4c_tls.c Makefile README
