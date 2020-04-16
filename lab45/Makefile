all: sc

sc: main.o myBigChars.o myReadkey.o myTerm.o SimpleComputer.o
	g++ -Wall -g main.o myBigChars.o myReadkey.o myTerm.o SimpleComputer.o -o sc

main.o: main.cpp
	g++ -Wall -c -g main.cpp -o main.o

myBigChars.o: myBigChars.cpp
	g++ -Wall -c -g myBigChars.cpp -o myBigChars.o

myReadkey.o: myReadkey.cpp
	g++ -Wall -c -g myReadkey.cpp -o myReadkey.o

myTerm.o: myTerm.cpp
	g++ -Wall -c -g myTerm.cpp -o myTerm.o

SimpleComputer.o: SimpleComputer.cpp
	g++ -Wall -c -g SimpleComputer.cpp -o SimpleComputer.o

clean:
	rm -rf *.o sc
