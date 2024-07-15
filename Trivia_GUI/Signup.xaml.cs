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
using System.Text.Json;

namespace Trivia_GUI
{
    /// <summary>
    /// Interaction logic for Signup.xaml
    /// </summary>
    public partial class Signup : Window
    {

        private ServerIsOffline? serverIsOffline = null; // Declare serverIsOffline outside the ConnectToServerAsync method

        class SignupResponse
        {
            public string? status { get; set; }
        }

        TriviaServer _socket;
        public Signup(TriviaServer socket)
        {
            _socket = socket;
            InitializeComponent();
        }


        private async void ConnectToServerAsync()
        {
            serverIsOffline = null;
            while (true)
            {
                try
                {
                    _socket = new TriviaServer();
                    Show();
                    break;
                }
                catch (Exception)
                {
                    if (serverIsOffline?.IsClosed == true)
                        Application.Current.Shutdown();

                    if (serverIsOffline == null)
                    {
                        serverIsOffline = new ServerIsOffline();
                        serverIsOffline.Show();
                        Hide();
                    }
                    await Task.Delay(1000); // Asynchronous delay without blocking UI thread
                }
            }

            serverIsOffline?.Close(); // Close the server offline window if it is open
        }

        private void btnSginup_Click(object sender, RoutedEventArgs e) {

            // Check if server is offline
            if (_socket == null){
                ConnectToServerAsync();
                return;
            }


            if (txtUsername.Text == "" || txtPassword.Text == "" || txtEmail.Text == ""){
                MessageBox.Show("Please fill all fields");
                return;
            }

            string username = txtUsername.Text;
            string password = txtPassword.Text;
            string email = txtEmail.Text;

            if (!_socket.signup(username, password, email)) {
                ConnectToServerAsync();
                return;
            }

            string response = _socket.Receive();

            if (response == "") {
                ConnectToServerAsync();
                return;
            }


            SignupResponse? res = JsonSerializer.Deserialize<SignupResponse>(response[5..]);

            if (res == null) {
                MessageBox.Show("Signup failed");
                return;
            }

            switch (res.status) {
                case "1":
                    Login main = new Login(_socket);
                    main.hide_SingupLabel();
                    main.Show();
                    this.Close();
                    break;
                case "Allready authentication":
                    MessageBox.Show("Allready authentication");
                    break;
                default:
                    MessageBox.Show("Signup failed");
                    break;
            }

        }

        private void txtUsername_TextChanged(object sender, TextChangedEventArgs e) {
            if(txtUsername.Text.Length > 0 && txtPassword.Text.Length > 0 && txtEmail.Text.Length > 0)
                btnSignup.IsEnabled = true;
            else
                btnSignup.IsEnabled = false;
        }

        private void txtPassword_TextChanged(object sender, TextChangedEventArgs e){
            if (txtUsername.Text.Length > 0 && txtPassword.Text.Length > 0 && txtEmail.Text.Length > 0)
                btnSignup.IsEnabled = true;
            else
                btnSignup.IsEnabled = false;
        }

        private void txtEmail_TextChanged(object sender, TextChangedEventArgs e) {
            if (txtUsername.Text.Length > 0 && txtPassword.Text.Length > 0 && txtEmail.Text.Length > 0)
                btnSignup.IsEnabled = true;
            else
                btnSignup.IsEnabled = false;
        }
    }
}
