using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using FB_Example.Properties;
using Mcs.Usb;
using Newtonsoft.Json;

namespace my_interface
{
    public partial class Form1 : Form
    {
        CMcsUsbListEntryNet RawPort;
        CMcsUsbListEntryNet DspPort;

        CMcsUsbListNet devices = new CMcsUsbListNet(); // Create object of CMscUsbListNet Class
        CMeaUSBDeviceNet mea = new CMeaUSBDeviceNet();

        const int Channels = 64*4;
        const int Checksum = 1; //Not the checksum, instead the digital input. Needed for synchronization.

        const int TotalChannels = Channels + Checksum;

        int PackageSize = 2000; // Number of samples per package

        // These values are not fix and are set through the GUI automatically
        int Samplerate    = 50000;
        int filtfreq      = 300;
        uint isi          = 250;
        uint isp          = 5000;
        int num_stim      = 5;

        int t_port        = 8701;
        int s_port        = 8750;
        int n_port        = 20;
        string auth_key   = "undisclosed";

        bool purge_at_start = true; // If this is true, then the DMA buffer is purged immediately after the program has been loaded on the DSP.
        
        int timestamp   = 0;

        volatile uint buffer_size = 0;

        public object _lock = 0;

        const uint LockMask = 64;

        int [] data;
        int frames_read;

        StreamReader python_stream;

        PyWrapper pywrapper;

        Thread ProcessPackages;
        Thread sendDataStream;

        public Form1()
        {
            _lock = 0;

            InitializeComponent();

            devices.DeviceArrival += new OnDeviceArrivalRemoval(devices_DeviceArrival);
            devices.DeviceRemoval += new OnDeviceArrivalRemoval(devices_DeviceRemoval);

            mea.ChannelDataEvent += new OnChannelData(mea_ChannelDataEvent);

            List<String> net_types = new List<String>();
            net_types.Add("No structure");
            net_types.Add("5x3  o circles");
            net_types.Add("10+2 / lines");
            net_types.Add("10+2 \\ lines");

            System.Diagnostics.Debug.WriteLine(net_types.Count);
            int i;
            for(i=0; i<net_types.Count; i++)
            {
                cB_SCU1_HS1.Items.Add(net_types[i]);
                cB_SCU1_HS2.Items.Add(net_types[i]);
                cB_SCU1_HS3.Items.Add(net_types[i]);
                cB_SCU1_HS4.Items.Add(net_types[i]);

                cB_SCU2_HS1.Items.Add(net_types[i]);
                cB_SCU2_HS2.Items.Add(net_types[i]);
                cB_SCU2_HS3.Items.Add(net_types[i]);
                cB_SCU2_HS4.Items.Add(net_types[i]);
            }
            cB_SCU1_HS1.SelectedIndex = 0;
            cB_SCU1_HS2.SelectedIndex = 0;
            cB_SCU1_HS3.SelectedIndex = 0;
            cB_SCU1_HS4.SelectedIndex = 0;
            cB_SCU2_HS1.SelectedIndex = 0;
            cB_SCU2_HS2.SelectedIndex = 0;
            cB_SCU2_HS3.SelectedIndex = 0;
            cB_SCU2_HS4.SelectedIndex = 0;

            btnDeviceOK.BackColor = Color.Red;
            
            SearchDevice();
        }

        void devices_DeviceRemoval(CMcsUsbListEntryNet entry)
        {
            SearchDevice();
        }

        void devices_DeviceArrival(CMcsUsbListEntryNet entry)
        {
            SearchDevice();
        }

