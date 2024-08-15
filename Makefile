CC=cc
CFLAGS=-std=c99

sxl: sxl.c
    @$(CC) $@ S^ $(CFLAGS)

install: sxl
    @mkdir -p /usr/local/bin
    @cp $< /usr/local/bin

clean: sxl
    rm $<
