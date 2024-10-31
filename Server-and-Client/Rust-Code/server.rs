use std::io::{self, Write, BufRead, BufReader, BufWriter};
use std::net::{TcpListener, TcpStream};

fn handle_client(mut stream: TcpStream) {
    let mut reader = BufReader::new(stream.try_clone().expect("Failed to clone stream"));
    let mut writer = BufWriter::new(&stream);

    loop {
        // Read message from the client
        let mut message = String::new();
        match reader.read_line(&mut message) {
            Ok(0) => {
                println!("Client disconnected.");
                break;
            },
            Ok(_) => {
                println!("Client:> {}", message.trim());

                // Check if the client says "bye"
                if message.trim().eq_ignore_ascii_case("bye") {
                    println!("Connection closed by client.");
                    break;
                }

                // Server response
                print!("Server:> ");
                io::stdout().flush().expect("Failed to flush stdout");

                let mut response = String::new();
                io::stdin().read_line(&mut response).expect("Failed to read response");
                let response = response.trim();

                // Send response to the client
                writer.write_all(response.as_bytes()).expect("Failed to write to client");
                writer.write_all(b"\n").expect("Failed to write newline");
                writer.flush().expect("Failed to flush writer");

                // Check if the server wants to close the connection
                if response.eq_ignore_ascii_case("bye") {
                    println!("Connection closed by server.");
                    break;
                }
            },
            Err(e) => {
                println!("Failed to read from client: {}", e);
                break;
            }
        }
    }
}

fn main() {
    // Get server IP and port from the user
    println!("Enter the IP address you want to listen on (use 0.0.0.0 for all interfaces): ");
    let mut ip_address = String::new();
    io::stdin().read_line(&mut ip_address).expect("Failed to read IP address");
    let ip_address = ip_address.trim();

    println!("Enter the port number to listen on: ");
    let mut port = String::new();
    io::stdin().read_line(&mut port).expect("Failed to read port number");
    let port = port.trim();

    let address = format!("{}:{}", ip_address, port);

    // Start the server
    let listener = TcpListener::bind(&address).expect("Failed to bind to address");
    println!("Server listening on {}", address);

    // Wait for a client to connect
    match listener.accept() {
        Ok((stream, addr)) => {
            println!("Client connected from {}", addr);
            handle_client(stream);
        },
        Err(e) => {
            println!("Failed to accept client: {}", e);
        }
    }
}
