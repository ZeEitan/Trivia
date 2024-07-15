using System;
using System.Collections.Generic;

using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using Server;
using System.Text.Json;
using Newtonsoft.Json;
using System.Printing;
using System.Net.Sockets;

namespace Trivia_GUI
{




    public class QuestionResponse
    {
        public Dictionary<string, string>? Answers { get; set; }//
        public string Question { get; set; }
        public int Status { get; set; }
    }




    public class PlayerResults
    {
        public string Username { get; set; }
        public uint CorrectAnswerCount { get; set; }
        public uint WrongAnswerCount { get; set; }
        public double AverageAnswerTime { get; set; }
    }

    public class GetGameResultsResponse
    {
        public uint Status { get; set; }
        public List<PlayerResults> Results { get; set; }
    }

    public partial class Question : Window
    {
        bool back;// boolianic variable for the loop that checks the game status
        bool back2;// boolianic variable for the loop that checks how many players answered the question
        TriviaServer _socket;
        ServerIsOffline? serverIsOffline = null;
        uint _gameId;
        string _hostName;
        string _myName;

        uint correctAnswerNum;

        uint _totalQuestions;
        uint _currQuestionQ;

        uint _maxTime;
        uint _currTime;

        int m_correctAnswerPlace;

        int _playersCurrentlyOnline;

        //     Question(_socket   , roomState.answerTimeout, roomState.questionCount, 0);
        public Question(TriviaServer socket, uint maxTime, uint totalQuestions, uint currQuestionQ, uint roomId, string hostName)
        {
            InitializeComponent();
            
            _socket = socket;
            _totalQuestions = totalQuestions;
            _maxTime = maxTime;

            back = true;
            back2 = true;
            _currTime = maxTime;
            //_currTime = 500;//temp

            _myName = _socket.getMyUsername();

            _currQuestionQ = currQuestionQ;

            _gameId = roomId;
            _hostName = hostName;

            borderOfAnswered.Visibility = Visibility.Hidden;

            playerUsername.Text = _socket.getMyUsername(); 

            Random random = new Random();
            int correctAnswerPlace = random.Next(1,5);
            //MessageBox.Show(correctAnswerPlace.ToString());
            this.m_correctAnswerPlace = correctAnswerPlace;

            QuestionResponse questionRes = getQuestion();
            question.Content = questionRes.Question;

            initializeAnswersOnScreen(questionRes);

            time.Text = _currTime.ToString();
            Thread thread = new Thread(() => countDown(UpdateTextBlock));
            thread.IsBackground = true; // Set the thread as a background thread
            thread.Start();
            
        }

        private void goBackToMainMenu(string message)
        {
            _currTime = 0;
            _socket.requestMainMenuHandler();
            MainMenu mainMenu = new MainMenu(_socket);
            if(message == "")
            {
            }
            else
            {
                MessageBox.Show(message);
            }
            
            mainMenu.Show();
            Close();
        }



        /*private void checkForHost()
        {
            while (back == true)
            {
                bool isHostInGame = getHostInGame(_gameId, _hostName);


                if (isHostInGame == false)
                {
                    this.Dispatcher.Invoke(() =>
                    {
                        goBackToMainMenu("The host left the game.");
                    });
                }

            }
        }*/
        private void countDown(Action<uint> textBlock)
        {
            while (back == true)
            {
                if (_currTime > 0)
                {
                    _currTime--;
                    Thread.Sleep(1000); // Adjust sleep time according to your needs

                    textBlock(_currTime);
                    bool isHostInGame = getHostInGame(_gameId, _hostName);


                    if (isHostInGame == false)
                    {
                        this.Dispatcher.Invoke(() =>
                        {
                            goBackToMainMenu("The host left the game.");
                        });
                    }
                }
                else
                {
                    back = false;
                    this.Dispatcher.Invoke(() =>
                    {
                        submitAnswer("", _maxTime); 
                    });

                }
                

            }
            this.Dispatcher.Invoke(() =>
            {
                setEnableStatusForButtons(false);
            });
            Thread.Sleep(1000);
        }
        private void UpdateTextBlock(uint _currTime)
        { 
            if (time != null) {
                time.Dispatcher.Invoke(() => time.Text = (_currTime).ToString());
            }
        }



        private void initializeAnswersOnScreen(QuestionResponse questionRes)
        {
            Button[] buttons = { btnAns1, btnAns2, btnAns3, btnAns4 };
            int k = 1;
            for (int i = 0; i < 4; i++)
            {
                if ((i + 1) == m_correctAnswerPlace)
                {
                    buttons[i].Content = questionRes.Answers["0"];
                }
                else
                {
                    buttons[i].Content = questionRes.Answers[k.ToString()];
                    k++;
                }
            }
            //fills the answer buttons with the strings of the answers 
        }



