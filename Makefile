CC=clang

# Use gcc instead of clang on windows because it has better posix support
ifeq ($(OS),Windows_NT)
  CC=gcc
endif

program.out: aul.tab.o lex.yy.o ast.o type.o symbols.o main.o
	$(CC) $^ -o $@

%.o: %.c
	$(CC) -c $< -o $@

%.tab.c: %.y
	bison -d $^

lex.yy.c: aul.l
	flex $^

clean:
	rm -f lex.yy.c aul.tab.* *.out *.o
