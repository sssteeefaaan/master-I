using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using MessagingClient.MessagingServiceReference;

namespace MessagingClient
{
    public partial class FormMessagingClient : Form
    {
        private IMessagingService _proxy;
        private User _loggedUser;
        public FormMessagingClient()
        {
            _proxy = new MessagingServiceClient();
            _loggedUser = null;
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void buttonLogin_Click(object sender, EventArgs e)
        {
            if (!ValidateInput())
                return;

            _loggedUser = _proxy.Login(textBoxUsername.Text, textBoxPassword.Text);
            if (_loggedUser == null)
            {
                MessageBox.Show("Error logging in!");
            }
            else
            {
                FormLoggedUser fLU = new FormLoggedUser(_proxy, _loggedUser);
                fLU.ShowDialog();
            }
        }

        private void buttonRegister_Click(object sender, EventArgs e)
        {
            if (!ValidateInput()) return;

            _loggedUser = _proxy.Register(textBoxUsername.Text, textBoxPassword.Text);
            if (_loggedUser == null)
            {
                MessageBox.Show("Error registering!");
            }
            else
            {

            }
        }

        private bool ValidateInput()
        {
            if (String.IsNullOrWhiteSpace(textBoxUsername.Text))
            {
                MessageBox.Show("Username cannot be empty!");
                return false;
            }
            if (String.IsNullOrWhiteSpace(textBoxPassword.Text))
            {
                MessageBox.Show("Password cannot be empty!");
                return false;
            }
            return true;
        }
    }
}
