all: guesthouse

guesthouse: src/*.cpp
	gcc -shared src/sqlite3.c -o sqlite3.dll 
	g++ src/*.cpp sqlite3.dll -Iinclude -o guesthouse 

run: guesthouse
	./guesthouse

clean:
	rm -rf guesthouse

