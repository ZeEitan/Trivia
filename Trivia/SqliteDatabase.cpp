#include "SqliteDatabase.h"


std::mutex dataBaseMutex; 


static int callbackString(void* data, int argc, char** argv, char** azColName);
static int callbackDouble(void* data, int argc, char** argv, char** azColName);
static int callbackInt(void* data, int argc, char** argv, char** azColName);
int callBackVector(void* data, int argc, char** argv, char** azColName);
static int callBackVectorScore(void* data, int argc, char** argv, char** azColName);

/*
Get the database object.
input:
*	void
output:
*	 Database object
*/
sqlite3* SqliteDatabase::getDB() {
    return this->_db;
}

/*
Open the database.
input:
*	void
output:
*	 Status: true- opened, false- not opened
*/
bool SqliteDatabase::open() {
    if (sqlite3_open("TriviaDB.db", &this->_db) == SQLITE_OK) {
        //char* errMessage = nullptr;
        //std::string sqlStatement = "DELETE FROM Users;";
        //int res = sqlite3_exec(this->_db, sqlStatement.c_str(), nullptr, nullptr, &errMessage);
        return true;
    }
    return false;
}

/*
Close the database.
input:
*	void
output:
*	 Status: true- closed, false- not closed
*/
bool SqliteDatabase::close() {
    if (this->_db != nullptr) {
        sqlite3_close(this->_db);
        this->_db = nullptr;
        return true;
    }

    return false;
}

/* NO NEED FOR MUTEX
checks if user exists
input:
*	username - username to check
output:
*	 Status: 
*    1- exists, 
*    0- does not exist
calls:
* here in addNewUser
*/
int SqliteDatabase::doesUserExists(std::string username) {
   
    char* errMessage = nullptr;
    std::string sqlStatement = "";
    int res = 0;
    std::string resultString;


    sqlStatement = "SELECT username FROM Users WHERE username = '" + username + "';";
    res = sqlite3_exec(this->_db, sqlStatement.c_str(), callbackString, &resultString, &errMessage);

    if (res != SQLITE_OK) {
        std::cerr << "Error: " << errMessage << std::endl;
        sqlite3_free(errMessage);
    }
    if (resultString == "") {
        
        return 0;
    }
   
   
    return 1;
}

/*
checks if passwords match
input:
*	username - username to check password
*   password - password to comapre
output:
*	 Status: 
*    0- does not match,
*    1- passwords match,
*    2- error selecting user,
*    3- user to compare with not found
calls:
* LoginManager -> login
*/
int SqliteDatabase::doesPasswordMatch(std::string username, std::string password) {
    std::unique_lock<std::mutex> lock(dataBaseMutex);

    char* errMessage = nullptr;
    std::string sqlStatement = "";
    int res = 0;
    std::string resultString;


    sqlStatement = "SELECT password FROM Users WHERE username = '" + username + "';";
    res = sqlite3_exec(this->_db, sqlStatement.c_str(), callbackString, &resultString, &errMessage);

    if (res != SQLITE_OK) {
        std::cerr << "Error: " << errMessage << std::endl;
        sqlite3_free(errMessage);
        lock.unlock();
        return 2;
    }


    if (resultString == "") {
        lock.unlock();
        return 3;
    }


    if (resultString == password) {
        lock.unlock();
        return 1;
    }

    lock.unlock(); 
    return 0;
}



