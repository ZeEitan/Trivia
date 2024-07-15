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

namespace Trivia_GUI
{
    /// <summary>
    /// Interaction logic for RoomLobbyMember.xaml
    /// </summary>
    /// 

    public class RoomStateResponse
    {
        public uint answerTimeout { get; set; }
        public bool hasGameBegun { get; set; }
        public string[] players { get; set; }
        public uint questionCount { get; set; }
        public string status { get; set; }
    }

    public partial class RoomLobbyMember : Window
    {
        TriviaServer _socket;
        string _roomName;
        uint _roomId;
        bool back = true;
        
        public RoomLobbyMember(TriviaServer socket, uint roomId, string roomName)// constuctor for member
        {
            _socket = socket;
            _roomId = roomId;
            
            InitializeComponent();
            Thread thread = new Thread(LoadRoomDataBack);
            thread.IsBackground = true; // Set the thread as a background thread
            thread.Start();
            _roomName = roomName;
            btnStartGame.IsEnabled = false;
            //MessageBox.Show("error");
        }

        //For Admin to create room TEST
        public RoomLobbyMember(TriviaServer socket, string roomName, uint roomNewId) // constuctor for admin
        {
            _socket = socket;
            _roomId = roomNewId;
            _roomName = roomName;
           
            InitializeComponent();
            Thread thread = new Thread(LoadRoomDataBack);
            thread.IsBackground = true; // Set the thread as a background thread
            thread.Start();
            btnStartGame.IsEnabled = true;
        }

        private void LoadRoomDataBack()
        {
            while (back)
            {
                Dispatcher.Invoke(() => LoadRoomData()); // Invoke loadRooms on the UI thread
                Thread.Sleep(3000); // Adjust sleep time according to your needs
            }
        }

        private void LoadRoomData()
        {
            string roomData = _socket.getRoomState(_roomId);
            if (roomData == "")
            {
                ServerIsOffline serverIsOffline = new ServerIsOffline();
                serverIsOffline.Show();
                Close();
            }

            //MessageBox.Show(roomData);
            /*
             m0091("answerTimeout": 3, "hasGameBegun":false,"players":["2","1"],"questionCount": 3, "status":"1"}
            roomData format
             */


            RoomStateResponse? roomState = JsonSerializer.Deserialize<RoomStateResponse>(roomData[5..]);
            if (roomState == null )
            {
                ServerIsOffline serverIsOffline = new ServerIsOffline();
                serverIsOffline.Show();
                Close();
                return;
            }
            if (roomState.status == "Room not found")
            {

                back = false;
                _socket.requestMainMenuHandler();
                MainMenu mainMenu = new MainMenu(_socket);
                MessageBox.Show("The host left the room so we are sending you to main menu to main menu.");
                mainMenu.Show();
                Close();
                return;

            }


            Titel.Content = _roomName;

            Members.Items.Clear();

            foreach (string player in roomState.players)
            {
                Members.Items.Add(player);
            }

            AdminName.Content = "Admin: " + roomState.players[0];


            
            if (roomState.hasGameBegun)
            {
                
                back = false;
                Question question = new Question(_socket, roomState.answerTimeout, roomState.questionCount, 1, _roomId, roomState.players[0]);
                question.Show();
                Close();
            }
            
        }

        private void startGame(object sender, RoutedEventArgs e)
        {
            string roomData = _socket.getRoomState(_roomId);
           
            if (roomData == "")
            {
                ServerIsOffline serverIsOffline = new ServerIsOffline();
                serverIsOffline.Show();
                Close();
            }
            RoomStateResponse? roomState = JsonSerializer.Deserialize<RoomStateResponse>(roomData[5..]);
            if (roomState == null || roomState.status != "1")
            {
                ServerIsOffline serverIsOffline = new ServerIsOffline();
                serverIsOffline.Show();
                Close();
                return;
            }
       



            string data = "{" + $"\"status\":\"1"+"}";
            string message = "k" + data.Length.ToString().PadLeft(4, '0') + data;
            _socket.Send(message);
           
            string response = _socket.Receive();
            this.back = false; 
            Question question = new Question(_socket, roomState.answerTimeout, roomState.questionCount, 1, _roomId, roomState.players[0]);
            question.Show();
            Close();
            
        }
        private void Members_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            
        }
    }
}
