#pragma once
#include "iostream"
#include "string"
#include "vector"

class Question{
protected:
	int m_questionId;
	std::string m_question;
	std::vector<std::string> m_possibleAnswers;
	std::string m_correctAnswer;

	
public:
	Question(int questionId, std::string m_question, std::vector<std::string> m_possibleAnswers, std::string m_correctAnswer);
	Question() = default;

	int getQuestionId();
	std::string getQuestion();
	std::vector<std::string> getPossibleAnswers();
	int getCorrectAnswerId();
	std::string getCorrectAnswer();

};

