using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.ServiceModel.Configuration;
using System.Text;
using Microsoft.Extensions.Logging;

namespace MessagingServerLib
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the class name "Service1" in both code and config file together.
    [ServiceBehavior(InstanceContextMode=InstanceContextMode.Single, ConcurrencyMode = ConcurrencyMode.Multiple)]
    public class MessagingService : IMessagingService
    {
        public Dictionary<string, Chat> GetChats(string username)
        {
            if (!StorageAdapter.Instance.Users.ContainsKey(username)) return null;
            return StorageAdapter.Instance.Users[username].Chats;
        }

        public List<Message> GetMessages(string username, string chatID)
        {
            if (!StorageAdapter.Instance.Chats.ContainsKey(chatID) || !StorageAdapter.Instance.Users.ContainsKey(username)) return null;
            Chat c = StorageAdapter.Instance.Chats[chatID];
            if (!c.Users.ContainsKey(username)) return null;
            return c.Messages;
        }

        public Chat JoinChat(string username, string chatID)
        {
            if (!StorageAdapter.Instance.Users.ContainsKey(username)) {
                OperationContext.Current.GetCallbackChannel<IMessageCallback>().OnError("Not logged in!");
                return null;
             }

            Chat c = null;

            if (StorageAdapter.Instance.Chats.ContainsKey(chatID))
            {
                c = StorageAdapter.Instance.Chats[chatID];
            }
            else
            {
                c = new Chat()
                {
                    ID = chatID,
                    Users = new Dictionary<string, User>(),
                    Messages = new List<Message>()
                };
                StorageAdapter.Instance.Chats.Add(c.ID, c);
            }

            if (!c.Users.ContainsKey(username))
            {
                User u = StorageAdapter.Instance.Users[username];
                u.Chats.Add(chatID, c);
                c.Users.Add(username, u);
            }

            return c;
        }

        public User Login(string username, string password)
        {
            if (!StorageAdapter.Instance.Users.ContainsKey(username)) return null;
            User u = StorageAdapter.Instance.Users[username];

            if (u.CheckPassword(password))
            {
                u.Callback = OperationContext.Current.GetCallbackChannel<IMessageCallback>();
                return u;
            }
            OperationContext.Current.GetCallbackChannel<IMessageCallback>().OnError("Wrong password!");
            return null;
        }

        public User Register(string username, string password)
        {
            
            if (StorageAdapter.Instance.Users.ContainsKey(username)){
                OperationContext.Current.GetCallbackChannel<IMessageCallback>().OnError("Username taken!");
                return null;
            }

            User u = new User(username, password);
            u.Callback = OperationContext.Current.GetCallbackChannel<IMessageCallback>();
            StorageAdapter.Instance.Users.Add(username, u);

            return u;
        }

        public string SendMessage(string username, Message m)
        {
            if (m == null) {
                OperationContext.Current.GetCallbackChannel<IMessageCallback>().OnError("Message cannot be empty!");
                return null;
            } else if (m.FromUser == null) {
                OperationContext.Current.GetCallbackChannel<IMessageCallback>().OnError("Sender cannot be empty!");
                return null;
            } else if (m.FromUser.Username != username) {
                OperationContext.Current.GetCallbackChannel<IMessageCallback>().OnError("Sender must be currently logged user!");
                return null;
            } else if (!StorageAdapter.Instance.Chats.ContainsKey(m.ChatID)) {
                OperationContext.Current.GetCallbackChannel<IMessageCallback>().OnError("Chat doesn't exist!");
                return null;
            } else if (!StorageAdapter.Instance.Users.ContainsKey(username)){
                OperationContext.Current.GetCallbackChannel<IMessageCallback>().OnError("User doesn't exist!");
                return null;
            }

            m.ID = Guid.NewGuid().ToString();
            Chat c = StorageAdapter.Instance.Chats[m.ChatID];
            
            if (!c.Users.ContainsKey(username)) return null;

            foreach(User u in c.Users.Values)
            {
                if(u.Username != m.FromUser.Username)
                {
                    u.Callback.OnMessage(m);
                }
            }

            c.Messages.Add(m);
            StorageAdapter.Instance.Messages.Add(m.ID, m);
            return m.ID;
        }
    }

    public sealed class StorageAdapter
    {
        private static StorageAdapter _instance = null;
        private static readonly object padlock = new object();
        private Dictionary<string, User> _users;
        private Dictionary<string, Chat> _chats;
        private Dictionary<string, Message> _messages;

        public StorageAdapter(Dictionary<string, User> users, Dictionary<string, Chat> chats, Dictionary<string, Message> messages)
        {
            _users = users;
            _chats = chats;
            _messages = messages;
        }
        public static StorageAdapter Instance
        {
            get
            {
                lock (padlock)
                {
                    if (_instance == null)
                    {
                        _instance = new StorageAdapter(new Dictionary<string, User>(), new Dictionary<string, Chat>(), new Dictionary<string, Message>());
                    }
                    return _instance;
                }
            }
        }

        public Dictionary<string, User> Users
        {
            get { return _users; }
        }

        public Dictionary<string, Chat> Chats
        {
            get { return _chats; }
        }

        public Dictionary<string, Message> Messages
        {
            get { return _messages; }
        }
    }
}
