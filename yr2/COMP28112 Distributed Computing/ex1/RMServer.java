import java.rmi.registry.* ;
import java.rmi.RemoteException ;
import java.rmi.server.UnicastRemoteObject ;

public class RMServer implements RemoteServer {

      public static String name;
      public String getId(String s) throws RemoteException {
        System.out.println("A client said: " + s) ;
        if(s.equals("whoRU")){
          if(name.equals("")){
            return "I don't have a name";
          }
          else{
            return "my name is "+ this.name ;
          }
        }
        else
          return "If you ask me nicely, I will tell you who I am";
      }

      public static void main(String[] args) {
         try {
           if(args.length<1){
             name = "";
           }
           else{
             name=args[0];
           }
           RMServer ms = new RMServer() ;
           RemoteServer stub = (RemoteServer) UnicastRemoteObject.exportObject(ms, 0) ;
           Registry registry = LocateRegistry.getRegistry() ;
           registry.rebind("myIDserver", stub) ;
           System.out.println("Server is running") ;
         } catch (Exception e) {
           System.out.println("Server failed with exception " + e) ;
           System.exit(1) ;
         }
      }
}