/*
inserts new user to the users table
input:
*	username - new username
*   password - new user password
*   mail - new user mail
output:
*	 Status: 
*    1- username taken,
*    0- new user added,
*    2- error inserting user
calls:
* LoginManager -> signup
*/
int SqliteDatabase::addNewUser(std::string username, std::string password, std::string mail) {
    std::unique_lock<std::mutex> lock(dataBaseMutex);

    char* errMessage = nullptr;
    std::string sqlStatement = "";
    int res = 0;
    std::string resultString;

    int status = this->doesUserExists(username);
    int status2 = this->doesMailExists(mail);

    if (status == 0 && status2 == 0) {
        std::string sqlStatement = "INSERT INTO Users (username, password, mail) VALUES ('" + username + "', '" + password + "', '" + mail + "');";
        res = sqlite3_exec(this->_db, sqlStatement.c_str(), callbackString, &resultString, &errMessage);
        if (res != SQLITE_OK) {
            std::cerr << "Error inserting user: " << errMessage << std::endl;
            sqlite3_free(errMessage);
            lock.unlock(); 
            return 1; 
        }

        this->addNewUserStatistics(username);
        lock.unlock(); 
        return 0;
    }


    lock.unlock(); 
    return 1;
}



/* NO NEED FOR MUTEX
checks if mail exists
input:
*	mail - mail to check
output:
*	 Status:
*    1- exists,
*    0- does not exist
calls:
* here in addNewUser
*/
int SqliteDatabase::doesMailExists(std::string mail) {
    
    char* errMessage = nullptr;
    std::string sqlStatement = "";
    int res = 0;
    std::string resultString;

    sqlStatement = "SELECT mail FROM Users WHERE mail = '" + mail + "';";
    res = sqlite3_exec(this->_db, sqlStatement.c_str(), callbackString, &resultString, &errMessage);

    if (res != SQLITE_OK) {
        std::cerr << "Error: " << errMessage << std::endl;
        sqlite3_free(errMessage);
      
        return 0;
    }

    if (resultString == "") {
        
        return 0;
    }

    
    return 1;
}


/*
removes the table given from the db
input:
*	tableName - table to delete
output:
* void
calls:
* _Source -> main
*/
void SqliteDatabase::clearTable(std::string tableName) {
    char* errMessage = nullptr;
	std::string sqlStatement = "DELETE FROM " + tableName + ";";
	int res = sqlite3_exec(this->_db, sqlStatement.c_str(), nullptr, nullptr, &errMessage);
    if (res != SQLITE_OK) {
		std::cerr << "Error: " << errMessage << std::endl;
		sqlite3_free(errMessage);
	}
    else {
		std::cout << "Table " << tableName << " cleared." << std::endl;
	}
}



/* NO NEED FOR MUTEX
inserts new user into the userStatistics table
input:
*	username - new username
output:
*	 Status:
*    1- error,
*    0- new user added to userStatistics
calls:
* here in addNewUser
*/
int SqliteDatabase::addNewUserStatistics(std::string username) {
    
    char* errMessage = nullptr;
    std::string sqlStatement = "";
    std::string resultString = "";
    int res = 0;

    sqlStatement = "INSERT INTO UserStatistics (username, gamesPlayed, correctAnswers, avgAnswerTime) VALUES ('" + username + "', 0, 0, 0.0);";
    res = sqlite3_exec(this->_db, sqlStatement.c_str(), callbackString, &resultString, &errMessage);
    if (res != SQLITE_OK) {
        std::cerr << "Error inserting user: " << errMessage << std::endl;
        sqlite3_free(errMessage);
        
        return 1;
    }
  
    
    return 0;
}




/* NO NEED FOR MUTEX HERE
gets the averaage answer time of a player
input:
*	usernmae - username of the player to get the data from
output:
*    -1 - Error,
calls:
* here in getUserStatistics
*/
double SqliteDatabase::getPlayerAverageAnswerTime(std::string username) {

    char* errMessage = nullptr;
    std::string sqlStatement = "";
    int res = 0;
    double resultDouble;
     
    sqlStatement = "SELECT avgAnswerTime FROM UserStatistics WHERE username = '" + username + "';";
    res = sqlite3_exec(this->_db, sqlStatement.c_str(), callbackDouble, &resultDouble, &errMessage);
    if (res != SQLITE_OK) {
        std::cerr << "Error: " << errMessage << std::endl;
        sqlite3_free(errMessage);
        return -1;
    }
    return resultDouble;
}