        //ffff
        private QuestionResponse getQuestion()
        {

            string message = "p0000";
            //MessageBox.Show(data + "\n" + message);
            _socket.Send(message);
            string response = _socket.Receive();
           
            if (response == null || response == "")
            {
                serverIsOffline = new ServerIsOffline();
                serverIsOffline.Show();
                Close();
            }
            response = response.Substring(5);
            
            //MessageBox.Show(response);
            QuestionResponse questionAfter = JsonConvert.DeserializeObject<QuestionResponse>(response);
            //MessageBox.Show("question: " + batmanMovie.Question);



            return questionAfter;
        }




        public bool getHostInGame(uint gameId, string hostName)
        {
            GetPlayersInGameReponse response = _socket.getPlayersInGame(gameId);
            string[] players = response.players;
            _playersCurrentlyOnline = players.Length;

            if (response.Status == 0)
            {
                return false;
            }

            bool isHere = false;
            for (int i = 0; i < players.Length; i++)
            {
                if (players[i] == hostName)
                {
                    isHere = true;
                }
            }
            if (isHere == true)
            {
                return true;
            }
            return false;


        }


        private void updatePlayersAnswered(int answered, int outOf)
        {
            answeredTextBlock.Text = answered.ToString() + "/" + outOf.ToString();
        }


        private void moveToNextQuestion()
        {
            if (_currQuestionQ < _totalQuestions)
            {
                Question question = new Question(_socket, _maxTime, _totalQuestions, (_currQuestionQ + 1), _gameId, _hostName); 
                question.Show();
                Close();
            }
            else if (_currQuestionQ == _totalQuestions)
            {

                //MessageBox.Show("*GAME RESULTS*");

                string message = "r0000";
                _socket.Send(message);
                string response = _socket.Receive();
                response = response.Substring(5);

                GetGameResultsResponse responseAfter = JsonConvert.DeserializeObject<GetGameResultsResponse>(response);

                string resultsStr = "";
                for(int i = 0;i < responseAfter.Results.Count; i++)
                {
                    string username = responseAfter.Results[i].Username;
                    double averageAnswerTime = responseAfter.Results[i].AverageAnswerTime;
                    uint correctAnswerCount = responseAfter.Results[i].CorrectAnswerCount;
                    uint wrongAnswerCount = responseAfter.Results[i].WrongAnswerCount;
                    string formattedAverageAnswerTime = averageAnswerTime.ToString("F2");

                    string col = "Name: "+username+", Average answer time: "+ formattedAverageAnswerTime + ", Correct answers"+correctAnswerCount+"/"+(correctAnswerCount+wrongAnswerCount)+"\n";
                    resultsStr += col;
                    resultsStr += "----------------------------------------------------------------\n";
                }
                MessageBox.Show(resultsStr);
                if (_myName == _hostName)
                {
                    _socket.endTheGame(_gameId); 
                   
                }
                goBackToMainMenu("");
            }
        }
        private void HowManyAnsweredLoader()
        {
            while (back2)
            {
                int answered = _socket.getHowManyAnswered(_currQuestionQ);
                this.Dispatcher.Invoke(() =>
                {
                    updatePlayersAnswered(answered, _playersCurrentlyOnline);
                });

                if (answered == _playersCurrentlyOnline)
                {
                    back2 = false;
                    back = false;
                    this.Dispatcher.Invoke(() =>
                    {
                        moveToNextQuestion();
                    });
                }
                Thread.Sleep(1000); 
            }
        }




        private void setEnableStatusForButtons(bool enable)
        {
            if (enable)
            {
                btnAns1.IsEnabled = true;
                btnAns2.IsEnabled = true;
                btnAns3.IsEnabled = true;
                btnAns4.IsEnabled = true;
                return;
            }
            btnAns1.IsEnabled = false;
            btnAns2.IsEnabled = false;
            btnAns3.IsEnabled = false;
            btnAns4.IsEnabled = false;
        }



        private void submitAnswer(string answer, uint timeToAnswer)
        {
            string data = "{" + $"\"answerTime\":{timeToAnswer},\"answer\":\"{answer}\"" + "}";
            string message = "q" + data.Length.ToString().PadLeft(4, '0') + data;
            _socket.Send(message);
            string response = _socket.Receive();
            

            Thread thread = new Thread(HowManyAnsweredLoader);
            thread.IsBackground = true; // Set the thread as a background thread
            thread.Start(); 

            
        }

