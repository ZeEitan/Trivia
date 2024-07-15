#include "LoginManager.h"

LoginManager::LoginManager(IDatabase* database) :
    m_database(database)
{
}

/*
signing up a new user 
input:
*	username - username of the new user
*   password - password of the new user
*   mail - mail of the new user
output:
*    true if the user was signed up successfully
*    false if the user was not signed up or the username already exists
*/
bool LoginManager::signup(std::string username, std::string password, std::string mail) {
    int ok = this->m_database->addNewUser(username, password, mail);
    if (ok != 0) {
        std::cout << "error signing up" << std::endl;
        return false;
    }
    else {
        std::cout << "signup completed" << std::endl;
        return true;
    }
}


/*
login a user
input:
*	username - username of the user
*   password - password of the user
output:
*   true if the user was logged in successfully
*   false if the user was not logged in or the password is incorrect or the username does not exist
*/
bool LoginManager::login(std::string username, std::string password, std::string& error, const SOCKET socket) {
    int loginStatus = this->m_database->doesPasswordMatch(username, password);

    if (loginStatus != 1) {
        switch (loginStatus) {
        case 0:
            error = "password does not match";
            break;
        case 2:
        case 3:
            error = "User does not exsites";
        }
        return false;
    }
    else {
        for (auto it = m_loggedUsers.begin(); it != m_loggedUsers.end(); ++it) {
            if (it->getUsername() == username) {
                std::cout << "User already logged in" << std::endl;
                error = "User already logged in";
                return false;
            }
        }

        std::cout << "login completed" << std::endl;
        LoggedUser loggedUser(username, socket);
        this->m_loggedUsers.push_back(loggedUser);
        return true; 
    }
}



/*
logout a user
input:
*	username - username of the user 
output:
*   true if the user was logged out successfully
*   false if the user was not logged in
*/
bool LoginManager::logout(std::string username){
	for (auto it = m_loggedUsers.begin(); it != m_loggedUsers.end(); ++it) {
        if (it->getUsername() == username) {
            m_loggedUsers.erase(it);
            return true;
        }
    }
    return false;
}