/* NO NEED FOR MUTEX HERE
gets the number of times the player got a correct answer
input:
*	usernmae - username of the player to get the data from
output:
*    -1 - Error,
calls:
* here in getUserStatistics
*/
int SqliteDatabase::getNumOfCorrectAnswers(std::string username) {

    char* errMessage = nullptr; 
    std::string sqlStatement = ""; 
    int res = 0; 
    int resultInt;  
     
    sqlStatement = "SELECT correctAnswers FROM UserStatistics WHERE username = '" + username + "';"; 
    res = sqlite3_exec(this->_db, sqlStatement.c_str(), callbackInt, &resultInt, &errMessage); 
    if (res != SQLITE_OK) { 
        std::cerr << "Error: " << errMessage << std::endl; 
        sqlite3_free(errMessage); 
        return -1;
    }
    return resultInt;
}



/* NO NEED FOR MUTEX HERE
gets the number of the player's answers(correct and incorrect)
input:
*	usernmae - username of the player to get the data from
output:
*    -1 - Error,
calls:
* here in getUserStatistics
*/
int SqliteDatabase::getNumOfTotalAnswers(std::string username) {

    char* errMessage = nullptr; 
    std::string sqlStatement = ""; 
    int res = 0; 
    int resultInt;

    sqlStatement = "SELECT questionsAnswered FROM UserStatistics WHERE username = '" + username + "';"; 
    res = sqlite3_exec(this->_db, sqlStatement.c_str(), callbackInt, &resultInt, &errMessage); 
    if (res != SQLITE_OK) { 
        std::cerr << "Error: " << errMessage << std::endl; 
        sqlite3_free(errMessage); 
        return -1;
    } 
    return resultInt;
}



/* NO NEED FOR MUTEX HERE
gets the number of games the player played
input:
*	usernmae - username of the player to get the data from
output:
*    -1 - Error,
calls:
* here in getUserStatistics
*/
int SqliteDatabase::getNumOfPlayerGames(std::string username) {

    char* errMessage = nullptr; 
    std::string sqlStatement = ""; 
    int res = 0; 
    int resultInt; 

    sqlStatement = "SELECT gamesPlayed FROM UserStatistics WHERE username = '" + username + "';"; 
    res = sqlite3_exec(this->_db, sqlStatement.c_str(), callbackInt, &resultInt, &errMessage); 
    if (res != SQLITE_OK) {  
        std::cerr << "Error: " << errMessage << std::endl; 
        sqlite3_free(errMessage); 
        return -1;
    }
    return resultInt;
}


/* NO NEED FOR MUTEX HERE
updates the average answer time of the user
input:
*	usernmae - username of the player to update the data
*   answerTime - the time it took to the user to answer the last question
output:
*    -1 - Error,
calls:
* here in addAnswer
*/
double SqliteDatabase::editPlayerAverageAnswerTime(std::string username, double answerTime) {
    

    //gets the current average answer time
    char* errMessage = nullptr;
    std::string sqlStatement = "";
    int res = 0; 
    double resultDouble;
    sqlStatement = "SELECT avgAnswerTime FROM UserStatistics WHERE username = '" + username + "';";
    res = sqlite3_exec(this->_db, sqlStatement.c_str(), callbackDouble, &resultDouble, &errMessage);
    if (res != SQLITE_OK) {
        std::cerr << "1Error: " << errMessage << std::endl;
        sqlite3_free(errMessage);
        
        return -1;
    }
    double currAvg = resultDouble;


    //gets the total questions answered
    errMessage = nullptr; 
    sqlStatement = "";   
    res = 0; 
    int resultInt;
    sqlStatement = "SELECT questionsAnswered FROM UserStatistics WHERE username = '" + username + "';";
    res = sqlite3_exec(this->_db, sqlStatement.c_str(), callbackInt, &resultInt, &errMessage);
    if (res != SQLITE_OK) { 
        std::cerr << "2Error: " << errMessage << std::endl; 
        sqlite3_free(errMessage); 
        
        return -1; 
    }
    int totalGames = resultInt;

    //calc the new average
    double newAvg = (currAvg + answerTime) / totalGames;


    errMessage = nullptr;
    sqlStatement = ""; 
    res = 0; 
    resultInt = 0; 
    sqlStatement = "UPDATE UserStatistics SET avgAnswerTime = "+std::to_string(newAvg)+" WHERE username = '" + username + "'; "; 
    
    res = sqlite3_exec(this->_db, sqlStatement.c_str(), callbackInt, &resultInt, &errMessage); 
    if (res != SQLITE_OK) { 
        std::cerr << "3Error: " << errMessage << std::endl; 
        sqlite3_free(errMessage); 
         
        return -1;
    }

    
    return 0;
}


