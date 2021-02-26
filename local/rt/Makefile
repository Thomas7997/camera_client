XENO_CONFIG := /usr/xenomai/bin/xeno-config
CFLAGS := $(shell $(XENO_CONFIG) --cflags --skin=alchemy) -g
LDFLAGS := $(shell $(XENO_CONFIG) --ldflags --skin=alchemy) -lgphoto2 -lcurl -lexiv2
CC := g++
SOURCES := telecam.c transferts.c video_auto.c photo_auto.c auto.c net.c connexions.c selection_optimale.cpp manage_selected.cpp errors.c scripts.c transactions.c usb_transactions.c model.c sd.c

EXECUTABLE := telecam

all: $(EXECUTABLE)

%: $(SOURCES)
	$(CC) -o $(EXECUTABLE) $(SOURCES) $(CFLAGS) $(LDFLAGS)
