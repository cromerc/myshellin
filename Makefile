CC=clang
CFLAGS=-Wall -Isrc/include -g -std=c11
LDFLAGS=
FILENAME=myshellin
SRC=src/myshellin.c src/loop.c src/console_line.c src/array.c src/builtins.c src/launch.c src/utils.c src/redirect.c
OBJ=$(SRC:.c=.o)

all: shell doc report

shell:
ifneq (, $(shell which bear))

ifeq (, $(wildcard ./compile_commands.json))
	$(MAKE) shellbear
else
	$(MAKE) shellbuild
endif

else
	$(MAKE) shellbuild
endif

shellbear:
	bear make shellbuild

shellbuild: $(OBJ)
	$(CC) $(CFLAGS) -o $(FILENAME) $^ $(LDFLAGS)

doc:
# if doxygen and bear are installed create the code documentation
ifneq (, $(shell which bear))
ifeq (, $(wildcard ./compile_commands.json))
	$(MAKE) cleanshell
	$(MAKE) shellbear
endif
ifneq (, $(shell which doxygen))
	doxygen doxygen.conf
	make -C docs/latex
	mv docs/latex/refman.pdf refman.pdf
endif
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
	rm -f src/*.o $(FILENAME) compile_commands.json

cleandoc:
	rm -rf refman.pdf docs compile_commands.json

cleanreport:
	make -C report clean
	rm -f Informe.pdf

.PHONY: all shell shellbear shellbuild doc report clean cleanshell cleandoc cleanreport
