all: guesthouse
DBSE : gcc -shared src/sqlite3.c -o sqlite3.dll 
guesthouse: src/*.cpp
	
	g++ src/*.cpp sqlite3.dll -Iinclude -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32 -o guesthouse 

run: guesthouse
	./guesthouse

clean:
	rm -rf guesthouse

