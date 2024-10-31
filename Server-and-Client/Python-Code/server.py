import socket

# Server-side Python code, translated from ServerCode.c

def get_server_details():
    ip_address = input("Enter the IP address you want to listen on: ")
    port = int(input("Enter the port number to listen on: "))
    return ip_address, port

def initialize_server_socket(ip_address, port):
    try:
        server_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server_sock.bind((ip_address, port))
        server_sock.listen(1)
        print(f"Server listening on {ip_address}:{port}")
        return server_sock
    except socket.error as e:
        print(f"Socket creation or binding failed: {e}")
        return None

def handle_client_communication(client_sock):
    try:
        while True:
            # Receive data from the client
            message = client_sock.recv(1024).decode()
            print(f"Client:> {message}")

            # Check if the client says "bye"
            if message.lower() == "bye":
                print("Connection closed by client.")
                break

            # Server response
            response = input("Server:> ")
            client_sock.sendall(response.encode())

            # Check if the server wants to close the connection
            if response.lower() == "bye":
                print("Connection closed by server.")
                break
    finally:
        client_sock.close()

# Main execution flow
if __name__ == "__main__":
    ip_address, port = get_server_details()
    server_sock = initialize_server_socket(ip_address, port)
    if server_sock:
        print("Waiting for a client to connect...")
        client_sock, _ = server_sock.accept()
        print("Client connected.")
        handle_client_communication(client_sock)
        server_sock.close()
