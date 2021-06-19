CC=clang
CFLAGS=-Wall -Isrc/include -DDEBUG -g
LDFLAGS=-lm
SRC=src/myshellin.c
OBJ=$(SRC:.c=.o)

all: myshellin informe

myshellin: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

informe:
# if pdflatex is installed create the informe
ifneq (, $(shell which pdflatex))
	make -C doc
	mv doc/Informe.pdf Informe.pdf
endif

clean: cleanmyshellin cleaninforme

cleanmyshellin:
	rm -f src/*.o myshellin

cleaninforme:
	make -C doc clean
	rm -f Informe.pdf

.PHONY: all myshellin informe clean cleanmyshellin cleaninforme
