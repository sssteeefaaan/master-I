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
using static System.Windows.Forms.VisualStyles.VisualStyleElement;


namespace MessagingClient
{
    public partial class FormLoggedUser : Form, ServiceReferenceMS.IMessagingServiceCallback
    {
        private string _openedChatID;
        public User LoggedUser { get; set; }
        public IMessagingService Proxy { get; set; }

        public FormLoggedUser(IMessagingService proxy, User loggedUser)
        {
            LoggedUser = loggedUser;
            Proxy = proxy;
            _openedChatID = null;
            InitializeComponent();
        }

        private void buttonSend_Click(object sender, EventArgs e)
        {
            if (!String.IsNullOrEmpty(textBoxInput.Text))
            {
                ServiceReferenceMS.Message m = new ServiceReferenceMS.Message()
                {
                    ID = Guid.NewGuid().ToString(),
                    ChatID = _openedChatID,
                    FromUser = LoggedUser.Username,
                    Content = textBoxInput.Text,
                    Timestamp = DateTime.Now
                };

                Proxy.SendMessage(LoggedUser.Username, m);
                textBoxInput.Text = "";
                LoggedUser.Chats[_openedChatID].Messages.Add(m);
                RefreshMessages();
            }
        }

        private void RefreshChatList()
        {
            listViewChats.Items.Clear();
            foreach (Chat c in LoggedUser.Chats.Values)
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
            LoggedUser.Chats = Proxy.GetChats(LoggedUser.Username);
            RefreshChatList();
            RefreshMessages();

            textBoxInput.Enabled =
                buttonSend.Enabled =
                listViewMessages.Enabled =
                false;
        }

        private void listViewChats_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (listViewChats.SelectedItems.Count != 1) return;
            textBoxInput.Enabled =
                buttonSend.Enabled =
                listViewMessages.Enabled =
                true;
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
            FormJoinChat f = new FormJoinChat(Proxy, LoggedUser);
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
            if (LoggedUser != null && !String.IsNullOrEmpty(_openedChatID))
            {
                listViewMessages.Items.Add(new ListViewItem
                {
                    Text = ""
                });

                foreach (ServiceReferenceMS.Message m in LoggedUser.Chats[_openedChatID].Messages)
                {
                    listViewMessages.Items.Add(new ListViewItem
                    {
                        Text = m.FromUser + ": " + m.Content
                    });

                    listViewMessages.Items.Add(new ListViewItem
                    {
                        Text = "@" + m.Timestamp.ToShortTimeString()
                    });

                    listViewMessages.Items.Add(new ListViewItem
                    {
                        Text = ""
                    });
                }

                listViewMessages.Items[listViewMessages.Items.Count - 1].EnsureVisible();
            }
        }

        private void logOutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DialogResult dr = MessageBox.Show("Are you sure you want to logout?", "Question", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
            if(dr == DialogResult.Yes)
            {
                Close();
            }
        }

        public void OnMessage(ServiceReferenceMS.Message m)
        {
            if(m != null)
            {
                if (LoggedUser.Chats.ContainsKey(m.ChatID))
                {
                    LoggedUser.Chats[m.ChatID].Messages.Add(m);
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

        private void textBoxInput_KeyPress(object sender, KeyPressEventArgs e)
        {
            if(e.KeyChar == ((char)Keys.Enter))
            {
                buttonSend.PerformClick();
            }
        }

        private void FormLoggedUser_FormClosed(object sender, FormClosedEventArgs e)
        {
            Proxy?.Logout(LoggedUser?.Username);
            LoggedUser = null;
            _openedChatID = null;
        }
    }
}
