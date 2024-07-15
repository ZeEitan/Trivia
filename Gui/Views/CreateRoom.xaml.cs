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
    /// Interaction logic for CreateRoom.xaml
    /// </summary>
    public partial class CreateRoom : UserControl
    {
        Socket socket;
        public CreateRoom(Socket socket)
        {
            InitializeComponent();
            this.socket = socket;
        }

        private void Txt_Room_Name_GotFocus(object sender, RoutedEventArgs e)
        {
            Txt_Room_Name.Text = "";
        }

        private void Btn_Create_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)//create room
        {
            try
            {
                string response = socket.CreateRoom(Txt_Room_Name.Text, (uint)Slide_Max_Users.Value, (uint)Slide_Questions_Count.Value, (uint)Slide_Answer_Timeout.Value);
                Responses.CreateRoomResponse createRoomResponse = Responses.ParseCreateRoom(response.Substring(5));
                
                Window.GetWindow(this)?.GetType().GetMethod("LoadAdminlobby")?.Invoke(Window.GetWindow(this), new object[] { Txt_Room_Name.Text, createRoomResponse.roomId });
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void Btn_Cancel_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)//cancel create room
        {
            Window.GetWindow(this)?.GetType().GetMethod("LoadMain")?.Invoke(Window.GetWindow(this), null);
        }

        private void Slide_Answer_Timeout_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            slideBarTimePerQuestion.Text = Slide_Answer_Timeout.Value.ToString();
            IsFilledAreFilled();
        }

        private void Slide_Questions_Count_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            slideBarQuestions.Text = Slide_Questions_Count.Value.ToString();
            IsFilledAreFilled();
        }

        private void Slide_Max_Users_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            slideBarMaxUsers.Text = Slide_Max_Users.Value.ToString();
            IsFilledAreFilled();
        }

        private void Txt_Room_Name_TextChanged(object sender, TextChangedEventArgs e)
        {
            IsFilledAreFilled();
        }

        private void IsFilledAreFilled()
        {
            if( Btn_Create == null || Slide_Answer_Timeout == null || Slide_Questions_Count == null || Slide_Max_Users == null || Txt_Room_Name == null)
            {
                return;
            }

            if (Slide_Answer_Timeout.Value > 1 && Slide_Questions_Count.Value > 1 && Slide_Max_Users.Value > 1 && Txt_Room_Name.Text.Length > 0 && Txt_Room_Name.Text != "Room Name")
            {
                Btn_Create.IsEnabled = true;
                Btn_Create.Cursor = Cursors.Hand;
            }
            else
            {
                Btn_Create.IsEnabled = false;
                Btn_Create.Cursor = Cursors.No;
                //Stack_Create.Cursor = Cursors.No;
            }
        }

    }
}
