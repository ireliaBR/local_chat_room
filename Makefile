

server:
	gcc -I ./src/include/ ./src/server.c ./src/include/*.c -g -o ./build/server
client:
	gcc -I ./src/include/ ./src/client.c ./src/include/*.c -g -o ./build/client
test:
	gcc -I ./src/include/ ./test/test.c ./src/include/*.c -g -o ./build/test
clean:
	rm -rf ./build/*