/* NO NEED FOR MUTEX
increaces the number of correct answers
input:
*	usernmae - username of the player to update the data
output:
*    -1 - Error,
*     0 - good
calls:
* here in addAnswer
*/
int SqliteDatabase::incNumOfCorrectAnswers(std::string username) {
   

    //increaces the number of correct answers
    char* errMessage = nullptr;
    std::string sqlStatement = "";
    int res = 0;
    int resultInt;
    sqlStatement = "UPDATE UserStatistics SET correctAnswers = correctAnswers + 1 WHERE username = '" + username + "'; ";
    res = sqlite3_exec(this->_db, sqlStatement.c_str(), callbackInt, &resultInt, &errMessage);
    if (res != SQLITE_OK) { 
        std::cerr << "1Error: " << errMessage << std::endl; 
        sqlite3_free(errMessage); 
        return -1; 
    }

    return 0;
}



/* NO NEED FOR MUTEX HERE
increaces the number of total answers
input:
*	usernmae - username of the player to update the data
output:
*    -1 - Error,
*     0 - good
calls:
* here in addAnswer
*/
int SqliteDatabase::incNumOfTotalAnswers(std::string username) {
    //increaces the number of total answers
    char* errMessage = nullptr; 
    std::string sqlStatement = ""; 
    int res = 0; 
    int resultInt; 
    sqlStatement = "UPDATE UserStatistics SET questionsAnswered = questionsAnswered + 1 WHERE username = '" + username + "'; "; 
    res = sqlite3_exec(this->_db, sqlStatement.c_str(), callbackInt, &resultInt, &errMessage);  
    if (res != SQLITE_OK) { 
        std::cerr << "Error: " << errMessage << std::endl; 
        sqlite3_free(errMessage); 
        return -1;
    }

    return 0;
}


/* NO NEED FOR MUTEX HERE
add answer to Statistics table
input:
*	usernmae - username of the player to update the data
*   isCorrect - true if the answer is correct and false if not
*   answerTime - answer time 
*   gameID - game id
output:
*    -1 - Error,
*     0 - good
calls: 
* here in addAnswer
*/
int SqliteDatabase::addAnswerToStatisticsTable(std::string username, bool isCorrect, double answerTime, int gameID) {
    char* errMessage = nullptr;
    std::string sqlStatement = "";
    std::string resultString = "";
    int res = 0;

    int isCrrct = isCorrect ? 1 : 0; // Simplified the conversion to integer

    // Check if the gameID already exists
    sqlStatement = "SELECT COUNT(*) FROM Statistics WHERE gameID = " + std::to_string(gameID) + ";";
    res = sqlite3_exec(this->_db, sqlStatement.c_str(), callbackString, &resultString, &errMessage);
    if (res != SQLITE_OK) {
        std::cerr << "Error checking gameID: " << errMessage << std::endl;
        sqlite3_free(errMessage);
        return 1;
    }

    // Convert resultString to an integer
    int count = std::stoi(resultString);

    // If count is greater than 0, the gameID already exists, so return an error
    if (count > 0) {
        return 1;
    }

    // Insert the new record
    sqlStatement = "INSERT INTO Statistics (gameID, username, isCorrect, answerTime) VALUES ("+ std::to_string(gameID)+", '"+username+"', "+ std::to_string(isCrrct)+", "+std::to_string(answerTime)+"); ";
    res = sqlite3_exec(this->_db, sqlStatement.c_str(), callbackString, &resultString, &errMessage);
    if (res != SQLITE_OK) {
        std::cerr << "Error inserting user: " << errMessage << std::endl;
        sqlite3_free(errMessage);
        return 1;
    }

    return 0;
}


