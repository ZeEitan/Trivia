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
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Threading;
using System.Net.Sockets;

namespace Gui.Views
{
    /// <summary>
    /// Interaction logic for RoomLobby.xaml
    /// </summary>
    public partial class RoomLobby : UserControl
    {
        Socket socket;
        uint roomId;
        string roomName;
        bool loadRoomData = true;

        public RoomLobby(Socket socket, uint roomId, string roomName)
        {
            InitializeComponent();
            this.socket = socket;
            this.roomId = roomId;
            this.roomName = roomName;
            LoadRoomDataBack();
        }

        private void LoadRoomDataBack()
        {
            Task.Run(() =>
            {
                while (loadRoomData)
                {
                    try
                    {
                        string roomState = socket.GetRoomState(roomId);
                        Responses.RoomStateResponse roomStateResponse = Responses.ParseRoomState(roomState.Substring(5));
                        Dispatcher.Invoke(() => LoadRoomData(roomStateResponse));
                    }
                    catch (Exception)
                    {
                        MessageBox.Show("Server is offline", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                        break;
                    }
                    Thread.Sleep(3000);
                }
            });
        }

        private void LoadRoomData(Responses.RoomStateResponse roomStateResponse)
        {

            if (roomStateResponse.status == "Room not found")
            {

                loadRoomData = false;
                socket.requestMainMenuHandler();
                MessageBox.Show("The host left the room so we are sending you to the main menu.");

                Window.GetWindow(this)?.GetType().GetMethod("LoadMain")?.Invoke(Window.GetWindow(this), null);
                return;

            }


            Txt_Room_Name.Content = roomName;
            if(roomStateResponse.players == null)
            {
                loadRoomData = false;
                Window.GetWindow(this)?.GetType().GetMethod("LoadMain")?.Invoke(Window.GetWindow(this), null);
                return;
            }

            Txt_Admin.Content = "Admin • " + roomStateResponse.players[0];

            Players.Items.Clear();

            foreach (string player in roomStateResponse.players)
            {
                Players.Items.Add(player);
            }



            if (roomStateResponse.hasGameBegun)
            {
                loadRoomData = false;
                object[] methodParams = new object[] { this.socket, roomStateResponse.answerTimeout, roomStateResponse.questionCount, (uint)1, roomId, socket.getMyUsername() };
                //LoadQuestion(Socket socket, uint maxTime, uint totalQuestions, uint currQuestionQ, uint roomId, string hostName)
                Window.GetWindow(this)?.GetType().GetMethod("LoadQuestion")?.Invoke(Window.GetWindow(this), methodParams);
            }
        }

        private void Btn_Leave_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            loadRoomData = false;
            socket.LeaveRoom();
            Window.GetWindow(this)?.GetType().GetMethod("LoadMain")?.Invoke(Window.GetWindow(this), null);
        }
    }
}
