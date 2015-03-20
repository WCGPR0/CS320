CFLAGS=-g -Wall -Wextra -Isrc -rdynamic -DNDEBUG
CC=g++ 

SOURCES=wu_prj1.cpp

all: 
	$(CC) $(CFLAGS) $(SOURCES)


clean:
	rm -rf build $(OBJECTS)
	find . -name "*.gc" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`

BADFUNCS='[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)'
check:
	@echo Files with potential bad functions
	@egrep $(BADFUNCS) $(SOURCES) || true
