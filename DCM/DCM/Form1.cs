using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;


namespace DCM
{
    public partial class Form1 : Form
    {
        public const byte k_egram = 0x47;
        public const byte k_echo = 0x49;
        public const byte k_estop = 0x62;
        public const byte k_pparams = 0x55;

        public const byte k_sync = 0x16;

        public SerialPort connection = new SerialPort();
        public byte[] CurrentData=new byte[16];
        public int DataPosition = 0;
        public bool DataProcess = false;
        public int I = 0;

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
                    connection.Open();
                    cmdCon.Text = "Disconnect"; //If connection successfully opens, the button text is changed
                    cmdSend.Enabled = true; //Send Button is enabled
                    timerReceive.Start(); //Check if there is data to receive on small time interval
                    txtDisplay.Text = "";
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message); //Error Handling
                }

            }
            else
            {
                cmdCon.Text = "Connect";
                cmdSend.Enabled = false;
                if (connection.IsOpen) //in case connection was previously interrupted
                {
                    connection.Close();
                    timerReceive.Stop(); //Stop checking data to receive
                }
            }
            
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            // Initialize Port Parameters
            connection.BaudRate = 19200;
            connection.DataBits = 8;
            connection.Parity = System.IO.Ports.Parity.None;
            connection.PortName = "COM1";
            connection.StopBits = StopBits.One;
            
        }

        private bool CheckFnCode(byte Input)
        {
            if (Input == k_echo || Input == k_egram || Input == k_estop || Input == k_pparams)
            {
                return true;
            }
            else
            {
                return false;
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

            if (connection.BytesToRead > 0)
            {
                while (connection.BytesToRead > 0)
                {
                    CurrentData[DataPosition]=(byte)connection.ReadByte();
                    DataPosition++;
                    if (DataPosition==16){
                        DataPosition = 0;
                        break;
                    }
                }
                if (DataPosition == 0)
                {
                    if ((CurrentData[0] == k_sync) && (CheckFnCode(CurrentData[1]) == true) && CheckSum(CurrentData) == true)
                    {
                        string Output = "";

                        for (int j = 0; j < 16; j++)
                        {
                            Output += CurrentData[j].ToString() + " ";
                        }
                        txtDisplay.Text += "Data Received: " + Output + "\r\n";
                    }
                    else
                    {
                        txtDisplay.Text += "Invalid Data Received!" + "\r\n";
                    }
                }
            }

        }

        private void cmdSend_Click(object sender, EventArgs e)
        {

            dataPackage DataP;
            DataP.data = new Byte[16];
            DataP.data[0] = k_sync;
            if (radioButton1.Checked == true)
            {
                DataP.data[1]=k_egram;
            }
            else if (radioButton2.Checked == true)
            {
                DataP.data[1]=k_echo;
            }
            else if (radioButton3.Checked == true)
            {
                DataP.data[1] = k_estop;
            }
            else if (radioButton4.Checked == true)
            {
                DataP.data[1] = k_pparams;
            }
            else if (radioButton5.Checked == true)
            {
                DataP.data[1] = 0x53;
            }
            DataP.data[2] = 2;
            DataP.data[3] = 3;
            DataP.data[4] = 4;
            DataP.data[5] = 5;
            DataP.data[6] = 6;
            DataP.data[7] = 7;
            DataP.data[8] = 8;
            DataP.data[9] = 9;
            DataP.data[10] = 54;
            DataP.data[11] = 11;
            DataP.data[12] = 12;
            DataP.data[13] = 13;
            DataP.data[14] = 14;
            for (int j = 2; j < 15; j++)
            {
                DataP.data[15] ^= DataP.data[j];
            }

            connection.Write(DataP.data, 0, 16);
        }
    }
}
