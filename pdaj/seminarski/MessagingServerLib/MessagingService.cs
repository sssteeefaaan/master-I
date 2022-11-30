using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;

namespace MessagingServerLib
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the class name "Service1" in both code and config file together.
    [ServiceBehavior(InstanceContextMode=InstanceContextMode.PerSession)]
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
            if (!StorageAdapter.Instance.Users.ContainsKey(username)) return null;

            Chat c = null;

            if (StorageAdapter.Instance.Chats.ContainsKey(chatID))
            {
                c = StorageAdapter.Instance.Chats[chatID];
            }
            else
            {
                c = new Chat();
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
                return u;
            }
            return null;
        }

        public User Register(string username, string password)
        {
            if (StorageAdapter.Instance.Users.ContainsKey(username)){
                return null;
            }

            User u = new User(username, password);
            StorageAdapter.Instance.Users.Add(username, u);

            return u;
        }

        public bool SendMessage(string username, Message m)
        {
            if (m == null ||
                m.FromUser == null ||
                m.FromUser.Username != username ||
                StorageAdapter.Instance.Messages.ContainsKey(m.ID) ||
                !StorageAdapter.Instance.Chats.ContainsKey(m.ChatID) ||
                !StorageAdapter.Instance.Users.ContainsKey(username)
                ) return false;
            
            Chat c = StorageAdapter.Instance.Chats[m.ChatID];
            
            if (!c.Users.ContainsKey(username)) return false;

            c.Messages.Add(m);
            StorageAdapter.Instance.Messages.Add(m.ID, m);
            return true;
        }
    }

    public sealed class StorageAdapter
    {
        private static StorageAdapter _instance = null;
        private static readonly object padlock = new object();
        private Dictionary<string, User> _users = null;
        private Dictionary<string, Chat> _chats = null;
        private Dictionary<string, Message> _messages = null;

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
