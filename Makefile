CC=clang
CFLAGS=-Wall -Isrc/include -g -std=c11
LDFLAGS=
FILENAME=myshellin
SRC=src/myshellin.c src/loop.c src/console_line.c src/array.c src/builtins.c src/launch.c src/utils.c src/redirect.c
OBJ=$(SRC:.c=.o)

all: shell doc report

shell: $(OBJ)
	$(CC) $(CFLAGS) -o $(FILENAME) $^ $(LDFLAGS)

doc:
# if doxygen is install create the code documentation
ifneq (, $(shell which doxygen))
	doxygen doxygen.conf
	make -C docs/latex
	mv docs/latex/refman.pdf refman.pdf
endif

report:
# if pdflatex is installed create the report
ifneq (, $(shell which pdflatex))
	make -C report
	mv report/Informe.pdf Informe.pdf
else ifneq (, $(shell which pdftex))
	make -C report
	mv report/Informe.pdf Informe.pdf
endif

clean: cleanshell cleandoc cleanreport

cleanshell:
	rm -f src/*.o $(FILENAME)

cleandoc:
	rm -rf refman.pdf docs

cleanreport:
	make -C report clean
	rm -f Informe.pdf

.PHONY: all shell doc report clean cleanshell cleandoc cleanreport
