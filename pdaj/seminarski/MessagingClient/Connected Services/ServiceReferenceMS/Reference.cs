﻿//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version:4.0.30319.42000
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

namespace MessagingClient.ServiceReferenceMS {
    using System.Runtime.Serialization;
    using System;
    
    
    [System.Diagnostics.DebuggerStepThroughAttribute()]
    [System.CodeDom.Compiler.GeneratedCodeAttribute("System.Runtime.Serialization", "4.0.0.0")]
    [System.Runtime.Serialization.DataContractAttribute(Name="User", Namespace="http://schemas.datacontract.org/2004/07/MessagingServerLib", IsReference=true)]
    [System.SerializableAttribute()]
    public partial class User : object, System.Runtime.Serialization.IExtensibleDataObject, System.ComponentModel.INotifyPropertyChanged {
        
        [System.NonSerializedAttribute()]
        private System.Runtime.Serialization.ExtensionDataObject extensionDataField;
        
        [System.Runtime.Serialization.OptionalFieldAttribute()]
        private System.Collections.Generic.Dictionary<string, MessagingClient.ServiceReferenceMS.Chat> ChatsField;
        
        [System.Runtime.Serialization.OptionalFieldAttribute()]
        private string PasswordField;
        
        [System.Runtime.Serialization.OptionalFieldAttribute()]
        private string UsernameField;
        
        [global::System.ComponentModel.BrowsableAttribute(false)]
        public System.Runtime.Serialization.ExtensionDataObject ExtensionData {
            get {
                return this.extensionDataField;
            }
            set {
                this.extensionDataField = value;
            }
        }
        
        [System.Runtime.Serialization.DataMemberAttribute()]
        public System.Collections.Generic.Dictionary<string, MessagingClient.ServiceReferenceMS.Chat> Chats {
            get {
                return this.ChatsField;
            }
            set {
                if ((object.ReferenceEquals(this.ChatsField, value) != true)) {
                    this.ChatsField = value;
                    this.RaisePropertyChanged("Chats");
                }
            }
        }
        
        [System.Runtime.Serialization.DataMemberAttribute()]
        public string Password {
            get {
                return this.PasswordField;
            }
            set {
                if ((object.ReferenceEquals(this.PasswordField, value) != true)) {
                    this.PasswordField = value;
                    this.RaisePropertyChanged("Password");
                }
            }
        }
        
        [System.Runtime.Serialization.DataMemberAttribute()]
        public string Username {
            get {
                return this.UsernameField;
            }
            set {
                if ((object.ReferenceEquals(this.UsernameField, value) != true)) {
                    this.UsernameField = value;
                    this.RaisePropertyChanged("Username");
                }
            }
        }
        
        public event System.ComponentModel.PropertyChangedEventHandler PropertyChanged;
        
        protected void RaisePropertyChanged(string propertyName) {
            System.ComponentModel.PropertyChangedEventHandler propertyChanged = this.PropertyChanged;
            if ((propertyChanged != null)) {
                propertyChanged(this, new System.ComponentModel.PropertyChangedEventArgs(propertyName));
            }
        }
    }
    
    [System.Diagnostics.DebuggerStepThroughAttribute()]
    [System.CodeDom.Compiler.GeneratedCodeAttribute("System.Runtime.Serialization", "4.0.0.0")]
    [System.Runtime.Serialization.DataContractAttribute(Name="Chat", Namespace="http://schemas.datacontract.org/2004/07/MessagingServerLib", IsReference=true)]
    [System.SerializableAttribute()]
    public partial class Chat : object, System.Runtime.Serialization.IExtensibleDataObject, System.ComponentModel.INotifyPropertyChanged {
        
        [System.NonSerializedAttribute()]
        private System.Runtime.Serialization.ExtensionDataObject extensionDataField;
        
        [System.Runtime.Serialization.OptionalFieldAttribute()]
        private string IDField;
        
        [System.Runtime.Serialization.OptionalFieldAttribute()]
        private System.Collections.Generic.List<MessagingClient.ServiceReferenceMS.Message> MessagesField;
        
        [System.Runtime.Serialization.OptionalFieldAttribute()]
        private System.Collections.Generic.Dictionary<string, MessagingClient.ServiceReferenceMS.User> UsersField;
        
        [global::System.ComponentModel.BrowsableAttribute(false)]
        public System.Runtime.Serialization.ExtensionDataObject ExtensionData {
            get {
                return this.extensionDataField;
            }
            set {
                this.extensionDataField = value;
            }
        }
        
        [System.Runtime.Serialization.DataMemberAttribute()]
        public string ID {
            get {
                return this.IDField;
            }
            set {
                if ((object.ReferenceEquals(this.IDField, value) != true)) {
                    this.IDField = value;
                    this.RaisePropertyChanged("ID");
                }
            }
        }
        
