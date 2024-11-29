import socket

#constants
SERVER_HOST = "192.168.192.153"
SERVER_PORT = 12345
INITAL_PAYLOAD = "A"
INCREMENT_SIZE = 10
MAX_PAYLOAD_SIZE = 1024
COMMAND = "sha512"

def main():
	print("Starting iterative fuzzer...")

	#connecting to the server
	try:
		with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
			client_socket.connect((SERVER_HOST, SERVER_PORT))
			print(f"Connected to server at {SERVER_HOST}: {SERVER_PORT}.")

			#now for the actual command stuff
			for size in range(1, MAX_PAYLOAD_SIZE + 1, INCREMENT_SIZE):
				try:
					#testing with the sha512 command
					payload_data = INITAL_PAYLOAD * size
					payload = f"{COMMAND} {payload_data}"

					#crafting the command and payload to send
					print(f"Sending a payload of size {len(payload_data)}.")
					client_socket.sendall(payload.encode('utf-8'))

					#receive the server's response
					response = client_socket.recv(4096) #random ahh buffer size
					print(f"Received response: {response.decode('utf-8')}.")

				except ConnectionResetError:
					print(f"Connection was reset by the server after payload size {size} was sent. Possible crash.")

				except Exception as e:
					print(f"An error occurred: {e}.")

			print("Finished sending payloads.")

	except Exception as exc: 
		print(f"An error occurred: {exc}.")

if __name__ == "__main__":
	main()
