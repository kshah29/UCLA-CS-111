# NAME: Kanisha Shah
# EMAIL: kanishapshah@gmail.com
# ID: 504958165


default:
	gcc -Wall -Wextra -lpthread -lprofiler -g -o lab2_list SortedList.c lab2_list.c

clean:
	rm -rf lab2_list *.tar.gz *.dSYM

tests: default
	rm -f  lab2b_list.csv

	# lab2b_1.png: mutex & spin 1,000 iterations, 1,2,4,8,12,16,24 threads
	# lab2b_2.png: mutex test again for 1,000 iterations and 1, 2, 4, 8, 16, 24 threads
	# All these test are included in lab2b_4 and lab2b_5

	# lab2b_3.png: --yield=id, 4 lists, 1,4,8,12,16 threads, and 1, 2, 4, 8, 16 iterations, no synchronization
	# lab2b_3.png: --yield=id, 4 lists, 1,4,8,12,16 threads, and 10, 20, 40, 80 iterations, --sync=s and --sync=m
	- ./lab2_list --yield=id --lists=4 --threads=1 --iterations=1 >> lab2b_list.csv
	- ./lab2_list --yield=id --lists=4 --threads=1 --iterations=2 >> lab2b_list.csv
	- ./lab2_list --yield=id --lists=4 --threads=1 --iterations=4 >> lab2b_list.csv
	- ./lab2_list --yield=id --lists=4 --threads=1 --iterations=8  >> lab2b_list.csv
	- ./lab2_list --yield=id --lists=4 --threads=1 --iterations=16  >> lab2b_list.csv
	- ./lab2_list --yield=id --lists=4 --threads=4 --iterations=1  >> lab2b_list.csv
	- ./lab2_list --yield=id --lists=4 --threads=4 --iterations=2 >> lab2b_list.csv
	- ./lab2_list --yield=id --lists=4 --threads=4 --iterations=4  >> lab2b_list.csv
	- ./lab2_list --yield=id --lists=4 --threads=4 --iterations=8  >> lab2b_list.csv
	- ./lab2_list --yield=id --lists=4 --threads=4 --iterations=16  >> lab2b_list.csv
	- ./lab2_list --yield=id --lists=4 --threads=8 --iterations=1 >> lab2b_list.csv
	- ./lab2_list --yield=id --lists=4 --threads=8 --iterations=2  >> lab2b_list.csv
	- ./lab2_list --yield=id --lists=4 --threads=8 --iterations=4  >> lab2b_list.csv
	- ./lab2_list --yield=id --lists=4 --threads=8 --iterations=8 >> lab2b_list.csv
	- ./lab2_list --yield=id --lists=4 --threads=8 --iterations=16 >> lab2b_list.csv
	- ./lab2_list --yield=id --lists=4 --threads=12 --iterations=1  >> lab2b_list.csv
	- ./lab2_list --yield=id --lists=4 --threads=12 --iterations=2  >> lab2b_list.csv
	- ./lab2_list --yield=id --lists=4 --threads=12 --iterations=4  >> lab2b_list.csv
	- ./lab2_list --yield=id --lists=4 --threads=12 --iterations=8  >> lab2b_list.csv
	- ./lab2_list --yield=id --lists=4 --threads=12 --iterations=16  >> lab2b_list.csv
	- ./lab2_list --yield=id --lists=4 --threads=16 --iterations=1  >> lab2b_list.csv
	- ./lab2_list --yield=id --lists=4 --threads=16 --iterations=2  >> lab2b_list.csv
	- ./lab2_list --yield=id --lists=4 --threads=16 --iterations=4  >> lab2b_list.csv
	- ./lab2_list --yield=id --lists=4 --threads=16 --iterations=8  >> lab2b_list.csv
	- ./lab2_list --yield=id --lists=4 --threads=16 --iterations=16  >> lab2b_list.csv

	./lab2_list --yield=id --lists=4 --threads=1 --iterations=1 --sync=m >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=1 --iterations=2 --sync=m >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=1 --iterations=4 --sync=m >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=1 --iterations=8 --sync=m >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=1 --iterations=16 --sync=m >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=4 --iterations=1 --sync=m >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=4 --iterations=2 --sync=m >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=4 --iterations=4 --sync=m >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=4 --iterations=8 --sync=m >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=4 --iterations=16 --sync=m >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=8 --iterations=1 --sync=m >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=8 --iterations=2 --sync=m >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=8 --iterations=4 --sync=m >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=8 --iterations=8 --sync=m >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=8 --iterations=16 --sync=m >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=12 --iterations=1 --sync=m >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=12 --iterations=2 --sync=m >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=12 --iterations=4 --sync=m >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=12 --iterations=8 --sync=m >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=12 --iterations=16 --sync=m >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=16 --iterations=1 --sync=m >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=16 --iterations=2 --sync=m >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=16 --iterations=4 --sync=m >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=16 --iterations=8 --sync=m >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=16 --iterations=16 --sync=m >> lab2b_list.csv

	./lab2_list --yield=id --lists=4 --threads=1 --iterations=1 --sync=s >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=1 --iterations=2 --sync=s >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=1 --iterations=4 --sync=s >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=1 --iterations=8 --sync=s >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=1 --iterations=16 --sync=s >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=4 --iterations=1 --sync=s >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=4 --iterations=2 --sync=s >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=4 --iterations=4 --sync=s >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=4 --iterations=8 --sync=s >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=4 --iterations=16 --sync=s >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=8 --iterations=1 --sync=s >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=8 --iterations=2 --sync=s >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=8 --iterations=4 --sync=s >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=8 --iterations=8 --sync=s >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=8 --iterations=16 --sync=s >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=12 --iterations=1 --sync=s >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=12 --iterations=2 --sync=s >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=12 --iterations=4 --sync=s >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=12 --iterations=8 --sync=s >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=12 --iterations=16 --sync=s >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=16 --iterations=1 --sync=s >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=16 --iterations=2 --sync=s >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=16 --iterations=4 --sync=s >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=16 --iterations=8 --sync=s >> lab2b_list.csv
	./lab2_list --yield=id --lists=4 --threads=16 --iterations=16 --sync=s >> lab2b_list.csv

	# lab2b_4.png: (symc=m) without yields, for 1000 iterations, 1,2,4,8,12 threads, and 1,4,8,16 lists
	./lab2_list --threads=1 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=2 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=4 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=8 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=12 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=16 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=24 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --lists=4 --threads=1 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --lists=4 --threads=2 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --lists=4 --threads=4 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --lists=4 --threads=8 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --lists=4 --threads=12 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --lists=8 --threads=1 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --lists=8 --threads=2 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --lists=8 --threads=4 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --lists=8 --threads=8 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --lists=8 --threads=12 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --lists=16 --threads=1 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --lists=16 --threads=2 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --lists=16 --threads=4 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --lists=16 --threads=8 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --lists=16 --threads=12 --iterations=1000 --sync=m >> lab2b_list.csv

	# lab2b_5.png: (symc=s) without yields, for 1000 iterations, 1,2,4,8,12 threads, and 1,4,8,16 lists
	./lab2_list --threads=1 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --threads=2 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --threads=4 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --threads=8 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --threads=12 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --threads=16 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --threads=24 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --lists=4 --threads=1 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --lists=4 --threads=2 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --lists=4 --threads=4 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --lists=4 --threads=8 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --lists=4 --threads=12 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --lists=8 --threads=1 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --lists=8 --threads=2 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --lists=8 --threads=4 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --lists=8 --threads=8 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --lists=8 --threads=12 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --lists=16 --threads=1 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --lists=16 --threads=2 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --lists=16 --threads=4 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --lists=16 --threads=8 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --lists=16 --threads=12 --iterations=1000 --sync=s >> lab2b_list.csv


profile:
	LD_PRELOAD=/usr/lib64/libprofiler.so
	CPUPROFILE=./raw.gperf ./lab2_list --threads=12 --iterations=1000 --sync=s
	pprof --text lab2_list raw.gperf > profile.out
	pprof --list=thread_function lab2_list raw.gperf >> profile.out
	rm -rf raw.gperf


graphs: tests
	./lab2_list.gp

dist: graphs profile
		tar -czvf lab2b-504958165.tar.gz lab2_list.c \
		SortedList.h SortedList.c Makefile README lab2b_list.csv \
		lab2b_1.png lab2b_2.png lab2b_3.png lab2b_4.png lab2b_5.png  \
		lab2_list.gp profile.out