/* NO NEED FOR MUTEX
increaces the number of games
input:
*	usernmae - username of the player to update the data
output:
*    -1 - Error,
*     0 - good
calls:
* here....addAnswer
*/
int SqliteDatabase::incNumOfPlayerGames(std::string username) {
    //increaces the number of games
    char* errMessage = nullptr; 
    std::string sqlStatement = ""; 
    int res = 0; 
    int resultInt; 
    sqlStatement = "UPDATE UserStatistics SET gamesPlayed = gamesPlayed + 1 WHERE username = '" + username + "'; "; 
    res = sqlite3_exec(this->_db, sqlStatement.c_str(), callbackInt, &resultInt, &errMessage);  
    if (res != SQLITE_OK) { 
        std::cerr << "1Error: " << errMessage << std::endl; 
        sqlite3_free(errMessage); 
        return -1;
    }
    return 0;
}


/*
add answer
input:
*	usernmae - username of the player to update the data
*   isCorrect - true if the answer is correct and false if not
*   answerTime - answer time 
*   gameID - game id
output:
*    -1 - Error,
*     0 - good
call:
* for now no calls
*/
int SqliteDatabase::addAnswer(std::string username, bool isCorrect, double answerTime, int gameID) {
    std::unique_lock<std::mutex> lock(dataBaseMutex); 


    int status1 = this->incNumOfTotalAnswers(username);
    //int status2 = this->incNumOfPlayerGames(username);
    int status3 = 0;
    if (isCorrect) {
        status3 = this->incNumOfCorrectAnswers(username);
    }
    int status4 = this->editPlayerAverageAnswerTime(username, answerTime);
    int status5 = this->addAnswerToStatisticsTable(username, isCorrect, answerTime, gameID);
    if (status1 !=0 || status3 != 0 || status4 != 0 || status5 != 0) {
        lock.unlock(); 
        return -1;
    }
    lock.unlock(); 
    return 0;
}

int SqliteDatabase::getPlayerScore(std::string username) {
    return 0;
}


/* NO NEED FOR MUTEX HERE
input:
* sqlite3* db - database
* std::string username - username
output:
* a vector of the struct GameHistory
calls:
* here in submitGameStatistics
*/
std::vector<GameHistory> getGameHistoryByUsername(sqlite3* db, const std::string& username) {

    std::vector<GameHistory> results;
    sqlite3_stmt* stmt;
    const char* sql = "SELECT * FROM GameHistory WHERE username = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        // Bind the username parameter
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

        // Execute the query and process the results
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            GameHistory gh;
            gh.username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            gh.gameId = sqlite3_column_int(stmt, 1);
            gh.averageAnswerTime = sqlite3_column_double(stmt, 2);
            gh.correctAnswerCount = sqlite3_column_int(stmt, 3);
            gh.queue = sqlite3_column_int(stmt, 4);
            results.push_back(gh);
        }
    }
    else {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    }

    
    sqlite3_finalize(stmt);
    return results;
}

int SqliteDatabase::getHowManyGamesAreRegistered(std::string username)
{
    return 0;
}

/* NO NEED FOR MUTEX HERE
input:
* username
* current length of the games in game history of the player
output:
* 0
calls:
* here in submitGameStatistics
*/
int SqliteDatabase::shiftGamesInHistory(std::string username, int currentLen)
{

    std::string sqlStatement = ""; 
    int resultInt = 0; 
    for (int i = currentLen; i > 0; i--) {

        sqlStatement = "UPDATE GameHistory SET queue = queue + 1 WHERE username = '" + username + "' AND queue = " + std::to_string(i) + ";"; 
        char* errMessage = nullptr; 
        int res = sqlite3_exec(this->_db, sqlStatement.c_str(), callbackInt, &resultInt, &errMessage); 
        if (res != SQLITE_OK) { 
            std::cerr << "SQL error: " << errMessage << std::endl; 
            sqlite3_free(errMessage); 
        }
    }
    return 0;
}



