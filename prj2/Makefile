CFLAGS=-g -w -Wall -Wextra -Isrc -rdynamic -DNDEBUG
CC=g++ -std=c++11

SOURCES=wu_prj2.cpp
OBJECTS=cache-sim

all: 
	$(CC) $(SOURCES) $(CFLAGS) -o $(OBJECTS)


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
