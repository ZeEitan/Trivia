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

namespace Gui
{
    /// <summary>
    /// Interaction logic for Singup.xaml
    /// </summary>
    public partial class Singup : Window
    {
        Socket socket;
        public Singup(Socket socket)
        {
            InitializeComponent();
            this.socket = socket;
        }

        private void Window_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ButtonState == MouseButtonState.Pressed)
                this.DragMove();
        }


        private void txtEmail_TextChanged(object sender, TextChangedEventArgs e)
        {
            isUserFilledUsernameAndPasswordAndEmail();
        }

        private void txtUsername_TextChanged(object sender, TextChangedEventArgs e)
        {
            isUserFilledUsernameAndPasswordAndEmail();
        }

        private void txtPassword_TextChanged(object sender, RoutedEventArgs e)
        {
            isUserFilledUsernameAndPasswordAndEmail();
        }

        private void isUserFilledUsernameAndPasswordAndEmail()
        {
            if (txtUsername.Text.Length > 0 && txtPassword.Password.Length > 0 && txtEmail.Text.Length > 0)
            {
                btnSingup.IsEnabled = true;
            }
            else
            {
                btnSingup.IsEnabled = false;
            }
        }

        private void btnExit_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }

        private void VerifySingup(string username, string password, string email)
        {
            socket.Signup(username, password, email); // CAN THROW EXCEPTION IF SINGUP FAILS OR SERVER IS DOWN
        }

        private void btnSingup_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                VerifySingup(txtUsername.Text, txtPassword.Password, txtEmail.Text);
                Login login = new Login();
                login.Show();
                this.Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }
    }
}
