using MessagingClient.MessagingServiceReference;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;


namespace MessagingClient
{
    public partial class FormLoggedUser : Form
    {
        private Dictionary<string, Chat> _userChats;
        private Chat _openedChat;
        private User _loggedUser;
        private IMessagingService _proxy;

        public FormLoggedUser(IMessagingService proxy, User loggedUser)
        {
            _loggedUser = loggedUser;
            _proxy = proxy;
            _openedChat = null;
            _userChats = null;
            InitializeComponent();
        }

        private void tableLayoutPanel3_Paint(object sender, PaintEventArgs e)
        {

        }

        private void buttonSend_Click(object sender, EventArgs e)
        {
            if (!String.IsNullOrEmpty(textBoxInput.Text))
            {
                MessagingServiceReference.Message m = new MessagingServiceReference.Message();
                m.Content = textBoxInput.Text;
                m.ChatID = _openedChat.ID;
                m.FromUser = _loggedUser;

                if(_proxy.SendMessage(_loggedUser.Username, m))
                {
                    _openedChat.Messages.Append(m);
                    listViewMessages.Items.Add(new ListViewItem
                    {
                        Text = m.FromUser + ": " + m.Content + "\n@" + m.Timestamp.ToShortTimeString()
                    });
                }
                else
                {
                    MessageBox.Show("Couldn't send the message!");
                }
            }
        }

        private void FormLoggedUser_Load(object sender, EventArgs e)
        {
            _userChats = _proxy.GetChats(_loggedUser.Username);
            foreach(Chat c in  _userChats.Values){
                listViewChats.Items.AddRange(new ListViewItem[]{
                    new ListViewItem(c.ID)
                });
            }
        }

        private void listViewChats_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (listViewChats.SelectedItems.Count != 1) return;
            string k = listViewChats.SelectedItems[0].Text;
            if(_openedChat != _userChats[k])
            {
                RefreshMessages();
            }
        }

        private void joinAChatToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Chat c = null;
            FormJoinChat f = new FormJoinChat(_proxy, _loggedUser, ref c);
            if(f.ShowDialog() == DialogResult.OK)
            {
                _openedChat = c;
                RefreshMessages();
            }
        }

        private void RefreshMessages()
        {
            listViewMessages.Items.Clear();
            foreach (MessagingServiceReference.Message m in _openedChat.Messages)
            {
                listViewMessages.Items.Add(new ListViewItem
                {
                    Text = m.FromUser + ": " + m.Content + "\n@" + m.Timestamp.ToShortTimeString()
                });
            }
        }

        private void logOutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DialogResult dr = MessageBox.Show("Are you sure you want to logout?", "Question", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
            if(dr == DialogResult.Yes)
            {
                _loggedUser = null;
                _openedChat = null;
                Close();
            }
        }
    }
}
