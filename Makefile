program.out: aul.tab.c lex.yy.c ast.c
	clang $^ -o $@ -lfl

%.tab.c: %.y
	bison -v -d $^

lex.yy.c: aul.l
	flex $^

clean:
	rm -f lex.yy.c aul.tab.* *.out *.o
