all:
	g++ -std=c++11 *.cpp -o main.exe

clean:
	rm -f *.o main.exe *.binary *.txt;
	

run:
	./main.exe