        private void SearchDevice()
        {
            System.Diagnostics.Debug.WriteLine("Search Device");

            devices.Initialize(DeviceEnumNet.MCS_MEAUSB_DEVICE); // Get list of MEA devices connect by USB

            RawPort = null;
            DspPort = null;

            rawSerial.Text = "not found";
            dspSerial.Text = "not found";


            for (uint i = 0; i < devices.Count; i++) // loop through number of devices found
            {
                if (devices.GetUsbListEntry(i).SerialNumber.EndsWith("A")) // check for each device if serial number ends with "A" (USB 1) This USB interface will be used by MC_Rack
                {
                    RawPort = devices.GetUsbListEntry(i); // assign to RawPort "handle"
                    rawSerial.Text = RawPort.SerialNumber;
                }
                if (devices.GetUsbListEntry(i).SerialNumber.EndsWith("B"))// check for each device if serial number ends with "B" (USB 2) This USB interface will be used to control DSP
                {
                    DspPort = devices.GetUsbListEntry(i);  // assign to DSPPort "handle"
                    dspSerial.Text = DspPort.SerialNumber;
                }
            }

            if (RawPort != null && DspPort != null)
            {
                btnDeviceOK.BackColor = Color.LawnGreen;
            }
            else if (RawPort != null || DspPort != null)
            {
                btnDeviceOK.BackColor = Color.Yellow;
            }
            else
            {
                btnDeviceOK.BackColor = Color.Red;
            }

            // Set Filters
            lock (_lock)
            {
                CMcsUsbFactoryNet factorydev = new CMcsUsbFactoryNet(); // Create object of class CMcsUsbFactoryNet (provides firmware upgrade and register access capabilities)
                if (DspPort != null && factorydev.Connect(DspPort, LockMask) == 0)  // if connect call returns zero, connect has been successful
                {
#if false
                double[] xcoeffs;
                double[] ycoeffs;
                mkfilterNet.mkfilter("Bu", 0, "Lp", 2, 1000.0 / 50000.0, 0, out xcoeffs, out ycoeffs);
                factorydev.WriteRegister(0x600, DoubleToFixedInt(1, 16, 30, xcoeffs[0])); // set b[0] fpr 100 Hz HP
                factorydev.WriteRegister(0x608, DoubleToFixedInt(1, 15, 30, xcoeffs[1])); // set b[1] fpr 100 Hz HP
                factorydev.WriteRegister(0x60C, DoubleToFixedInt(1, 30, 30, ycoeffs[1])); // set a[1] fpr 100 Hz HP
                factorydev.WriteRegister(0x610, DoubleToFixedInt(1, 16, 30, xcoeffs[2])); // set b[2] fpr 100 Hz HP
                factorydev.WriteRegister(0x614, DoubleToFixedInt(1, 30, 30, ycoeffs[2])); // set a[2] fpr 100 Hz HP
                factorydev.WriteRegister(0x61C, 0x00000001); // enable
                mkfilterNet.mkfilter("Bu", 0, "Hp", 2, 100.0 / 50000.0, 0, out xcoeffs, out ycoeffs);
                factorydev.WriteRegister(0x620, DoubleToFixedInt(1, 16, 30, xcoeffs[0])); // set b[0] fpr 100 Hz HP
                factorydev.WriteRegister(0x628, DoubleToFixedInt(1, 15, 30, xcoeffs[1])); // set b[1] fpr 100 Hz HP
                factorydev.WriteRegister(0x62C, DoubleToFixedInt(1, 30, 30, ycoeffs[1])); // set a[1] fpr 100 Hz HP
                factorydev.WriteRegister(0x630, DoubleToFixedInt(1, 16, 30, xcoeffs[2])); // set b[2] fpr 100 Hz HP
                factorydev.WriteRegister(0x634, DoubleToFixedInt(1, 30, 30, ycoeffs[2])); // set a[2] fpr 100 Hz HP
                factorydev.WriteRegister(0x63C, 0x00000001); // enable
#endif
                    factorydev.Disconnect();
                }
            }
        }

        static uint DoubleToFixedInt(int vk, int nk, int commaPos, double valF)
        {
            valF *= 1 << nk;
            if (valF > 0)
            {
                valF += 0.5;
            }
            else
            {
                valF -= 0.5;
            }
            ulong mask = ((ulong)1 << (vk + nk + 1)) - 1;
            ulong val = (ulong)valF;
            uint value = (uint)(val & mask);
            if (commaPos > nk)
            {
                value = value << (commaPos - nk);
            }

            return value;
        }


