
SRC = mhx.c
OUT = mhx


CFLAGS = -O2 -s -Wall

CC = gcc

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

clean:
	rm -f $(OUT)


install: $(OUT)
	cp $(OUT) /usr/local/bin/

uninstall: $(OUT)
        rm /usr/local/bin/$(OUT)

.PHONY: all clean install

