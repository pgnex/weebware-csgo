using loader.Authentication;
using loader.Functions;
using SafeRequest.NET;
using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace weebware_loader.Forms {
    public partial class Emailnput : Form {
        Response mainResponse;
        public Emailnput(Response response) {
            mainResponse = response;
            InitializeComponent();
        }

        private void btnSubmitEmail_Click(object sender, EventArgs e) {
            if (txtEmail.Text == string.Empty) {
                MessageBox.Show("Invalid email");
                return;
            }

            if (!txtEmail.Text.Contains("@") || !txtEmail.Text.Contains(".")) {
                MessageBox.Show("Invalid email");
                return;
            }

            Response response = Networking.addEmail(txtEmail.Text);
            if (response.GetData<bool>("added")) {
                Form mainForm = new Main(mainResponse);
                mainForm.Show();
                Hide();
            }
        }
    }
}
