using System;
using System.Collections.Generic;
using System.Diagnostics.Eventing.Reader;
using System.IO;
using System.Linq;
using System.Runtime.Remoting.Metadata.W3cXsd2001;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;

namespace MessagingServerLib
{
    [ServiceContract(SessionMode=SessionMode.Required, CallbackContract = typeof(IMessageCallback))]
    public interface IMessagingService
    {
        [OperationContract(IsOneWay = false)]
        User Register(string username, string password);

        [OperationContract(IsOneWay = false)]
        User Login(string username, string password);

        [OperationContract(IsOneWay = false)]
        Chat JoinChat(string username, string chatID);

        [OperationContract(IsOneWay = true)]
        void Logout(string username);

        [OperationContract(IsOneWay = true)]
        void SendMessage(string username, Message m);

        [OperationContract(IsOneWay = false)]
        Dictionary<string, Chat> GetChats(string username);

        [OperationContract(IsOneWay = false)]
        List<Message> GetMessages(string username,string chatID);
    }

    [DataContract(IsReference = true)]
    public class User
    {
        [DataMember]
        public bool IsValid { get; set; }
        [DataMember]
        public string Username { get; set; }

        [DataMember]
        public string Password { get; set; }

        public IMessageCallback Callback { get; set; }

        [DataMember]
        public Dictionary<string, Chat> Chats { get; set; }

        public User()
        {
            IsValid = false;
            Username = "";
            Password = "";
            Chats = new Dictionary<string, Chat>();
        }
        public User(string username, string password)
        {
            IsValid = false;
            Username = username;
            Password = password;
            Chats = new Dictionary<string, Chat>();
        }

        public bool CheckPassword(string password)
        {
            return Password == password;
        }
    }

    [DataContract(IsReference = true)]
    public class Chat
    {
        [DataMember]
        public string ID { get; set; }
        public Dictionary<string, User> Users { get; set; }
        [DataMember]
        public List<Message> Messages { get; set; }

        public Chat()
        {
            ID = Guid.NewGuid().ToString();
            Users = new Dictionary<string, User>();
            Messages = new List<Message>();
        }
    }

    [DataContract(IsReference = true)]
    public class Message
    {
        [DataMember]
        public string ID { get; set; }

        [DataMember]
        public DateTime Timestamp { get; set; }

        [DataMember]
        public string FromUser { get; set; }

        [DataMember]
        public string Content { get; set; }

        [DataMember]
        public string ChatID { get; set; }

        public Message(string fromUser, string content, string chatID)
        {
            ID = Guid.NewGuid().ToString();
            Timestamp = DateTime.Now;
            FromUser = fromUser;
            Content = content;
            ChatID = chatID;
        }
    }
}
