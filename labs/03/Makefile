all:
	flex lexical_scan.l
	gcc lex.yy.c -o lexical_scan -ll
	python3 code_generator.py > example.ac
clean:
	rm *.c || true
	rm lexical_scan || true
	rm *.ac || true
	rm *.out || true

