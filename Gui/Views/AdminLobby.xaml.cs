using System;
using System.Collections.Generic;
using System.Linq;
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

namespace Gui.Views
{
    /// <summary>
    /// Interaction logic for AdminLobby.xaml
    /// </summary>
    public partial class AdminLobby : UserControl
    {
        Socket socket;
        bool loadRoomData = true;
        string roomName;

        uint maxTime;
        uint totalQuestions;
        uint gameId;


        public AdminLobby(Socket socket, string roomName, uint roomId)
        {
            InitializeComponent();
            this.socket = socket;
            this.roomName = roomName;
            this.gameId = roomId;
            Txt_Room_Name.Content = roomName;
            LoadRoomDataBack();
        }

        private void Btn_Close_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            socket.CloseRoom();
            Window.GetWindow(this)?.GetType().GetMethod("LoadMain")?.Invoke(Window.GetWindow(this), null);
        }

        private void Btn_Start_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            socket.StartGame();
            loadRoomData = false;
            object[] methodParams = new object[] { this.socket, this.maxTime, this.totalQuestions, (uint)1, this.gameId, socket.getMyUsername() };
            //LoadQuestion(Socket socket, uint maxTime, uint totalQuestions, uint currQuestionQ, uint roomId, string hostName)
            Window.GetWindow(this)?.GetType().GetMethod("LoadQuestion")?.Invoke(Window.GetWindow(this), methodParams);
        }

        private void LoadRoomDataBack()
        {
            Task.Run(() =>
            {
                while (loadRoomData)
                {
                    try
                    {
                        string roomState = socket.GetRoomState(gameId);
                        Responses.RoomStateResponse roomStateResponse = Responses.ParseRoomState(roomState.Substring(5));
                        this.maxTime = roomStateResponse.answerTimeout;
                        this.totalQuestions = roomStateResponse.questionCount;
                        
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
            Txt_Room_Name.Content = roomName;
            if (roomStateResponse.players == null)
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
        }
    }
}
