EXE = froud
all: $(EXE)
$(EXE): main.o
	g++ main.o -o $(EXE)

main.o: main.cpp
	g++ -I.. main.cpp -c -o main.o

.PHONY: run clean

run: $(EXE)
	./$(EXE)

clean:
	rm -f *.o $(EXE)
