all : MANAGE TRANSFERT2 SYNC USER ARCHITECTURE CONSTRUCTION CAPTURE_EXEC CAPTURE_ERRORS MANAGE_SELECTED TRANSFERTS

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

MODEL : model.c
	gcc -o models model.c -lcurl

SYNC : sync.o
	gcc -o sync sync.o

USER : user.o 
	gcc -o user user.o

CAPTURE_EXEC : capture_exec.o
	gcc -o capture_exec capture_exec.o

CAPTURE_ERRORS : capture_errors.o
	gcc -o capture_errors capture_errors.o -lcurl

MANAGE_SELECTEDS : manage_selected.cpp
	g++ -o selected selected.cpp manage_selected.cpp -lcurl -lexiv2 -lgphoto2 -lgphoto2_port

TRANSFERTS : transferts.c
	gcc -Wall -o transferts transferts.c net.c -lcurl -lexiv2 -lgphoto2 -lgphoto2_port

MANAGE_OPTIMALE : selection_optimale.cpp
	g++ -o selection_optimale selection_optimale.cpp manage_selected.cpp -lcurl -lexiv2 -lgphoto2 -lgphoto2_port

ARCHITECTURE : architecture.o 
	gcc -o architecture architecture.o

GENERATE_ERRORS : generate_errors.c
	gcc -o generate_errors generate_errors.c -lcurl

AUTO : auto.c manage_auto.c
	gcc -o auto manage_auto.c auto.c -lgphoto2

SELECTION : selection.cpp manage_selected.cpp
	g++ -o selection selection.cpp manage_selected.cpp -lcurl -lexiv2 -lgphoto2 -lgphoto2_port

WIFI : wifi_status.c
	gcc -o wifi_status wifi_status.c

VIDEO_AUTO : video_auto.c auto.c
	gcc -o video_auto video_auto.c auto.c -lgphoto2 -lgphoto2_port

PHOTO_AUTO : photo_auto.c auto.c
	gcc -o photo_auto photo_auto.c auto.c -lgphoto2 -lgphoto2_port

CONSTRUCTION : architecture
	./architecture
