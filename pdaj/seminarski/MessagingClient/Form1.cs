using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.ServiceModel;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using MessagingClient.ServiceReferenceMS;

namespace MessagingClient
{
    public partial class FormMessagingClient : Form
    {
        private IMessagingService _proxy;
        private User _loggedUser;
        private FormLoggedUser _formLoggedUser;
        public FormMessagingClient()
        {
            _formLoggedUser = new FormLoggedUser(null, null);
            _proxy = new MessagingServiceClient(new InstanceContext(_formLoggedUser));
            _formLoggedUser._proxy = _proxy;
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
                _formLoggedUser._loggedUser = _loggedUser;
                _formLoggedUser.ShowDialog();
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
                _formLoggedUser._loggedUser = _loggedUser;
                _formLoggedUser.ShowDialog();
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
