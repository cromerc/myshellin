all: report

report:
	# we do this twice to make sure the toc is updated
	pdflatex -synctex=1 -interaction=nonstopmode "Informe.tex"
	pdflatex -synctex=1 -interaction=nonstopmode "Informe.tex"

clean:
	rm -f Informe.pdf
	find . -type f -name '*.log' -exec rm {} +
	find . -type f -name '*.toc' -exec rm {} +
	find . -type f -name '*.gz' -exec rm {} +
	find . -type f -name '*.aux' -exec rm {} +
	find . -type f -name '*.out' -exec rm {} +

.PHONY: all report clean
