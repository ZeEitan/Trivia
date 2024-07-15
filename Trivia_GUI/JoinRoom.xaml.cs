using Microsoft.VisualBasic;
using Server;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace Trivia_GUI
{
    /// <summary>
    /// Interaction logic for JoinRoom.xaml
    /// </summary>
    /// 
    public class RoomData
    {
        public string? name { get; set; }
        public uint id { get; set; }
        public uint maxPlayers { get; set; }
        public uint timePerQuestion { get; set; }
        public uint numOfQuestionsinGame { get; set; }
    }

    public class RoomResponed
    {
        public string? status { get; set; }
        public RoomData[]? rooms { get; set; }
    }

    public partial class JoinRoom : Window
    {

        bool back = true;

        TriviaServer _socket;
        RoomData[]? _rooms;

        public JoinRoom(TriviaServer socket)
        {
            _socket = socket;
            InitializeComponent();
            loadRooms();

            Thread thread = new Thread(backLoad);
            thread.IsBackground = true; // Set the thread as a background thread
            thread.Start();
        }

        private void backLoad()
        {
            while (back)
            {
                Dispatcher.Invoke(() => loadRooms()); // Invoke loadRooms on the UI thread
                Thread.Sleep(2000); // Adjust sleep time according to your needs
            }
        }

        private void Rooms_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if(Rooms.SelectedItem != null)
                btnJoinRoom.IsEnabled = true;
            else 
                btnJoinRoom.IsEnabled = false;
            SelectedRoom.Content = "Selected room: " + Rooms.SelectedItem;
        }

        private void btnJoinRoom_Click(object sender, RoutedEventArgs e)
        {
            if (_rooms == null)
                return;

            foreach (RoomData room in _rooms)
            {
                if (room.name == Rooms.SelectedItem.ToString())
                {
                    back = false;
                    string response = _socket.joinRoom(room.id);

                    RoomLobbyMember roomLobbyMember = new RoomLobbyMember(_socket, room.id, room.name);
                    roomLobbyMember.Show();
                    Close();



                    return;
                }

            }
        }

        private void loadRooms()
        {
            string roomsString = _socket.getRooms();

            if (roomsString == null)
                Close();

            RoomResponed? res = JsonSerializer.Deserialize<RoomResponed>(roomsString.Substring(5));

            List<string> RoomNames = new List<string>();

            if (res == null || res.rooms == null) return;

            _rooms = res.rooms;

            foreach (RoomData roomData in res.rooms)
            {
                RoomNames.Add(roomData.name);
            }
            Rooms.ItemsSource = RoomNames;
        }

        private void GoBack_Click(object sender, RoutedEventArgs e)
        {
            MainMenu statistics = new MainMenu(_socket);
            statistics.Show();
            Close();
        }
    }
}
