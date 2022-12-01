using MessagingClient.ServiceReferenceMS;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.ServiceModel.Channels;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;


namespace MessagingClient
{
    public partial class FormLoggedUser : Form, ServiceReferenceMS.IMessagingServiceCallback
    {
        private string _openedChatID;
        public User _loggedUser;
        public IMessagingService _proxy;

        public FormLoggedUser(IMessagingService proxy, User loggedUser)
        {
            _loggedUser = loggedUser;
            _proxy = proxy;
            _openedChatID = null;
            InitializeComponent();
        }

        private void tableLayoutPanel3_Paint(object sender, PaintEventArgs e)
        {

        }

        private void buttonSend_Click(object sender, EventArgs e)
        {
            if (!String.IsNullOrEmpty(textBoxInput.Text))
            {
                ServiceReferenceMS.Message m = new ServiceReferenceMS.Message()
                {
                    ID = "",
                    ChatID = _openedChatID,
                    FromUser = _loggedUser,
                    Content = textBoxInput.Text,
                    Timestamp = DateTime.Now
                };

                m.ID = _proxy.SendMessage(_loggedUser.Username, m);
                if (!String.IsNullOrEmpty(m.ID))
                {
                    textBoxInput.Text = "";
                    _loggedUser.Chats[_openedChatID].Messages.Add(m);
                    RefreshMessages();
                }
                else
                {
                    MessageBox.Show("Couldn't send the message!");
                }
            }
        }

        private void RefreshChatList()
        {
            listViewChats.Items.Clear();
            foreach (Chat c in _loggedUser.Chats.Values)
            {
                listViewChats.Items.AddRange(new ListViewItem[]{
                    new ListViewItem(c.ID)
                });
            }
        }

        private void FormLoggedUser_Load(object sender, EventArgs e)
        {
            listViewChats.AutoResizeColumns(ColumnHeaderAutoResizeStyle.HeaderSize);
            listViewMessages.AutoResizeColumns(ColumnHeaderAutoResizeStyle.HeaderSize);
            _loggedUser.Chats = _proxy.GetChats(_loggedUser.Username);
            RefreshChatList();
        }

        private void listViewChats_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (listViewChats.SelectedItems.Count != 1) return;
            string k = listViewChats.SelectedItems[0].Text;
            if(k != null && (_openedChatID == null || _openedChatID != k))
            {
                _openedChatID = k;
                RefreshMessages();
            }
        }

        private void joinAChatToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Chat c = new Chat();
            FormJoinChat f = new FormJoinChat(_proxy, _loggedUser);
            if(f.ShowDialog() == DialogResult.OK)
            {
                _openedChatID = f._chat.ID;
                RefreshChatList();
                RefreshMessages();
            }
        }

        private void RefreshMessages()
        {

            listViewMessages.Items.Clear();
            foreach (ServiceReferenceMS.Message m in _loggedUser.Chats[_openedChatID].Messages)
            {
                listViewMessages.Items.Add(new ListViewItem
                {
                    Text = m.FromUser.Username + ": " + m.Content + "\n@" + m.Timestamp.ToShortTimeString()
                });
            }
        }

        private void logOutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DialogResult dr = MessageBox.Show("Are you sure you want to logout?", "Question", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
            if(dr == DialogResult.Yes)
            {
                _loggedUser = null;
                _openedChatID = null;
                Close();
            }
        }

        public void OnMessage(ServiceReferenceMS.Message m)
        {
            if(m != null)
            {
                if (_loggedUser.Chats.ContainsKey(m.ChatID))
                {
                    _loggedUser.Chats[m.ChatID].Messages.Add(m);
                }

                if (_openedChatID == m.ChatID)
                {
                    RefreshMessages();
                }
            }
        }

        public void OnInformation(string information)
        {
            MessageBox.Show(information, "INFO", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        public void OnError(string error)
        {
            MessageBox.Show(error, "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);

        }
    }
}
