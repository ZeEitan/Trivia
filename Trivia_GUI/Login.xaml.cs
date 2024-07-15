using Server;
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

namespace Trivia_GUI
{
    /// <summary>
    /// Interaction logic for Login.xaml
    /// </summary>
    class LoginResponse
    {
        public string? status { get; set; } // Define status property in LoginResponse class donot change the name of the property
    }

    public partial class Login : Window
    {
        private ServerIsOffline? _serverIsOffline = null; // Declare serverIsOffline outside the ConnectToServerAsync method
        TriviaServer? _socket;

        public Login()
        {
            _socket = null;
            InitializeComponent();
            ConnectToServerAsync();
        }

        public Login(TriviaServer socket)
        {
            _socket = socket;
            InitializeComponent();
        }

        private async void ConnectToServerAsync()
        {
            _serverIsOffline = null;
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
                    if (_serverIsOffline?.IsClosed == true)
                        Application.Current.Shutdown();

                    if (_serverIsOffline == null)
                    {
                        _serverIsOffline = new ServerIsOffline();
                        _serverIsOffline.Show();
                        Hide();
                    }
                    await Task.Delay(1000); // Asynchronous delay without blocking UI thread
                }
            }

            _serverIsOffline?.Close(); // Close the server offline window if it is open
        }

        private void btnLogin_Click(object sender, RoutedEventArgs e)
        {

            // Check if server is offline
            if (_socket == null)
            {
                MessageBox.Show("Server is offline");
                return;
            }

            string username = txtUsername.Text; // Get username from text box
            string password = txtPassword.Text; // Get password from text box

            if(!_socket.login(username, password))
            {
                ConnectToServerAsync();
                return;
            }

            string response = _socket.Receive();
            if (response == "")
            { // If server is offline
                ConnectToServerAsync();
                return;
            }

            LoginResponse? res = JsonSerializer.Deserialize<LoginResponse>(response.Substring(5));

            if (res == null)
            {
                MessageBox.Show("Login failed");
                return;
            }

            switch (res.status)
            {
                case "1":
                    _socket.setMyUsername(username);
                    MainMenu mainMenu = new MainMenu(_socket);
                    mainMenu.Show();
                    this.Close();
                    break;
                default:
                    MessageBox.Show(res.status);
                    break;
            }
        }

        private void Signup_Click(object sender, RoutedEventArgs e)
        {
            if (_socket == null)
            {
                ConnectToServerAsync();
                return;
            }

            Signup signup = new Signup(this._socket);
            signup.Show();
            this.Close();
        }

        public void hide_SingupLabel()
        {
            Signup.IsEnabled = false;
        }

        private void txtUsername_TextChanged(object sender, TextChangedEventArgs e)
        {

            if (txtUsername.Text.Length > 0 && txtPassword.Text.Length > 0)
                btnLogin.IsEnabled = true;
            else
                btnLogin.IsEnabled = false;

        }

        private void txtPassword_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (txtUsername.Text.Length > 0 && txtPassword.Text.Length > 0)
                btnLogin.IsEnabled = true;
            else
                btnLogin.IsEnabled = false;
        }

    }
}