        private void btnAns1_Click(object sender, RoutedEventArgs e)
        {
            setEnableStatusForButtons(false);
            borderOfAnswered.Visibility = Visibility.Visible;
            back = false;


            if (1 == m_correctAnswerPlace)
                //btnAns1.Background = new SolidColorBrush(Colors.Green);
                btnAns1.Foreground = new SolidColorBrush(Colors.Green);
            else
                //btnAns1.Background = new SolidColorBrush(Colors.Red);
                btnAns1.Foreground = new SolidColorBrush(Colors.Red);

            
            uint timeToAnswer = _maxTime - _currTime ;
            /*string data = "{" + $"\"answerTime\":{timeToAnswer},\"answer\":\"{btnAns1.Content}\"" + "}";
            string message = "q" + data.Length.ToString().PadLeft(4, '0') + data;
            _socket.Send(message);*/

            //string response = submitAnswer((btnAns1.Content).ToString(), timeToAnswer);
            submitAnswer((btnAns1.Content).ToString(), timeToAnswer);



            /*    Thread thread = new Thread(HowManyAnsweredLoader);
                thread.IsBackground = true; // Set the thread as a background thread
                thread.Start();*/

        }






        private void btnAns2_Click(object sender, RoutedEventArgs e)
        {
            setEnableStatusForButtons(false);
            borderOfAnswered.Visibility = Visibility.Visible;
            back = false;


            if (2 == m_correctAnswerPlace)
                //btnAns2.Background = new SolidColorBrush(Colors.Green);
                btnAns2.Foreground = new SolidColorBrush(Colors.Green);
            else
                //btnAns2.Background = new SolidColorBrush(Colors.Red);
                btnAns2.Foreground = new SolidColorBrush(Colors.Red);


            uint timeToAnswer = _maxTime - _currTime ; 
            /*string data = "{" + $"\"answerTime\":{timeToAnswer},\"answer\":\"{btnAns2.Content}\"" + "}";
            string message = "q" + data.Length.ToString().PadLeft(4, '0') + data;
            _socket.Send(message);*/

            //string response = submitAnswer((btnAns2.Content).ToString(), timeToAnswer);
            submitAnswer((btnAns2.Content).ToString(), timeToAnswer);

            /*   Thread thread = new Thread(HowManyAnsweredLoader);
               thread.IsBackground = true; // Set the thread as a background thread
               thread.Start();*/

        }
        





        private void btnAns3_Click(object sender, RoutedEventArgs e)
        {
            setEnableStatusForButtons(false);
            borderOfAnswered.Visibility = Visibility.Visible;
            back = false;


            if (3 == m_correctAnswerPlace)
                //btnAns3.Background = new SolidColorBrush(Colors.Green);
                btnAns3.Foreground = new SolidColorBrush(Colors.Green);
            else
                //btnAns3.Background = new SolidColorBrush(Colors.Red);
                btnAns3.Foreground = new SolidColorBrush(Colors.Red);


            uint timeToAnswer = _maxTime - _currTime; 
            /*string data = "{" + $"\"answerTime\":{timeToAnswer},\"answer\":\"{btnAns3.Content}\"" + "}";
            string message = "q" + data.Length.ToString().PadLeft(4, '0') + data;
            _socket.Send(message);*/

            //string response = submitAnswer((btnAns3.Content).ToString(), timeToAnswer);
            submitAnswer((btnAns3.Content).ToString(), timeToAnswer);


            /* Thread thread = new Thread(HowManyAnsweredLoader);
             thread.IsBackground = true; // Set the thread as a background thread
             thread.Start();*/
        }






        private void btnAns4_Click(object sender, RoutedEventArgs e)
        {
            setEnableStatusForButtons(false);
            borderOfAnswered.Visibility = Visibility.Visible;
            back = false;


            if (4 == m_correctAnswerPlace)
                //btnAns4.Background = new SolidColorBrush(Colors.Green);
                btnAns4.Foreground = new SolidColorBrush(Colors.Green);
            else
                //btnAns4.Background = new SolidColorBrush(Colors.Red);
                btnAns4.Foreground = new SolidColorBrush(Colors.Red);
            

            uint timeToAnswer = _maxTime - _currTime;
            /*string data = "{" + $"\"answerTime\":{timeToAnswer},\"answer\":\"{btnAns4.Content}\"" + "}";
            string message = "q" + data.Length.ToString().PadLeft(4, '0') + data;
            _socket.Send(message);*/

            //string response = submitAnswer((btnAns4.Content).ToString(), timeToAnswer);
            submitAnswer((btnAns4.Content).ToString(), timeToAnswer);


            /*           Thread thread = new Thread(HowManyAnsweredLoader);
                       thread.IsBackground = true; // Set the thread as a background thread
                       thread.Start();*/

        }
    }
}
