CC=clang

# Use gcc instead of clang on windows because it has better posix support
ifeq ($(OS),Windows_NT)
  CC=gcc
endif

program.out: main.c aul.tab.c lex.yy.c ast.c
	$(CC) $^ -o $@

%.tab.c: %.y
	bison -d $^

lex.yy.c: aul.l
	flex $^

clean:
	rm -f lex.yy.c aul.tab.* *.out *.o