        private void ConnectMEA_Click(object sender, EventArgs e)
        {

            CMcsUsbFactoryNet factorydev = new CMcsUsbFactoryNet(); // Create object of class CMcsUsbFactoryNet (provides firmware upgrade and register access capabilities)
            lock (_lock)
            {
                if (factorydev.Connect(DspPort, LockMask) == 0)  // if connect call returns zero, connect has been successful
                {
                    //int Thresh = (int)(Convert.ToDouble(SpikeThresh.Text) / (5000000 / Math.Pow(2, 24) / 10)); // 5 V input range ADC, 24bit ADC, 10 volt hardware gain
                    int Thresh = Convert.ToInt32(10);
                    int circuit = 15;

                    // Write example patterns to MEA
                    if (Thresh == 2)
                    {
                        for (int i = 0; i < 128; i++)
                        {
                            factorydev.WriteRegister(0x1800 + (0x0004 * (uint)i), (2 << 0) + (2 << 6) + (3 << 12));
                        }
                    }

                    factorydev.WriteRegister(0x1004, (uint)circuit);
                    factorydev.WriteRegister(0x1000, (uint)Thresh);

                    factorydev.Disconnect();
                }
            }

            CMeaUSBDeviceNet dev = new CMeaUSBDeviceNet();
            if (dev.Connect(DspPort, LockMask) == 0)
            {
                //dev.SetDigoutSource(0, DigitalSourceEnumNet.Feedback, 0); // set digital out 1 dev.SetDigoutSource(0) to the Feedback Register element 0  DigitalSourceEnumNet.Feedback, 0

                dev.Disconnect();
            }
        }

        private void UploadDSPBinary_Click(object sender, EventArgs e)
        {
            CMcsUsbFactoryNet factorydev = new CMcsUsbFactoryNet();

            UploadDSPBinary.Enabled = false;
            tBFilteringFreq.Enabled = false;
            tBSamplingFreq.Enabled = false;
            StopDSP.Enabled = true;

            if (mea.Connect(RawPort) == 0)
            {
                int ChannelsInBlock;

                mea.SetSampleRate(Samplerate, 1, 0);
                mea.SetNumberOfAnalogChannels(Channels, 0, 0, 0, 0); // Read raw data
                mea.EnableChecksum(false, 0); // Do not send checksum
                mea.EnableDigitalIn(true, 0); // However, do send the digital inputs (single element, but two packages)
                mea.SetDataMode(DataModeEnumNet.dmSigned32bit, 0);
                ChannelsInBlock = mea.GetChannelsInBlock();

                System.Diagnostics.Debug.WriteLine("---");
                System.Diagnostics.Debug.WriteLine(ChannelsInBlock);
                System.Diagnostics.Debug.WriteLine("---");

                pBLengthBufferQueue.Maximum = (int)(PackageSize * 50);
                mea.SetSelectedData(TotalChannels, pBLengthBufferQueue.Maximum, PackageSize, SampleSizeNet.SampleSize32Signed, ChannelsInBlock);
                mea.StartDacq();
            }

            string FirmwareFile;

            FirmwareFile = @"C:\Users\MEA2100\Documents\Stephan\DSP\Open_Loop_mini\Release\";
            FirmwareFile += "Open_Loop_mini.bin";
            lock (_lock)
            {
                factorydev.LoadUserFirmware(FirmwareFile, DspPort);           // Code for uploading compiled binary

                System.Threading.Thread.Sleep(100);

                if (factorydev.Connect(DspPort, LockMask) == 0)  // if connect call returns zero, connect has been successful
                {
                    uint state = 0;
                    
                    // Wait until DSP is ready receiving data. Then, send initialization data
                    System.Diagnostics.Debug.WriteLine("Wait for DSP to be ready");
                    while (state != 0x0002)
                        state = factorydev.ReadRegister(0x1000);
                    System.Diagnostics.Debug.WriteLine("Send all values");
                    factorydev.WriteRegister(0x1004, Convert.ToUInt32(cB_SCU1.Checked));
                    factorydev.WriteRegister(0x1008, Convert.ToUInt32(cB_SCU2.Checked));
                    factorydev.WriteRegister(0x100c, (uint)num_stim);
                    factorydev.WriteRegister(0x1010, Convert.ToUInt32(rBRandom.Checked));
                    UInt32 circuit_values = 0;
                    circuit_values |= (((UInt32)cB_SCU1_HS1.SelectedIndex) & 0xf) << 0;
                    circuit_values |= (((UInt32)cB_SCU1_HS2.SelectedIndex) & 0xf) << 4;
                    circuit_values |= (((UInt32)cB_SCU1_HS3.SelectedIndex) & 0xf) << 8;
                    circuit_values |= (((UInt32)cB_SCU1_HS4.SelectedIndex) & 0xf) << 12;
                    circuit_values |= (((UInt32)cB_SCU2_HS1.SelectedIndex) & 0xf) << 16;
                    circuit_values |= (((UInt32)cB_SCU2_HS2.SelectedIndex) & 0xf) << 20;
                    circuit_values |= (((UInt32)cB_SCU2_HS3.SelectedIndex) & 0xf) << 24;
                    circuit_values |= (((UInt32)cB_SCU2_HS4.SelectedIndex) & 0xf) << 28;
                    factorydev.WriteRegister(0x1014, circuit_values);
                    factorydev.WriteRegister(0x1018, isi*1000);
                    factorydev.WriteRegister(0x101c, isp);
                    factorydev.WriteRegister(0x1020, Convert.ToUInt32(cBMultiPatternStim.Checked));
                    factorydev.WriteRegister(0x1000, 0); // Let DSP know all data has been sent

                    // Wait until DSP is finished set up
                    System.Diagnostics.Debug.WriteLine("Wait for DSP to be ready");
                    while (state != 0x0001)
                        state = factorydev.ReadRegister(0x1000);

                    System.Diagnostics.Debug.WriteLine("Initialization is finished");

                    factorydev.Disconnect();
                }
            }

            timestamp = 0;
            pywrapper.Clear();

            ProcessPackages = new Thread(new ThreadStart(ThreadFunction));
            ProcessPackages.Start();

            sendDataStream = new Thread(new ThreadStart(sendDataStreamFunction));
            sendDataStream.Start();
        }

