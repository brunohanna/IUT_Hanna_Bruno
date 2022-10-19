using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace RobotInterface
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void TextBoxEmission_TextChanged(object sender, TextChangedEventArgs e)
        {

        }

        private void TextBoxReception_TextChanged(object sender, TextChangedEventArgs e)
        {
        }

        private void Button_Envoyer_Click1(object sender, RoutedEventArgs e)
        {
            if(buttonEnvoyer.Background == Brushes.Beige)
            {
                buttonEnvoyer.Background = Brushes.RoyalBlue;
            }
            else buttonEnvoyer.Background = Brushes.Beige;
            SendMessage(0b0);
        }

        private void TextBoxEmission_KeyUp(object sender, KeyEventArgs e)
        {
            if(e.Key == Key.Enter)
            {
                SendMessage(0b1);
            }
        }
        void SendMessage(int enter)
        {
            string msg = TextBoxEmission.Text;
            TextBoxEmission.Text = "";
            TextBoxReception.Text = TextBoxReception.Text + "Reçu : " + msg;
            if(enter==0)
            {
                TextBoxReception.Text = TextBoxReception.Text + "\n";
            }
        }
    }
}
