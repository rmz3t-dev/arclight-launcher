all:
	clang -o server server.cpp cJSON/cJSON.c -static -w

start-server:
	./server