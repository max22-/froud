all: froud
froud: froud.o main.o
	g++ froud.o main.o -o froud

main.o: main.cpp froud.h
	g++ main.cpp -c -o main.o

froud.o: froud.cpp froud.h
	g++ froud.cpp -c -o froud.o

.PHONY: run clean

run: froud
	./froud

clean:
	rm -f *.o froud
