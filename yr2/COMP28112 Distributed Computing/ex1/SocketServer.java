import java.io.* ;
import java.net.* ;

class SocketServer {
/* Sets up a server on port 8181 which you can telnet to.
 * If you send a message, it will reply and then disconnect.
*/

public static void main(String [] args) throws IOException {
  final int port = 8181 ;
  ServerSocket listener = new ServerSocket(port) ;
  System.out.println("Server is running") ;
  // Listen for clients ....
  while (true) {
      Socket client = listener.accept() ;
      if(args.length<1){
          new SessionHandler(client,"").start() ;
      }
      else{
          new SessionHandler(client,args[0]).start() ;
      }
    }
  }
}

class SessionHandler extends Thread {
// an instance created by the server for each client

private BufferedReader in ;
private PrintWriter out ;
private Socket client ;
public String name ;

  SessionHandler(Socket s, String get) {
    client = s ;  this.name=get;
    }

  public void run() {
    try {
      in = new BufferedReader(new InputStreamReader(
         client.getInputStream())) ;

      String line = in.readLine();

      out = new PrintWriter(client.getOutputStream(), true) ;
      System.out.println("A client said: " + line) ;
      if(line.equals("whoRU")){
          if(this.name.equals("")){
              out.println("I do not have a name");
          }
          else{
              out.println("my name is "+this.name);
          }
      }
      else{
          out.println("If you ask me nicely, I will tell you who I am") ;
      }
    } catch (Exception e) { System.out.println("Server error " + e) ; }
    try { client.close() ; }
    catch (Exception e) {;}
  }
}
