.PHONY: globalFunc clean

SCENARIO ?= DEFAULT

ifeq ($(SCENARIO), SINGLE)
globalFunc: functions
else ifeq ($(SCENARIO), STATIC)
globalFunc: staticFunctions.a functions
else ifeq ($(SCENARIO), DYNAMIC)
globalFunc: dynamicFunctions.so functions
endif

functions: main.o func1.o func2.o
	gcc main.o func1.o func2.o -o functions

main.o: main.c
	gcc -c main.c -o main.o

func1.o: func1.c
	gcc -c func1.c -o func1.o

func2.o: func2.c
	gcc -c func2.c -o func2.o

staticFunctions.a: func1.o func2.o
	ar rcs staticFunctions.a func1.o func2.o

dynamicFunctions.so: func1.o func2.o
	gcc -shared -o dynamicFunctions.so func1.o func2.o

clean:
	rm -f *.o *.a *.so functions
