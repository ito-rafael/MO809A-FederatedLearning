import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class UDPClient_solution {

  public static void main(String[] args) throws Exception {
    
    DatagramSocket s = new DatagramSocket(4321); //create a socket specifying the client port
    InetAddress hostAddress = InetAddress.getByName("abcd::c30c:0:0:2"); //this is the address of the server     

    while (true) {  

      byte[] buf = new byte[1000]; //array used for both the in and out messages
      DatagramPacket dp = new DatagramPacket(buf, buf.length); //prepare a packet object, which will be the packet received by the client 

      BufferedReader stdin = new BufferedReader(new InputStreamReader(System.in));  
 
      System.out.println("What should I send to the server?\n");
      String outMessage = stdin.readLine();
 
      if (outMessage.equals("bye")){ //end the application if the message is "bye"
         System.out.println("Terminating the application\n");
         s.close();
         break;
      }

      String outString = "Client says: " + outMessage;
      buf = outString.getBytes(); //store outstring in buf

      System.out.println("Sending " + outMessage);

      DatagramPacket out = new DatagramPacket(buf, buf.length, hostAddress, 1234); //prepare the out packet, where 1234 is the server port 
      s.send(out); //send the packet

      s.receive(dp); //block until a packet is received from the server. Then, store the received message inside the byte array buf of DatagramPacket dp
      String rcvd = "Received from " + dp.getAddress() + ", " + dp.getPort() + ": "
          + new String(dp.getData()) + "\n";
      System.out.println(rcvd);
    }
  }
}