        private void sendDataStreamFunction()
        {
            //while (true)
            //    pywrapper.sendDataStream();
        }

        private void ThreadFunction()
        {
            UInt32 value;
            CMcsUsbFactoryNet factorydev = new CMcsUsbFactoryNet();

            while (true)
            {
                pywrapper.ProcessPackages();
                pBLengthInQueue.Invoke(new MethodInvoker(delegate { pBLengthInQueue.Value = Math.Min(pywrapper.length_messages(), pBLengthInQueue.Maximum); }));
                pBLengthOutQueue.Invoke(new MethodInvoker(delegate { pBLengthOutQueue.Value = Math.Min(pywrapper.length(), pBLengthOutQueue.Maximum); }));
                pBLengthBufferQueue.Invoke(new MethodInvoker(delegate { pBLengthBufferQueue.Value = Math.Min((int)(buffer_size), pBLengthBufferQueue.Maximum); }));

                lock (_lock)
                {
                    if (factorydev.Connect(DspPort, LockMask) == 0)
                    {
                        value = factorydev.ReadRegister(0x1600);
                        textBox1.Invoke(new MethodInvoker(delegate { textBox1.Text = value.ToString(); }));

                        value = factorydev.ReadRegister(0x1604);
                        textBox2.Invoke(new MethodInvoker(delegate { textBox2.Text = value.ToString(); }));

                        value = factorydev.ReadRegister(0x1608);
                        textBox3.Invoke(new MethodInvoker(delegate { textBox3.Text = value.ToString(); }));

                        value = factorydev.ReadRegister(0x160c);
                        textBox4.Invoke(new MethodInvoker(delegate { textBox4.Text = value.ToString(); }));

                        value = factorydev.ReadRegister(0x1610);
                        textBox5.Invoke(new MethodInvoker(delegate { textBox5.Text = value.ToString(); }));

                        value = factorydev.ReadRegister(0x1614);
                        textBox6.Invoke(new MethodInvoker(delegate { textBox6.Text = value.ToString(); }));

                        value = factorydev.ReadRegister(0x1618);
                        textBox7.Invoke(new MethodInvoker(delegate { textBox7.Text = value.ToString(); }));

                        value = factorydev.ReadRegister(0x161c);
                        textBox8.Invoke(new MethodInvoker(delegate { textBox8.Text = value.ToString(); }));


                        factorydev.Disconnect();
                    }
                }
            }
        }

