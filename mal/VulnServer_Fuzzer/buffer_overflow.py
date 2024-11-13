import socket

def send_trun_command():
	target_ip = "L Bozo"
	target_port = 9999

	#connecting to VulnServer
	with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
		try: #trying to connect to vulnserver
			s.connect(target_ip, target_port)
			print(f"Connected to {target_ip}:{target_port}")

			#sending the TRUN HELLO command
			command = "TRUN HELLO\r\n"
			s.sendall(command.encode())
			print(f"Command sent: '{command}'")

			#receiving the response
			response = s.recv(1024)
			print("Response from server: " + response)

		#error handling, my beloved
		except ConnectionError as e:
			print(f"Failed to connect to server or send data: {e}")

if __name__ == "__main__":
	send_trun_command()
