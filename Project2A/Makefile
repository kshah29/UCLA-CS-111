# NAME: Kanisha Shah
# EMAIL: kanishapshah@gmail.com
# ID: 504958165


build:
	gcc -Wall -Wextra -lpthread  -g -o lab2_add lab2_add.c
	gcc -Wall -Wextra -lpthread  -g -o lab2_list SortedList.c lab2_list.c

clean:
	rm -rf lab2_add lab2_list *.tar.gz *.dSYM

tests: build
	rm -f  lab2_add.csv lab2_list.csv

# lab2_add-1.png: with yields, threads (2,4,8,12) and iterations (10, 20, 40, 80, 100, 1000, 10000, 100000)
	./lab2_add --yield --threads=2 --iterations=10 >> lab2_add.csv
	./lab2_add --yield --threads=4 --iterations=10 >> lab2_add.csv
	./lab2_add --yield --threads=8 --iterations=10 >> lab2_add.csv
	./lab2_add --yield --threads=12 --iterations=10 >> lab2_add.csv

	./lab2_add --yield --threads=2 --iterations=20 >> lab2_add.csv
	./lab2_add --yield --threads=4 --iterations=20 >> lab2_add.csv
	./lab2_add --yield --threads=8 --iterations=20 >> lab2_add.csv
	./lab2_add --yield --threads=12 --iterations=20 >> lab2_add.csv

	./lab2_add --yield --threads=2 --iterations=40 >> lab2_add.csv
	./lab2_add --yield --threads=4 --iterations=40 >> lab2_add.csv
	./lab2_add --yield --threads=8 --iterations=40 >> lab2_add.csv
	./lab2_add --yield --threads=12 --iterations=40 >> lab2_add.csv

	./lab2_add --yield --threads=2 --iterations=80 >> lab2_add.csv
	./lab2_add --yield --threads=4 --iterations=80 >> lab2_add.csv
	./lab2_add --yield --threads=8 --iterations=80 >> lab2_add.csv
	./lab2_add --yield --threads=12 --iterations=80 >> lab2_add.csv

	./lab2_add --yield --threads=2 --iterations=100 >> lab2_add.csv
	./lab2_add --yield --threads=4 --iterations=100 >> lab2_add.csv
	./lab2_add --yield --threads=8 --iterations=100 >> lab2_add.csv
	./lab2_add --yield --threads=12 --iterations=100 >> lab2_add.csv

	./lab2_add --yield --threads=2 --iterations=1000 >> lab2_add.csv
	./lab2_add --yield --threads=4 --iterations=1000 >> lab2_add.csv
	./lab2_add --yield --threads=8 --iterations=1000 >> lab2_add.csv
	./lab2_add --yield --threads=12 --iterations=1000 >> lab2_add.csv

	./lab2_add --yield --threads=2 --iterations=10000 >> lab2_add.csv
	./lab2_add --yield --threads=4 --iterations=10000 >> lab2_add.csv
	./lab2_add --yield --threads=8 --iterations=10000 >> lab2_add.csv
	./lab2_add --yield --threads=12 --iterations=10000 >> lab2_add.csv

	# lab2_add-2.png: yield and non-yield threads (2, 8) and of iterations (100, 1000, 10000, 100000)
	./lab2_add --threads=2 --iterations=100 >> lab2_add.csv
	./lab2_add --threads=2 --iterations=1000 >> lab2_add.csv
	./lab2_add --threads=2 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=2 --iterations=100000 >> lab2_add.csv

	./lab2_add --threads=8 --iterations=100 >> lab2_add.csv
	./lab2_add --threads=8 --iterations=1000 >> lab2_add.csv
	./lab2_add --threads=8 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=8 --iterations=100000 >> lab2_add.csv

	./lab2_add --yield --threads=2 --iterations=100 >> lab2_add.csv
	./lab2_add --yield --threads=2 --iterations=1000 >> lab2_add.csv
	./lab2_add --yield --threads=2 --iterations=10000 >> lab2_add.csv
	./lab2_add --yield --threads=2 --iterations=100000 >> lab2_add.csv

	./lab2_add --yield --threads=8 --iterations=100 >> lab2_add.csv
	./lab2_add --yield --threads=8 --iterations=1000 >> lab2_add.csv
	./lab2_add --yield --threads=8 --iterations=10000 >> lab2_add.csv
	./lab2_add --yield --threads=8 --iterations=100000 >> lab2_add.csv

	#  lab2_add-3.png.: single thread, non-yield as a function of the number of iterations.
	./lab2_add --threads=1 --iterations=10 >> lab2_add.csv
	./lab2_add --threads=1 --iterations=100 >> lab2_add.csv
	./lab2_add --threads=1 --iterations=1000 >> lab2_add.csv
	./lab2_add --threads=1 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=1 --iterations=100000 >> lab2_add.csv

	# lab2_add-4.png.: yield, threads (2, 4, 8, 12) and iterations (10,000 for mutexes and CAS, 1,000 for spin locks)
	./lab2_add --yield --threads=2 --iterations=10000 --sync=m >> lab2_add.csv
	./lab2_add --yield --threads=4 --iterations=10000 --sync=m >> lab2_add.csv
	./lab2_add --yield --threads=8 --iterations=10000 --sync=m >> lab2_add.csv
	./lab2_add --yield --threads=12 --iterations=10000 --sync=m >> lab2_add.csv

	./lab2_add --yield --threads=2 --iterations=10000 --sync=c >> lab2_add.csv
	./lab2_add --yield --threads=4 --iterations=10000 --sync=c >> lab2_add.csv
	./lab2_add --yield --threads=8 --iterations=10000 --sync=c >> lab2_add.csv
	./lab2_add --yield --threads=12 --iterations=10000 --sync=c >> lab2_add.csv

	./lab2_add --yield --threads=2 --iterations=1000 --sync=s >> lab2_add.csv
	./lab2_add --yield --threads=4 --iterations=1000 --sync=s >> lab2_add.csv
	./lab2_add --yield --threads=8 --iterations=1000 --sync=s >> lab2_add.csv
	./lab2_add --yield --threads=12 --iterations=1000 --sync=s >> lab2_add.csv

	# lab2_add-5.png.: iterations (e.g. 10,000), no yield, threads (1,2,4,8,12), all sync
	./lab2_add --threads=1 --iterations=10000 --sync=m >> lab2_add.csv
	./lab2_add --threads=2 --iterations=10000 --sync=m >> lab2_add.csv
	./lab2_add --threads=4 --iterations=10000 --sync=m >> lab2_add.csv
	./lab2_add --threads=8 --iterations=10000 --sync=m >> lab2_add.csv
	./lab2_add --threads=12 --iterations=10000 --sync=m >> lab2_add.csv

	./lab2_add --threads=1 --iterations=10000 --sync=s >> lab2_add.csv
	./lab2_add --threads=2 --iterations=10000 --sync=s >> lab2_add.csv
	./lab2_add --threads=4 --iterations=10000 --sync=s >> lab2_add.csv
	./lab2_add --threads=8 --iterations=10000 --sync=s >> lab2_add.csv
	./lab2_add --threads=12 --iterations=10000 --sync=s >> lab2_add.csv

	./lab2_add --threads=1 --iterations=10000 --sync=c >> lab2_add.csv
	./lab2_add --threads=2 --iterations=10000 --sync=c >> lab2_add.csv
	./lab2_add --threads=4 --iterations=10000 --sync=c >> lab2_add.csv
	./lab2_add --threads=8 --iterations=10000 --sync=c >> lab2_add.csv
	./lab2_add --threads=12 --iterations=10000 --sync=c >> lab2_add.csv

	./lab2_add --threads=1 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=2 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=4 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=8 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=12 --iterations=10000 >> lab2_add.csv


	# lab2_list-1.png: single thread, iterations (10, 100, 1000, 10000, 20000)
	- ./lab2_list --threads=1 --iterations=10 >> lab2_list.csv
	- ./lab2_list --threads=1 --iterations=100 >> lab2_list.csv
	- ./lab2_list --threads=1 --iterations=1000 >> lab2_list.csv
	- ./lab2_list --threads=1 --iterations=10000 >> lab2_list.csv
	- ./lab2_list --threads=1 --iterations=20000 >> lab2_list.csv

	# lab2_list-2.png: parallel threads (2,4,8,12) and iterations (1, 10,100,1000)
	- ./lab2_list --threads=2 --iterations=1 >> lab2_list.csv
	- ./lab2_list --threads=2 --iterations=10 >> lab2_list.csv
	- ./lab2_list --threads=2 --iterations=100 >> lab2_list.csv
	- ./lab2_list --threads=2 --iterations=1000 >> lab2_list.csv

	- ./lab2_list --threads=4 --iterations=1 >> lab2_list.csv
	- ./lab2_list --threads=4 --iterations=10 >> lab2_list.csv
	- ./lab2_list --threads=4 --iterations=100 >> lab2_list.csv
	- ./lab2_list --threads=4 --iterations=1000 >> lab2_list.csv

	- ./lab2_list --threads=8 --iterations=1 >> lab2_list.csv
	- ./lab2_list --threads=8 --iterations=10 >> lab2_list.csv
	- ./lab2_list --threads=8 --iterations=100 >> lab2_list.csv
	- ./lab2_list --threads=8 --iterations=1000 >> lab2_list.csv

	- ./lab2_list --threads=12 --iterations=1 >> lab2_list.csv
	- ./lab2_list --threads=12 --iterations=10 >> lab2_list.csv
	- ./lab2_list --threads=12 --iterations=100 >> lab2_list.csv
	- ./lab2_list --threads=12 --iterations=1000 >> lab2_list.csv

	# lab2_list-2.png: threads (2,4,8,12) and iterations (1, 2,4,8,16,32) with yield=i,d,il,dl
	- ./lab2_list --threads=2 --iterations=1 --yield=i >> lab2_list.csv
	- ./lab2_list --threads=2 --iterations=2 --yield=i >> lab2_list.csv
	- ./lab2_list --threads=2 --iterations=4 --yield=i >> lab2_list.csv
	- ./lab2_list --threads=2 --iterations=8 --yield=i >> lab2_list.csv
	- ./lab2_list --threads=2 --iterations=16 --yield=i >> lab2_list.csv
	- ./lab2_list --threads=2 --iterations=32 --yield=i >> lab2_list.csv
	- ./lab2_list --threads=2 --iterations=1 --yield=d >> lab2_list.csv
	- ./lab2_list --threads=2 --iterations=2 --yield=d >> lab2_list.csv
	- ./lab2_list --threads=2 --iterations=4 --yield=d >> lab2_list.csv
	- ./lab2_list --threads=2 --iterations=8 --yield=d >> lab2_list.csv
	- ./lab2_list --threads=2 --iterations=16 --yield=d >> lab2_list.csv
	- ./lab2_list --threads=2 --iterations=32 --yield=d >> lab2_list.csv
	- ./lab2_list --threads=2 --iterations=1 --yield=il >> lab2_list.csv
	- ./lab2_list --threads=2 --iterations=2 --yield=il >> lab2_list.csv
	- ./lab2_list --threads=2 --iterations=4 --yield=il >> lab2_list.csv
	- ./lab2_list --threads=2 --iterations=8 --yield=il >> lab2_list.csv
	- ./lab2_list --threads=2 --iterations=16 --yield=il >> lab2_list.csv
	- ./lab2_list --threads=2 --iterations=32 --yield=il >> lab2_list.csv
	- ./lab2_list --threads=2 --iterations=1 --yield=dl >> lab2_list.csv
	- ./lab2_list --threads=2 --iterations=2 --yield=dl >> lab2_list.csv
	- ./lab2_list --threads=2 --iterations=4 --yield=dl >> lab2_list.csv
	- ./lab2_list --threads=2 --iterations=8 --yield=dl >> lab2_list.csv
	- ./lab2_list --threads=2 --iterations=16 --yield=dl >> lab2_list.csv
	- ./lab2_list --threads=2 --iterations=32 --yield=dl >> lab2_list.csv

	- ./lab2_list --threads=4 --iterations=1 --yield=i >> lab2_list.csv
	- ./lab2_list --threads=4 --iterations=2 --yield=i >> lab2_list.csv
	- ./lab2_list --threads=4 --iterations=4 --yield=i >> lab2_list.csv
	- ./lab2_list --threads=4 --iterations=8 --yield=i >> lab2_list.csv
	- ./lab2_list --threads=4 --iterations=16 --yield=i >> lab2_list.csv
	- ./lab2_list --threads=4 --iterations=32 --yield=i >> lab2_list.csv
	- ./lab2_list --threads=4 --iterations=1 --yield=d >> lab2_list.csv
	- ./lab2_list --threads=4 --iterations=2 --yield=d >> lab2_list.csv
	- ./lab2_list --threads=4 --iterations=4 --yield=d >> lab2_list.csv
	- ./lab2_list --threads=4 --iterations=8 --yield=d >> lab2_list.csv
	- ./lab2_list --threads=4 --iterations=16 --yield=d >> lab2_list.csv
	- ./lab2_list --threads=4 --iterations=32 --yield=d >> lab2_list.csv
	- ./lab2_list --threads=4 --iterations=1 --yield=il >> lab2_list.csv
	- ./lab2_list --threads=4 --iterations=2 --yield=il >> lab2_list.csv
	- ./lab2_list --threads=4 --iterations=4 --yield=il >> lab2_list.csv
	- ./lab2_list --threads=4 --iterations=8 --yield=il >> lab2_list.csv
	- ./lab2_list --threads=4 --iterations=16 --yield=il >> lab2_list.csv
	- ./lab2_list --threads=4 --iterations=32 --yield=il >> lab2_list.csv
	- ./lab2_list --threads=4 --iterations=1 --yield=dl >> lab2_list.csv
	- ./lab2_list --threads=4 --iterations=2 --yield=dl >> lab2_list.csv
	- ./lab2_list --threads=4 --iterations=4 --yield=dl >> lab2_list.csv
	- ./lab2_list --threads=4 --iterations=8 --yield=dl >> lab2_list.csv
	- ./lab2_list --threads=4 --iterations=16 --yield=dl >> lab2_list.csv
	- ./lab2_list --threads=4 --iterations=32 --yield=dl >> lab2_list.csv

	- ./lab2_list --threads=8 --iterations=2 --yield=i >> lab2_list.csv
	- ./lab2_list --threads=8 --iterations=4 --yield=i >> lab2_list.csv
	- ./lab2_list --threads=8 --iterations=8 --yield=i >> lab2_list.csv
	- ./lab2_list --threads=8 --iterations=12 --yield=i >> lab2_list.csv
	- ./lab2_list --threads=8 --iterations=2 --yield=d >> lab2_list.csv
	- ./lab2_list --threads=8 --iterations=4 --yield=d >> lab2_list.csv
	- ./lab2_list --threads=8 --iterations=8 --yield=d >> lab2_list.csv
	- ./lab2_list --threads=8 --iterations=12 --yield=d >> lab2_list.csv
	- ./lab2_list --threads=8 --iterations=2 --yield=il >> lab2_list.csv
	- ./lab2_list --threads=8 --iterations=4 --yield=il >> lab2_list.csv
	- ./lab2_list --threads=8 --iterations=8 --yield=il >> lab2_list.csv
	- ./lab2_list --threads=8 --iterations=12 --yield=il >> lab2_list.csv
	- ./lab2_list --threads=8 --iterations=2 --yield=dl >> lab2_list.csv
	- ./lab2_list --threads=8 --iterations=4 --yield=dl >> lab2_list.csv
	- ./lab2_list --threads=8 --iterations=8 --yield=dl >> lab2_list.csv
	- ./lab2_list --threads=8 --iterations=12 --yield=dl >> lab2_list.csv

	- ./lab2_list --threads=12 --iterations=2 --yield=i >> lab2_list.csv
	- ./lab2_list --threads=12 --iterations=4 --yield=i >> lab2_list.csv
	- ./lab2_list --threads=12 --iterations=8 --yield=i >> lab2_list.csv
	- ./lab2_list --threads=12 --iterations=12 --yield=i >> lab2_list.csv
	- ./lab2_list --threads=12 --iterations=2 --yield=d >> lab2_list.csv
	- ./lab2_list --threads=12 --iterations=4 --yield=d >> lab2_list.csv
	- ./lab2_list --threads=12 --iterations=8 --yield=d >> lab2_list.csv
	- ./lab2_list --threads=12 --iterations=12 --yield=d >> lab2_list.csv
	- ./lab2_list --threads=12 --iterations=2 --yield=il >> lab2_list.csv
	- ./lab2_list --threads=12 --iterations=4 --yield=il >> lab2_list.csv
	- ./lab2_list --threads=12 --iterations=8 --yield=il >> lab2_list.csv
	- ./lab2_list --threads=12 --iterations=12 --yield=il >> lab2_list.csv
	- ./lab2_list --threads=12 --iterations=2 --yield=dl >> lab2_list.csv
	- ./lab2_list --threads=12 --iterations=4 --yield=dl >> lab2_list.csv
	- ./lab2_list --threads=12 --iterations=8 --yield=dl >> lab2_list.csv
	- ./lab2_list --threads=12 --iterations=12 --yield=dl >> lab2_list.csv

	 # lab2_list-3.png.: threads (12) and iterations (32), yield=i,d,il,dl, sync=s,m
	./lab2_list --threads=12 --iterations=32 --yield=i --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=32 --yield=d --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=32 --yield=il --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=32 --yield=dl --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=32 --yield=i --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=32 --yield=d --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=32 --yield=il --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=32 --yield=dl --sync=s >> lab2_list.csv

	# lab2_list-4.png: iterations (e.g. 1000), threads (1, 2, 4, 8, 12, 16, 24), sync: mutex, spin, no yield
	./lab2_list --threads=1 --iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=2 --iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=4 --iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=8 --iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=16 --iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=24 --iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=1 --iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=2 --iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=4 --iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=8 --iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=16 --iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=24 --iterations=1000 --sync=s >> lab2_list.csv


graphs: tests
	./lab2_add.gp
	./lab2_list.gp

dist: graphs
		tar -czvf lab2a-504958165.tar.gz lab2_add.c lab2_list.c \
		SortedList.h SortedList.c Makefile README lab2_add.csv lab2_list.csv \
		lab2_add-1.png lab2_add-2.png lab2_add-3.png lab2_add-4.png \
		lab2_add-5.png lab2_list-1.png lab2_list-2.png lab2_list-3.png \
		lab2_list-4.png lab2_add.gp lab2_list.gp
