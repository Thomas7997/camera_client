all : MANAGE TRANSFERT2 SYNC USER ARCHITECTURE CONSTRUCTION CAPTURE_EXEC CAPTURE_ERRORS MANAGE_SELECTED

manage_selected.o : manage_selected.c
	gcc -c -Wall manage_selected.c

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

capture_exec.o : capture_exec.c
	gcc -c -Wall capture_exec.c

capture_errors.o : capture_errors.c
	gcc -c -Wall capture_errors.c

MANAGE_SELECTED : manage_selected.c
	gcc -o manage_selected manage_selected.c -lcurl

MANAGE : manage_captures.o
	gcc -o manage manage_captures.o -lcurl

CAPTURE : capture.o
	gcc -o capture capture.o -lcurl

TRANSFERT2 : transfert2.o
	gcc -o transfert2 transfert2.o

SYNC : sync.o
	gcc -o sync sync.o

USER : user.o 
	gcc -o user user.o

CAPTURE_EXEC : capture_exec.o
	gcc -o capture_exec capture_exec.o

CAPTURE_ERRORS : capture_errors.o
	gcc -o capture_errors capture_errors.o -lcurl

ARCHITECTURE : architecture.o 
	gcc -o architecture architecture.o

CONSTRUCTION : architecture
	./architecture
