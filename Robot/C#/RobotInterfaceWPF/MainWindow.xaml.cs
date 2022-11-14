using ExtendedSerialPort;
using System;
using System.Collections.Generic;
using System.IO.Ports;
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
using System.Windows.Threading;

namespace RobotInterfaceWPF
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        ReliableSerialPort serialPort1;

        public MainWindow()
        {
            InitializeComponent();

            serialPort1 = new ReliableSerialPort("COM4", 115200, Parity.None, 8, StopBits.One);
            serialPort1.DataReceived += SerialPort1_DataReceived;
            serialPort1.Open();

            timerAffichage = new DispatcherTimer
            {
                Interval = new TimeSpan(0, 0, 0, 0, 100)
            };
            timerAffichage.Tick += timerAffichage_Tick;
            timerAffichage.Start();
        }

        private void timerAffichage_Tick(object sender, EventArgs e)
        {
            // throw new NotImplementedException();
            if (receivedText != "")
            {
                TextBoxReception.Text += receivedText;
                receivedText = "";
            }

        }

        string receivedText = "";
        private DispatcherTimer timerAffichage;

        public void SerialPort1_DataReceived(object sender, DataReceivedArgs e)
        {
            //throw new NotImplementedException();
            receivedText += Encoding.UTF8.GetString(e.Data, 0, e.Data.Length);
        }

        private void Button_Envoyer_Click1(object sender, RoutedEventArgs e)
        {
            if (buttonEnvoyer.Background == Brushes.Beige)
            {
                buttonEnvoyer.Background = Brushes.RoyalBlue;
            }
            else buttonEnvoyer.Background = Brushes.Beige;
            SendMessage();
        }

        private void TextBoxEmission_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                SendMessage();
            }
        }
        void SendMessage()
        {
            serialPort1.WriteLine(TextBoxEmission.Text);
            TextBoxEmission.Text = "";
            //TextBoxReception.Text = TextBoxReception.Text + "Reçu : " + msg;
            //if (enter == 0)
            //{
            //    TextBoxReception.Text = TextBoxReception.Text + "\n";
            //}
        }

        private void buttonClear_Click(object sender, RoutedEventArgs e)
        {
            TextBoxReception.Text = "";
        }
    }
}