/* NO NEED FOR MUTEX HERE
input:
* username
* correct answer count
* average answer time 
* game id
* queue in the history(always 1 cause its the latest game)
output:
* 0
calls:
* here in submitGameStatistics
*/
int SqliteDatabase::submitGameToGameHistory(std::string username, int correctAnswerCount, double averageAnswerTime, int gameId, int queue)
{
    std::vector<std::string> vec;
    std::string resultString; 
    std::string sqlStatement = "INSERT INTO GameHistory (username, gameId, averageAnswerTime, correctAnswerCount, queue) VALUES ('"+username+"', "+std::to_string(gameId)+", "+std::to_string(averageAnswerTime)+", "+std::to_string(correctAnswerCount)+", "+ std::to_string(queue)+");";
    char* errMessage = nullptr; 
    int res = sqlite3_exec(this->_db, sqlStatement.c_str(), callbackString, &resultString, &errMessage); 
    if (res != SQLITE_OK) { 
        std::cerr << "SQL error: " << errMessage << std::endl; 
        sqlite3_free(errMessage); 
    } 
    return 0;
}




/* 
input:
* username
* correct answer count
* average answer time
* game id
output:
* 0
calls:
* Game -> submitGameStatsToDB
*/
int SqliteDatabase::submitGameStatistics(std::string username, int correctAnswerCount, double averageAnswerTime, int gameId)
{
    std::unique_lock<std::mutex> lock(dataBaseMutex); 

    int gamesTotal = 0; 
    int correctAnswers = 0; 
    double averageTime = 0.0;


    char* errMessage = nullptr;
    int res = 0;
    std::string sqlStatement = "";
    std::string resultString = "";
    int resultInt = 0;




    std::vector<GameHistory> myVector = getGameHistoryByUsername(this->_db, username);  
    std::cout <<username <<"---------- Length ----------- "<< myVector.size() << std::endl;

    if ((myVector.size() + 1) <= MAX_LENGTH_OF_GAME_HISTORY) {
        shiftGamesInHistory(username, myVector.size());
        gamesTotal = myVector.size() + 1;
    }
    else {
        shiftGamesInHistory(username, MAX_LENGTH_OF_GAME_HISTORY); 

        resultString = "";
        sqlStatement = "DELETE FROM GameHistory WHERE username = '" + username + "' AND queue = " +std::to_string(MAX_LENGTH_OF_GAME_HISTORY + 1)+ ";"; 
        errMessage = nullptr; 
        res = sqlite3_exec(this->_db, sqlStatement.c_str(), callbackString, &resultString, &errMessage); 
        if (res != SQLITE_OK) { 
            std::cerr << "SQL error: " << errMessage << std::endl; 
            sqlite3_free(errMessage); 
        }
        gamesTotal = MAX_LENGTH_OF_GAME_HISTORY;
    }
    submitGameToGameHistory(username, correctAnswerCount, averageAnswerTime, gameId, 1); 

    
    for (int i = 0; i < myVector.size(); i++) {
        averageTime += myVector[i].averageAnswerTime;
    }
    averageTime += averageAnswerTime;
    averageTime = averageTime / (double(gamesTotal));
    



    resultInt = 0;
    sqlStatement = "UPDATE UserStatistics SET correctAnswers = correctAnswers + " +std::to_string(correctAnswerCount)+ ", gamesPlayed = gamesPlayed + 1, avgAnswerTime = "+std::to_string(averageTime) + " WHERE username = '" + username + "'; ";
    errMessage = nullptr;
    res = sqlite3_exec(this->_db, sqlStatement.c_str(), callbackInt, &res, &errMessage);
    if (res != SQLITE_OK) {
        std::cerr << "SQL error: " << errMessage << std::endl;
        sqlite3_free(errMessage);
    }

    lock.unlock(); 
    return 0;


}



