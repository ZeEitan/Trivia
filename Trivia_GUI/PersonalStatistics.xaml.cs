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
    /// Interaction logic for PersonalStatistics.xaml
    /// </summary>
    /// 
    class PersonalStatisticsRes
    { 
        public string[]? statistics { get; set; }
        public string? status { get; set; }
    }

    public partial class PersonalStatistics : Window
    {
        TriviaServer _socket;
        
        public PersonalStatistics(TriviaServer socket){
            _socket = socket;
            InitializeComponent();
            loadStatistics();
        }

        private void loadStatistics()
        {
            string statistics = _socket.getPersonalStatistics();


            if (statistics == "")
            {
                ServerIsOffline serverIsOffline = new ServerIsOffline();
                serverIsOffline.Show();
                Close();
            }

            PersonalStatisticsRes? personalStatistics = JsonSerializer.Deserialize<PersonalStatisticsRes>(statistics[5..]);

            if(personalStatistics == null || personalStatistics.statistics  == null || personalStatistics.statistics.Length != 4)
            {
                ServerIsOffline serverIsOffline = new ServerIsOffline();
                serverIsOffline.Show();
                Close();
                return;
            }

            GamesPlayed.Content += personalStatistics.statistics[0];
            CorrectAnswers.Content += personalStatistics.statistics[1];
            QuestionsAnswered.Content += personalStatistics.statistics[2];
            AvgAnswerTime.Content += personalStatistics.statistics[3];


        }

        private void GoBack_Click(object sender, RoutedEventArgs e)
        {
            Statistics statistics = new Statistics(_socket);
            statistics.Show();
            Close();
        }
    }
}
