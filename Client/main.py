import socket
import json

SERVER_IP = '127.0.0.1'
SERVER_PORT = 8200

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

try:
    client_socket.connect((SERVER_IP, SERVER_PORT))
    print("Connected to the server")
except ConnectionRefusedError:
    print("Connection to the server failed. Make sure the server is running.")

print("Login")
data = {"username": "user1", "password": "1234"}
json_data = json.dumps(data).encode()
client_socket.sendall(b"a" + f"{len(json_data):0{4}d}".encode() + json_data)
response = client_socket.recv(1024)
print(response.decode())

print("Signup")
data = {"username": "user2", "password": "1234", "mail": "user1@gmail.com"}
json_data = json.dumps(data).encode()
client_socket.sendall(b"b" + f"{len(json_data):0{4}d}".encode() + json_data)
response = client_socket.recv(1024)
print(response.decode())

input("Press Enter to exit...")
client_socket.close()

