#game:
#	g++ main.cpp -lraylib -o main

editor:
	g++ editor.cpp -lraylib -o editor

clean:
	rm editor main
