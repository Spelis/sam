game:
	g++ main.cpp -lraylib -o main -g

editor:
	g++ editor.cpp -lraylib -o editor

clean:
	rm editor main
