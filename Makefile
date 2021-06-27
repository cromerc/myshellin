CC=clang
CFLAGS=-Wall -Isrc/include -DDEBUG -g -std=c11
LDFLAGS=
FILENAME=myshellin
SRC=src/myshellin.c src/loop.c src/console_line.c src/array.c src/builtins.c
OBJ=$(SRC:.c=.o)

all: myshellin

myshellin: $(OBJ)
	$(CC) $(CFLAGS) -o $(FILENAME) $^ $(LDFLAGS)

informe:
# if pdflatex is installed create the informe
ifneq (, $(shell which pdflatex))
	make -C doc
	mv doc/Informe.pdf Informe.pdf
else ifneq (, $(shell which pdftex))
	make -C doc
	mv doc/Informe.pdf Informe.pdf
endif

clean: cleanmyshellin

cleanmyshellin:
	rm -f src/*.o $(FILENAME)

cleaninforme:
	make -C doc clean
	rm -f Informe.pdf

.PHONY: all myshellin informe clean cleanmyshellin cleaninforme
