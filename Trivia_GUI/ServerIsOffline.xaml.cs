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
    /// Interaction logic for ServerIsOffline.xaml
    /// </summary>
    public partial class ServerIsOffline : Window
    {
        public ServerIsOffline(){
            InitializeComponent();
        }

        public bool IsClosed { get; private set; }

        protected override void OnClosed(EventArgs e){
            base.OnClosed(e);
            IsClosed = true;
        }
    }


}
