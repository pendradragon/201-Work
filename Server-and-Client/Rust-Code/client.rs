use std::io::{self, Write, BufRead, BufReader};
use std::net::TcpStream;

fn main() {
    // Get server IP and port from the user
    println!("Enter server IP address: ");
    let mut ip_address = String::new();
    io::stdin().read_line(&mut ip_address).expect("Failed to read IP address");
    let ip_address = ip_address.trim();

    println!("Enter server port number: ");
    let mut port = String::new();
    io::stdin().read_line(&mut port).expect("Failed to read port number");
    let port = port.trim();

    let address = format!("{}:{}", ip_address, port);

    // Connect to the server
    match TcpStream::connect(&address) {
        Ok(mut stream) => {
            println!("Connected to server at {}", address);

            let reader = BufReader::new(stream.try_clone().expect("Failed to clone stream"));
            loop {
                print!("You:> ");
                io::stdout().flush().expect("Failed to flush stdout");

                // Get message from the user
                let mut message = String::new();
                io::stdin().read_line(&mut message).expect("Failed to read message");
                let message = message.trim();

                // Send message to the server
                stream.write_all(message.as_bytes()).expect("Failed to write to server");
                stream.write_all(b"\n").expect("Failed to write newline");

                // Check if the client wants to end the chat
                if message.eq_ignore_ascii_case("bye") {
                    println!("Connection closed by client.");
                    break;
                }

                // Receive response from server
                let mut response = String::new();
                let mut reader = BufReader::new(&stream);
                if reader.read_line(&mut response).is_ok() {
                    println!("Server:> {}", response.trim());
                    if response.trim().eq_ignore_ascii_case("bye") {
                        println!("Connection closed by server.");
                        break;
                    }
                } else {
                    println!("Server disconnected.");
                    break;
                }
            }
        },
        Err(e) => {
            println!("Failed to connect: {}", e);
        }
    }
}
