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
        Robot robot = new Robot();
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
            /*if (robot.receivedText != "")
            {
                TextBoxReception.Text += robot.receivedText;
                robot.receivedText = "";
            }*/
            while (robot.byteListReceived.Count>0)
            {
                var c = robot.byteListReceived.Dequeue();
                TextBoxReception.Text += "0x" + c.ToString("X2") + " ";
            }
            
        }

        private DispatcherTimer timerAffichage;

        public void SerialPort1_DataReceived(object sender, DataReceivedArgs e)
        {
            //throw new NotImplementedException();
            //robot.receivedText += Encoding.UTF8.GetString(e.Data, 0, e.Data.Length);
            foreach (var lastw in e.Data)
            {
                robot.byteListReceived.Enqueue(lastw);
            }
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

        private void buttonTest_Click(object sender, RoutedEventArgs e)
        {
            byte[] bytelist = new byte[40];
            for (int i = 0; i<40; i++)
            {
                bytelist[i] = (byte)(2 * i);
            }
            serialPort1.Write(bytelist, 0, 40);
        }
    }
}
