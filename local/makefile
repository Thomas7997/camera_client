all : MANAGE TRANSFERT2 SYNC USER ARCHITECTURE CONSTRUCTION

sync.o : sync.c 
	gcc -c -Wall sync.c

manage_captures.o : manage_captures.c
	gcc -c -Wall manage_captures.c

transfert2.o : transfert2.c
	gcc -c -Wall transfert2.c

architecture.o : architecture.c
	gcc -c -Wall architecture.c

user.o : user.c
	gcc -c -Wall user.c

capture.o : capture.c
	gcc -c -Wall capture.c

MANAGE : manage_captures.o
	gcc -o manage manage_captures.o -lcurl

CAPTURE : capture.o
	gcc -o capture capture.o

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
