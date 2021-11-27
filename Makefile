CC=clang

program-suffix:=

ifeq ($(OS),Windows_NT)
  program-suffix:=.exe
  shared-library-suffix:=.dll
# Use gcc instead of clang on windows because it has better posix support
  CC=gcc
  else ifeq ($(shell uname -s),Darwin)
    shared-library-suffix:=.dylib
	else
	shared-library-suffix:=.so
  endif

LIB-OBJS:=error.o
LIB-OBJS+=aul.tab.o lex.yy.o ast.o type.o
LIB-OBJS+=symbols.o semantics.o
PROGRAM-OBJS+=main.o

PROGRAM:=build/bin/aulc$(program-suffix)

LIBNAME:=aul
LIB:=build/lib/libaul$(shared-library-suffix)

CFLAGS:=-fPIC -std=gnu11
LDFLAGS:=-Lbuild/lib

ifneq ($(OS),Windows_NT)
  ifeq ($(shell uname -s),Darwin)
    LDFLAGS+=-Wl,-rpath,'@loader_path'
    else
    LDFLAGS+=-Wl,-rpath,'$$ORIGIN'
  endif
endif

$(PROGRAM): $(LIB) $(PROGRAM-OBJS)
	mkdir -p build/bin
	cp $(LIB) build/bin/
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(PROGRAM-OBJS) -laul

$(LIB): $(LIB-OBJS)
	mkdir -p build/lib
	$(CC) $(CFLAGS) $(LDFLAGS) -shared $^ -o $@

%.tab.c: %.y
	bison -d $^

lex.yy.c: aul.l
	flex $^

clean:
	rm -rf lex.yy.c aul.tab.* *.o build/
