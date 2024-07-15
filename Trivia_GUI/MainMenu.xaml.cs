using Server;
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

namespace Trivia_GUI
{
    /// <summary>
    /// Interaction logic for MainMenu.xaml
    /// </summary>
    public partial class MainMenu : Window
    {
        TriviaServer _socket;

        public MainMenu(TriviaServer socket)
        {
            _socket = socket;
            InitializeComponent();
        }

        private void btnCreateRoom_Click(object sender, RoutedEventArgs e)
        {
            CreateRoom createRoom = new CreateRoom(_socket);
            createRoom.Show();
            this.Close();
        }

        private void btnJoinRoom_Click(object sender, RoutedEventArgs e)
        {
            JoinRoom joinRoom = new JoinRoom(_socket);
            joinRoom.Show();
            this.Close();
        }

        private void btnStatistics_Click(object sender, RoutedEventArgs e)
        {
            Statistics highscores = new Statistics(_socket);
            highscores.Show();
            this.Close();
        }

    }
}
