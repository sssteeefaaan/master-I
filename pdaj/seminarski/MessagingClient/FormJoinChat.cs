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
    public partial class FormJoinChat : Form
    {
        private IMessagingService _proxy;
        private User _loggedUser;
        private Chat _chat;
        public FormJoinChat(IMessagingService proxy, User loggedUser, ref Chat chat)
        {
            _proxy = proxy;
            _loggedUser = loggedUser;
            DialogResult = DialogResult.None;
            _chat = chat;
            InitializeComponent();
        }

        private void buttonJoin_Click(object sender, EventArgs e)
        {
            if (String.IsNullOrWhiteSpace(textBoxChat.Text))
            {
                MessageBox.Show("Chat ID cannot be empty!");
                return;
            }

            _chat = _proxy.JoinChat(_loggedUser.Username, textBoxChat.Text);
            if (_chat != null)
            {
                _loggedUser.Chats.Add(_chat.ID, _chat);
                MessageBox.Show("Successfully joined chat " + _chat.ID);

                DialogResult = DialogResult.OK;
                Close();
            }
            else
            {
                MessageBox.Show("Couldn't join the chat " + textBoxChat.Text);
            }
        }
    }
}
