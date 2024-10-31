import java.io.*;
import java.net.*;
import java.util.Scanner;

public class Server {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        System.out.print("Enter the IP address to listen on (use 0.0.0.0 for all interfaces): ");
        String ipAddress = scanner.nextLine();

        System.out.print("Enter the port number to listen on: ");
        int port = scanner.nextInt();
        scanner.nextLine();  // Consume newline

        try (ServerSocket serverSocket = new ServerSocket(port, 1, InetAddress.getByName(ipAddress))) {
            System.out.println("Server listening on " + ipAddress + ":" + port);
            System.out.println("Waiting for a client to connect...");

            try (Socket clientSocket = serverSocket.accept();
                 PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true);
                 BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()))) {
                 
                System.out.println("Client connected.");

                String message;
                while (true) {
                    message = in.readLine();
                    if (message == null || "bye".equalsIgnoreCase(message)) {
                        System.out.println("Connection closed by client.");
                        break;
                    }
                    System.out.println("Client:> " + message);

                    System.out.print("Server:> ");
                    String response = scanner.nextLine();
                    out.println(response);

                    if ("bye".equalsIgnoreCase(response)) {
                        System.out.println("Connection closed by server.");
                        break;
                    }
                }
            } catch (IOException e) {
                System.err.println("Client communication error: " + e.getMessage());
            }
        } catch (IOException e) {
            System.err.println("Server setup error: " + e.getMessage());
        } finally {
            scanner.close();
        }
    }
}