        [System.Runtime.Serialization.DataMemberAttribute()]
        public System.Collections.Generic.List<MessagingClient.ServiceReferenceMS.Message> Messages {
            get {
                return this.MessagesField;
            }
            set {
                if ((object.ReferenceEquals(this.MessagesField, value) != true)) {
                    this.MessagesField = value;
                    this.RaisePropertyChanged("Messages");
                }
            }
        }
        
        [System.Runtime.Serialization.DataMemberAttribute()]
        public System.Collections.Generic.Dictionary<string, MessagingClient.ServiceReferenceMS.User> Users {
            get {
                return this.UsersField;
            }
            set {
                if ((object.ReferenceEquals(this.UsersField, value) != true)) {
                    this.UsersField = value;
                    this.RaisePropertyChanged("Users");
                }
            }
        }
        
        public event System.ComponentModel.PropertyChangedEventHandler PropertyChanged;
        
        protected void RaisePropertyChanged(string propertyName) {
            System.ComponentModel.PropertyChangedEventHandler propertyChanged = this.PropertyChanged;
            if ((propertyChanged != null)) {
                propertyChanged(this, new System.ComponentModel.PropertyChangedEventArgs(propertyName));
            }
        }
    }
    
    [System.Diagnostics.DebuggerStepThroughAttribute()]
    [System.CodeDom.Compiler.GeneratedCodeAttribute("System.Runtime.Serialization", "4.0.0.0")]
    [System.Runtime.Serialization.DataContractAttribute(Name="Message", Namespace="http://schemas.datacontract.org/2004/07/MessagingServerLib", IsReference=true)]
    [System.SerializableAttribute()]
    public partial class Message : object, System.Runtime.Serialization.IExtensibleDataObject, System.ComponentModel.INotifyPropertyChanged {
        
        [System.NonSerializedAttribute()]
        private System.Runtime.Serialization.ExtensionDataObject extensionDataField;
        
        [System.Runtime.Serialization.OptionalFieldAttribute()]
        private string ChatIDField;
        
        [System.Runtime.Serialization.OptionalFieldAttribute()]
        private string ContentField;
        
        [System.Runtime.Serialization.OptionalFieldAttribute()]
        private MessagingClient.ServiceReferenceMS.User FromUserField;
        
        [System.Runtime.Serialization.OptionalFieldAttribute()]
        private string IDField;
        
        [System.Runtime.Serialization.OptionalFieldAttribute()]
        private System.DateTime TimestampField;
        
        [global::System.ComponentModel.BrowsableAttribute(false)]
        public System.Runtime.Serialization.ExtensionDataObject ExtensionData {
            get {
                return this.extensionDataField;
            }
            set {
                this.extensionDataField = value;
            }
        }
        
        [System.Runtime.Serialization.DataMemberAttribute()]
        public string ChatID {
            get {
                return this.ChatIDField;
            }
            set {
                if ((object.ReferenceEquals(this.ChatIDField, value) != true)) {
                    this.ChatIDField = value;
                    this.RaisePropertyChanged("ChatID");
                }
            }
        }
        
        [System.Runtime.Serialization.DataMemberAttribute()]
        public string Content {
            get {
                return this.ContentField;
            }
            set {
                if ((object.ReferenceEquals(this.ContentField, value) != true)) {
                    this.ContentField = value;
                    this.RaisePropertyChanged("Content");
                }
            }
        }
        
        [System.Runtime.Serialization.DataMemberAttribute()]
        public MessagingClient.ServiceReferenceMS.User FromUser {
            get {
                return this.FromUserField;
            }
            set {
                if ((object.ReferenceEquals(this.FromUserField, value) != true)) {
                    this.FromUserField = value;
                    this.RaisePropertyChanged("FromUser");
                }
            }
        }
        
        [System.Runtime.Serialization.DataMemberAttribute()]
        public string ID {
            get {
                return this.IDField;
            }
            set {
                if ((object.ReferenceEquals(this.IDField, value) != true)) {
                    this.IDField = value;
                    this.RaisePropertyChanged("ID");
                }
            }
        }
        
        [System.Runtime.Serialization.DataMemberAttribute()]
        public System.DateTime Timestamp {
            get {
                return this.TimestampField;
            }
            set {
                if ((this.TimestampField.Equals(value) != true)) {
                    this.TimestampField = value;
                    this.RaisePropertyChanged("Timestamp");
                }
            }
        }
        
        public event System.ComponentModel.PropertyChangedEventHandler PropertyChanged;
        
