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
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the interface name "IService1" in both code and config file together.
    [ServiceContract(SessionMode=SessionMode.Required, CallbackContract = typeof(IMessageCallback))]
    public interface IMessagingService
    {
        [OperationContract]
        User Register(string username, string password);
        [OperationContract]
        User Login(string username, string password);
        [OperationContract]
        Chat JoinChat(string username, string chatID);
        [OperationContract]
        string SendMessage(string username, Message m);
        [OperationContract]
        Dictionary<string, Chat> GetChats(string username);
        [OperationContract]
        List<Message> GetMessages(string username,string chatID);
    }

    [DataContract(IsReference = true)]
    public class User
    {
        [DataMember]
        public string Username { get; set; }

        [DataMember]
        public string Password { get; set; }

        public IMessageCallback Callback { get; set; }

        [DataMember]
        public Dictionary<string, Chat> Chats { get; set; }

        public User(string username, string password)
        {
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
        [DataMember]
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
        public User FromUser { get; set; }

        [DataMember]
        public string Content { get; set; }

        [DataMember]
        public string ChatID { get; set; }

        public Message(User fromUser, string content, string chatID)
        {
            ID = Guid.NewGuid().ToString();
            Timestamp = DateTime.Now;
            FromUser = fromUser;
            Content = content;
            ChatID = chatID;
        }
    }
}
