#include "Question.h"

Question::Question(int questionId, std::string question, std::vector<std::string> possibleAnswers, std::string correctAnswer) {
    this->m_questionId = questionId;
    this->m_possibleAnswers = possibleAnswers;
    this->m_correctAnswer = correctAnswer;
    this->m_question = question;

}



int Question::getQuestionId()
{
    return this->m_questionId;
}

std::string Question::getQuestion() {
    return this->m_question;
}

std::vector<std::string> Question::getPossibleAnswers() {
    return this->m_possibleAnswers;
}

int Question::getCorrectAnswerId() {
    return 0;
}

std::string Question::getCorrectAnswer()
{
    return this->m_correctAnswer;
}