        protected void RaisePropertyChanged(string propertyName) {
            System.ComponentModel.PropertyChangedEventHandler propertyChanged = this.PropertyChanged;
            if ((propertyChanged != null)) {
                propertyChanged(this, new System.ComponentModel.PropertyChangedEventArgs(propertyName));
            }
        }
    }
    
    [System.CodeDom.Compiler.GeneratedCodeAttribute("System.ServiceModel", "4.0.0.0")]
    [System.ServiceModel.ServiceContractAttribute(ConfigurationName="ServiceReferenceMS.IMessagingService", CallbackContract=typeof(MessagingClient.ServiceReferenceMS.IMessagingServiceCallback), SessionMode=System.ServiceModel.SessionMode.Required)]
    public interface IMessagingService {
        
        [System.ServiceModel.OperationContractAttribute(Action="http://tempuri.org/IMessagingService/Register", ReplyAction="http://tempuri.org/IMessagingService/RegisterResponse")]
        MessagingClient.ServiceReferenceMS.User Register(string username, string password);
        
        [System.ServiceModel.OperationContractAttribute(Action="http://tempuri.org/IMessagingService/Register", ReplyAction="http://tempuri.org/IMessagingService/RegisterResponse")]
        System.Threading.Tasks.Task<MessagingClient.ServiceReferenceMS.User> RegisterAsync(string username, string password);
        
        [System.ServiceModel.OperationContractAttribute(Action="http://tempuri.org/IMessagingService/Login", ReplyAction="http://tempuri.org/IMessagingService/LoginResponse")]
        MessagingClient.ServiceReferenceMS.User Login(string username, string password);
        
        [System.ServiceModel.OperationContractAttribute(Action="http://tempuri.org/IMessagingService/Login", ReplyAction="http://tempuri.org/IMessagingService/LoginResponse")]
        System.Threading.Tasks.Task<MessagingClient.ServiceReferenceMS.User> LoginAsync(string username, string password);
        
        [System.ServiceModel.OperationContractAttribute(Action="http://tempuri.org/IMessagingService/JoinChat", ReplyAction="http://tempuri.org/IMessagingService/JoinChatResponse")]
        MessagingClient.ServiceReferenceMS.Chat JoinChat(string username, string chatID);
        
        [System.ServiceModel.OperationContractAttribute(Action="http://tempuri.org/IMessagingService/JoinChat", ReplyAction="http://tempuri.org/IMessagingService/JoinChatResponse")]
        System.Threading.Tasks.Task<MessagingClient.ServiceReferenceMS.Chat> JoinChatAsync(string username, string chatID);
        
        [System.ServiceModel.OperationContractAttribute(Action="http://tempuri.org/IMessagingService/SendMessage", ReplyAction="http://tempuri.org/IMessagingService/SendMessageResponse")]
        string SendMessage(string username, MessagingClient.ServiceReferenceMS.Message m);
        
        [System.ServiceModel.OperationContractAttribute(Action="http://tempuri.org/IMessagingService/SendMessage", ReplyAction="http://tempuri.org/IMessagingService/SendMessageResponse")]
        System.Threading.Tasks.Task<string> SendMessageAsync(string username, MessagingClient.ServiceReferenceMS.Message m);
        
        [System.ServiceModel.OperationContractAttribute(Action="http://tempuri.org/IMessagingService/GetChats", ReplyAction="http://tempuri.org/IMessagingService/GetChatsResponse")]
        System.Collections.Generic.Dictionary<string, MessagingClient.ServiceReferenceMS.Chat> GetChats(string username);
        
        [System.ServiceModel.OperationContractAttribute(Action="http://tempuri.org/IMessagingService/GetChats", ReplyAction="http://tempuri.org/IMessagingService/GetChatsResponse")]
        System.Threading.Tasks.Task<System.Collections.Generic.Dictionary<string, MessagingClient.ServiceReferenceMS.Chat>> GetChatsAsync(string username);
        
        [System.ServiceModel.OperationContractAttribute(Action="http://tempuri.org/IMessagingService/GetMessages", ReplyAction="http://tempuri.org/IMessagingService/GetMessagesResponse")]
        System.Collections.Generic.List<MessagingClient.ServiceReferenceMS.Message> GetMessages(string username, string chatID);
        
        [System.ServiceModel.OperationContractAttribute(Action="http://tempuri.org/IMessagingService/GetMessages", ReplyAction="http://tempuri.org/IMessagingService/GetMessagesResponse")]
        System.Threading.Tasks.Task<System.Collections.Generic.List<MessagingClient.ServiceReferenceMS.Message>> GetMessagesAsync(string username, string chatID);
    }
    
    [System.CodeDom.Compiler.GeneratedCodeAttribute("System.ServiceModel", "4.0.0.0")]
    public interface IMessagingServiceCallback {
        
