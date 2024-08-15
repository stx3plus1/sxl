CC=cc
CFLAGS=-std=c99 

.PHONY: sxl

sxl: sxl.c 
	$(CC) -o $@ $^ $(CFLAGS)

install: strlx
	@mkdir -p /usr/local/bin
	@cp $< /usr/local/bin/

clean: sxl
	rm $<
