# Calculator for multiple clients and single server
Simple command line calculator using only system calls and signals in Linux. Each client sends an expression to the server, while the server calculates it and returns to each client the result.
* In the client code, we check if the user gave the right operators & valid numeric values, otherwise the client shuts down.
* The server handles only integers (positive or negative). If something else is entered in the expression, the client will receive an error message and will shut down. The calculation result will also be an integer (rounded down).
* After 60 seconds pass since the server was started or since the last request from a client arrived, the server shuts down. If now a new client tries to send a signal (SIGUSR1) to the server, the server will ignore the signal.
* Server cannot calculate X / 0. If the client sends zero as the right operand, the server will send an error message & will not send a result, therefor the client shuts down after 30 seconds.


## Getting Started
Copy the project to a local directory:
Clone with SSH 
```
git@github.com:OBerger96/calculator-clients-server.git
```
Clone with HTTPS
```
https://github.com/OBerger96/calculator-clients-server.git
```

## To Run the project
Make a directory.
```
mkdir name_dir
```
Put server.c in the directory.
```
mv from_dir/server.c to_dir/server.c
```
Put client.c in the directory.
```
mv from_dir/client.c to_dir/client.c
```
Compile server.c program.
```
gcc server.c -o server.out
```
Compile client.c program.
```
gcc client.c -o client.out
```
Run the server as a job.
```
./server.out &
```
Run the client as a job.
```
./client.out server's pid left_operand operator right_operand &

Client's arguments list:
1. server's pid
2. left_operand - an integer
3. operator - 1(+) | 2(-) | 3(*) | 4(/)
4. right_operand - an integer
```

## Screenshots
![screenshot](https://github.com/OBerger96/calculator-clients-server/blob/master/images/calculator-clients-server0.jpeg)
![screenshot](https://github.com/OBerger96/calculator-clients-server/blob/master/images/calculator-clients-server1.jpeg)

## Author
* **Omer Berger** - [calculator-multiple-clients-server](https://github.com/OBerger96/calculator-multiple-clients-server)
