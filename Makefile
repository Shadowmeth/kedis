all:
	g++ -Wall -Wextra -O2 -g ./server/server.cpp -o ./server/server
	g++ -Wall -Wextra -O2 -g ./client/client.cpp -o ./client/client
