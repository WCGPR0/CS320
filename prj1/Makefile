CFLAGS=-g -w -Wall -Wextra -Isrc -rdynamic -DNDEBUG
CC=g++

SOURCES=wu_prj1.cpp
OBJECTS=predictors

all: 
	$(CC) $(SOURCES) -o $(OBJECTS)


clean:
	rm -rf build $(OBJECTS)
	find . -name "*.gc" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`

BADFUNCS='[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)'
check:
	@echo Files with potential bad functions
	@egrep $(BADFUNCS) $(SOURCES) || true
dist:
	tar -czvf vwu4.tar.gz $(SOURCES) Makefile wu_prj1report.pdf
