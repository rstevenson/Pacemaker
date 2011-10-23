using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using System.Threading;

namespace DCM
{
    public partial class Form1 : Form
    {

        public SerialPort connection = new SerialPort();
        public byte[] IncomingData = new byte[16];
        public byte[] OutgoingData = new byte[16];

        public int DataPosition = 0;

        public Form1()
        {
            InitializeComponent();
        }

        private void cmdCon_Click(object sender, EventArgs e)
        {
            if (cmdCon.Text == "Connect")
            {
                try
                {
                    connection.Open();          //If connection fails to open, the code will jump directly to error handling

                    cmdCon.Text = "Disconnect"; //The button text changes from "Connect" to "Disconnect"
                    cmdSend.Enabled = true;     //Send Button is enabled
                    timerReceive.Start();       //Start the timer that check if there is data coming from the port

                    txtDisplay.Text = "";       //Clear Display Box
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);    //Error Handling
                }

            }
            else
            {
                cmdCon.Text = "Connect";    //The button text changes from "Disconnect" to "Connect"
                cmdSend.Enabled = false;    //Send Button is disabled

                if (connection.IsOpen)      //Check if the connection is still opened
                {
                    connection.Close();     //Close connection
                    timerReceive.Stop();    //Stop checking data to receive
                }
            }
            
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            // Initialize Port Parameters, hard coded for now
            connection.BaudRate = 57600;
            connection.DataBits = 8;
            connection.Parity = System.IO.Ports.Parity.None;
            connection.PortName = "COM1";
            connection.StopBits = StopBits.One;
        }

        private bool CheckFnCode(byte Input)
        {
            if (Input == (byte)InternalConstant.k_echo || Input == (byte)InternalConstant.k_egram || Input == (byte)InternalConstant.k_estop || Input == (byte)InternalConstant.k_pparams)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        private string ReturnFnCode(byte Input)
        {
            if (Input == (byte)InternalConstant.k_echo)
            {
                return "k_echo";
            }
            else if(Input == (byte)InternalConstant.k_egram)
            {
                return "k_egram";
            }            
            else if(Input == (byte)InternalConstant.k_estop)
            {
                return "k_estop";
            }
            else if (Input == (byte)InternalConstant.k_pparams)
            {
                return "k_pparams";
            }
            else
            {
                return "FnCode not recognized";
            }
        }

        private bool CheckSum(byte[] TheData)
        {
            byte TempByte=0;
            for (int j = 2; j < 15;j++)
            {
                TempByte ^= TheData[j];
            }

            if (TempByte == TheData[15])
            {
                return true;
            }else{
                return false;
            }
        }

        private void timerReceive_Tick(object sender, EventArgs e)
        {

            if (connection.BytesToRead > 0) //Check if there is data in the receiving buffer of the port
            {
                while (connection.BytesToRead > 0)         //If there is still data in buffer and data packet is not complete
                {
                    txtDisplay.Text += connection.ReadByte().ToString("X") + " ";
                }
                txtDisplay.Text += "\r\n";
            }

        }

        private void cmdSend_Click(object sender, EventArgs e)
        {

            //Check which Synchronization byte the user would like to send
            if (radioButton6.Checked == true)
            {
                OutgoingData[0] = (byte)InternalConstant.k_sync;
            }
            else if (radioButton7.Checked == true)
            {
                OutgoingData[0] = (byte)InternalConstant.k_sync-1;
            }

            //Check which FnCode the user would like to send
            if (radioButton1.Checked == true)
            {
                OutgoingData[1]=(byte)InternalConstant.k_egram;
            }
            else if (radioButton2.Checked == true)
            {
                OutgoingData[1] = (byte)InternalConstant.k_echo;
            }
            else if (radioButton3.Checked == true)
            {
                OutgoingData[1] = (byte)InternalConstant.k_estop;
            }
            else if (radioButton4.Checked == true)
            {
                OutgoingData[1] = (byte)InternalConstant.k_pparams;
            }
            else if (radioButton5.Checked == true)
            {
                OutgoingData[1] = 0x53; //Use an invalid FnCode
            }

            Random rndGen=new Random();
            connection.Write(OutgoingData, 0, 2);
            OutgoingData[2] = (byte)rndGen.Next(254);
            OutgoingData[15] = OutgoingData[2];
            connection.Write(OutgoingData, 2, 1);
            Thread.Sleep(2);
            //Calculate the Check Sum of the data packet
            for (int j = 3; j < 15; j++)
            {
                OutgoingData[j] = (byte)rndGen.Next(254);
                OutgoingData[15] ^= OutgoingData[j];
                connection.Write(OutgoingData, j, 1);
                Thread.Sleep(2);
            }
            //Send the data packet through the port
            connection.Write(OutgoingData,15, 1);

            txtDisplay.SelectionStart=txtDisplay.Text.Length;
            txtDisplay.ScrollToCaret();
        }
    }
}
