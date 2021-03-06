CC=clang
CXX=clang++

program-suffix:=

ifeq ($(OS),Windows_NT)
  program-suffix:=.exe
  shared-library-suffix:=.dll
# Use gcc instead of clang on windows because it has better posix support + clang doesn't like -fPIC for some reason
  CC=gcc
  CXX=g++
  else ifeq ($(shell uname -s),Darwin)
    shared-library-suffix:=.dylib
	else
	shared-library-suffix:=.so
  endif

LIB-OBJS:=error.o
LIB-OBJS+=aul.tab.o lex.yy.o
LIB-OBJS+=ast.o astPrinter.o
PROGRAM-OBJS+=main.o

PROGRAM:=build/bin/aulc$(program-suffix)

LIBNAME:=aul
LIB:=build/lib/libaul$(shared-library-suffix)

# -I . to include flexLexer.h which may be in the current working directory.
CXXFLAGS:=-fPIC -std=gnu++17 -g -Og -I .
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
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(PROGRAM-OBJS) -laul

$(LIB): $(LIB-OBJS)
	mkdir -p build/lib
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -shared $^ -o $@

aul.tab.cc: aul.y
	bison -d --language=c++ -Wall -Wcounterexamples -Wdangling-alias $^

lex.yy.cc: aul.l
	flex $^

clean:
	rm -rf lex.yy.cc aul.tab.* *.o build/
