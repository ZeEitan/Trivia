using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Security.Cryptography;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using static Gui.Responses;

namespace Gui.Views
{
    /// <summary>
    /// Interaction logic for Main.xaml
    /// </summary>
    public partial class Main : UserControl
    {
        Socket socket;
        bool loadRooms = true;
        RoomData[] rooms;

        public Main(Socket socket)
        {
            this.socket = socket;
            InitializeComponent();
            BackLoad();
        }

        private void BackLoad()
        {
            Task.Run(() =>
            {
                while (loadRooms)//Check if the UserControl is still loaded
                {
                    // Load rooms in the background
                    Dispatcher.Invoke(() => LoadRooms()); // Invoke LoadRooms on the UI thread
                    Thread.Sleep(3000); // Adjust sleep time according to your needs
                }
            });
        }

        private void LoadRooms()
        {
            try
            {
                if (loadRooms == false)
                    return;

                string roomsString = socket.getRooms();

                if (roomsString == null)
                {
                    Window.GetWindow(this)?.Close();
                    return;
                }

                RoomResponed roomResponed = Responses.ParseRoomResponed(roomsString.Substring(5));
                rooms = roomResponed.rooms;

                List<string> RoomNames = new List<string>();

                if(rooms == null)
                    return;

                foreach (RoomData roomData in roomResponed.rooms)
                {
                    RoomNames.Add(roomData.name);
                }
                Rooms.ItemsSource = RoomNames;
            }
            catch (Exception)
            {
                Window.GetWindow(this)?.Close();
            }
        }

        private void Rooms_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            Btn_Join.IsEnabled = Rooms.SelectedItem != null;
            Btn_Join.Cursor = Rooms.SelectedItem != null ? Cursors.Hand : Cursors.Arrow;
        }

        private void Btn_Join_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {

            if (rooms == null)
                return;

            foreach (RoomData room in rooms)
            {
                if (room.name == Rooms.SelectedItem.ToString())
                {
                    string response = socket.JoinRoom(room.id);
                    if (response[0] == 'c')// Error
                    {
                        Error error = Responses.ParseError(response.Substring(5));
                        MessageBox.Show(error.status, "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                    }
                    else
                    {
                        loadRooms = false;

                        Window.GetWindow(this)?.GetType().GetMethod("SetSelectedRoom")?.Invoke(Window.GetWindow(this), new object[] { room.id, room.name });
                        Window.GetWindow(this)?.GetType().GetMethod("LoadRoomLobby")?.Invoke(Window.GetWindow(this), null);

                        return;
                    }

                }
            }

        }

        private void Btn_create_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)//create room
        {
            loadRooms = false;
            Window.GetWindow(this)?.GetType().GetMethod("LoadCreateRoom")?.Invoke(Window.GetWindow(this), null);
            Window.GetWindow(this)?.Close();
        }

        private void Btn_Top_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)//leaderboard
        {
            loadRooms = false;
            Window.GetWindow(this)?.GetType().GetMethod("LoadTop")?.Invoke(Window.GetWindow(this), null);
            Window.GetWindow(this)?.Close();
        }
    }
}
