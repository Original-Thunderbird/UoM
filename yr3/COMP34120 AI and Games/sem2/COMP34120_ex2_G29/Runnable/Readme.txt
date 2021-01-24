Execute following commands in order in different windows
IMPORTANT - ALL COMMANDS BELOW MUST BE RUN AT THE ROOT FOLDER
- rmiregistry
- java -classpath poi-3.7-20101029.jar: -Djava.rmi.server.hostname=127.0.0.1 comp34120.ex2.Main

Run Leader1(Moving window with width 10):
- java -cp ./jars/commons-math3-3.6.1.jar:. -Djava.rmi.server.hostname=127.0.0.1 Leader1

Run Leader2(Weighting least square with forgetting factor 1.07):
- java -cp ./jars/commons-math3-3.6.1.jar:. -Djava.rmi.server.hostname=127.0.0.1 Leader2

Run Leader3(Optimal)(Recursive least square with forgetting factor 1.07):
- java -cp ./jars/commons-math3-3.6.1.jar:. -Djava.rmi.server.hostname=127.0.0.1 Leader3
(Click start three times in GUI to reach the optimal value)