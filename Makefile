TARGET = imgsize

SRC = $(wildcard src/*.c)
OBJ = $(SRC:%.c=%.o)

CFLAGS 	= -Isrc
LIBS 	= -lgd

prefix = /usr/local
PREFIX = $(prefix)

all: $(TARGET)


$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LIBS)


clean:
	$(RM) $(OBJECTS) $(TARGET)


install: imgsize
	install -m 0755 $(TARGET) $(PREFIX)/bin

uninstall: imgsize
	$(RM) $(PREFIX)/bin/$(TARGET)


.PHONY: all clean

# vim:ft=make
