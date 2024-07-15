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

namespace Gui.Views
{
    /// <summary>
    /// Interaction logic for Top.xaml
    /// </summary>
    public partial class Top : UserControl
    {
        Socket socket;
        public Top(Socket socket)
        {
            InitializeComponent();
            this.socket = socket;
            LoadData();
        }

        private void LoadData(){
            string response = socket.GetStatistics();
            Responses.StatisticsResponse statisticsResponse = Responses.ParseStatistics(response.Substring(5));

            First.Text = statisticsResponse.usernames[0] + "\n" + statisticsResponse.correctAnswers[0] + "\n" + statisticsResponse.averageAnswerTimes[0];
            Second.Text = statisticsResponse.usernames[1] + "\n" + statisticsResponse.correctAnswers[1] + "\n" + statisticsResponse.averageAnswerTimes[1];
            Third.Text = statisticsResponse.usernames[2] + "\n" + statisticsResponse.correctAnswers[2] + "\n" + statisticsResponse.averageAnswerTimes[2];
        }
    }
}