/*
input:
* username
* game id
* question id
* is answer correct
* asnwer time
* answer
output:
* 0 
calls:
* Game -> submitAnswer
*/
int SqliteDatabase::submitAnswer(std::string username, int gameId, int questionId, int isCorrect, double answerTime, std::string answer) {
    std::unique_lock<std::mutex> lock(dataBaseMutex); 

    std::vector<std::string> vec;
    std::string sqlStatement = "INSERT INTO Answers (username, gameId, questionId, playerAnswer, answerTime, isCorrect) VALUES ('"
        + username + "', " + std::to_string(gameId) + ", " + std::to_string(questionId) + ", '" + answer + "', "
        + std::to_string(answerTime) + ", " + std::to_string(isCorrect) + ");";
    std::cout << sqlStatement << std::endl;
    char* errMessage = nullptr;

    std::string resultString = "";
    int res = sqlite3_exec(this->_db, sqlStatement.c_str(), callbackString, &resultString, &errMessage);
    if (res != SQLITE_OK) {
        std::cerr << "SQL error: " << errMessage << std::endl;
        sqlite3_free(errMessage);
    }
    lock.unlock(); 
    return 0;
}


/*
input:
*
output:
* vector of Struct Score length 3, first second and third places
calls:
* StatisticsManager -> getHighScore
*/
std::vector<Score> SqliteDatabase::getHighScores() {
    std::unique_lock<std::mutex> lock(dataBaseMutex); 

    std::vector<Score> vec; 
    std::string sqlStatement = "SELECT username, correctAnswers, avgAnswerTime FROM UserStatistics "
        "ORDER BY correctAnswers DESC, avgAnswerTime ASC LIMIT 3;"; 
    char* errMessage = nullptr; 

    int res = sqlite3_exec(this->_db, sqlStatement.c_str(), callBackVectorScore, &vec, &errMessage); 
    if (res != SQLITE_OK) { 
        std::cerr << "SQL error: " << errMessage << std::endl; 
        sqlite3_free(errMessage); 
    }
    lock.unlock(); 
    return vec; 
}



/*
input:
* username
output:
* vector of strings that contains the users statistics
calls:
* MenuRequestHandler -> getPersonalStats
* StatisticsManager -> getUserStatistics
*/
std::vector<std::string> SqliteDatabase::getUserStatistics(std::string username) {
    std::unique_lock<std::mutex> lock(dataBaseMutex); 
    std::vector<std::string> vec;

    std::string correctAnswers = std::to_string(getNumOfCorrectAnswers(username)); 
    std::string playerGames = std::to_string(getNumOfPlayerGames(username)); 
    std::string totalAnswers = std::to_string(getNumOfTotalAnswers(username));
    std::string averageAnswerTime = std::to_string(getPlayerAverageAnswerTime(username)); 
    std::string wrongAnswers = std::to_string(std::stoi(totalAnswers) - std::stoi(correctAnswers)); 
    vec.push_back(playerGames); 
    vec.push_back(correctAnswers);  
    vec.push_back(wrongAnswers); 
    vec.push_back(averageAnswerTime); 

    lock.unlock(); 
    return vec;
}





/*
input:
* question id
output:
* the question as string
calls:
* Game -> GetQuestionFromDatabase
*/
std::string SqliteDatabase::getQuestion(int questionId)
{
    std::unique_lock<std::mutex> lock(dataBaseMutex);

    std::string sqlStatement = "SELECT question FROM Questions WHERE id = " + std::to_string(questionId) + ";";

    std::cout << sqlStatement << std::endl; 
    char* errMessage = nullptr; 

    std::string resultString = "";
    int res = sqlite3_exec(this->_db, sqlStatement.c_str(), callbackString, &resultString, &errMessage); 
    if (res != SQLITE_OK) { 
        std::cerr << "SQL error: " << errMessage << std::endl; 
        sqlite3_free(errMessage); 
    }
    lock.unlock();
    return resultString;
}


