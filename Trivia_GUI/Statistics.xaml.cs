using System;
using System.Collections.Generic;
using System.Linq;
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
using Server;

namespace Trivia_GUI
{
    /// <summary>
    /// Interaction logic for Statistics.xaml
    /// </summary>
    /// 

    class HighscoresRespond {

        public string[]? statistics { get; init; }
        public string?  status { get; init; }
    }

    public partial class Statistics : Window
    {
        readonly TriviaServer _socket ;

        public Statistics(TriviaServer socket)
        {
            _socket = socket;
            InitializeComponent();
            loadStatistics();
        }

        private void loadStatistics(){
            string statistics = _socket.getStatistics();

            if(statistics == "")
            {
                ServerIsOffline serverIsOffline = new ServerIsOffline();
                serverIsOffline.Show();
                Close();
                return;
            }

            HighscoresRespond? res = JsonSerializer.Deserialize<HighscoresRespond>(statistics[5..]);

            if(res == null || res.statistics == null)
            {
                ServerIsOffline serverIsOffline = new ServerIsOffline();
                serverIsOffline.Show();
                Close();
                return;
            }
            try
            {
                Top1Name.Content = res.statistics[0];
                Top2Name.Content = res.statistics[1];
                Top3Name.Content = res.statistics[2];
            }
            catch
            {
                // ignored
            }
        }

        private void btnPesonalStatistics_Click(object sender, RoutedEventArgs e)
        {
            PersonalStatistics personalStatistics = new PersonalStatistics(_socket);
            personalStatistics.Show();

            Close();
        }

        private void GoBack_Click(object sender, RoutedEventArgs e)
        {
            MainMenu statistics = new MainMenu(_socket);
            statistics.Show();
            Close();
        }
    }
}
