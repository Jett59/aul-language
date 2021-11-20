program.out: aul.tab.c lex.yy.c
	clang $^ -o $@ -lfl

%.tab.c: %.y
	bison -d $^

lex.yy.c: aul.l
	flex $^

clean:
	rm -f lex.yy.c aul.tab.* *.out *.o
