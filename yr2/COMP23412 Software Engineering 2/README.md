# COMP23412
## COMP23412.0 Hello MVC!
Get familiar with Eventlite biuld, test & run by a toy project with the same configuration.


### 1. Getting Started
Open project in Eclipse **Neon** or **Mars** (only these 2 versions are guaraneed to work with project), go to "launchers" folder.

To debug: right click "Hello MVC test.launch", then "Debug As...", go to "Debug configurations", select "Hello MVC test" under "Maven build", Click "Debug" below.

To run: right click "Hello MVC run.launch", then "Run As...", go to "Run configurations", select "Hello MVC run" under "Maven build", Click "Run" below.
This starts Tomcat and runs the server.

For web client, open a browser and go to http://localhost:8080

To play with REST API: try
```
curl -H "Accept: application/json" http//localhost:8080/api/greeting
curl -H "Accept: application/json" http//localhost:8080/api/greeting/1
```

To run server outside Eclipse: right click "Hello MVC package.launch", then "Run As...", go to "Run configurations", select "Hello MVC package" under "Maven build", Click "Run" below.
Then start the server (assume you are under project root dir)
```
java -jar target/comp23412-hello-mvc-0.1.0.jar
```
Now you can go to the client and have a look.

If you face the error "Tomcat failed to start because some other processes are occupying port 8080", that may indicate you didn't completely stop running the server from Eclipse. Under Windows, Ctrl+Alt+Del to open process management and kill the Java SE Bianry, now try again.


### 2. Technologies
Java, Eclipse

## COMP23412.1 Eventlite
Given a mere Spring MVC skeleton, develop an EventBrite-like website. Following waterfall methodology.

## 1. Getting Started
Basically the same as above, just change "Hello MVC" to "Eventlite".

Admin Users & Password:
 - User Name: Rob - Password: Haines
 - User Name: Caroline - Password: Jay
 - User Name: Markel - Password: Vigo

## 2. Technologies
Java, Spring, MVC pattern, Eclipse


## 3. Result
[Screencast of all features](https://youtu.be/ndO5eWCL2q4)
