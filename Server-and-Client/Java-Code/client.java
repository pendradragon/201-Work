import java.io.*;
import java.net.*;
import java.util.Scanner;

public class Client {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        
        System.out.print("Enter server IP address: ");
        String ipAddress = scanner.nextLine();
        
        System.out.print("Enter server port number: ");
        int port = scanner.nextInt();
        scanner.nextLine();  // Consume newline

        try (Socket clientSocket = new Socket(ipAddress, port);
             PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true);
             BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()))) {
             
            System.out.println("Connected to server successfully.");
            
            String message;
            while (true) {
                System.out.print("You:> ");
                message = scanner.nextLine();
                out.println(message);

                if ("bye".equalsIgnoreCase(message)) {
                    System.out.println("Connection closed by client.");
                    break;
                }

                String response = in.readLine();
                if (response == null || "bye".equalsIgnoreCase(response)) {
                    System.out.println("Connection closed by server.");
                    break;
                }
                System.out.println("Server:> " + response);
            }
        } catch (IOException e) {
            System.err.println("Communication error: " + e.getMessage());
        } finally {
            scanner.close();
        }
    }
}
