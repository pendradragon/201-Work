import socket

# Client-side Python code, translated from Client.c

def get_server_details():
    ip_address = input("Enter server IP address: ")
    port = int(input("Enter server port number: "))
    return ip_address, port

def connect_to_server(ip_address, port):
    try:
        client_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client_sock.connect((ip_address, port))
        print("Connected to server successfully.")
        return client_sock
    except socket.error as e:
        print(f"Connection failed: {e}")
        return None

def chat_with_server(client_sock):
    try:
        while True:
            # Get user input
            message = input("You:> ")
            
            # Send the message to the server
            client_sock.sendall(message.encode())
            
            # Check if client wants to end chat
            if message.lower() == "bye":
                print("Connection closed by client.")
                break
            
            # Receive server's response
            response = client_sock.recv(1024).decode()
            print(f"Server:> {response}")
            
            # Check if server closed the connection
            if response.lower() == "bye":
                print("Connection closed by server.")
                break
    finally:
        client_sock.close()

# Main execution flow
if __name__ == "__main__":
    ip_address, port = get_server_details()
    client_sock = connect_to_server(ip_address, port)
    if client_sock:
        chat_with_server(client_sock)
