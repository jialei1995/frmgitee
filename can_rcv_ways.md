### c#工程中怎么处理收到的can数据

```c#
//界面加载的时候初始化线程
Thread recv_data = new Thread(Read_USB_CAN_Data);//load时就开启此线程
recv_data.Start();
```

线程中的数据处理

```c#
unsafe private void Read_USB_CAN_Data()
{
    UInt32 res = new UInt32();//只需要新建一个变量就够了 不然一直新建 不释放 程序太占内存了
    while (recv_thread_run)//为真才处理数据，关闭时才清零   窗口关闭的时候置false  窗口打开为真
    {
        while (Update.down_flag == true)//如果下位机程序需要升级，则此线程慢慢收数据（放缓一点） 不要影响那边刷程序
            Thread.Sleep(10);
        res = VCI_GetReceiveNum(m_devtype, m_devind, m_canind);
        if (res == 0) //如果没有can数据则线程休眠，不占用内存
        {
            BatPack.communicate++; ;
            Thread.Sleep(10);
            continue;//重新收can数据判断是否读到数据--不执行后面的code 
        }
        //res>0 需要处理数据了
        UInt32 con_maxlen = 50;
        IntPtr pt = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(VCI_CAN_OBJ)) * (Int32)con_maxlen);
        //真正接收can数据的函数 将收到的数据存到指针里面 
        res = VCI_Receive(m_devtype, m_devind, m_canind, pt, con_maxlen, 100);
        for (UInt32 i = 0; i < res; i++)//一个个处理收到的can数据
        {
            BatPack.communicate = 0 ;
            VCI_CAN_OBJ obj = (VCI_CAN_OBJ)Marshal.PtrToStructure((IntPtr)((UInt32)pt + i * Marshal.SizeOf(typeof(VCI_CAN_OBJ))), typeof(VCI_CAN_OBJ));
            GRecvObj TempObj = new GRecvObj();
            TempObj.ID = obj.ID;
            for (int j = 0; j < 8; j++)
            {
                TempObj.Data[j] = obj.Data[j];
            }
            ReadDataQueue.Enqueue(TempObj);//收到的数据进队列 等会在队列中处理数据
        }
        Marshal.FreeHGlobal(pt);//每次处理完释放指针
    }
}

//给收的can数据类型定个类型
public class GRecvObj
{
    public uint ID;
    public uint TimeStamp;
    public byte TimeFlag;
    public byte SendType;
    public byte RemoteFlag;//是否是远程帧
    public byte ExternFlag;//是否是扩展帧
    public byte DataLen;
    public byte[] Data = new byte[8];
}
private Queue<GRecvObj> ReadDataQueue = new Queue<GRecvObj>();  //初始化直接定义一个队列
```

对于收到的can数据的处理，在定时器中处理：在定时器中先将各种收到的can数据 存到不同的变量中，然后直接将变量解析到控件中显示

