CC = gcc
CFLAGS += -Wall -std=c99 -g
LDLIBS = -lgcov

driver: driver.o map.o value.o input.o
doubleTest: doubleTest.o value.o
stringTest: stringTest.o value.o
mapTest: mapTest.o map.o value.o

doubleTest.o: doubleTest.c value.c
stringTest.o: stringTest.c value.c
mapTest.o: mapTest.c map.c value.c
driver.o: driver.c map.c value.c input.c
map.o: map.c value.c
value.o: value.c
input.o: input.c

doubleTest.c: value.h
stringTest.c: value.h
mapTest.c: map.h value.h
driver.c: map.h value.h input.h
map.c: map.h value.h
value.c: value.h
input.c: input.h

map.h: value.h input.h
value.h: input.h

clean:
	rm -f doubleTest stringTest mapTest driver doubleTest.o stringTest.o mapTest.o driver.o map.o value.o input.o *.gcda *gcno *gcov