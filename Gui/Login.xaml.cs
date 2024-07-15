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
using System.Data.SqlClient;
using System.Configuration;
using System.Net.Sockets;
namespace Gui
{
    /// <summary>
    /// Interaction logic for Login.xaml
    /// </summary>
    public partial class Login : Window
    {
        Socket socket;

        public Login()
        {
            InitializeComponent();
            socket = new Socket();
        }

        public Login(Socket socket)
        {
            InitializeComponent();
            this.socket = socket;
        }

        private void Window_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ButtonState == MouseButtonState.Pressed)
                this.DragMove();
        }

        private void btnLogin_Click(object sender, RoutedEventArgs e)
        {
            try {
                VerifyUser(txtUsername.Text, txtPassword.Password);
                //MessageBox.Show("Login Successfully", "Congrats", MessageBoxButton.OK, MessageBoxImage.Information);
                socket.setMyUsername(txtUsername.Text);
                Menu menu = new Menu(socket);
                menu.Show();
                this.Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void VerifyUser(string username, string password)
        {
            socket.Login(username, password); // CAN THROW EXCEPTION IF LOGIN FAILS OR SERVER IS DOWN
        }

        private void btnExit_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }

        private void txtUsername_TextChanged(object sender, TextChangedEventArgs e)
        {
            isUserFilledUsernameandPassword();
        }

        private void txtPassword_TextChanged(object sender, RoutedEventArgs e)
        {
            isUserFilledUsernameandPassword();
        }

        private void isUserFilledUsernameandPassword()
        {
            if (txtUsername.Text.Length > 0 && txtPassword.Password.Length > 0)
            {
                btnLogin.IsEnabled = true;
            }
            else
            {
                btnLogin.IsEnabled = false;
            }
        }

        private void Txt_Signup_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            Singup singup = new Singup(socket);
            singup.Show();
            this.Close();
        }

        private void test_Click(object sender, RoutedEventArgs e)
        {
            socket.Send("test");
        }
    }
}