```c#
unsafe private bool ReceiveCanData()
{
    GRecvObj item = null;
    while(ReadDataQueue.Count > 0)
    {
        item = ReadDataQueue.Dequeue();//处理队头数据，数据是can的一帧数据  ID+8 byte数据
        if (item == null)
            return false;//队列中无数据
        switch (item.ID)
        {
            case 0x100:
                break;
            case 0x110:
                BatPack.ProductID[0] = item.Data[0];
                break;
            case 0x201://对于同一帧ID的数据 可以通过Data[0]的不同表示多个数据
                if (item.Data[0] == 0x01)
                {
                    BatPack.ReadyFlag = item.Data[1];
                    BatPack.PackTotalVol = Convert.ToUInt16(((item.Data[2] << 8) + item.Data[3]) & 0x7fff);
                    BatPack.PackSoc = Convert.ToUInt16(((item.Data[4] << 8) + item.Data[5]) & 0x7fff);
                    BatPack.PackSoh = Convert.ToUInt16(((item.Data[6] << 8) + item.Data[7]) & 0x7fff);
                }
                else if(item.Data[0] == 0x02)
                {
                    BatPack.MaxVol = Convert.ToUInt16(((item.Data[1] << 8) + item.Data[2]) & 0x7fff);
                    BatPack.MaxVolNumber = (byte)(item.Data[3] & 0x7f);
                    BatPack.MinVol = Convert.ToUInt16(((item.Data[4] << 8) + item.Data[5]) & 0x7fff);
                    BatPack.MinVolNumber = (byte)(item.Data[6] & 0x7f);
                }
                else if (item.Data[0] == 0x03)
                {
                    BatPack.MaxTemp = Convert.ToUInt16(((item.Data[1] << 8) + item.Data[2]) & 0x7fff);
                    BatPack.MaxTempNumber = (byte)(item.Data[3] & 0x7f);
                    BatPack.MinTemp = Convert.ToUInt16(((item.Data[4] << 8) + item.Data[5]) & 0x7fff);
                    BatPack.MinTempNumber = (byte)(item.Data[6] & 0x7f);
                }
                else if (item.Data[0] == 0x04)
                {
                    BatPack.AverageTemp = Convert.ToInt16(((item.Data[1] << 8) + item.Data[2]) & 0x7fff);
                    BatPack.AverageVol = Convert.ToUInt16(((item.Data[3] << 8) + item.Data[4]) & 0x7fff);
                    BatPack.LecuInterTemp = Convert.ToUInt16(((item.Data[5] << 8) + item.Data[6]) & 0x7fff);
                }
                else if (item.Data[0] == 0x05)
                {
                    BatPack.ErrorMsg = item.Data[2];
                    BatPack.LevelAlarm1 = item.Data[4];
                    BatPack.LevelAlarm2 = item.Data[6];
                }
                else if (item.Data[0] == 0x06)
                {
                    if (BatPack.LECUProtocolVerion == 0x67)
                    {
                        BatPack.BlanceNum = item.Data[1];
                        BatPack.BlanceState = item.Data[5];
                    }
                    else
                    {
                        BatPack.PackCrruent = (Int16)(((item.Data[2] << 8) + (item.Data[3])) - 30000);
                    }
                }

                else if (item.Data[0] == 0x07)
                {
                    BatPack.SBlanceState = (UInt32)((item.Data[2] << 24) + (item.Data[3] << 16) + (item.Data[4] << 8) + item.Data[5]);
                }
                else if (item.Data[0] == 0x08)
                {
                    BatPack.PackCrruent = (Int16)((((item.Data[2]) << 8) + (item.Data[3])) - 32000);
                    BatPack.PackTotalDischarge = ((UInt32)((item.Data[4] << 24) + ((item.Data[5]) << 16) + ((item.Data[6]) << 8) + (item.Data[7])));
                }
                break;
                // 系统时间
            case 0x605:
                int year = 2000 + Convert.ToInt32(item.Data[1]);
                int month = Convert.ToInt32(item.Data[2]);
                int day = Convert.ToInt32(item.Data[3]);
                int hour = Convert.ToInt32(item.Data[4]);
                int mins = Convert.ToInt32(item.Data[5]);
                int seconds = Convert.ToInt32(item.Data[6]);
                BatPack.Rstate = item.Data[7];
                try
                {
                    BatPack.LecuTime = new DateTime(year, month, day, hour, mins, seconds);
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }

                break;
            case 0x60A:
                if(item.Data[0] == 0x47)
                {
                    BatPack.ProductID[0] = item.Data[2];
                    BatPack.ProductID[1] = item.Data[3];
                    BatPack.ProductID[2] = item.Data[4];
                    BatPack.ProductID[3] = item.Data[5];
                }
                break;
            case 0x401:
                readallargflag++;
                
                break;
            case 0x501:
                readallargflag++;
                if (item.Data[0] == 0x01)
                {
                    BatPack.ChargeReqCrruent = (Int16)((item.Data[1] << 8) + item.Data[2] - 32000);
                    BatPack.ChargeReqVol = (UInt16)((item.Data[3] << 8) + item.Data[4]);
                }
                break;
            case 0x1801f4f5:
                if ((item.Data[0] == 0x47) && (item.Data[1] == 0x54))
                {
                    ReadAmMsg.MsgNum = item.Data[5];
                    ReadAmMsg.NextMsgNum = 0;
                    lbAlarmNum.Text = item.Data[5].ToString();
                }
                break;
            case 0x1802f4f5:
                {
                    if (ReadAmMsg.NextMsgNum == item.Data[5])
                    {
                        ReadAmMsg.NextFramNum = 0;
                    }
                }
                break;
            default:
                break;
        }
    }
    return true;
}
```

