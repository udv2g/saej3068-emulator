CFLAGS=
DEPS = lincp-emu.h lin-cp.h info_code_handler.h ids_handler.h protocol_version_handler.h globals.h info_codes.h InfoCodeStrings.h config.h j3072_handler.h
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
    RTFLAGS=""
    CC=clang
else
    RTFLAGS="-lrt"
    CC=gcc
endif

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

%.ev.o: %.c $(DEPS)
	$(CC) -c -o $*.ev.o $< $(CFLAGS) -D EV_CONFIG

%.se.o: %.c $(DEPS)
	$(CC) -c -o $*.se.o $< $(CFLAGS)

all: bus ev se

bus: msleep.o lincp-emu.o bus.c $(DEPS)
	$(CC) $(RTFLAGS) -o bus bus.c msleep.o lincp-emu.o $(CFLAGS)

ev: msleep.o lincp-emu.o lincp.ev.o lin-cp.ev.o globals.o info_code_handler.ev.o protocol_version_handler.ev.o ids_handler.ev.o scheduler.o j3072_handler.ev.o $(DEPS)
	$(CC) $(RTFLAGS) -o ev lincp.ev.o msleep.o lincp-emu.o lin-cp.ev.o globals.o info_code_handler.ev.o protocol_version_handler.ev.o ids_handler.ev.o scheduler.o j3072_handler.ev.o $(CFLAGS) -D EV_CONFIG

se: msleep.o lincp-emu.o lincp.se.o lin-cp.se.o info_code_handler.se.o protocol_version_handler.se.o ids_handler.se.o scheduler.o j3072_handler.se.o $(DEPS)
	$(CC) $(RTFLAGS) -o se lincp.se.o msleep.o lincp-emu.o lin-cp.se.o globals.o info_code_handler.se.o protocol_version_handler.se.o ids_handler.se.o scheduler.o j3072_handler.se.o $(CFLAGS)

clean:
	rm *.o ev se bus
