using System;
using System.ServiceModel;
using System.Windows.Forms;
using MessagingClient.ServiceReferenceMS;

namespace MessagingClient
{
    public partial class FormMessagingClient : Form
    {
        private User _loggedUser;
        private FormLoggedUser _formLoggedUser;
        private InstanceContext _ctx;
        public FormMessagingClient()
        {
            _loggedUser = null;
            _formLoggedUser = new FormLoggedUser(null, null);
            _ctx = new InstanceContext(_formLoggedUser);
             _formLoggedUser.Proxy = new MessagingServiceClient(_ctx);
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void buttonLogin_Click(object sender, EventArgs e)
        {
            if (!ValidateInput())
                return;
            
            _loggedUser = _formLoggedUser.Proxy.Login(textBoxUsername.Text, textBoxPassword.Text);
            if (_loggedUser == null || !_loggedUser.IsValid)
            {
                MessageBox.Show("Error logging in!");
            }
            else
            {
                _formLoggedUser.LoggedUser = _loggedUser;
                _formLoggedUser.ShowDialog();
            }
        }

        private void buttonRegister_Click(object sender, EventArgs e)
        {
            if (!ValidateInput()) return;

            _loggedUser = _formLoggedUser.Proxy.Register(textBoxUsername.Text, textBoxPassword.Text);
            if (_loggedUser == null || !_loggedUser.IsValid)
            {
                MessageBox.Show("Error registering!");
            }
            else
            {
                _formLoggedUser.LoggedUser = _loggedUser;
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
