CC=cc

.PHONY: sxl

sxl: sxl.c 
	@$(CC) -o $@ $^ 

install: sxl
	@cp $< /usr/bin/

clean: sxl
	@rm $<
