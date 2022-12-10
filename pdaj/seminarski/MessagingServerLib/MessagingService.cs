using System;
using System.Collections.Generic;
using System.ServiceModel;

namespace MessagingServerLib
{
    [ServiceBehavior(
        InstanceContextMode=InstanceContextMode.PerSession,
        ConcurrencyMode = ConcurrencyMode.Multiple,
        UseSynchronizationContext = true
        )
    ]
    public class MessagingService : IMessagingService
    {
        public Dictionary<string, Chat> GetChats(string username)
        {
            if (!StorageAdapter.Instance.Users.ContainsKey(username)) return null;
            return StorageAdapter.Instance.Users[username].Chats;
        }

        public List<Message> GetMessages(string username, string chatID)
        {
            if (!StorageAdapter.Instance.Chats.ContainsKey(chatID))
            {
                //OperationContext.Current.GetCallbackChannel<IMessageCallback>().OnError($"Chat '{chatID}' doesn't exist!");
                return null;
            }

            if (!StorageAdapter.Instance.Users.ContainsKey(username))
            {
                //OperationContext.Current.GetCallbackChannel<IMessageCallback>().OnError($"User '{username}' doesn't exist!");
                return null;
            }
              
            Chat c = StorageAdapter.Instance.Chats[chatID];

            if (!c.Users.ContainsKey(username))
            {
                //OperationContext.Current.GetCallbackChannel<IMessageCallback>().OnError($"User '{ username }' isn't in the chat '{ chatID }'!");
                return null;
            }

            return c.Messages;
        }

        public Chat JoinChat(string username, string chatID)
        {
            if (!StorageAdapter.Instance.Users.ContainsKey(username)) {
                //OperationContext.Current.GetCallbackChannel<IMessageCallback>().OnError("Not logged in!");
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
            if (!StorageAdapter.Instance.Users.ContainsKey(username))
            {
                //OperationContext.Current.GetCallbackChannel<IMessageCallback>().OnError($"Account with the username '{ username }' doesn't exist!");
                return new User();
            }
            User u = StorageAdapter.Instance.Users[username];

            if (u.CheckPassword(password))
            {
                u.Callback = OperationContext.Current.GetCallbackChannel<IMessageCallback>();
                return u;
            }
            //OperationContext.Current.GetCallbackChannel<IMessageCallback>().OnError("Wrong password!");
            return new User();
        }

        public void Logout(string username)
        {
            if (StorageAdapter.Instance.Users.ContainsKey(username))
            {
                StorageAdapter.Instance.Users[username].Callback = null;
            }
        }

        public User Register(string username, string password)
        {
            
            if (StorageAdapter.Instance.Users.ContainsKey(username)){
                //OperationContext.Current.GetCallbackChannel<IMessageCallback>().OnError("Username taken!");
                return new User();
            }

            User u = new User(username, password);
            u.IsValid = true;
            u.Callback = OperationContext.Current.GetCallbackChannel<IMessageCallback>();
            StorageAdapter.Instance.Users.Add(username, u);

            return u;
        }

        public void SendMessage(string username, Message m)
        {
            var ctx = OperationContext.Current.GetCallbackChannel<IMessageCallback>();

            if (m == null) {
                ctx.OnError("Message cannot be empty!");
                return;
            } else if (m.FromUser == null) {
                ctx.OnError("Sender cannot be empty!");
                return;
            }
            else if (m.FromUser != username) {
                ctx.OnError("Sender must be currently logged user!");
                return;
            } else if (!StorageAdapter.Instance.Chats.ContainsKey(m.ChatID)) {
                ctx.OnError("Chat doesn't exist!");
                return;
            } else if (!StorageAdapter.Instance.Users.ContainsKey(username)){
                ctx.OnError("User doesn't exist!");
                return;
            }

            Chat c = StorageAdapter.Instance.Chats[m.ChatID];

            if (!c.Users.ContainsKey(username)) {
                ctx.OnError($"User '{username}' hasn't joined the chat '{m.ChatID}'!");
                return;
            }

            foreach(User u in c.Users.Values)
            {
                if(u.Callback != null && u.Callback != ctx && u.Username != m.FromUser)
                {
                    u.Callback.OnMessage(m);
                }
            }
            
            c.Messages.Add(m);
            StorageAdapter.Instance.Messages.Add(m.ID, m);
        }
    }

    public sealed class StorageAdapter
    {
        private static StorageAdapter _instance = null;
        private static readonly object padlock = new object();
        private Dictionary<string, User> _users;
        private Dictionary<string, Chat> _chats;
        private Dictionary<string, Message> _messages;

        public StorageAdapter(
            Dictionary<string, User> users,
            Dictionary<string, Chat> chats,
            Dictionary<string, Message> messages
            )
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
                        _instance = new StorageAdapter(
                            new Dictionary<string, User>(),
                            new Dictionary<string, Chat>(),
                            new Dictionary<string, Message>());
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
