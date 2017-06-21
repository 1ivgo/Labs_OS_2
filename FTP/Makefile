all: main clean
main: .SERVER .CLIENT
.SERVER:
	cd server; \
	g++ server.c -o server;\
	cd ..
.CLIENT:
	cd client;\
	g++ client.c -o client;\
	cd ..
clean:
	cd server;\
	rm File.txt;\
	cd ..