using Server;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Text.Json;

namespace Trivia_GUI
{
    /// <summary>
    /// Interaction logic for CreateRoom.xaml
    /// </summary>

    public class createRoomResponse
    {
        
        public string status { get; set; }
        public string roomId { get; set; }
    }
    public partial class CreateRoom : Window
    {
        TriviaServer? _socket;
        ServerIsOffline? serverIsOffline = null;

        public CreateRoom(TriviaServer socket) {
            _socket = socket;
            InitializeComponent();
        }

        private void txtRoomName_TextChanged(object sender, TextChangedEventArgs e){
            enabledCreateRoomBTN();
        }
        

        private void btnCreateRoom_Click(object sender, RoutedEventArgs e){
            string roomName = txtRoomName.Text;
            int maxUsers = (int)SlideMaxUsers.Value;
            int questionsCount = (int)slideQuestionsCount.Value;
            int answerTimeout = (int)slideAnswerTimeout.Value;

            string data = "{" + $"\"roomName\":\"{roomName}\",\"maxUsers\":{maxUsers},\"questionCount\":{questionsCount},\"answerTimeout\":{answerTimeout}" + "}";
            string message = "h" + data.Length.ToString().PadLeft(4, '0') + data;
            //MessageBox.Show(data + "\n" + message);
            _socket.Send(message);

            string response = _socket.Receive();

            //MessageBox.Show(response);
            createRoomResponse? roomState = JsonSerializer.Deserialize<createRoomResponse>(response[5..]);
            

            if (response == null){
                serverIsOffline = new ServerIsOffline();
                serverIsOffline.Show();
                Close();
            }
            //MessageBox.Show(roomState.roomId.ToString());
            RoomLobbyMember roomLobbyMember = new RoomLobbyMember(_socket, roomName , uint.Parse(roomState.roomId));
            roomLobbyMember.Show();
            Close();
        }

        private void enabledCreateRoomBTN()
        {
            if (slideAnswerTimeout.Value > 0 && slideQuestionsCount.Value > 0 && SlideMaxUsers.Value > 0 && txtRoomName.Text.Length > 0)
                btnCreateRoom.IsEnabled = true;
            else
                btnCreateRoom.IsEnabled = false;
        }

        private void SlideMaxUsers_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            labelMaxUsers.Content = "Max Users: " + SlideMaxUsers.Value;
            enabledCreateRoomBTN();
        }

        private void slideQuestionsCount_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            labelQuestionsCount.Content = "Questions Count: " + slideQuestionsCount.Value;
            enabledCreateRoomBTN();
        }

        private void slideAnswerTimeout_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            labelAnswerTimeout.Content = "Answer Timeout (sec): " + slideAnswerTimeout.Value;
            enabledCreateRoomBTN();
        }

        private void GoBack_Click(object sender, RoutedEventArgs e)
        {
            MainMenu statistics = new MainMenu(_socket);
            statistics.Show();
            Close();
        }
    }
}
