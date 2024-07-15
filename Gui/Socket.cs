using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Sockets;
using System.Windows;
using static Gui.Responses;
using System.Threading;
using Newtonsoft.Json;

namespace Gui
{


    public class GetPlayersInGameReponse
    {
        public int Status { get; set; }
        public string[] players { get; set; }
    }

    public class GetHowManyAnsweredResponse
    {
        public int Answered { get; set; }
        public int OutOf { get; set; }

    }


    public class Socket
    {
        TcpClient _client;
        bool cheackServerStatusTask = true;
        string username;

        public Socket()
        {
            try{
                _client = new TcpClient("127.0.0.1", 8200); //Port 8200 localhost
            }
            catch (Exception)
            {
                throw new Exception("Server is not available");
            }
            //StartServerCheck();
        }
        public string getMyUsername()
        {
            return username;
        }
        public void setMyUsername(string username)
        {
            this.username = username;
        }
        private void StartServerCheck()
        {
            Task.Run(async () =>
            {
                while (cheackServerStatusTask)
                {
                    await Task.Delay(3000); // Adjust the delay as needed
                    CheckServerStatus();
                }
            });
        }

        private void CheckServerStatus()
        {
            try
            {
                using (var client = new TcpClient("127.0.0.1", 8200))
                {
                    // If connection is successful, server is online
                    Console.WriteLine("Server is online");
                }
            }
            catch (Exception)
            {
                // If connection fails, server is offline
                Application.Current.Shutdown();
            }
        }

        public void Close()
        {
            cheackServerStatusTask = false;
            _client.Close();
        }

        public string getPersonalStatistics()
        {
            string messsage = "j0000";
            Send(messsage);
            return Receive();
        }

        public string getRooms()
        {
            string message = "e0000";
            Send(message);
            return Receive();
        }

        public void Send(string message)
        {
            try
            {
                NetworkStream stream = _client.GetStream();
                byte[] data = Encoding.ASCII.GetBytes(message);
                stream.Write(data, 0, data.Length);
            }
            catch (Exception)
            {
                throw new Exception("Server is not available");
            }
        }

        public string Receive()
        {
            try
            {
                NetworkStream stream = _client.GetStream();
                byte[] data = new byte[1024];
                int bytes = stream.Read(data, 0, data.Length);
                return Encoding.ASCII.GetString(data, 0, bytes);
            }
            catch (Exception)
            {
                throw new Exception("Server is not available");
            }
        }

        public void Login(string username, string password)
        {
            // CreateRoom the data JSON string
            string data = $"{{ \"username\": \"{username}\", \"password\": \"{password}\" }}";

            // CreateRoom the message with length information
            string messageLength = data.Length.ToString().PadLeft(4, '0');
            string message = $"a{messageLength}{data}";

            // Send the message
            Send(message);
            string response = Receive();
            if (response[0] != 'a')
            {
                throw new Exception(Responses.ParseError(response.Substring(5)).status); // Parse the error message and throw the error status message
            }
            
        }

        public void Signup(string username, string password, string email)
        {
            // CreateRoom the data JSON string
            string data = "{ \"username\": \"" + username + "\", \"password\": \"" + password + "\", \"mail\": \"" + email + "\"}";
            string messsage = "b" + data.Length.ToString().PadLeft(4, '0') + data;

            Send(messsage);
            string response = Receive();
            if (response[0] != 'b')
            {
                throw new Exception(Responses.ParseError(response.Substring(5)).status); // Parse the error message and throw the error status message
            }
        }

        public string JoinRoom(uint roomId)
        {
            string data = "{ \"roomId\": " + roomId + "}";
            string messsage = "g" + data.Length.ToString().PadLeft(4, '0') + data;

            Send(messsage);
            return Receive();
        }

        public string GetRoomState(uint roomId)
        {
            string data = "{ \"roomId\": " + roomId + "}";
            string messsage = "m" + data.Length.ToString().PadLeft(4, '0') + data;
            Send(messsage);
            return Receive();
        }

        public string CreateRoom(string roomName, uint maxPlayers, uint questionCount, uint answerTimeout)
        {
            string data = "{ \"roomName\": \"" + roomName + "\", \"maxUsers\": " + maxPlayers + ", \"questionCount\": " + questionCount + ", \"answerTimeout\": " + answerTimeout + "}";
            string messsage = "h" + data.Length.ToString().PadLeft(4, '0') + data;
            Send(messsage);
            string response = Receive();

            if (response[0] != 'h')
            {
                throw new Exception(Responses.ParseError(response.Substring(5)).status); // Parse the error message and throw the error status message
            }

            return response;
        }

        public string LeaveRoom()
        {
            string messsage = "n0000";
            Send(messsage);
            return Receive();
        }

        public string CloseRoom()
        {
            string messsage = "l0000";
            Send(messsage);
            return Receive();
        }

        public string StartGame()
        {
            //string messsage = "k0000";
            string data = "{" + $"\"status\":\"1" + "}";
            string message = "k" + data.Length.ToString().PadLeft(4, '0') + data;
            Send(message);
            return Receive();
        }

        public string GetStatistics()
        {
            string message = "i0000";
            Send(message);
            return Receive();
        }


        public string requestMainMenuHandler()
        {
            string message = "s0000";

            Send(message);

            return Receive();
        }
        public GetPlayersInGameReponse getPlayersInGame(uint gameId)
        {
            string data = "{ \"gameId\": " + gameId + "}";
            string message = "t" + data.Length.ToString().PadLeft(4, '0') + data;
            Send(message);

            string result = Receive();
            result = result.Substring(5);
            GetPlayersInGameReponse resultAfter = JsonConvert.DeserializeObject<GetPlayersInGameReponse>(result);

            string show = "";
            for (int i = 0; i < resultAfter.players.Length; i++)
            {
                show += resultAfter.players[i];
                show += " ";
            }
            //MessageBox.Show(show);

            return resultAfter;
        }
        public int getHowManyAnswered(uint questionNum)
        {
            string data = "{ \"questionNum\": " + questionNum + "}";
            string message = "v" + data.Length.ToString().PadLeft(4, '0') + data;
            Send(message);

            string result = Receive();
            result = result.Substring(5);
            GetHowManyAnsweredResponse resultAfter = JsonConvert.DeserializeObject<GetHowManyAnsweredResponse>(result);

            return resultAfter.Answered;
        }


        public void endTheGame(uint gameId)
        {
            string data = "{ \"gameId\": " + gameId + "}";
            string message = "w" + data.Length.ToString().PadLeft(4, '0') + data;
            Send(message);

            string result = Receive();
        }
    }
}
