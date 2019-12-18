To run the client you will need the following jar files in your classpath and they can be found in the directory /opt/info/courses/COMP28112/ex2/libraries (you don't need to make a copy of these files).
	- commons-httpclient-3.0.1.jar
	- commons-codec-1.3.jar
	- commons-logging-1.1.jar
	- xalan.jar
	- xercesImpl.jar
	- serializer.jar
	
You also need to set the following system properties (using the -D option when running Java)
	javax.xml.parsers.DocumentBuilderFactory=org.apache.xerces.jaxp.DocumentBuilderFactoryImpl and
	javax.xml.transform.TransformerFactory=org.apache.xalan.xsltc.trax.TransformerFactoryImpl 
	
To invoke ClientReserve from the command line:

java -classpath $CLASSPATH -Djavax.xml.parsers.DocumentBuilderFactory=org.apache.xerces.jaxp.DocumentBuilderFactoryImpl \
-Djavax.xml.transform.TransformerFactory=org.apache.xalan.xsltc.trax.TransformerFactoryImpl \
uk.ac.manchester.cs.comp28112.lab2.ClientReserve

To invoke ClientRecMsg from the command line:

java -classpath $CLASSPATH -Djavax.xml.parsers.DocumentBuilderFactory=org.apache.xerces.jaxp.DocumentBuilderFactoryImpl \
-Djavax.xml.transform.TransformerFactory=org.apache.xalan.xsltc.trax.TransformerFactoryImpl \
uk.ac.manchester.cs.comp28112.lab2.ClientRecMsg http://jewel.cs.man.ac.uk:3010/queue/msg/24?username=your_username\&password=your_password

