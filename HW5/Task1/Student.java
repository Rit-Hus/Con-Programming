
import java.net.*;
import java.io.*;

public class Student {
    static int SERVER_PORT = 5010;

    public static void main(String[] args) {
        if (args.length < 1) {
            System.out.println("Usage: java Student <your_index>");
            return;
        }

        String myIndex = args[0];

        try (Socket socket = new Socket("localhost", SERVER_PORT)) {
            BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            
            // Student waits for thier parteners ID
            String partnerIndex = in.readLine();
            
            // Srudent prints thier own ID and thier partners
            System.out.println("Student " + myIndex + ": My partner is Student " + partnerIndex);
            
        } catch (IOException e) {
            System.err.println("Student " + myIndex + " could not connect to Teacher.");
        }
    }
}
