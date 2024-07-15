#include <iostream>
#include <thread>
#include <exception>
#include "Server.h"
#include "WSAInitializer.h"
#include "IDatabase.h"

#pragma comment (lib, "ws2_32.lib")

int main() {
    try {

        WSAInitializer wsaInit;
        SqliteDatabase* db = new SqliteDatabase();
        Server server(db);
        std::thread serverThread(&Server::run, &server);
        serverThread.detach();
        while (true) {
			std::string input;
			std::cin >> input;
            if (input == "exit") {
				break;
			}
            if (input == "clear") {
				db->clearTable("Answers");
			}
         
		}
    }
    catch (std::exception& e) {
        std::cout << "Error occured: " << e.what() << std::endl;
    }

	return 0;
}