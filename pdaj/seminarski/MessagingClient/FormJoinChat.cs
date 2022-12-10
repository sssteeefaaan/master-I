using MessagingClient.ServiceReferenceMS;
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
        public Chat _chat;
        public FormJoinChat(IMessagingService proxy, User loggedUser)
        {
            _proxy = proxy;
            _loggedUser = loggedUser;
            DialogResult = DialogResult.None;
            _chat = null;
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
                if(!_loggedUser.Chats.ContainsKey(_chat.ID))
                    _loggedUser.Chats.Add(_chat.ID, _chat);

                DialogResult = DialogResult.OK;
                Close();
            }
            else
            {
                MessageBox.Show("Couldn't join the chat " + textBoxChat.Text);
            }
        }

        private void FormJoinChat_Load(object sender, EventArgs e)
        {

        }

        private void textBoxChat_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == ((char)Keys.Enter))
            {
                buttonJoin.PerformClick();
            }
        }
    }
}
