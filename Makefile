all: guesthouse

guesthouse: src/*.cpp
	g++ src/*.cpp -Iinclude -o guesthouse

run: guesthouse
	./guesthouse

clean:
	rm -rf guesthouse
