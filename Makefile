all: guesthouse
DBSE : gcc -shared src/sqlite3.c -o sqlite3.dll 
guesthouse: src/*.cpp
	
	g++ src/*.cpp sqlite3.dll -Iinclude -o guesthouse 

run: guesthouse
	./guesthouse

clean:
	rm -rf guesthouse

