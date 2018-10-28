LATEX = latex
BIBTEX = bibtex
PDFLATEX = pdflatex

PARTS = STLmanual.tex intro.tex g711.tex g711iplc.tex g726.tex g727.tex g728.tex g722.tex rpe.tex rate.tex eid.tex mnru.tex sv56.tex reverb.tex truncate.tex freqresp.tex stereoop.tex basop.tex utl.tex bibliography.bib

STLmanual.pdf : $(PARTS)
	$(PDFLATEX) $<; $(BIBTEX) $*; $(PDFLATEX) $<; $(PDFLATEX) $<

clean:
	\rm -f STLmanual.pdf *.aux *.log *.out *.toc
