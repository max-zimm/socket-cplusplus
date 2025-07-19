g++ socket/server.cpp -o server -lws2_32
g++ socket/client.cpp -o client -lws2_32

./server &
sleep 1



./client

rm -f client
rm -f server