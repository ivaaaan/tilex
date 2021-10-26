all: compile

compile:
	gcc main.c -o ./tilex -lX11

format:
	clang-format -i -style=WebKit *.c