        private void StopDSP_Click(object sender, EventArgs e)
        {
            UploadDSPBinary.Enabled = true;
            StopDSP.Enabled         = false;
            tBFilteringFreq.Enabled = true;
            tBSamplingFreq.Enabled  = true;

            CMcsUsbFactoryNet factorydev = new CMcsUsbFactoryNet(); // Create object of class CMcsUsbFactoryNet (provides firmware upgrade and register access capabilities)
            lock (_lock)
            {
                if (factorydev.Connect(DspPort, LockMask) == 0)  // if connect call returns zero, connect has been successful
                {
                    factorydev.Coldstart(CFirmwareDestinationNet.MCU1);
                    factorydev.Disconnect();
                }
            }
        }

        void mea_ChannelDataEvent(CMcsUsbDacqNet dacq, int CbHandle, int numFrames)
        {
            uint[] stimpattern = pywrapper.ReadStimPattern();
            //System.Diagnostics.Debug.WriteLine(stimpattern[1]);
            while (numFrames >= PackageSize)
            {
                buffer_size = (uint)numFrames;
                frames_read = 0;
                if (PackageSize*3 < numFrames && purge_at_start)
                {
                    System.Diagnostics.Debug.WriteLine("!!!!!");
                    System.Diagnostics.Debug.WriteLine("Purge the data buffer");
                    System.Diagnostics.Debug.WriteLine("!!!!!");
                    while(numFrames > PackageSize)
                    {
                        data = mea.ChannelBlock_ReadFramesI32(0, PackageSize, out frames_read);
                        numFrames = (int)mea.ChannelBlock_AvailFrames(0);
                    }
                    System.Diagnostics.Debug.WriteLine("Buffer is purged");
                    purge_at_start = false;
                }
                else if (numFrames >= PackageSize)
                {
                    data = mea.ChannelBlock_ReadFramesI32(0, PackageSize, out frames_read);
                }
                else
                {
                    break;
                }
                numFrames = (int)mea.ChannelBlock_AvailFrames(0);

                timestamp = timestamp + 1;

                ChannelData cdData = new ChannelData();
                cdData.Data = data;
                cdData.timestamp = timestamp;
                cdData.stimpattern = stimpattern;

                pywrapper.Enqueue(cdData);
                pywrapper.sendDataStream();
            }
        }

        private void cB_SCU1_CheckedChanged(object sender, EventArgs e)
        {
            if(cB_SCU1.Checked)
            {
                cB_SCU1_HS1.Enabled = true;
                cB_SCU1_HS2.Enabled = true;
                cB_SCU1_HS3.Enabled = true;
                cB_SCU1_HS4.Enabled = true;
            }
            else
            {
                cB_SCU1_HS1.Enabled = false;
                cB_SCU1_HS2.Enabled = false;
                cB_SCU1_HS3.Enabled = false;
                cB_SCU1_HS4.Enabled = false;
                cB_SCU1_HS1.SelectedIndex = 0;
                cB_SCU1_HS2.SelectedIndex = 0;
                cB_SCU1_HS3.SelectedIndex = 0;
                cB_SCU1_HS4.SelectedIndex = 0;
            }
        }

