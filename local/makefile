all : TRANSFERT TRANSFERT2 SYNC USER ARCHITECTURE CONSTRUCTION

sync.o : sync.c 
	gcc -c -Wall sync.c

transfert2.o : transfert2.c
	gcc -c -Wall transfert2.c

architecture.o : architecture.c
	gcc -c -Wall architecture.c

user.o : user.c
	gcc -c -Wall user.c

capture.o : capture.c
	gcc -c -Wall capture.c

CAPTURE : capture.o
	gcc -Wall -o capture capture.o

TRANSFERT : transfert.o
	gcc -o transfert transfert.o

TRANSFERT2 : transfert2.o
	gcc -o transfert2 transfert2.o

SYNC : sync.o
	gcc -o sync sync.o

USER : user.o 
	gcc -o user user.o

ARCHITECTURE : architecture.o 
	gcc -o architecture architecture.o

CONSTRUCTION : architecture
	./architecture
