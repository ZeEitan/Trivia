import socket
from time import sleep

SERVER_IP = '127.0.0.1'
SERVER_PORT = 8200

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

try:
    client_socket.connect((SERVER_IP, SERVER_PORT))
    print("Connected to the server")
except ConnectionRefusedError:
    print("Connection to the server failed. Make sure the server is running.")

data = client_socket.recv(1024)
print("Received from server:", data.decode())

message = "Hello"
client_socket.sendall(message.encode())

sleep(5)

message = "EXIT"
client_socket.sendall(message.encode())

input("Press Enter to exit...")
client_socket.close()
