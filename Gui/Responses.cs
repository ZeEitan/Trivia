using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;

namespace Gui
{
    public class Responses
    {
        public struct Error
        {
            public string status { get; set; }
        }

        public static Error ParseError(string response)
        {
            return JsonConvert.DeserializeObject<Error>(response);
        }

        public struct PersonalStatisticsRes
        {
            public string[] statistics { get; set; }
            public string status { get; set; }
        }

        public static PersonalStatisticsRes ParsePersonalStatistics(string response)
        {
            return JsonConvert.DeserializeObject<PersonalStatisticsRes>(response);
        }

        public struct RoomData
        {
            public string name { get; set; }
            public uint id { get; set; }
            public uint maxPlayers { get; set; }
            public uint timePerQuestion { get; set; }
            public uint numOfQuestionsinGame { get; set; }
        }

        public struct RoomResponed
        {
            public string status { get; set; }
            public RoomData[] rooms { get; set; }
        }

        public static RoomResponed ParseRoomResponed(string response)
        {
            return JsonConvert.DeserializeObject<RoomResponed>(response);
        }

        public struct JoinRoomResponed
        {
            string status { get; set; }
        }

        public static JoinRoomResponed ParseJoinRoom(string response)
        {
            return JsonConvert.DeserializeObject<JoinRoomResponed>(response);
        }

        public struct RoomStateResponse
        {
            public uint answerTimeout { get; set; }
            public bool hasGameBegun { get; set; }
            public string[] players { get; set; }
            public uint questionCount { get; set; }
            public string status { get; set; }
        }

        public static RoomStateResponse ParseRoomState(string response)
        {
            return JsonConvert.DeserializeObject<RoomStateResponse>(response);
        }

        public struct CreateRoomResponse
        {
            public string status { get; set; }
            public uint roomId { get; set; }
        }

        public static CreateRoomResponse ParseCreateRoom(string response)
        {
            return JsonConvert.DeserializeObject<CreateRoomResponse>(response);
        }

        public struct StatisticsResponse
        {
            public string[] usernames { get; set; }
            public int[] correctAnswers { get; set; }
            public double[] averageAnswerTimes { get; set; }
            public string status { get; set; }
        }

        public static StatisticsResponse ParseStatistics(string response)
        {
            return JsonConvert.DeserializeObject<StatisticsResponse>(response);
        }

    }
}

