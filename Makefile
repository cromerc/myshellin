CC=clang
CFLAGS=-Wall -Isrc/include -std=c11
LDFLAGS=
FILENAME=myshellin
SRC=src/myshellin.c src/loop.c src/console_line.c src/array.c src/builtins.c src/launch.c src/utils.c src/redirect.c
OBJ=$(SRC:.c=.o)

all: shell informe

shell: $(OBJ)
	$(CC) $(CFLAGS) -o $(FILENAME) $^ $(LDFLAGS)

report:
# if pdflatex is installed create the report
ifneq (, $(shell which pdflatex))
	make -C doc
	mv doc/Informe.pdf Informe.pdf
else ifneq (, $(shell which pdftex))
	make -C doc
	mv doc/Informe.pdf Informe.pdf
endif

clean: cleanshell cleanreport

cleanshell:
	rm -f src/*.o $(FILENAME)

cleanreport:
	make -C doc clean
	rm -f Informe.pdf

.PHONY: all shell report clean cleanshell cleanreport
