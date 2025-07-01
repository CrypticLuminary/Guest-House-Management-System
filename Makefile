# Current Date and Time (UTC - YYYY-MM-DD HH:MM:SS formatted): 2025-07-01 13:53:20
# Current User's Login: CrypticLuminary

# Default target
all: guesthouse

# Build SQLite3 DLL
DBSE: 
	gcc -shared src/sqlite3.c -o sqlite3.dll 

# Build main executable
guesthouse: src/*.cpp sqlite3.dll
	g++ src/*.cpp sqlite3.dll -Iinclude -o guesthouse 

# Run the program
run: guesthouse
	./guesthouse

# Clean build files
clean:
	rm -rf ./guesthouse.exe ./guesthouse ./sqlite3.dll

# Phony targets (these are not files)
.PHONY: all run clean DBSE