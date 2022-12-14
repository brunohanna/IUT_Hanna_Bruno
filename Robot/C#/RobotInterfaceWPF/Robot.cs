using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RobotInterfaceWPF
{
    public class Robot
    {
        public string receivedText = "";
        public float distanceTelemetreDroit;
        public float distanceTelemetreCentre;
        public float distanceTelemetreGauche;

        public bool Led1State;
        public bool Led2State; 
        public bool Led3State;

        public int SpeedMotorDroit;
        public int SpeedMotorGauche;

        public Queue<byte> byteListReceived = new Queue<byte>();

        public Robot()
        {
            
        }
    }
}