        private void cB_SCU2_CheckedChanged(object sender, EventArgs e)
        {
            if (cB_SCU2.Checked)
            {
                cB_SCU2_HS1.Enabled = true;
                cB_SCU2_HS2.Enabled = true;
                cB_SCU2_HS3.Enabled = true;
                cB_SCU2_HS4.Enabled = true;
            }
            else
            {
                cB_SCU2_HS1.Enabled = false;
                cB_SCU2_HS2.Enabled = false;
                cB_SCU2_HS3.Enabled = false;
                cB_SCU2_HS4.Enabled = false;
                cB_SCU2_HS1.SelectedIndex = 0;
                cB_SCU2_HS2.SelectedIndex = 0;
                cB_SCU2_HS3.SelectedIndex = 0;
                cB_SCU2_HS4.SelectedIndex = 0;
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if((cB_SCU1.Checked == false) && (cB_SCU2.Checked == false))
                    return;

            //TODO These features should be implemented
            if (cB_SCU1_HS1.SelectedIndex > 1)
                throw new System.InvalidOperationException("This network type has not been implemented yet.");
            if (cB_SCU1_HS2.SelectedIndex > 1)
                throw new System.InvalidOperationException("This network type has not been implemented yet.");
            if (cB_SCU1_HS3.SelectedIndex > 1)
                throw new System.InvalidOperationException("This network type has not been implemented yet.");
            if (cB_SCU1_HS4.SelectedIndex > 1)
                throw new System.InvalidOperationException("This network type has not been implemented yet.");

            if (cB_SCU2_HS1.SelectedIndex > 1)
                throw new System.InvalidOperationException("This network type has not been implemented yet.");
            if (cB_SCU2_HS2.SelectedIndex > 1)
                throw new System.InvalidOperationException("This network type has not been implemented yet.");
            if (cB_SCU2_HS3.SelectedIndex > 1)
                throw new System.InvalidOperationException("This network type has not been implemented yet.");
            if (cB_SCU2_HS4.SelectedIndex > 1)
                throw new System.InvalidOperationException("This network type has not been implemented yet.");

            // Load all the parameters from the file
            Samplerate = Int32.Parse(tBSamplingFreq.Text)*1000;
            filtfreq   = Int32.Parse(tBFilteringFreq.Text);
            isp        = UInt32.Parse(tBInterSpikePeriod.Text);
            isi        = UInt32.Parse(tBIntStimInt.Text);
            num_stim   = Int32.Parse(tBNumStim.Text);

            t_port    = Int32.Parse(tBTPort.Text);
            s_port    = Int32.Parse(tBSPort.Text);
            n_port    = Int32.Parse(tBNumPorts.Text);
            auth_key  = tBAuthKey.Text;
            pywrapper = new PyWrapper(DspPort, LockMask, _lock, t_port, s_port, auth_key, n_port);

            // Disable all the relevant fields
            cB_SCU1_HS1.Enabled = false;
            cB_SCU1_HS2.Enabled = false;
            cB_SCU1_HS3.Enabled = false;
            cB_SCU1_HS4.Enabled = false;
            cB_SCU2_HS1.Enabled = false;
            cB_SCU2_HS2.Enabled = false;
            cB_SCU2_HS3.Enabled = false;
            cB_SCU2_HS4.Enabled = false;
            cB_SCU1.Enabled     = false;
            cB_SCU2.Enabled     = false;
            
            tBSamplingFreq.Enabled     = false;
            tBFilteringFreq.Enabled    = false;
            tBNumStim.Enabled          = false;
            tBInterSpikePeriod.Enabled = false;
            tBIntStimInt.Enabled       = false;

            tBAuthKey.Enabled   = false;
            tBTPort.Enabled   = false;
            tBSPort.Enabled = false;
            tBNumPorts.Enabled = false;
            bSTScript.Enabled = false;

            rBRandom.Enabled = false;
            rBSorted.Enabled = false;

            // Transform all parameters into a string
            string args = t_port.ToString() + " " + n_port.ToString() + " " + auth_key + " " + s_port.ToString()
                + " " +
                Samplerate.ToString() + " " + filtfreq.ToString() + " " + isi.ToString() + " " + isp.ToString() + " " + num_stim.ToString() 
                + " " +
                Convert.ToInt32(cB_SCU1.Checked).ToString() + " " + Convert.ToInt32(cB_SCU2.Checked).ToString();
            // SCU1
            args += " " + cB_SCU1_HS1.SelectedIndex;
            args += "," + cB_SCU1_HS2.SelectedIndex;
            args += "," + cB_SCU1_HS3.SelectedIndex;
            args += "," + cB_SCU1_HS4.SelectedIndex;
            // SCU2
            args += "," + cB_SCU2_HS1.SelectedIndex;
            args += "," + cB_SCU2_HS2.SelectedIndex;
            args += "," + cB_SCU2_HS3.SelectedIndex;
            args += "," + cB_SCU2_HS4.SelectedIndex;

            //Start the program
            run_python(args);

            btnScriptRunning.BackColor = Color.Yellow;
        }

        private void run_python(string args)
        {
            ProcessStartInfo psi       = new ProcessStartInfo();
            psi.FileName               = "C:\\Python38\\python.exe";
            psi.Arguments              = "C:\\Users\\MEA2100\\Documents\\Stephan\\Python\\Transform\\main.py " + args;
            psi.RedirectStandardOutput = true;
            psi.UseShellExecute        = false;

            Process proc = Process.Start(psi);
            python_stream = proc.StandardOutput;

            ProcessPackages = new Thread(new ThreadStart(PythonThreadFunction));
            ProcessPackages.Start();
        }

        private void PythonThreadFunction()
        {
            string s;
            int i;
            i = -1;
            while (true)
            {
                s = python_stream.ReadLine();
                if (s != null)
                {
                    System.Diagnostics.Debug.WriteLine(s);
                    btnScriptRunning.BackColor = Color.LawnGreen;
                    if(i==-1)
                        UploadDSPBinary.Invoke(new MethodInvoker(delegate { UploadDSPBinary.Enabled = true; }));
                    i = 0;
                }
                else
                {
                    if (i != -1)
                        i += 1;
                    Thread.Sleep(100);
                    btnScriptRunning.BackColor = Color.Yellow;
                    if (i > 10)
                    {
                        System.Diagnostics.Debug.WriteLine("The Python transofrmation script has exited.");
                        btnScriptRunning.BackColor = Color.Red;

                        UploadDSPBinary.Invoke(new MethodInvoker(delegate { UploadDSPBinary.Enabled = false; }));

                        cB_SCU1_HS1.Invoke(new MethodInvoker(delegate { cB_SCU1_HS1.Enabled = true; }));
                        cB_SCU1_HS2.Invoke(new MethodInvoker(delegate { cB_SCU1_HS2.Enabled = true; }));
                        cB_SCU1_HS3.Invoke(new MethodInvoker(delegate { cB_SCU1_HS3.Enabled = true; }));
                        cB_SCU1_HS4.Invoke(new MethodInvoker(delegate { cB_SCU1_HS4.Enabled = true; }));
                        cB_SCU1_HS1.Invoke(new MethodInvoker(delegate { cB_SCU2_HS1.Enabled = true; }));
                        cB_SCU2_HS2.Invoke(new MethodInvoker(delegate { cB_SCU2_HS2.Enabled = true; }));
                        cB_SCU2_HS3.Invoke(new MethodInvoker(delegate { cB_SCU2_HS3.Enabled = true; }));
                        cB_SCU2_HS4.Invoke(new MethodInvoker(delegate { cB_SCU2_HS4.Enabled = true; }));
                        cB_SCU1.Invoke(new MethodInvoker(delegate { cB_SCU1.Enabled = true; }));
                        cB_SCU2.Invoke(new MethodInvoker(delegate { cB_SCU2.Enabled = true; }));

                        tBSamplingFreq.Invoke(new MethodInvoker(delegate { tBSamplingFreq.Enabled = true; }));
                        tBFilteringFreq.Invoke(new MethodInvoker(delegate { tBFilteringFreq.Enabled = true; }));
                        tBNumStim.Invoke(new MethodInvoker(delegate { tBNumStim.Enabled = true; }));
                        tBInterSpikePeriod.Invoke(new MethodInvoker(delegate { tBInterSpikePeriod.Enabled = true; }));
                        tBIntStimInt.Invoke(new MethodInvoker(delegate { tBIntStimInt.Enabled = true; }));

                        tBSPort.Invoke(new MethodInvoker(delegate { tBAuthKey.Enabled = true; }));
                        tBAuthKey.Invoke(new MethodInvoker(delegate { tBTPort.Enabled = true; }));
                        tBSPort.Invoke(new MethodInvoker(delegate { tBSPort.Enabled = true; }));
                        //tBNumPorts.Invoke(new MethodInvoker(delegate { tBNumPorts.Enabled = true; }));
                        bSTScript.Invoke(new MethodInvoker(delegate { bSTScript.Enabled = true; }));

                        rBRandom.Invoke(new MethodInvoker(delegate { rBRandom.Enabled = true; }));
                        rBSorted.Invoke(new MethodInvoker(delegate { rBSorted.Enabled = true; }));
                        return;
                    }
                }
            }
        }

        private void bStartStim_Click(object sender, EventArgs e)
        {
            uint[] data = new uint[256];
            data[0] = 8;
            pywrapper.dataMessages.Enqueue(data);
        }
            }
}
