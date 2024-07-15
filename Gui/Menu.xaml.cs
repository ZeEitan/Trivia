using Gui.Views;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
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
using System.Windows.Shapes;
using static Gui.Responses;

namespace Gui
{
    /// <summary>
    /// Interaction logic for Menu.xaml
    /// </summary>
    public partial class Menu : Window
    {
        Socket socket;
        uint selectedRoomId { get; set;}
        string selectedRoomName { get; set; }

        public Menu(Socket socket)
        {
            InitializeComponent();
            this.socket = socket;
            LoadStatistics();
            Main mainContentControl = new Main(socket);
            MainContentControl.Content = mainContentControl;
        }

        public void SetSelectedRoom(uint roomId, string roomName)
        {
            selectedRoomId = roomId;
            selectedRoomName = roomName;
        }

        private void LoadStatistics()
        {
            try
            {
                string statistics = socket.getPersonalStatistics();

                PersonalStatisticsRes personalStatistics = Responses.ParsePersonalStatistics(statistics.Substring(5));

                Games.Content = "Total games played • " + personalStatistics.statistics[0];
                Correct.Content = "Total correct answers • " + personalStatistics.statistics[1];
                //Questions.Content = "Questions • " + personalStatistics.statistics[2];
                Time.Content = "Your average answer time • " + personalStatistics.statistics[3];
            }
            catch (Exception)
            {
                MessageBox.Show("Server is offline", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                Close();
            }
        }

        public void LoadRoomLobby()
        {
            RoomLobby roomLobby = new RoomLobby(socket, selectedRoomId, selectedRoomName);
            MainContentControl.Content = roomLobby;
        }

        public void LoadCreateRoom(){ 
            CreateRoom createRoom = new CreateRoom(socket);
            MainContentControl.Content = createRoom;
            
        }

        public void LoadQuestion(Socket socket, uint maxTime, uint totalQuestions, uint currQuestionQ, uint roomId, string hostName)
        {
            MainContentControl.Content = null;
            Question question = new Question(socket, maxTime, totalQuestions, currQuestionQ, roomId, hostName);
            GameContentControl.Content = question; 
            
        }

        public void LoadMain()
        {
            LoadStatistics();
            GameContentControl.Content = null ;
            Main mainContentControl = new Main(socket);
            MainContentControl.Content = mainContentControl;
        }

        public void LoadAdminlobby(String roomName, uint roomId)
        {
            AdminLobby adminLobby = new AdminLobby(socket, roomName, roomId);
            MainContentControl.Content = adminLobby;
        }

        public void LoadTop(){
            Top top = new Top(socket);
            MainContentControl.Content = top;
        }

        private void Btn_Home_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            Main mainContentControl = new Main(socket);
            socket.CloseRoom();
            socket.LeaveRoom();
            MainContentControl.Content = mainContentControl;
        }

        private void Btn_Logout_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            socket.Close();
            MainContentControl.Content = null;
            new Login().Show();
            Close();
        }

        private void Window_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ButtonState == MouseButtonState.Pressed)
                this.DragMove();
        }

    }
}
