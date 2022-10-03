import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class UDPClient {
  public static void main(String args[]) throws Exception {

    // create a socket specifying the client port as 4321
    DatagramSocket s = new DatagramSocket(4321);
    // set server IPv6 address
    InetAddress hostAddress = InetAddress.getByName("abcd::c30c:0:0:2");

    while (true) {

      // array used for both the in/out messages
      byte[] buffer = new byte[1024];
      // prepare a packet object, which will be the packet received by the client
      DatagramPacket dp = new DatagramPacket(buffer, buffer.length);

      // read input from user
      BufferedReader stdin = new BufferedReader(new InputStreamReader(System.in));
      System.out.println("Please, type the message to be sent to the server:");
      String outMessage = stdin.readLine();

      // if message == "bye", then close this program
      if (outMessage.equals("bye")){
         System.out.println("\nEnding this program.\n");
         s.close();
         break;
      }

      // store the message
      buffer = outMessage.getBytes();
      // prepare to send the out packet to the server port 1234
      DatagramPacket out = new DatagramPacket(buffer, buffer.length, hostAddress, 1234);
      // send packet
      s.send(out);

      // block until a packet is received from the server
      s.receive(dp);
      // store the received message inside the byte array buf of DatagramPacket dp
      String msg_received = "\nMessage received from " + dp.getAddress() + ":" + dp.getPort() + "\nMessage: " + new String(dp.getData()) + "\n";
      // print message received
      System.out.println(msg_received);
    }
  }
}