/*
input:
* question id
output:
* a vector of the answer to the questions
calls:
* Game -> getAnswersFromDatabase
*/
std::vector<std::string> SqliteDatabase::getAnswers(int questionId)
{
    std::unique_lock<std::mutex> lock(dataBaseMutex); 

    std::vector<std::string> final;

    std::string sqlStatement = "SELECT correct FROM Questions WHERE id = " + std::to_string(questionId) + ";";
    std::cout << sqlStatement << std::endl;
    char* errMessage = nullptr;
    std::string resultString = "";
    int res = sqlite3_exec(this->_db, sqlStatement.c_str(), callbackString, &resultString, &errMessage);
    if (res != SQLITE_OK) {
        std::cerr << "SQL error: " << errMessage << std::endl;
        sqlite3_free(errMessage);
    }
    final.push_back(resultString);


    sqlStatement = "SELECT wrong1 FROM Questions WHERE id = " + std::to_string(questionId) + ";";
    errMessage = nullptr; 
    resultString = "";
    res = sqlite3_exec(this->_db, sqlStatement.c_str(), callbackString, &resultString, &errMessage); 
    if (res != SQLITE_OK) {  
        std::cerr << "SQL error: " << errMessage << std::endl; 
        sqlite3_free(errMessage);  
    }
    final.push_back(resultString);


    sqlStatement = "SELECT wrong2 FROM Questions WHERE id = " + std::to_string(questionId) + ";";
    errMessage = nullptr; 
    resultString = ""; 
    res = sqlite3_exec(this->_db, sqlStatement.c_str(), callbackString, &resultString, &errMessage); 
    if (res != SQLITE_OK) { 
        std::cerr << "SQL error: " << errMessage << std::endl; 
        sqlite3_free(errMessage); 
    } 
    final.push_back(resultString);


    sqlStatement = "SELECT wrong3 FROM Questions WHERE id = " + std::to_string(questionId) + ";";
    errMessage = nullptr; 
    resultString = "";  
    res = sqlite3_exec(this->_db, sqlStatement.c_str(), callbackString, &resultString, &errMessage); 
    if (res != SQLITE_OK) {  
        std::cerr << "SQL error: " << errMessage << std::endl; 
        sqlite3_free(errMessage); 
    }
    final.push_back(resultString);

    lock.unlock(); 
    return final;
}



static int callbackString(void* data, int argc, char** argv, char** azColName) {
    if (argc > 0 && argv[0]) {
        std::string* resultString = static_cast<std::string*>(data);
        *resultString = argv[0];
    }
    return 0;
}

static int callbackDouble(void* data, int argc, char** argv, char** azColName) {
    if (argc > 0 && argv[0]) {
        double* resultDouble = static_cast<double*>(data);
        *resultDouble = std::stod(argv[0]);
    }
    return 0;
}

static int callbackInt(void* data, int argc, char** argv, char** azColName) {
    if (argc > 0 && argv[0]) {
        int* resultInt = static_cast<int*>(data);
        *resultInt = std::stoi(argv[0]); 
    }
    return 0;
}



static int callBackVector(void* data, int argc, char** argv, char** azColName) {
    std::vector<std::string>* usernames = static_cast<std::vector<std::string>*>(data);
    usernames->push_back(argv[0]);
    return 0;
}


static int callBackVectorScore(void* data, int argc, char** argv, char** azColName) {
    std::vector<Score>* vec = static_cast<std::vector<Score>*>(data);

    Score score;
    score.username = argv[0] ? argv[0] : "";
    score.correctAnswerCount = argv[1] ? std::stoi(argv[1]) : 0;
    score.averageAnswerTime = argv[2] ? std::stod(argv[2]) : 0.0;

    vec->push_back(score);

    return 0;
}
SqliteDatabase::SqliteDatabase(sqlite3* _db)
    : _db(_db)
{
}
