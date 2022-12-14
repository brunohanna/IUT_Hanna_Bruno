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
            while (robot.byteListReceived.Count > 0)
            {
                var c = robot.byteListReceived.Dequeue();
                //TextBoxReception.Text += "0x" + c.ToString("X2") + " ";
                DecodeMessage(c);
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
            // Test d'envoi d'un message
            /*byte[] bytelist = new byte[40];
            for (int i = 0; i < 40; i++)
            {
                bytelist[i] = (byte)(2 * i);
            }
            serialPort1.Write(bytelist, 0, 40);*/

            //Test de décodage texte
            int msgFunction = (int)MessageFunction.Text;
            string payload = "Bonjour";
            int payloadLength = payload.Length;
            byte[] payloadBytes = Encoding.ASCII.GetBytes(payload);
            UartEncodeAndSendMessage(msgFunction, payloadLength, payloadBytes);



            payloadBytes = new byte[3];
            payloadBytes[0] = 45;
            payloadBytes[1] = 55;
            payloadBytes[2] = 25;
            UartEncodeAndSendMessage((int)MessageFunction.IR, payloadBytes.Length, payloadBytes);

            payloadBytes = new byte[2];
            payloadBytes[0] = 1;
            payloadBytes[1] = 1;
            UartEncodeAndSendMessage((int)MessageFunction.LED, payloadBytes.Length, payloadBytes);

            payloadBytes = new byte[2];
            payloadBytes[0] = 14;
            payloadBytes[1] = 15;
            UartEncodeAndSendMessage((int)MessageFunction.Speed, payloadBytes.Length, payloadBytes);

            payloadBytes = new byte[5];
            payloadBytes[0] = 1;
            payloadBytes[1] = 1;
            payloadBytes[2] = 0xFF;
            payloadBytes[3] = 0xFF;
            payloadBytes[4] = 0xFF;
            UartEncodeAndSendMessage((int)MessageFunction.RobotState, payloadBytes.Length, payloadBytes);
            //Test de décodage LED
        }

        byte CalculateChecksum(int msgFunction, int msgPayloadLength, byte[] msgPayload)
        {
            byte checksum = 0;
            checksum ^= 0xFE;
            checksum ^= (byte)(msgFunction >> 8);
            checksum ^= (byte)(msgFunction >> 0);
            checksum ^= (byte)(msgPayloadLength >> 8);
            checksum ^= (byte)(msgPayloadLength >> 0);

            for (int i = 0; i < msgPayloadLength; i++)
            {
                checksum ^= msgPayload[i];
            }
            return checksum;
        }

        void UartEncodeAndSendMessage(int msgFunction, int msgPayloadLength, byte[] msgPayload)
        {
            byte[] message = new byte[msgPayloadLength + 6];
            int pos = 0;
            message[pos++] = 0xFE;
            message[pos++] = (byte)(msgFunction >> 8);
            message[pos++] = (byte)(msgFunction >> 0);
            message[pos++] = (byte)(msgPayloadLength >> 8);
            message[pos++] = (byte)(msgPayloadLength >> 0);
            for (int i = 0; i < msgPayloadLength; i++)
            {
                message[pos++]= msgPayload[i];
            }
            message[pos++] = CalculateChecksum(msgFunction, msgPayloadLength, msgPayload);

            serialPort1.Write(message, 0, msgPayloadLength + 6);
        }
        public enum StateReception
        {
            Waiting,
            FunctionMSB,
            FunctionLSB,
            PayloadLengthMSB,
            PayloadLengthLSB,
            Payload,
            CheckSum
        }

        StateReception rcvState = StateReception.Waiting;
        int msgDecodedFunction = 0;
        int msgDecodedPayloadLength = 0;
        byte[] msgDecodedPayload;
        int msgDecodedPayloadIndex = 0;
        private void DecodeMessage(byte c)
        {
            switch (rcvState)
            {
                case StateReception.Waiting:
                    if(c==0xFE) rcvState = StateReception.FunctionMSB;
                    break;
                case StateReception.FunctionMSB:
                    msgDecodedFunction = (int)(c<<8);
                    rcvState = StateReception.FunctionLSB;
                    break;
                case StateReception.FunctionLSB:
                    msgDecodedFunction += (int)(c);
                    rcvState = StateReception.PayloadLengthMSB;
                    break;
                case StateReception.PayloadLengthMSB:
                    msgDecodedPayloadLength = (int)(c<<8);
                    rcvState = StateReception.PayloadLengthLSB;
                    break;
                case StateReception.PayloadLengthLSB:
                    msgDecodedPayloadLength += (int)(c);
                    if (msgDecodedPayloadLength == 0)
                        rcvState = StateReception.CheckSum;
                    else if (msgDecodedPayloadLength >= 1024)
                        rcvState = StateReception.Waiting;
                    else
                    {
                        msgDecodedPayload = new byte[msgDecodedPayloadLength];
                        msgDecodedPayloadIndex = 0;
                        rcvState = StateReception.Payload;
                    }
                    break;
                case StateReception.Payload:                    
                    msgDecodedPayload[msgDecodedPayloadIndex] = c;
                    msgDecodedPayloadIndex++;
                    if(msgDecodedPayloadIndex>= msgDecodedPayloadLength)
                        rcvState = StateReception.CheckSum;
                    break;
                case StateReception.CheckSum:
                    byte calculatedChecksum = CalculateChecksum(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
                    if (calculatedChecksum == c)
                    {
                        //Success, on a un message valide
                        //TextBoxReception.Text += " OK \n";
                        ProcessDecodedMessage(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
                    }
                    else
                    {
                        TextBoxReception.Text += " Pas OK \n";
                    }
                    rcvState = StateReception.Waiting;                    
                    break;
                default:
                    rcvState = StateReception.Waiting;
                    break;
            }
        }
        void ProcessDecodedMessage(int msgFunction, int msgPayloadLength, byte[] msgPayload)
        {
            switch((MessageFunction)msgFunction)
            {
                case MessageFunction.Text:
                    //UartEncodeAndSendMessage(msgFunction, msgPayloadLength, msgPayload);
                    break;
                case MessageFunction.LED:
                    if(msgPayloadLength==2)
                    {
                        switch(msgPayload[0])
                        {
                            case 0:
                                robot.Led1State = (msgPayload[1] == 1);
                                Led1CheckB.IsChecked = robot.Led1State;
                                break;
                            case 1:
                                robot.Led2State = (msgPayload[1] == 1);
                                Led2CheckB.IsChecked = robot.Led2State;
                                break;
                            case 2:
                                robot.Led3State = (msgPayload[1] == 1);
                                Led3CheckB.IsChecked = robot.Led3State;
                                break;
                        }
                    }
                    break;
                case MessageFunction.IR:
                    if (msgPayloadLength <= 3)
                    {
                        robot.distanceTelemetreGauche = (float)msgPayload[0];
                        robot.distanceTelemetreCentre = (float)msgPayload[1];
                        robot.distanceTelemetreDroit = (float)msgPayload[2];
                        TelemetreTextBox.Text =
                            "IR Gauche : " + robot.distanceTelemetreGauche.ToString("N2") + " cm \n" +
                            "IR Centre : " + robot.distanceTelemetreCentre.ToString("N2") + " cm \n" +
                            "IR Droit : " + robot.distanceTelemetreDroit.ToString("N2") + " cm";
                    }
                    break;
                case MessageFunction.Speed:
                    if (msgPayloadLength <= 2)
                    {
                        robot.SpeedMotorGauche = (int)msgPayload[0];
                        robot.SpeedMotorDroit = (int)msgPayload[1];
                        MoteursTextBox.Text = "Vitesse Gauche : " + robot.SpeedMotorGauche.ToString() + "%\n" + "Vitesse Droit : " + robot.SpeedMotorDroit.ToString() + "%";
                    }
                    break;
                case MessageFunction.RobotState:
                    int instant = (((int)msgPayload[1]) << 24) + (((int)msgPayload[2]) << 16) + (((int)msgPayload[3]) << 8) + ((int)msgPayload[4]);
                    TextBoxReception.Text += "\nRobot␣State␣:␣" + ((StateRobot)(msgPayload[0])).ToString() + "␣-␣" + instant.ToString() + "␣ms";
                    break;
                default:
                    break;
            }
        }
        public enum MessageFunction
        {
            Text = 0x0080,
            LED = 0x0020,
            IR = 0x0030,
            Speed = 0x0040,
            RobotState = 0x0050,

        }

        public enum StateRobot
        {
            STATE_ATTENTE = 0,
            STATE_ATTENTE_EN_COURS = 1,
            STATE_AVANCE = 2,
            STATE_AVANCE_EN_COURS = 3,
            STATE_TOURNE_GAUCHE = 4,
            STATE_TOURNE_GAUCHE_EN_COURS = 5,
            STATE_TOURNE_DROITE = 6,
            STATE_TOURNE_DROITE_EN_COURS = 7,
            STATE_TOURNE_SUR_PLACE_GAUCHE = 8,
            STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS = 9,
            STATE_TOURNE_SUR_PLACE_DROITE = 10,
            STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS = 11,
            STATE_ARRET = 12,
            STATE_ARRET_EN_COURS = 13,
            STATE_RECULE = 14,
            STATE_RECULE_EN_COURS = 15
        }

        private void Led1CheckB_Checked(object sender, RoutedEventArgs e)
        {

        }
        private void Led1CheckB_Unchecked(object sender, RoutedEventArgs e)
        {

        }

        private void Led2CheckB_Checked(object sender, RoutedEventArgs e)
        {

        }
        private void Led2CheckB_Unchecked(object sender, RoutedEventArgs e)
        {

        }

        private void Led3CheckB_Checked(object sender, RoutedEventArgs e)
        {

        }

        private void Led3CheckB_Unchecked(object sender, RoutedEventArgs e)
        {

        }
    }
}
