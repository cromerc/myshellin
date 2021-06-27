informe:
	# we do this twice to make sure the toc is updated
	pdflatex -synctex=1 -interaction=nonstopmode "Informe.tex"
	pdflatex -synctex=1 -interaction=nonstopmode "Informe.tex"

clean:
	rm -f *.log *.toc *.gz *.aux sections/*.aux *.out Informe.pdf

.PHONY: informe clean
