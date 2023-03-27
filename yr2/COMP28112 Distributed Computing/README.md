# COMP28112
## COMP28112.1 Servers and Clients
Investigate 3 ways of client-server interaction: socket, servlet and RMI(Java's RPC implementation, deprecated after i.8)


### 1. Getting Started
#### 1.1 Socket
Server: 
```
# compile
javac SocketServer.java
# run
java SocketServer
```
Client:

After the server terminal says 'Server starts running', start another terminal, enter cmd:
```
telnet (name of your PC) 8181
```
port 8181 is listened by server which can be changed in SocketServer.java. If successful, the terminal will show(foo is my PC name):
```
Trying (your IP address)...
Connected to foo.lan.
Escape character is '^]'.
```
now type 'whoRU' or other random string to see different response. The telnet will end immediately after getting response, restart it to try with other messages.

#### 1.2 RMI
Server:
```
# run rmiregistry
rmiregistry &
# compile
javac RMServer.java
# run
java RMServer
```
Client:

After the server terminal says 'Server starts running', start another terminal.
```
# compile
javac RMClient.java
# run
java RMClient (arg1) (arg2)
```
where arg1 is your PC name, arg2 is whoRU or other random string to play with. Client will stop immediately after getting response.

#### 1.3 Servlet
Server:
```
javac -classpath ./jsdk.jar SimpleServlet.java
./servletrunner -d (absolue path of ex1 folder)
```
(Error: Could not find or load main class sun.servlet.http.HttpServlet?)


### 2. Technologies
Java (for RMI, only version before 1.9 allowed)



## COMP28112.2 Hotel Booking
We have a server that records hotel and band slots (smaller number for earlier slots), implement a client for booking hotel rooms and band slots and their number must match. Manually introduced random server down may appear. Anyone can hold at most 2 slots at the same time. Functionalities need to implement include: booking a slot (with possible error handling); cancel booking for a slot (with possible error handling); checking slots you have booked; finding free slots.


### 1. Getting Started
Client:
```
# compile
./compile
# run
./run
```
Server:
not available, held by professor

### 2. Technologies
Java, XML, HTTP



## COMP28112.3 Multi-instance server simulation
Simulation of multi-server query processing pipeline with statistics gathered, a simpler, single thread version of USC CSCI-402.


### 1. Getting Started
```
python lab3.py (queue_size) (arrival_rate) (number_of_servers) (execution_time)
```


### 2. Technologies
python