using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Sockets;
using System.Windows;
using Trivia_GUI;
using Newtonsoft.Json;
using System.Security.AccessControl;
using System.Data;

namespace Server
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

    public class TriviaServer
    {
        TcpClient client;
        string username;

        public string getMyUsername()
        {
            return username;
        }
        public void setMyUsername(string username)
        {
            this.username = username;
        }

        public TriviaServer()
        {
            client = new TcpClient("127.0.0.1", 8200);

        }

        public Boolean Send(string message)
        {
            try
            {
                NetworkStream stream = client.GetStream();
                byte[] data = Encoding.ASCII.GetBytes(message);
                stream.Write(data, 0, data.Length);
            }
            catch (Exception)
            {
                Close();
                return false;
            }
            return true;
        }

        public string Receive()
        {
            try
            {
                NetworkStream stream = client.GetStream();
                byte[] data = new byte[1024];
                int bytes = stream.Read(data, 0, data.Length);
                return Encoding.ASCII.GetString(data, 0, bytes);
            }
            catch (Exception)
            {
                Close();
                return "";
            }

        }

        public void Close()
        {
            client.Close();
        }

        public string getPersonalStatistics()
        {
            string messsage = "j0000";
            Send(messsage);
            return Receive();
        }

        public string getStatistics()
        {
            string message = "i0000";

            Send(message);

            return Receive();
        }

        public bool login(string username, string password)
        {
            string data = "{ \"username\": \"" + username + "\", \"password\": \"" + password + "\"}";
            string messsage = "a" + data.Length.ToString().PadLeft(4, '0') + "{ \"username\": \"" + username + "\", \"password\": \"" + password + "\"}";

            return Send(messsage);
        }

        public bool signup(string username, string password, string email)
        {
            string data = "{ \"username\": \"" + username + "\", \"password\": \"" + password + "\", \"mail\": \"" + email + "\"}";
            string messsage = "b" + data.Length.ToString().PadLeft(4, '0') + data;

            return Send(messsage);
        }

        public string getRooms()
        {
            string message = "e0000";

            Send(message);

            return Receive();
        }

        public string requestMainMenuHandler()
        {
            string message = "s0000";

            Send(message);

            return Receive();
        }

        public string joinRoom(uint roomId)
        {
            string data = "{ \"roomId\": " + roomId + "}";
            string messsage = "g" + data.Length.ToString().PadLeft(4, '0') + data;

            Send(messsage);
            return Receive();
        }

        public string getRoomState(uint roomId)
        {
            string data = "{ \"roomId\": " + roomId + "}";
            string messsage = "m" + data.Length.ToString().PadLeft(4, '0') + data;
            Send(messsage);
            return Receive();
        }

        public string getRoomStateAdmin()
        {
            return getRoomState(0);
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
