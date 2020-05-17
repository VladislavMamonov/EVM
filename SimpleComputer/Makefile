all: bin/sc

bin/sc: build/main.o build/myBigChars.o build/myReadkey.o build/myTerm.o build/cpu.o build/SimpleComputer.o
	g++ -Wall -g build/main.o build/myBigChars.o build/myReadkey.o build/myTerm.o build/cpu.o build/SimpleComputer.o -o bin/sc

build/main.o: main.cpp
	g++ -Wall -c -g main.cpp -o build/main.o

build/myBigChars.o: myBigChars.cpp
	g++ -Wall -c -g myBigChars.cpp -o build/myBigChars.o

build/myReadkey.o: myReadkey.cpp
	g++ -Wall -c -g myReadkey.cpp -o build/myReadkey.o

build/myTerm.o: myTerm.cpp
	g++ -Wall -c -g myTerm.cpp -o build/myTerm.o

build/cpu.o: cpu.cpp
	g++ -Wall -c -g cpu.cpp -o build/cpu.o

build/SimpleComputer.o: SimpleComputer.cpp
	g++ -Wall -c -g SimpleComputer.cpp -o build/SimpleComputer.o

clean:
	rm -rf build/*.o bin/sc
