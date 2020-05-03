all:
	yacc -d syntax_analyzer.y
	flex syntax_analyzer.l
	gcc -w y.tab.c lex.yy.c -ll -o syntax_analyzer

clean:
	rm -rf y.tab.c y.tab.h lex.yy.c syntax_analyzer
