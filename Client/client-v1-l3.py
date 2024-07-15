import socket
import json

SERVER_IP = '127.0.0.1'
SERVER_PORT = 8200

FAIL = "\033[91mFail\033[0m"
SUCCESS = "\033[1;32mSuccess\033[0m"

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

try:
    client_socket.connect((SERVER_IP, SERVER_PORT))
    print("Connected to the server")
    print()
except ConnectionRefusedError:
    print("Connection to the server failed. Make sure the server is running.")


def passOrFail(response_from_server):
    response_from_server = response_from_server.decode()
    print(response_from_server)
    response_from_server = response_from_server[5:]
    response_from_server = json.loads(response_from_server)
    if response_from_server["status"] == "1":
        print(SUCCESS)
    else:
        print(FAIL)


def sendRequest(data_to_server, request_type):
    json_data = json.dumps(data_to_server).encode()
    print(b"" + request_type.encode() + f"{len(json_data):0{4}d}".encode() + json_data)
    client_socket.sendall(b"" + request_type.encode() + f"{len(json_data):0{4}d}".encode() + json_data)
    response = client_socket.recv(1024)
    passOrFail(response)
    print()


def login(username, password):
    login_data = {"username": username, "password": password}
    sendRequest(login_data, 'a')


def signup(username, password, mail):
    signup_data = {"username": username, "password": password, "mail": mail}
    sendRequest(signup_data, 'b')


print("Login without signup need to " + FAIL)
login("user1", "1234")

print("Signup need to " + SUCCESS)
signup("user1", "1234", " user1@gmail.com")

print("Signup with the same username need to " + FAIL)
data = {"username": "user1", "password": "1234", "mail": "user1@gmail.com"}
signup("user1", "1234", "user1@gmail.com")

print("Login need to after signup " + SUCCESS)
login("user1", "1234")

print("Login without logout need to " + FAIL)
login("user1", "1234")

input("Press Enter to exit...")
client_socket.close()
