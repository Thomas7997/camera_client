all : TRANSFERT SYNC USER ARCHITECTURE CONSTRUCTION

sync.o : sync.c 
	gcc -c -Wall sync.c

transfert.o : transfert.c
	 gcc -c -Wall transfert.c

architecture.o : architecture.c
	gcc -c -Wall architecture.c

user.o : user.c
	gcc -c -Wall user.c

TRANSFERT : transfert.o
	gcc -o transfert transfert.o

SYNC : sync.o
	gcc -o sync sync.o

USER : user.o 
	gcc -o user user.o


ARCHITECTURE : architecture.o 
	gcc -o architecture architecture.o

CONSTRUCTION : architecture
	./architecture
