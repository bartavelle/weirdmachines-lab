
interpreter: interpreter.c
	$(CC) -Wall -O0 -Wall -o interpreter interpreter.c

clean:
	rm -f interpreter

test: interpreter
	./interpreter `./exploit` SECRET123

frama-c:
	frama-c-gui -main frama_c_main -slevel 10 -val interpreter.c

