using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CNV2
{  
    public partial class Form1 : Form
    {
        /*****************Syntethis parametres*******************/
        float RFin = 10; //опорная частота
        float RFout = 2100; //выходная частота
        float VCO = 0;
        ushort INT = 0;
        byte DIV = 1; //делитель
        uint Rcounter = 2;
        byte D = 0; //doubler bit
        byte T = 1; // Ref divide by 2
        float fPFD = 0;

        //Двумерный массив команды записи, номера регистра, кол-ва байт (4) и значения регистра R0-R5
        byte[,] userSettings = new byte[6, 7] { {0x01, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00}, //W R0 
                                                {0x01, 0x01, 0x04, 0x08, 0x00, 0x80, 0x11}, //W R1
                                                {0x01, 0x02, 0x04, 0x19, 0x00, 0xBF, 0xC2}, //W R2   
                                                {0x01, 0x03, 0x04, 0x00, 0x60, 0x04, 0xB3}, //W R3
                                                {0x01, 0x04, 0x04, 0x00, 0x81, 0x40, 0x3C}, //W R4
                                                {0x01, 0x05, 0x04, 0x00, 0x58, 0x00, 0x05}  //W R5
                                                                                           };

        byte[] terminator = {0x3A};  
        string tString = string.Empty;
        int current_register = 5;
        bool allow_to_send_next = true;
        bool transmission_in_progress = false;
        bool error_in_transmission = false;
        bool syth_configured = false;

        int ms_wait = 300; //время задержки (timeout)

        SerialPort COM_Port;
        delegate void SetTextCallback(string text);
        
        string[] ports = SerialPort.GetPortNames();      

        public Form1()
        {
            InitializeComponent();
            
            for (int i = 0; i < ports.Length; i++)
            {
                COM_list.Items.Add( ports[i] );
            }
            
            COM_Port = new SerialPort();
        }


        private void button1_Click(object sender, EventArgs e)
        {
            if (!COM_Port.IsOpen)
            {
                ports = SerialPort.GetPortNames();
                COM_list.Items.Clear();
                for (int i = 0; i < ports.Length; i++)
                {
                    COM_list.Items.Add(ports[i]);
                }
                messageLabel.Text = "";
                Connect.Text = "Подключиться";
            }
            else
            {
                foreach (int i in registerChecks.CheckedIndices)
                {
                    registerChecks.SetItemCheckState(i, CheckState.Unchecked);
                }
                messageText.Text = "";
                readData.Text = "";
                Task.Delay(1000).Wait();
                try
                {
                    COM_Port.Close();
                }
                catch (IOException)
                {
 
                }
                messageLabel.Text = "";
                Connect.Text = "Подключиться";
                current_register = 5; //0
                error_in_transmission = false;
                allow_to_send_next = true;
                transmission_in_progress = false;
                ports = SerialPort.GetPortNames();
                COM_list.Items.Clear();
                for (int i = 0; i < ports.Length; i++)
                {
                    COM_list.Items.Add(ports[i]);
                }
            }
        }

        private void Connect_Click(object sender, EventArgs e)
        {
            if (COM_list.SelectedIndex != -1)
            {
                if (!COM_Port.IsOpen)
                {
                    try
                    {
                        COM_Port.PortName = ((string)COM_list.SelectedItem);
                        COM_Port.BaudRate = 9600;
                        COM_Port.DataBits = 8;
                        COM_Port.Parity = System.IO.Ports.Parity.None;
                        COM_Port.StopBits = System.IO.Ports.StopBits.One;
                        COM_Port.ReadTimeout = 1000;
                        COM_Port.WriteTimeout = 1000;

                        try
                        {
                            COM_Port.Open();
                        }
                        catch (IOException)
                        {
                            messageLabel.Text = "Ошибка при работе с портом";
                            Connect.Text = "Подключиться";
                            return;
                        }

                        Connect.Text = "Отключиться";
                        messageLabel.Text = "Подключение установлено";
                    }
                    catch (UnauthorizedAccessException)
                    {
                        messageLabel.Text = "Ошибка при подключении к порту";
                    }
                }
                else
                {
                    try
                    {
                        COM_Port.Close();
                    }
                    catch (IOException)
                    {
                        messageLabel.Text = "Ошибка при работе с портом";
                        Connect.Text = "Подключиться";
                        return;
                    }

                    messageLabel.Text = "Соединение с портом разорвано ";
                    Connect.Text = "Подключиться";
                }
            }         
        }

        public void Send_Click(object sender, EventArgs e)
        {
            if ((COM_Port.IsOpen)  && (transmission_in_progress == false)) // &&(settingsList.SelectedIndex != -1)
            {
                //Возвращаем старые настройки
                userSettings[0, 3] = 0x00;
                userSettings[0, 4] = 0x00;
                userSettings[0, 5] = 0x00;
                userSettings[4, 4] = 0x81;

                RFout = (float)SynthFreq.Value;
                VCO = RFout;
                DIV = 1;
                while (VCO < 2200)
                {
                    VCO *= 2;
                    DIV *= 2;
                }

                fPFD = RFin*((float) ( 1 + D )/( Rcounter * (1 + T) ) );
                INT = (ushort) (RFout * DIV / fPFD);
                userSettings[0, 3] |= (byte)(INT >> 9);
                userSettings[0, 4] |= (byte)(INT >> 1);
                userSettings[0, 5] |= (byte)(INT << 7);
                userSettings[4, 4] |= (byte)((DIV-1) << 4);
                if (INT * fPFD / DIV != RFout)
                {
                    messageText.Text = "Неверные настройки";
                    return;
                }

                foreach (int i in registerChecks.CheckedIndices)
                {
                   registerChecks.SetItemCheckState(i, CheckState.Unchecked);
                }
                messageText.Text = "";
                readData.Text = "";
                Task.Delay(1000).Wait();

                current_register = 5; //0
                error_in_transmission = false;
                transmission_in_progress = true;

                for (int i = 5; i >= 0; i--)
                {
                    byte[] data_to_send = new byte[7];
                    for (int j = 0; j < 7; j++)
                    {
                        data_to_send[j] = userSettings[current_register, j];
                    }
                    
                    try 
                    {
                        COM_Port.Write(terminator, 0, 1);
                        COM_Port.Write(data_to_send, 0, 7);
                        COM_Port.Write(terminator, 0, 1);
                    }
                    catch (IOException)
                    {
                        COM_cable_disconnected();
                        break;
                    }

                    allow_to_send_next = false;

                    if (current_register >= 0) 
                    {
                        try
                        {
                            COM_Port.DiscardInBuffer();
                            Task.Delay(ms_wait).Wait();
                            COM_port_read();
                            COM_Port.DiscardInBuffer();
                        }
                        catch (IOException)
                        {
                            COM_cable_disconnected();
                            break;
                        }
                    }

                    if ((allow_to_send_next == false) || (error_in_transmission == true))
                    {
                      if (allow_to_send_next == false) messageText.Text = "Превышено время ожидания ответа";
                      if (error_in_transmission == true) messageText.Text = "Ошибка в процессе передачи";
                      current_register = 5; //0
                      error_in_transmission = false;
                      allow_to_send_next = true;
                      transmission_in_progress = false;
                      break;
                    }
                        
                    if (current_register == -1)
                    {
                       messageText.Text = "Запись регистров успешно завершена";
                       current_register = 5; //0
                       error_in_transmission = false;
                       transmission_in_progress = false;

                       if (allow_to_send_next == true)
                       {
                           if (syth_configured == true) readData.Text = "Синтезатор сконфигурирован";
                           else readData.Text = "Ошибка конфигурации синтезатора";
                       }
                            else readData.Text = "Превышено время ожидания ответа от синтезатора";
                    }
                }
            }

        }

        void COM_cable_disconnected()
        {
            foreach (int i in registerChecks.CheckedIndices)
            {
                registerChecks.SetItemCheckState(i, CheckState.Unchecked);
            }
            messageText.Text = "";
            readData.Text = "";
            Task.Delay(1000).Wait();
            try
            {
                COM_Port.Close();
            }
            catch (IOException)
            {

            }
            messageLabel.Text = "Порт не отвечает";
            Connect.Text = "Подключиться";
            current_register = 5; //0
            error_in_transmission = false;
            allow_to_send_next = true;
            transmission_in_progress = false;
            ports = SerialPort.GetPortNames();
            COM_list.Items.Clear();
            for (int i = 0; i < ports.Length; i++)
            {
                COM_list.Items.Add(ports[i]);
            }
        }

        void COM_port_read()
        {
            if (COM_Port.IsOpen)
            {
                int dataLength = COM_Port.BytesToRead;
                if (dataLength != 0) //!=
                {
                    byte[] data = new byte[dataLength];
                    int nbrDataRead = COM_Port.Read(data, 0, dataLength);

                    tString = Encoding.ASCII.GetString(data, 0, nbrDataRead);

                    SetText(tString);
                    //readData.Text = dataLength.ToString();
                    //readData.Text = tString;
                }
            }
            else
            { 
                COM_cable_disconnected();
            }           
        }

        
        public void SetText(string text)
        {           
            if (this.readData.InvokeRequired)
            {
                SetTextCallback d = new SetTextCallback(SetText);
                this.Invoke(d, new object[] { text });
            }
            else
            {
                if (text == "ACK")
                {
                    allow_to_send_next = true;
                    registerChecks.SetItemChecked(current_register, true);
                    current_register--;
                }
                else if (text == "NACK")
                {
                    error_in_transmission = true;
                }
                else if (text == "ACKSUCCESS")
                {
                    syth_configured = true;
                    allow_to_send_next = true;
                    registerChecks.SetItemChecked(current_register, true);
                    current_register--;
                }
                else if (text == "ACKFAIL")
                {
                    syth_configured = false;
                    allow_to_send_next = true;
                    registerChecks.SetItemChecked(current_register, true);
                    current_register--;
                }
                else
                {
                    error_in_transmission = true;
                }
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void label2_Click(object sender, EventArgs e)
        {

        }
    }
}