        [System.ServiceModel.OperationContractAttribute(IsOneWay=true, Action="http://tempuri.org/IMessagingService/OnMessage")]
        void OnMessage(MessagingClient.ServiceReferenceMS.Message m);
        
        [System.ServiceModel.OperationContractAttribute(IsOneWay=true, Action="http://tempuri.org/IMessagingService/OnInformation")]
        void OnInformation(string information);
        
        [System.ServiceModel.OperationContractAttribute(IsOneWay=true, Action="http://tempuri.org/IMessagingService/OnError")]
        void OnError(string error);
    }
    
    [System.CodeDom.Compiler.GeneratedCodeAttribute("System.ServiceModel", "4.0.0.0")]
    public interface IMessagingServiceChannel : MessagingClient.ServiceReferenceMS.IMessagingService, System.ServiceModel.IClientChannel {
    }
    
    [System.Diagnostics.DebuggerStepThroughAttribute()]
    [System.CodeDom.Compiler.GeneratedCodeAttribute("System.ServiceModel", "4.0.0.0")]
    public partial class MessagingServiceClient : System.ServiceModel.DuplexClientBase<MessagingClient.ServiceReferenceMS.IMessagingService>, MessagingClient.ServiceReferenceMS.IMessagingService {
        
        public MessagingServiceClient(System.ServiceModel.InstanceContext callbackInstance) : 
                base(callbackInstance) {
        }
        
        public MessagingServiceClient(System.ServiceModel.InstanceContext callbackInstance, string endpointConfigurationName) : 
                base(callbackInstance, endpointConfigurationName) {
        }
        
        public MessagingServiceClient(System.ServiceModel.InstanceContext callbackInstance, string endpointConfigurationName, string remoteAddress) : 
                base(callbackInstance, endpointConfigurationName, remoteAddress) {
        }
        
        public MessagingServiceClient(System.ServiceModel.InstanceContext callbackInstance, string endpointConfigurationName, System.ServiceModel.EndpointAddress remoteAddress) : 
                base(callbackInstance, endpointConfigurationName, remoteAddress) {
        }
        
        public MessagingServiceClient(System.ServiceModel.InstanceContext callbackInstance, System.ServiceModel.Channels.Binding binding, System.ServiceModel.EndpointAddress remoteAddress) : 
                base(callbackInstance, binding, remoteAddress) {
        }
        
        public MessagingClient.ServiceReferenceMS.User Register(string username, string password) {
            return base.Channel.Register(username, password);
        }
        
        public System.Threading.Tasks.Task<MessagingClient.ServiceReferenceMS.User> RegisterAsync(string username, string password) {
            return base.Channel.RegisterAsync(username, password);
        }
        
        public MessagingClient.ServiceReferenceMS.User Login(string username, string password) {
            return base.Channel.Login(username, password);
        }
        
        public System.Threading.Tasks.Task<MessagingClient.ServiceReferenceMS.User> LoginAsync(string username, string password) {
            return base.Channel.LoginAsync(username, password);
        }
        
        public MessagingClient.ServiceReferenceMS.Chat JoinChat(string username, string chatID) {
            return base.Channel.JoinChat(username, chatID);
        }
        
        public System.Threading.Tasks.Task<MessagingClient.ServiceReferenceMS.Chat> JoinChatAsync(string username, string chatID) {
            return base.Channel.JoinChatAsync(username, chatID);
        }
        
        public string SendMessage(string username, MessagingClient.ServiceReferenceMS.Message m) {
            return base.Channel.SendMessage(username, m);
        }
        
        public System.Threading.Tasks.Task<string> SendMessageAsync(string username, MessagingClient.ServiceReferenceMS.Message m) {
            return base.Channel.SendMessageAsync(username, m);
        }
        
        public System.Collections.Generic.Dictionary<string, MessagingClient.ServiceReferenceMS.Chat> GetChats(string username) {
            return base.Channel.GetChats(username);
        }
        
        public System.Threading.Tasks.Task<System.Collections.Generic.Dictionary<string, MessagingClient.ServiceReferenceMS.Chat>> GetChatsAsync(string username) {
            return base.Channel.GetChatsAsync(username);
        }
        
        public System.Collections.Generic.List<MessagingClient.ServiceReferenceMS.Message> GetMessages(string username, string chatID) {
            return base.Channel.GetMessages(username, chatID);
        }
        
        public System.Threading.Tasks.Task<System.Collections.Generic.List<MessagingClient.ServiceReferenceMS.Message>> GetMessagesAsync(string username, string chatID) {
            return base.Channel.GetMessagesAsync(username, chatID);
        }
    }
}
