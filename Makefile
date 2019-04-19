CFLAGS= -g -Wall -pedantic -std=c99
LFLAGS= -g

all: test-colapri-arreglo test-colapri-lista

colapri-arreglo.o: colapri.h colapri-arreglo.h

colapri-lista.o: colapri.h colapri-lista.h

test-colapri-arreglo.o: colapri.h colapri-arreglo.h

test-colapri-lista.o: colapri.h colapri-arreglo.h colapri-lista.h

test-colapri-arreglo: colapri-arreglo.o test-colapri-arreglo.o
	gcc $(LFLAGS) -o test-colapri-arreglo colapri-arreglo.o test-colapri-arreglo.o
	
test-colapri-lista: colapri-arreglo.o colapri-lista.o test-colapri-lista.o
	gcc $(LFLAGS) -o test-colapri-lista colapri-arreglo.o colapri-lista.o test-colapri-lista.o

clean:
	rm -f *.o test-colapri-arreglo test-colapri-lista
