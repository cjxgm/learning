test:
	./digraph.pl demo.rst | rst2latex | pdflatex
	mv texput.pdf demo.pdf
	rm -f texput.*
	rm -rf digraph/
	evince demo.pdf

