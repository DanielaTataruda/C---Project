CC		= gcc
CFLAGS	= `pkg-config --cflags gtk+-3.0` -std=c99
LDLIBS	= `pkg-config --libs gtk+-3.0` -lm -lpng
FILES	= $(wildcard kody/*.c)

projekt: $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o projekt $(LDLIBS)

clean: projekt
	rm projekt
