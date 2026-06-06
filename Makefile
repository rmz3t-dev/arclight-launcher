all:
	clang -o server server.cpp -static

start-server:
	./server