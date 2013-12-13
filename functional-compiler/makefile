all: to-html
clean:
	rm -rf digraph/
	rm -f demo.html
to-html: demo.rst
	./digraph.pl demo.rst | rst2html > demo.html
test: all
	xdg-open demo.html > /dev/null
commit: clean
	git add . -A
	git commit -m "`date`" -p

