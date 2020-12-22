all : COMPILATION EXEC

COMPILATION : telecam.c transferts.c photo_auto.c auto.c net.c connexions.c usb_transactions.c transactions.c errors.c scripts.c selection_optimale.cpp
	g++ -I/usr/xenomai/include/mercury -I/usr/xenomai/include -march=armv4t -D_GNU_SOURCE -D_REENTRANT -fasynchronous-unwind-tables -D__MERCURY__ -I/usr/xenomai/include/alchemy -Wl,--no-as-needed -lalchemy -lcopperplate /usr/xenomai/lib/xenomai/bootstrap.o -Wl,--wrap=main -Wl,--dynamic-list=/usr/xenomai/lib/dynlist.ld -L/usr/xenomai/lib -lmercury -lpthread -lrt -march=armv4t -lalchemy -o telecam telecam.c transferts.c photo_auto.c auto.c net.c connexions.c selection_optimale.cpp manage_selected.cpp errors.c scripts.c transactions.c usb_transactions.c -lgphoto2 -lcurl -lexiv2

EXEC : ./telecam
