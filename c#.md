### 快捷键

ctrl+k+d   自动对齐，注意k弹起来再按d ctrl是一直按下的
ctrl+k+c   选中注释，先k再c
#region (注释)+ #endregion  将需要折叠的代码放到region中间，然后点击小-，即可折叠。

alt+shift+f10：查看类在哪个命名空间中

### 数据类型

字符串 string var="nihao 你好";  可以为空
字符   char  c = 'x'; 不能为空
int  整数
double  小数
float   小数
decimal  金钱（小数）
decimal money=100m;   最后面有个m
String  与 string  一样的都能定义数据

String：是.net平台的类  string：c#的关键字

+: 连接字符串   当+两边有一边是字符串，则+的作用就是连接
+：两边都是数字  则+作用是相加

Console.WriteLine("第一个数字是{0},第二个数字是{1}",n1,n2);

### 控制台输入

的数据只能用string去存储,不能说输入的是数字就用int去存，收到字符串后再去转
string  var = Console.ReadLine();

### @的作用

字符串前加个@，则路径只需要一个\即可。
ps：
string path=@"F:\a\b\c\d\e\1.txt"  如果没有@,则需要 写成 "F:\\A\\B\\C\\D\\1.txt"
@：取消\在“”中的转义作用
@：将“”中的字符串按照原格式输出

### 英文国际化：

程序中的messageBox.Show("nihao"); 在中文系统中会显示成确定，更改当前的系统的语言为英文，则运行后确定就会变成ok

### c# 中的Uint32 不能存0xff ff ff ff会溢出

只有uint 或 long 才能装下0xff ff ff ff  long里面是包含正负符号的  uint 是无符号的
坑死人了，c语言uint32就能存下 c#存不下导致函数处理数据 异常，还不会退出来提示。太他妈坑了

### winform中textbox

文本框输入自动变成中文字体--调整textbox属性中的行为->ImeMode为NoControl  以后输入的中文也会转化成 英文字符

### 委托

委托的用法：实现多个子窗口给主窗口发送msg
主窗口class中
1.**类外声明delagate**
public delegate void mydelegate(string s);
public partial class main : Form
{
	public main()
	{
		InitializeComponent();
	}
	//2.定义委托的实现--在主窗体中
	private void showmsg(string s)
	{
		this.textBox1.Text += s + "\r\n";
	}

```
private void button1_Click(object sender, EventArgs e)
	{
```


```
for (int i = 1; i < 4; i++)
		{
			code.child child = new code.child(i);
			child.Show();//创建子窗体并显示
			//3.关联委托方法--怎么调用呢--利用子窗体对象调用子窗体内的方法
			//n个子窗体都绑定的主窗体的showmsg方法，所以在3个子窗体点击按钮都可以影响主窗体的textbox
			//给委托变量赋值---在调用类中写
			child.mydel += showmsg;
		}
	}
}
子窗口类中：子窗口不能直接控制主窗口里面的控件，则可以通过委托的方法去控制主窗体中的控件
public partial class child : Form
{
	public child(int num)
	{
		InitializeComponent();
		this.Text += "." + num;
	}
	//定义委托变量--------------------------可以在类外通过定义子窗体变量去访问到子窗体中的委托变量来
	//-------谁使用谁创建委托变量----------
	public mydelegate mydel { get; set; } //或者可以写成public mydelegate mydel =null;
	private void button1_Click(object sender, EventArgs e)
	{
		//用委托变量-调用委托方法
		mydel(textBox1.Text.Trim());//子窗体中委托变量的调用会直接调用到主窗体中的showmsg函数
	}
```


exp:

```csharp
delegate void UserDelegate();//声明void  void类型的委托--类外--类内都可以？？这里就在类内呀
event UserDelegate RstEvent, ChangeEvent;//定义此委托类型的变量 RstEvent ChangeEvent--这个也在类内
线程或者定时器
{
	if(成立)
	{
		RstEvent += new UserDelegate(TestReset);//在线程中不可以直接操作控件，通过RstEvent委托主线程去修改控件
		RstEvent?.Invoke();  RstEvent变量不为空肯定可以执行
	}

	if(成立)
	{
		ChangeEvent += new UserDelegate(UpdateConfig);//在线程中不可以直接操作控件，通过委托主线程去修改控件
		ChangeEvent += new UserDelegate(StreamPro);//在线程中不可以直接操作控件，通过委托主线程去修改控件
		ChangeEvent?.Invoke();
	}
}
```

?.Invoke是什么意思：
判断一下这个委托是不是为null；如果是则不执行委托，如果不是则执行该委托---该委托可能同时是多个函数；
如果test1 = fun1;
test1+=fun2;
test1?.Invoke("梨花");  则这里就会执行fun1 再执行fun2。
Hello!梨花
你好!梨花
Ending......
invoke和begininvoke的区别
invoke：在拥有此空间基础窗口句柄的线程上执行指定的委托---或者说是普通数据函数变量
begininvoke：在创建控件的基础句柄所在的线程上异步执行指定的委托---控件变量



### 数据库链接

```c#
//winform  链接sqlite  需要在管理NuGet程序包里面下载data.sqlite包
private void Form1_Load(object sender, EventArgs e)
{
	SQLiteConnectionStringBuilder connectionString = new SQLiteConnectionStringBuilder();//构造连接数据库的字符串
	SQLiteConnection SqlConnection;//连接数据库
	MessageBox.Show(Application.StartupPath);
	connectionString.DataSource = Application.StartupPath + @"\test.db";
	SqlConnection = new SQLiteConnection(connectionString.ToString());
	SqlConnection.Open();

	var sql = "select * from staff_test";

	SQLiteDataAdapter da = new SQLiteDataAdapter(sql, SqlConnection);

	DataSet ds = new DataSet();
	da.Fill(ds);
	DataTable re = ds.Tables[0];//re返回表中所有数据
	//restr string数组将数据库每行数据都存起来
	List<string> restr = new List<string>();
	foreach (DataRow v in re.Rows)//遍历表中得数据存到restr
	{
		string key = v["name"].ToString();
		string age = v["age"].ToString();
		string addr = v["age"].ToString();
		restr.Add(key + "-" + age+"-"+addr);
	}

	//显示到文本框
	foreach (var v in restr)
	{
		richTextBox1.Text += v;
		richTextBox1.Text += "\n";
	}
}
//执行sql语句
var sql = "INSERT INTO staff_test ( name, age, addr) VALUES ( 'insetagsin', 10, 'Beijing');";
SQLiteCommand SqlCmd = new SQLiteCommand(sql, SqlConnection);
int changedRows = SqlCmd.ExecuteNonQuery();
if (changedRows > 0)
{
	MessageBox.Show("ok");
}

```

### 程序中读取配置文件数据

读取配置文件：
1.需要在引用中添加System.Configuration引用，添加之后debug目录会出现-项目名称.exe.config文件
2.using System.Configuration;
3.写读方法读取配置文件中对应的keyname的value
string readvalue(string keyname)//如果配置文件中没有对应的key硬读取会报错
{
	string ret="";
	Configuration cfg = ConfigurationManager.OpenExeConfiguration(path);
	try//用try包含起来即使出错也不会将程序卡死不会运行
	{
		ret = cfg.AppSettings.Settings[keyname].Value;
	}catch (Exception ex)
	{
		MessageBox.Show(ex.Message);//打印程序出错的原因，并不会导致程序退出
	}
	return ret;
}
4.写方法往配置文件中写入key+value--都是string
void writevalue(string keyname, string value)
{
	//获取配置文件句柄，只需要传入它所在的路径即可，内部知道具体的文件名字是什么
	Configuration cfg = ConfigurationManager.OpenExeConfiguration(path);
	foreach (string key in cfg.AppSettings.Settings.AllKeys)
	{
		if (key == keyname)
		{
			cfg.AppSettings.Settings.Remove(key);//如果有现存的key先删除原来的key
		}
	}
	cfg.AppSettings.Settings.Add(keyname, value);//如果没有对应的key，直接增加key+value
	cfg.Save(ConfigurationSaveMode.Modified);
	ConfigurationManager.RefreshSection("AppSettings");
}
private void Form1_Load(object sender, EventArgs e)
{
	path = Application.ExecutablePath;//这个是配置文件所在的路径
}

### vs上推荐安装的软件

IndentRainbow  缩进彩虹
Color Printing  字符------装不上
Viasfora		彩虹大括号
Indent Guides	成对的括号前连结虚线
VS安装扩展缓慢问题解决:
https://blog.csdn.net/x12901/article/details/105529110?spm=1001.2101.3001.6650.4&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7Edefault-4-105529110-blog-121101430.pc_relevant_blogantidownloadv1&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7Edefault-4-105529110-blog-121101430.pc_relevant_blogantidownloadv1&utm_relevant_index=8
先在扩展中点击详情信息在网页上下载vsix文件直接双击安装即可。

### io操作

FileStream fs = new FileStream("test.txt",FileMode.Append);文件句柄，FileMode可以添加,创建,只读打开
StreamWriter sw = new StreamWriter(fs);//写要通过streamwriter
sw.WriteLine("test write\r\n");//WriteLine默认会换行  \r\n也是换行，这样写会中间空一行
sw.WriteLine("test write2\r\n");
sw.Close();//最后要关闭写数据sw
fs.Close();//关闭文件句柄

FileStream fs = new FileStream("test.txt", FileMode.Open);//读用FileMode.Open
StreamReader sr = new StreamReader(fs);
MessageBox.Show(sr.ReadToEnd());

File.WriteAllText("filewiite", "nihai");		另一种写数据的方法
MessageBox.Show(File.ReadAllText("filewiite"));读数据的方法
File.WriteAllText("filewiite", "dasdasdas\r\ndasdasdas\r\n",Encoding.UTF8);以固定编码格式写入
MessageBox.Show(File.ReadAllText("filewiite",Encoding.UTF8));以同样编码格式去读

try {
	Directory.Delete(@"GC");//文件夹不为空不可以直接删除，最好用try包含,若空直接删除
}catch{//若不为空，进入catch
	if (MessageBox.Show("这个文件夹不是空的是否要删除", "提示", MessageBoxButtons.YesNo) == DialogResult.Yes)
	{
		Directory.Delete(@"GC",true);
	}
}

FileInfo fi = new FileInfo("test.txt");必须创建对象才可以调用方法，不是静态方法不能直接用类调用
MessageBox.Show(fi.Length.ToString());字节流

openFileDialog1
saveFileDialog1
folderBrowserDialog1

### 数组转化

ArrayList 转换成byte数组
1.
	//C# ArrayList 转换成byte数组
	ArrayList al = new ArrayList();
	byte[] array = (byte[])al.ToArray(typeof(byte));
2.
	Uart.buffer.CopyTo(0, ReDataBuf, 0, Uart.buffer.Count);

### 其他

可以在视图-->tab键顺序修改每个控件的索引-->然后在事件处理函数中可以获取对应的索引将控件分开处理
int n = (sender as Button).TabIndex;
ShowWarning("当前按钮索引是"+n);

vs软件打开Debug的时候，打开can工程就会报错，多点击几次继续程序才能运行
解决办法：ctrl+alt+e   把Managed Debugging Assistants 选项点击开  去掉"LoaderLock"  重新调试就好了

创建大小为cnt的数组的函数

int [] getElement(int cnt)
{
	//int[] ret=new int[5];  返回值是个不确定大小的数组 用LIst完成（此函数有可能只有小于cnt的个数）
	List`<int>` ret=new List`<int>`(cnt);//这样就好了

```
return ret.ToArray();//不能直接返回ret，需要toArray。List转array
```


```
//集合转数组可以用ToArray  那数组怎么转集合呀
	new List
```

`<int>`(array);----直接可以将数组转化成集合

}

### can升级上位机code分析：

显示界面拖入：openFileDialog类型的控件命名-UpdataFile，可以作为全局var调用

升级前打开加载bin文件：
private void btOpenFile_Click(object sender, EventArgs e)
{
	if (m_bOpen == 0)
	{
		MessageBox.Show("can未连接");
		return;
	}
	//是个控件可以直接托进来的句柄
	UpdataFile.Filter = "bin files(*.bin)|*.bin|All files(*.*)|*.*";
	if (UpdataFile.ShowDialog() == DialogResult.OK)
	{
		try
		{
			tbFilePath.Text = UpdataFile.FileName;//把打开的文件的路径放到textbox显示
		}
		catch (ArgumentException ex)
		{
			MessageBox.Show(ex.ToString());
			tbFilePath.Text = "";
		}
	}
}

点击更新按钮：
private void btUpdata_Click(object sender, EventArgs e)
{
	if (tbFilePath.Text != "")//若刚才打开文件失败 则提示打开文件
	{
		try
		{
    Update.UpdateFileName = UpdataFile.SafeFileName;//获取要升级的文件名，没啥用
			Update.down_fi = new FileInfo(UpdataFile.FileName);//获取当前文件的信息-是个结构体

```
Update.down_file_path = UpdataFile.FileName;//文件的路径加名字
			Update.down_file_size = (int)Update.down_fi.Length;//文件信息中包含文件大小
			//只读方式打开文件 获得句柄
			Update.down_fs = new FileStream(UpdataFile.FileName, FileMode.Open, FileAccess.Read);
			//根据文件句柄获得 二进制文件句柄-用它可以直接读文件 会自动偏移指针
			Update.down_rf = new BinaryReader(Update.down_fs);
```


```
baUpdataProgress.Maximum = (int)(Update.down_file_size / 1024);//设置进度条最大尺寸
			baUpdataProgress.Minimum = 0;
			Update.down_flag = true;//为真才能刷程序
		}
		catch (ArgumentException ex)
		{
			MessageBox.Show(ex.ToString());
		}
	}
	else
	{
		MessageBox.Show("先选择要更新的文件");
	}
 }
```


升级的线程：
unsafe private void Down_Load_Bin_File()
{
	int send_bytes_ones = 0;
	int timer = 0;
	//若can没打开或者未点击升级按钮，则此线程sleep，不占用资源
	if ((m_bOpen == 1) && (Update.down_flag == true))
	{
		byte[] buff = new byte[8];
		UInt32 res = new UInt32();
		res = VCI_GetReceiveNum(m_devtype, m_devind, m_canind);//看底层读了多少can数据
		if (res >= 0)
		{
			UInt32 con_maxlen = 50;
			IntPtr pt = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(VCI_CAN_OBJ)) * (Int32)con_maxlen);

```
res = VCI_Receive(m_devtype, m_devind, m_canind, pt, con_maxlen, 100);
			for (UInt32 i = 0; i < res; i++)
			{
				VCI_CAN_OBJ obj = (VCI_CAN_OBJ)Marshal.PtrToStructure((IntPtr)((UInt32)pt + i * Marshal.SizeOf(typeof(VCI_CAN_OBJ))), typeof(VCI_CAN_OBJ));
```


```
if (obj.ID == 0x120)
				{
					if (obj.Data[0] == Update.down_sum)//看下位机发来的上一帧数据的校验是否正确
					{
						Update.down_reply_flag = 1;//下位机回复 flag置1
						break;
					}
				}
			}
		}
```


```
//在每个步骤都是上位机先发送数据  更新sum校验
		//从机返回收到数据的sum校验 pc进入下个步骤发数据
```


```
//步骤1：发下位机ID
		if (Update.down_step == 0)
		{
			if (Update.down_reply_flag == 0)
			{
				Update.down_sum = down_cmd();//发下位机ID
			}
			else
			{
				Update.down_reply_flag = 0;
				Update.down_step = 1;	//收到下位机回复 进入步骤2
				FrmUpdate_SetBt(false);//更新按钮不可点击
			}
		}
```


```
//步骤2：发bin大小 + ID
		if (Update.down_step == 1)
		{
```


```
if (Update.down_reply_flag == 0)
			{
```


```
timer++;
				if ((timer % 20 == 1))//若收不到 回复 延时20次再发bin大小
				{
					Update.down_sum = send_size();
				}
			}
			else
			{
				timer = 0;
				Update.down_step = 2;//收到回复 进入步骤3  准备步骤3要发的数据  bin的第一包
				Update.down_reply_flag = 0;
				buff = Update.down_rf.ReadBytes(8);
				Update.down_send_bytes += 8;//更新已发送的can数据 数量
				send_bytes_ones = 8;
			}
		}
		//步骤3：发bin 数据 一直发完
		if (Update.down_step == 2)
		{
			if (Update.down_reply_flag == 0)
			{
				timer++;
				if ((timer % 20 == 1))//
					Update.down_sum = send_file(buff, send_bytes_ones);
			}
			else
			{
				timer = 0;
				//若待发的bin超过8字节
				if ((Update.down_file_size - Update.down_send_bytes) >= 8)
				{
					//再读8字节
					buff = Update.down_rf.ReadBytes(8);
					//已发送字节+8
					Update.down_send_bytes += 8;
					send_bytes_ones = 8;
				}
				//bin发完了 已发送字节=总bin大小 进入下一步升级step
				else if (Update.down_file_size == Update.down_send_bytes)
				{
					Update.down_step = 3;  //进入步骤4
					Update.down_reply_flag = 0;
				}
				else//还没发完，最后一包数量<8字节
				{
					//缺几个字节没发就读几个字节
					buff = Update.down_rf.ReadBytes((Update.down_file_size - Update.down_send_bytes));
					send_bytes_ones = Update.down_file_size - Update.down_send_bytes;
					Update.down_send_bytes = Update.down_file_size; //更新已发送字节
				}
				if (Update.down_step != 3)//没到下一阶段，则直接发送bin下去--能进else分支说明下位机有回复，直接发送下一帧数据即可
				{
					Update.down_sum = send_file(buff, send_bytes_ones);
				}
				//更新进度
				FrmUpdate_SetProgress((int)(Update.down_file_size / 1024), (int)(Update.down_send_bytes / 1024), Update.down_send_bytes.ToString() + '/' + Update.down_file_size.ToString());
			}
		}
		if (Update.down_step == 3)//到了最后阶段
		{
			if (Update.down_reply_flag == 0)
			{
				Update.down_sum = send_over();//发结束帧
				Update.down_reply_flag = 0;
			}
			else
			{
				Update.down_flag = false;
				Update.down_reply_flag = 0;
				Update.down_send_bytes = 0;
				Update.down_step = 0;
				Update.down_rf.Close();
				MessageBox.Show("下载完成");
				FrmUpdate_SetBt(true);//升级按钮可点击
			}
		}
```


```
}
	else//不需要升级则此线程休眠 不占内存
	{
		Thread.Sleep(3);
	}
}
```


can升级上位机code分析：
显示界面拖入：openFileDialog类型的控件命名-UpdataFile，可以作为全局var调用

升级前打开加载bin文件：
private void btOpenFile_Click(object sender, EventArgs e)
{
	if (m_bOpen == 0)
	{
		MessageBox.Show("can未连接");
		return;
	}
	//是个控件可以直接托进来的句柄
	UpdataFile.Filter = "bin files(*.bin)|*.bin|All files(*.*)|*.*";
	if (UpdataFile.ShowDialog() == DialogResult.OK)
	{
		try
		{
			tbFilePath.Text = UpdataFile.FileName;//把打开的文件的路径放到textbox显示
		}
		catch (ArgumentException ex)
		{
			MessageBox.Show(ex.ToString());
			tbFilePath.Text = "";
		}
	}
}

点击更新按钮：
private void btUpdata_Click(object sender, EventArgs e)
{
	if (tbFilePath.Text != "")//若刚才打开文件失败 则提示打开文件
	{
		try
		{
    Update.UpdateFileName = UpdataFile.SafeFileName;//获取要升级的文件名，没啥用
			Update.down_fi = new FileInfo(UpdataFile.FileName);//获取当前文件的信息-是个结构体

```
Update.down_file_path = UpdataFile.FileName;//文件的路径加名字
			Update.down_file_size = (int)Update.down_fi.Length;//文件信息中包含文件大小
			//只读方式打开文件 获得句柄
			Update.down_fs = new FileStream(UpdataFile.FileName, FileMode.Open, FileAccess.Read);
			//根据文件句柄获得 二进制文件句柄-用它可以直接读文件 会自动偏移指针
			Update.down_rf = new BinaryReader(Update.down_fs);
```


```
baUpdataProgress.Maximum = (int)(Update.down_file_size / 1024);//设置进度条最大尺寸
			baUpdataProgress.Minimum = 0;
			Update.down_flag = true;//为真才能刷程序
		}
		catch (ArgumentException ex)
		{
			MessageBox.Show(ex.ToString());
		}
	}
	else
	{
		MessageBox.Show("先选择要更新的文件");
	}
 }
```


升级的线程：
unsafe private void Down_Load_Bin_File()
{
	int send_bytes_ones = 0;
	int timer = 0;
	//若can没打开或者未点击升级按钮，则此线程sleep，不占用资源
	if ((m_bOpen == 1) && (Update.down_flag == true))
	{
		byte[] buff = new byte[8];
		UInt32 res = new UInt32();
		res = VCI_GetReceiveNum(m_devtype, m_devind, m_canind);//看底层读了多少can数据
		if (res >= 0)
		{
			UInt32 con_maxlen = 50;
			IntPtr pt = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(VCI_CAN_OBJ)) * (Int32)con_maxlen);

```
res = VCI_Receive(m_devtype, m_devind, m_canind, pt, con_maxlen, 100);
			for (UInt32 i = 0; i < res; i++)
			{
				VCI_CAN_OBJ obj = (VCI_CAN_OBJ)Marshal.PtrToStructure((IntPtr)((UInt32)pt + i * Marshal.SizeOf(typeof(VCI_CAN_OBJ))), typeof(VCI_CAN_OBJ));
```


```
if (obj.ID == 0x120)
				{
					if (obj.Data[0] == Update.down_sum)//看下位机发来的上一帧数据的校验是否正确
					{
						Update.down_reply_flag = 1;//下位机回复 flag置1
						break;
					}
				}
			}
		}
```


```
//在每个步骤都是上位机先发送数据  更新sum校验
		//从机返回收到数据的sum校验 pc进入下个步骤发数据
```


```
//步骤1：发下位机ID
		if (Update.down_step == 0)
		{
			if (Update.down_reply_flag == 0)
			{
				Update.down_sum = down_cmd();//发下位机ID
			}
			else
			{
				Update.down_reply_flag = 0;
				Update.down_step = 1;	//收到下位机回复 进入步骤2
				FrmUpdate_SetBt(false);//更新按钮不可点击
			}
		}
```


```
//步骤2：发bin大小 + ID
		if (Update.down_step == 1)
		{
```


```
if (Update.down_reply_flag == 0)
			{
```


```
timer++;
				if ((timer % 20 == 1))//若收不到 回复 延时20次再发bin大小
				{
					Update.down_sum = send_size();
				}
			}
			else
			{
				timer = 0;
				Update.down_step = 2;//收到回复 进入步骤3  准备步骤3要发的数据  bin的第一包
				Update.down_reply_flag = 0;
				buff = Update.down_rf.ReadBytes(8);
				Update.down_send_bytes += 8;//更新已发送的can数据 数量
				send_bytes_ones = 8;
			}
		}
		//步骤3：发bin 数据 一直发完
		if (Update.down_step == 2)
		{
			if (Update.down_reply_flag == 0)
			{
				timer++;
				if ((timer % 20 == 1))//
					Update.down_sum = send_file(buff, send_bytes_ones);
			}
			else
			{
				timer = 0;
				//若待发的bin超过8字节
				if ((Update.down_file_size - Update.down_send_bytes) >= 8)
				{
					//再读8字节
					buff = Update.down_rf.ReadBytes(8);
					//已发送字节+8
					Update.down_send_bytes += 8;
					send_bytes_ones = 8;
				}
				//bin发完了 已发送字节=总bin大小 进入下一步升级step
				else if (Update.down_file_size == Update.down_send_bytes)
				{
					Update.down_step = 3;  //进入步骤4
					Update.down_reply_flag = 0;
				}
				else//还没发完，最后一包数量<8字节
				{
					//缺几个字节没发就读几个字节
					buff = Update.down_rf.ReadBytes((Update.down_file_size - Update.down_send_bytes));
					send_bytes_ones = Update.down_file_size - Update.down_send_bytes;
					Update.down_send_bytes = Update.down_file_size; //更新已发送字节
				}
				if (Update.down_step != 3)//没到下一阶段，则直接发送bin下去--能进else分支说明下位机有回复，直接发送下一帧数据即可
				{
					Update.down_sum = send_file(buff, send_bytes_ones);
				}
				//更新进度
				FrmUpdate_SetProgress((int)(Update.down_file_size / 1024), (int)(Update.down_send_bytes / 1024), Update.down_send_bytes.ToString() + '/' + Update.down_file_size.ToString());
			}
		}
		if (Update.down_step == 3)//到了最后阶段
		{
			if (Update.down_reply_flag == 0)
			{
				Update.down_sum = send_over();//发结束帧
				Update.down_reply_flag = 0;
			}
			else
			{
				Update.down_flag = false;
				Update.down_reply_flag = 0;
				Update.down_send_bytes = 0;
				Update.down_step = 0;
				Update.down_rf.Close();
				MessageBox.Show("下载完成");
				FrmUpdate_SetBt(true);//升级按钮可点击
			}
		}
```


```
}
	else//不需要升级则此线程休眠 不占内存
	{
		Thread.Sleep(3);
	}
}
```

## 控件

### pictureBox
创建对应的对象x
x.Image = Image.FromFile() 文件路径 ，则对应的图片会自动显示到picturebox中

Directory.GetFiles(文件加路径);获取文件加中所有文件的全路径名



### TextBox
TextBox： 单行  多行  输入信息或者获取信息
MultiLine：是否多行
WordWarp：是否可以自动换行
PasswdChar：输入是否以*显示--作密码时使用
方法：AppendText  clear  focus（获取焦点）  Select  Selectall

事件：TextChanged

### button
backGroudImage  Image  类似的，都不会覆盖显示的数据
BackColor 按钮背景色   ForeCloro：文字颜色

判断字符串是否为空：
string.IsNullOrEmpty(mystr);
				内容		标题		确定				内容前的icon
MessageBox.Show("请输入xx","登录页面",MessageBoxButton.OK,MessageBoxIcon.Error);

void radioButton1_CheckedChanged(object sender, EventArgs e)
{
	RadioButton rd = (RadioButton)sender;//获取发出此信号的对象，强转成某个类型
	if (rd.Checked)//调用对象的方法
	{
		MessageBox.Show("rd1 is check");
	}
}

### listbox
items：列表中项的属性
MultiColumn：是否可以多列显示
DataSource：列表数据来源
DisplayMember
ValueMember：实际值
事件：SelectIndexChanged
方法
listBox1.Items.Clear();
listBox1.Items.Add(12);
listBox1.Items.Add("json");
string[] list = { "123", "abc", "test" };
listBox1.Items.AddRange(list);//直接增加一个string列表的内容
listBox1.Items.Insert(index, "jialei");//在index位置插入
int index = listBox1.Items.IndexOf("json");//获取数据为json 字串的index
listBox1.Items.Remove("test");//移除内容为test的项
listBox1.Items.RemoveAt(5);//移除index为5的项
bool bl=listBox1.Items.Contains("string");//判断listBox1是否包含string

为避免listbox更新数据的时候闪烁
listBox1.BeginUpdate();
//增加大量数据
listBox1.EndUpdate();

需要引入新的类文件UserInfo.cs：  mylist里面放的是类型为UserInfo类的多个对象
List<UserInfo> mylist = new List<UserInfo>();
mylist.Add(
		new UserInfo(){  //不需要定义对象，直接新建匿名对象当入参
			id = 1,
			name = "admin"
		}
	);
listBox1.DataSource = mylist;//显示的项的来源
listBox1.DisplayMember = "name";//项显示的文本对应的属性名
listBox1.ValueMember = "id";	//项的实际值对应的属性名
如果项的实际值对应的属性是id，则当光标在admin时，listBox1.SelectedValue就是1
如果项的实际值对应的属性是name，则当光标在admin时，listBox1.SelectedValue就是”admin“

### 单选框  多选框
checkBox   多选框控件
radiobutton    单选控件
GroupbOX   容器，将多个单选控件放进去，则只能单选了.常用属性：visible:设置为true则分组框中的所有对象都一起被隐藏
panel：类似groupbox，区分控件组
checkedlistbox:带复选框的列表框控件
combox：

### 表格 DataGridView

常用事件

CellClick  			单击单元格
CellContentClick	单击单元格中的内容才能触发
CellValueChanged 	失去焦点时才触发--不好
CurrentCellDirtStateChanged	单元格状态因其内容更改时发生
private void DgvUsers_CellContentClick(object sender,DataGridViewCellEventArgs e)
{
	//定位发生内容改变的单元格
	DataGridView cell = DgvUsers.Rows[e.RowIndex].Cells[e.ColumnIndex];
	if(cell.FormattedValue.Tostring()=="删除")
	{}else
	{}
}

往DataGridView插入列
DataGridViewTextBoxColumn coll = new DataGridViewTextBoxColumn();
coll.Name = "colCHOSE";
coll.HeaderText = "选择";
dataGridView2.Columns.Add(coll);

DataGridViewTextBoxColumn coll2 = new DataGridViewTextBoxColumn();
coll2.Name = "colID";
coll2.HeaderText = "编号";
dataGridView2.Columns.Add(coll2);

DataGridViewTextBoxColumn coll3 = new DataGridViewTextBoxColumn();
coll3.Name = "colNAME";
coll3.HeaderText = "姓名";
dataGridView2.Columns.Add(coll3);

DataGridViewTextBoxColumn coll4 = new DataGridViewTextBoxColumn();
coll4.Name = "colDEL";
coll4.HeaderText = "删除";
dataGridView2.Columns.Add(coll4);

//往DataGridView插入行，需要先将行里面需要写的数据都填完，最后将行DataGridViewRow ADD到表格中
DataGridViewRow dr = new DataGridViewRow();//新建行
DataGridViewCheckBoxCell cell1 = new DataGridViewCheckBoxCell();//第一个单元格放的是checkbox
cell1.Value = true;
DataGridViewTextBoxCell cell2 = new DataGridViewTextBoxCell();//第2个单元格放的是文本
cell2.Value = "15";
DataGridViewTextBoxCell cell3 = new DataGridViewTextBoxCell();
cell3.Value = "admin";

DataGridViewLinkCell cell4 = new DataGridViewLinkCell();//第4个单元格放的是链接文本
cell4.Value = "删除";
dr.Cells.Add(cell1);//将4个单元格都加入DataGridViewRow dr中
dr.Cells.Add(cell2);
dr.Cells.Add(cell3);
dr.Cells.Add(cell4);
//将dr add到单元格的行中
dataGridView2.Rows.Add(dr);

### RichTextBox

插入图片：
此控件不是图片容器，不能直接放图片，需要通过剪切板
Clipboard.Clear();//清除剪切板中其他内容
Image bmp = global::test.Properties.Resources.仓储仓库;
Clipboard.SetImage(bmp);//将图片加载到剪切板中
richTextBox1.Paste();//将剪切板内容粘贴到富文本框
richTextBox1。SaveFile(@"d\xxx.rtf");//保存文件的时候保存成rtf格式

### combobox的操作

 与LIstBox一样存在问题：若指定了Datasource，就不能直接更改项

的集合，需要把项的集合的修改转化成对数据源的修改上
exp：
在coblist控件，增加数据源为
coblist.DataSource = mylist;//显示的项的来源
coblist.DisplayMember = "name";//项显示的文本对应的属性名
coblist.ValueMember = "id";	//项的实际值对应的属性名
所以，数据项不多的情况下，又要修改项的集合，用Item.Add添加项
事件：SelectIndexChanged
获取combobox对象中选中的item 类型转化成每个item的数据类型，再.name
(coblist.SelectedItem as UserInfo).name

### picturebox

Image:需要将要设置到控件的图片放到本项目的res里面
ImageLocation：通过本地图片所在的绝对路径来加载图片

### ProgressBar进度条

progressBar1.Maximum = max;最大值
progressBar1.Step = 2;
progressBar1.Value = 0;当前值
progressBar1.PerformStep();根据step往后step跳
一般在定时器中去step增加进度条的数值

### Timer

```csharp
1.System.Windows.Forms.Timer : 
	独占一个线程，即如果主线程使用Timer，Timer会阻塞主线程
	默认Enabled是不启用，在代码中去启用
	如果单词任务执行超过时间间隔，则会影响下次触发
2.Syetem.Timer.Timer  基于服务用的Timer---无法拖拽使用，得用代码创建
	时间间隔，触发Elapsed事件，
	System.Timers.Timer t = new System.Timers.Timer(1000);//实例化Timer类，设置间隔时间为10000毫秒； 
	//下面添加回调函数
	方法1：t.Elapsed += new System.Timers.ElapsedEventHandler(theout);//到达时间的时候执行theout事件； 
	方法2:t.Elapsed += theout;
	t.AutoReset = true;//设置是执行一次（false）还是一直执行(true)； 
	t.Enabled = true;//是否执行System.Timers.Timer.Elapsed事件； 

	public void theout(object source, System.Timers.ElapsedEventArgs e)
	{
		#####label2.Text = DateTime.Now.ToString();//不可以直接修改UI控件
		怎么处理呢？可以在委托中修改UI元素
	}

	public void theout(object source, System.Timers.ElapsedEventArgs e)
	{
		Action act = chgtime;//Action act就是委托
		this.Invoke(act, e);
	}
	private void chgtime()
	{
		label2.Text = DateTime.Now.ToString();
	}
3.Syetem.Threading.Timer	基于线程的定时器
单词运行时间过长并不会影响下次的触发，回调方法执行
System.Threading.Timer timer4 = new System.Threading.Timer(
	new System.Threading.TimerCallback(
	o => {	count += 2;
			Action act = chgtime;//将修改控件数据放到委托函数内，这个委托不传参数
			this.Invoke(act, e);}),
			null,100,1000);
 timer4 = new System.Threading.Timer(new System.Threading.TimerCallback(
                 o=> {
                    count += 2;
                    Action<int> act = chgtime;//<内是入参类型>
                    this.Invoke(act, count);//第二个参数是入参
                }),null,100,1000);
private void chgtime(int count)//委托函数传参数
{
	txtcount.Text = count.ToString();
}
```

### ImageList 图片集合

```csharp
与Timer一样是后台控件，不可见的控件
if (Directory.Exists(path))//判断路径是否存在
{ 
	string[] files = Directory.GetFiles(path);//获取所有的路径string
	if (files.Length > 0)//文件夹中不止一个文件
	{
		string[] filetype = { ".jpg",".png"};//类型数组
		foreach (string file in files)//遍历所有的文件夹中的文件
		{
			//如果遍历的当前文件是jpg或者png类型的文件，GetExtension获取文件类型
			if (filetype.Contains(Path.GetExtension(file)))
			{ 
				Image img = Image.FromFile(file);//定义img变量，就是图片
				//获取不带扩展名的文件名
				string keyName = Path.GetFileNameWithoutExtension(file);
				imageList1.Images.Add(keyName,img);//增加图片到ImageList的方法
			}
		}
		//怎么显示list中的图片呢？
		label1.ImageKey = "xxx";
		label1.ImageIndex = 2;
	}
}
Add(Image/Icon)  	直接增加Image
Add(keystring,Image/Icon) 键值对增加 
Contains(Image/Icon)  判断是否包含某图片
INdexOf(Image/Icon)	  获取某图片的索引
		Remove(Image)   根据图片删除
		RemoveAt(int)	根据下标删除
		RemoveByKey(string) 	根据key删除
SetKeyName(int,string)  根据索引设置key名字

```

### invoke

```csharp
this.Invoke(
	new Action(() =>		//不含入参的invoke
	{
		label1.Text = "当前在" + "0页面";
	})
);

//含入参的invoke
this.Invoke(new Action<int>((rucan) =>
{
	label1.Text = "当前在" + rucan + "页面";
}),index);
其中index是传进来给匿名函数的实际的入参，下面这团是Invoke的第一个参数，直接写的匿名函数
new Action<int>((rucan) =>				//int是入参类型，rucan是函数的虚入参
{
	label1.Text = "当前在" + rucan + "页面";
})
    
invoke与begininvoke
在多线程编程中，我们经常要在工作线程中去更新界面显示，而在多线程中直接调用界面控件的方法是错误的做法，
Invoke 和 BeginInvoke 就是为了解决这个问题而出现的，使你在多线程中安全的更新界面显示。
正确的做法是将工作线程中涉及更新界面的代码封装为一个方法，通过 Invoke 或者 BeginInvoke 去调用，
两者的区别就是一个导致工作线程等待，而另外一个则不会。
使用方法：
//定义一个委托 ，像是一个数据类型一样的东西。是函数的数据类型：入参是string返回值是void的函数类型
public delegate void MyInvoke(string str); delegate就是委托
//更新界面的方法
private void UpdateTextBox(string str);
{
this.TextBox1.Text=str; //更新
}
//启动一个线程
Thread thread=new Thread(new ThreadStart(DoWork));
thread.Start();
private void DoWork() //线程函数中不直接操作控件，而是通过委托变量mi去操作
{
//比如将界面的TextBox内容设置一下
MyInvoke mi=new MyInvoke(UpdateTextBox); //定义此委托变量mi的时候需要传进去一个函数（此函数就是入参string，返回值void）
this.BeginInvoke(mi,new object[]{"我是一个文本框"}); //需要传入委托变量与入参，入参是new出来的string。
}
Thread thread=new Thread(
	()=>{			//=>   goseto   ()内是入参
		dosth();
	}
);
```


## 其他

枚举类型与int类型是相互兼容的，可以直接转换，
Enum.parse();  Enum.TryParse(); 类型转换 将str转换成enum类型
(要转换的枚举类型)Enum.parse(typeof(要转换的枚举类型)，”要转换的字符串“);
枚举：  声明在命名空间之内，class之外，以供此命名空间内的其他方法调用
[public] enum ENUM_NAME
{
	值1，
	值2，
	。。。
	值n
}
使用 ENUM_NAME var=ENUM_NAME.值1;定义变量并赋值，枚举类型的值是通过枚举名点出来的。
所有类型的数据都可以.ToString();转换成string

结构：
[public可省略] struct STRUCT_NAME
{
	public 类型 mem1;  如果前面不加public，则这些成员外部无法访问
	public	类型 mem2
}

#### 捕获错误：
try	{convert.toint32(xx)}
catch (Exception ex){提示输入错误}
如果try里面的程序能成功执行则不会跳到catch，如果try里面不能成功执行则会跳到catch

#### 获取当前sys时间：
Datetime.Now 格式是时间的格式，需要转换成字符串 Tostring()；
Datetime.Now.Hour()  获取小时
Datetime.Now.Second()  获取秒

#### 构造函数可以重载。
类当中有一个默认的无参数的构造函数，当写一个新的构造函数后，不管有没有，之前的默认的无参数的构造函数都会被干掉。

#### 常用函数

Path.getfilename(); 获取路径下的文件名

获取字符串中某个字符最后一次出现的索引：获取路径最后一个\后的名称
int index = str.lastindexof("\\");

str=str.substring(index+1); 获取str中的子串

静态类如果我们想使用类里面的方法可以直接通过 Class.way();去调用
普通的类可以必须一个对象，通过对象才能调用方法。

FILE
-创建文件
FILE.create(@"c:\a\b\c.txt");
-删除
FILE.delete(@"c:\a\b\c.txt");
-复制
FILE.copy(@"c:\a\b\old.txt",@"c:\a\b\new.txt");

静态类里面的字段全局可以访问。

SoundPlayer 创建对象sp
sp.SoundLocation=@"wav格式的音乐路径";
sp.Play()  播放音乐



获得焦点
label.Focus();让文本框获得焦点

自动换行
btnword.WordWrap = true/false;  设置文本框中的内容是否自动换行
MessageBox.Show(str);  打印提示

int.Parse(string):将str转换为int


直接下载网页为html格式，下载后会附带当前网页的所有图片出来，在一个文件夹中。

阿里巴巴ICON下载，可以挑选图片的背景颜色。下载png格式即可

直接改FORM1的backgroundImage,可以设置图片进去

OpenFileDialog ofd = new OpenFileDialog()：点击按钮之后弹出选择文件的对话框
ofd.Title="请选择文件";设置此dialog的标题
ofd.InitialDiretory = @"路径"; 设置初始化路径
ofd.Multiselect=true;  设置可以多选，一次性选择多个文件
ofd.ShowDialog();  显示窗口
ofd.Filter=""
string[] path = ofd.Filenames  获取最后选择的全部文件的全路径
ofd.FileName			带路径的文件名
SafeFileName			纯粹的文件名
FileInfo fi = new  FileInfo(openFileDialog1.FileName);获取文件的多种信息
int down_file_size = (int)fi.Length;//文件信息中包含文件大小 精确到字节

//只读方式打开文件 获得文件句柄
Update.down_fs = new FileStream(UpdataFile.FileName, FileMode.Open, FileAccess.Read);
//根据文件句柄获得 二进制文件句柄
Update.down_rf = new BinaryReader(Update.down_fs);

buff = Update.down_rf.ReadBytes(len);可以利用句柄从文件中读对应的长度的数据 出来

Path.GetFileName(path[i]);根据全路径获取文件名

listbox.Items.Add(string);  listbox增加新项，
listbox.SelectedIndex;  获取选中的条目的索引
listbox.Items.Count  listbox中的项个数
随机数产生
Random r = new Random();  产生随机数
str = r.Next(0,10).tostring();产生0-10的随机数转换成string

#### 进程线程
--通过进程打开应用程序
Process.getprocess
Process.Start("notepad");  打开记事本
Process.Start("iexplore"，”https://xx“);  打开浏览器
----通过进程打开指定的文件
Process p = new Process(); create new process
ProcessStartInfo psi = new ProcessStartInfo("文件路径");
p.StartInfo = psi;
p.start();
---线程
Thead th = new Thread(Test);  Test是个函数，新建线程，执行的是Test，这里的Test入参在下面传
th.IsbackGround = true;  设置线程后台运行
th.start(arg：给Test传参数在这里传)   开启线程

Control.CheckForIlegalCrossThreadCalls = false；取消跨线程对象检查

直接写个线程处理事情：----线程报错不会报出来，需要try catch捕获
Task.Run(()=>
	while(True)
	{
		dosth;
	}
);

Thread.Sleep()会阻塞线程，Task.Delay()不会。
不知道为什么显示异常的时候把代码try  catch(Exception ex) 起来,再去调试的时候异常就会爆出来

#### 关键字
new
创建对象。
关键字new还可以用来隐藏从父类继承的同名函数：在子类的函数定义时需要 public new void t(){} 加new
---访问修饰符修饰类内成员
public:
private:私有的，只能在当前类内部访问。类中的成员如果不加访问修饰符默认都是private
protected:可以类内访问，继承的子类中也可以访问
internal:当前项目中可以访问----public所有项目都可以访问
protected internal:
---访问修饰符修饰类
public
internal

public partial class Uart  在另一个文件也可以定义相同的类
public sealed class Teacher:Person{}  可以继承于别的类，但是不能被别的类继承

选中图片或文字，直接按住ctrl挪动鼠标，就可以复制，不需要ctrl+c+v

#### 不可见控件
timer

serialport 控件如果放到页面1，则页面2无法访问，最好将页面1中的控件赋值给静态类中，更好的做法是声明变量，则在页面2的cs文件中可以直接访问

textbox.appendtext(str) == textbox.text += str;
serialport.readexisting();以字符串方式读

byte data = (byte)serialport.readbyte();  以int方式读，需要类型转换成byte才能给byte赋值
str= convert.tostring(data,16).toupper();  转换成byte才能继续转换成str，不能直接int转换成str
因为textbox里面放的是string

发生le.exe退出，需要将当前文件夹中的licence文件删除再去运行，不然licence的存在有问题。

疑问：C#中界面一中的uart与界面2中的uart之前的关系是什么，为啥有2个界面都创建了port_DataReceivedds中断函数
界面1中的中断函数只处理了crc校验，与vol的显示
界面2中的uart中断中进行了复杂的uart协议的处理
界面3中的uart负责保护值的显示与设置
当前是界面x，则其他的uart就是死的，只有界面x中的uart是处于链接状态的，当前界面的uart才可以进行uart的通信呢

form1.cs中的117行crc校验，后面的数据不是应该-4的<<，-3的不左移吗？为啥啊

(Uart.RcvBuf.ReDataBuf[Uart.RcvBuf.ReDataBuf.Length - 3] << 8 | Uart.RcvBuf.ReDataBuf[Uart.RcvBuf.ReDataBuf.Length - 4]).ToString("X"))

byte[] firmWareBuf = new byte[] { };数组具体有多长，看你放进去多少数据，他是会变化的。

C#:
ctrl+k+d   自动对齐，注意k弹起来再按d ctrl是一直按下的
ctrl+k+c   选中注释，先k再c
#region (注释)+ #endregion  将需要折叠的代码放到region中间，然后点击小-，即可折叠。

数据类型：
字符串 string var="nihao 你好";  可以为空
字符   char  c = 'x'; 不能为空
int  整数
double  小数
float   小数
decimal  金钱（小数）   decimal money=100m;   最后面有个m
String  与 string  一样的都能定义数据
String：.net平台的类  string：c#的关键字

符号：

+ ：连接字符串   当+两边有一边是字符串，则+的作用就是连接
  +：两边都是数字  则+作用是连接
  占位符： {0} {1} {n}
  Console.WriteLine("第一个数字是{0},第二个数字是{1}",n1,n2); 先挖坑，再填坑
  控制台输入的数据只能用string去存储,不能说输入的是数字就用int去存
  string  var = Console.ReadLine();
  怎么截取控制台输入的多个数据？不会的时候就分3次从控制台读取
  在表示路径的字符串前加个@，则路径只需要一个\即可。ps：
  string path=@"F:\a\b\c\d\e\1.txt"  如果没有@,则需要
  string path="F:\\A\\B\\C\\D\\1.TXT"
  @：取消\在”“中的转义作用
  @：将”“中的字符串按照原格式输出

除了类型兼容的类型转换：double--int
类型不兼容的类型强制转换用convert。
Convert.toDouble(var);  转换工厂
枚举类型与int类型是相互兼容的，可以直接转换，
Enum.parse();  Enum.TryParse(); 类型转换 将str转换成enum类型
(要转换的枚举类型)Enum.parse(typeof(要转换的枚举类型)，”要转换的字符串“);
枚举：  声明在命名空间之内，class之外，以供此命名空间内的其他方法调用
[public] enum ENUM_NAME
{
	值1，
	值2，
	。。。
	值n
}
使用 ENUM_NAME var=ENUM_NAME.值1;定义变量并赋值，枚举类型的值是通过枚举名点出来的。
所有类型的数据都可以.ToString();转换成string

结构：
[public可省略] struct STRUCT_NAME
{
	public 类型 mem1;  如果前面不加public，则这些成员外部无法访问
	public	类型 mem2
}

以下两者都类似C语言传地址区别就是
out:要求在函数内部必须给传出参数x赋值，在函数外部x可以没值。
ref：要求在函数外部必须给参数x赋值，函数内部可以修改也可以不修改

params:函数传参数要求是数组,加上params参数后可以直接传数组中每个数据
注意这个params可变参数必须是形参列表中的最后一个参数;一个形参列表中最多只能出现一个可变参数。
int []s = {1,2,3};
void test(params int[] score)
调用：test(1,2,3);  直接传数组中各个参数即可。

求数组长度：arr.Length() 内置方法

重载：函数名称相同，参数不同。
参数不同分为：如果参数个数相同，则类型不能相同
如果参数类型相同则个数不能相同。
方法的重载跟返回值没关系---

命名空间：可以理解为类的文件夹，命名空间有很多类
怎么在项目2中用项目一里面的类：
在项目2的开始:using 项目一的名称，则项目1里面的类在项目2都能用了
-----------------------------泛型集合
List:
List<int> l = new List<int>();
l.removeat(0);删除0上面的元素后，再次访问l的时候0位置就是删除之前1位置的数据

ArrayList:
ArrayList list=new ArrayList();更优越的数组
（长度可变，存放类型不单一）
Add(任意类型);Add(new int[]{1,2,3});直接添加数组
ArrayList.Count();相当于数组中的length
AddRange(集合); 添加集合
clear();清空
remove(x);传入谁就删除谁
removeat(0);删除下标是0的元素。
reverse();反转
sort();排序--类型差不多才能比较，要不然不能比较
removerange(0,4);删除0-4的元素
insert(1,"元素"); 在下标前插入。
insertrange(0,new string[] {"xx","yy"}); 插入集合
contains(1); 判断集合是否包含1这个数据

---

HashTable:键值对集合（键是唯一的）
增：
add(键，值);键与值都可以是任意object类型
th[x] = y;也是添加数据的方法,如果原key存在直接覆盖对应的值
包含：
th.contains() == th.containskey()
删：
clear();清空
remove(key);根据key删

键值对集合怎么遍历：
foreach(var item in ht.Keys()){Console.WriteLine(ht[item])}
--------------var
var.GetType();获取var的数据类型--c#每个数据有自己的类型，强类型语言。

var关键字，可以定义任意类型数据，可以根据值自己推断类型。
var n=12,"str",3.24,111m
var缺点，声明变量的时候必须初始化。
-------------------stopwatch
stopwatch st = new Stopwatch();
st.start();(运行代码) st.stop();  运行结束打印st.Elapsed可以查到耗时时长。

(int[])x; 将x转化为数组类型
int[] n = new int[100];新建长度为100的数组。
if(x is Persion) (i is int[]) 判断x是什么类型，Persion类，数组
int[] n = new int[]{};  新建长度不固定的数组，数组的实际长度根据你添加进去的值去变化。

-------注意事项
头文件只能声明，不能直接定义。ps:头文件中只能extern int x;   .c文件中int x=10;

一、字符串转换为字符数组
char[] tempChar = sourceString.ToCharArray();

二、字符数组转换为字符串
//方法一
string str = string.Join("", tempChar);

//方法二
string str = string.Concat<char>(tempChar);

//方法三
string str = new string(tempChar);

c# string转int
Convert.ToInt16(st)；
int.Parse(st)
ushrot.Parse(st)

foreach (var item in data); 可以直接遍历二维数组，会默认一行一行的去遍历

--------------imagelist与picturebox的关系：
imagelist是存放图片的列表属于后台控件
picture是显示图片的box，前台控件，我们可以在picturebox中去显示图片
常用：picturebox.Image = imagelist.Image[x];通过给x赋值不同让box中显示不同的图片

string path=Application.StartupPath;  这个路径对应某个目录

-------------关闭窗口提示
private void Form1_FormClosing(object sender, FormClosingEventArgs e)
{
	//DialogResult接收dialog的选择结果  			标题	是否两个按钮			是否关闭前面显示warning图标
    DialogResult dr = MessageBox.Show("是否关闭", "t提示", MessageBoxButtons.YesNo, MessageBoxIcon.Warning);
    if (dr == DialogResult.Yes)
    {
        e.Cancel = false;//FormClosingEventArgs.cancel窗口的关闭事件=false就是不关闭
    }
    else
    {
        e.Cancel=true;//=true就是关闭
    }

----------------listview单位是ITEM
listView1.Items.Add(textBox1.Text.Trim());往listview中增加项
listView1.Items.RemoveAt(listView1.SelectedItems[0].Index);//删除listview中的选中项

---------------treeview单位是NODE

设置窗体图标：左上角的ICON  需要放置.ico文件到属性中的icon里面即可。

字符串转char数组，char数组转字符串
字符串转换成Char数组
string str="abcdefghijklmnopqretuvwxyz";
char[] chars=str.ToCharArray();

char数组转换成字符串
char[] chars=new char[]{'a','b','c','d'};
string str=new string(chars);

string转byte[]:
byte[] byteArray = System.Text.Encoding.Default.GetBytes ( str );
byte[]转string：
string str = System.Text.Encoding.Default.GetString ( byteArray );
string转ASCII byte[]:
byte[] byteArray = System.Text.Encoding.ASCII.GetBytes ( str );
ASCII byte[]转string:
string str = System.Text.Encoding.ASCII.GetString ( byteArray );

调试：逐过程-不进入函数内部
跳出：不执行函数内部的剩余代码，直接点击跳出就会回到本函数执行完的下一行。
不设置断点，直接右键有个运行到光标处，可以直接运行到光标处停止，这样就不需要打断点了。

委托与匿名方法：
委托：存放对方法的引用，内部包含一个指向某个方法的指针
[修饰符，范围，可有可无]  delegate  [返回类型] [委托名称] [参数列表]
委托的作用：在类B中使用类A中的方法，通常需要定义类A对象，然后A对象.fun() 调用
委托类型  委托对象 = 函数---这里的函数不带括号，因为不是函数的调用，只是将函数地址赋给委托。
之后调用的时候可以委托对象（入参）;
匿名方法-加委托
委托类型  委托变量 = delegate (int x,int y)
{
	return x+y;
};必须有分号，这相当于一行代码将函数赋值给委托变量
这种委托的调用： 委托变量.invoke(3,4);

App.config文件的使用  vs
修改的时候只能在xml文件中修改  需要引用程序集：System.Configuration;
命名空间using System.Configuration;

<?xml version="1.0" encoding="utf-8"?>

<configuration>
    <startup> 
        <supportedRuntime version="v4.0" sku=".NETFramework,Version=v4.6"/>
    </startup>
	<appSettings>
		<add key="baud" value="500"/>
	</appSettings>
</configuration>

string serverIP = ConfigurationManager.AppSettings["ServerIP"];程序中获取配置文件中的值
string user = ConfigurationManager.AppSettings["user"];程序中获取配置文件中的值


收银系统：
List<FatherClass> list = new List<FatherClass>();  new父类类型的变量，用list
这样放数据的时候都堆到一起了，数据不容易查找，都不知道每个下标存的啥。

List<List<FatherClass>> list = new List<List<FatherClass>>();
这种方法存的数据里面已经包含了商品类型，商品数量，相当于每次add的时候相当于存的同一类商品

Guid.NewGuid().ToString()；获取唯一编码

构造函数传参方法1：
public Sam(string id, double price, double count): base(id, price, count)

c#可以直接返回数组：
PS：
public static int[] test()
{
	int[] a = new int[]{ 1, 2, 3 };//这里是堆还是栈都可以
	//int[] a = { 1, 2, 3 };  这个是栈
	return a;
}
static void Main(string[] args)
{
	int[] vs = test();//直接定义数据变量直接=赋值就行，数组可以直接赋值，相当于一个数据类型
	for (int i = 0; i < vs.Length; i++)
	{
		Console.WriteLine(vs[i]);
	}
}

java-----------
javac hello.java 会编译生成hello.class二进制文件
java hello   会运行此二进制文件，运行的时候不用带.class
java没有头文件
hello.java-----------java的文件命名
public class hello{

```
public static void main(String args[]){
		System.out.println("Hello world!");
	}
}
```


规范：类名首字母大写，文件名与类名要一致。、

数据类型：
int float
int p2[] = {1，2，4}   静态分配，p在栈中，124在堆中
int p[] = new int[100];      数组，在堆
char str[] = new char[100];  数组，在堆
java里面数组都都在堆中
char str[] = "123"  错误，不能这样定义字符串
应该写String str2 = "123";  正确
java中堆空间不用的时候直接将指针指向NULL就可以了，java的垃圾回收机制会自动释放堆空间。
小数点数字默认是double类型

函数：
void (*fun)(struct Person *p){}这里实现struct中的函数即可
struct Person{
	void (*fun)(struct Person *p);
};

对于直接在函数中new新对象需要我们主动释放p，不然如果函数重复执行就会大量内存泄露。
void fun()
{
	Person * p = new Person();//p是栈对象指向堆空间，函数结束会释放p的栈空间，但是对空间不会释放
}
解决办法，专门定义一个class
class sp{
	Person * p;//栈对象
	sp(Person * other)
	{
		p = other;
	}
	~sp()
	{
		if(p) delete p;
	}
}

void fun()这个函数中p本身是栈对象，函数结束就会执行sp的析构函数，将Person释放。
{
	sp * p = new Person();
}
using System;

namespace YhTest1
{
    public delegate void GreetDelegate(string name);//定义委托类型
    class Program
    {
        private static void English(string name)
        {
            Console.WriteLine("Hello!" + name);
        }
        private static void Chinse(string name)
        {
            Console.WriteLine("你好!" + name);
        }
        static void Main(string[] args)
        {
            GreetDelegate test1;//用委托类型GreetDelegate 定义变量test1
            test1 = English;//给变量test1赋值English函数
            test1?.Invoke("小明");//这句话可以打印出来，因为此时test1不是null的
            test1 = null;
            test1?.Invoke("lihua");//这句话不可以打印出来，括号里面是给test传的参数
            test1 = Chinse;

​		}

可以通过这种方法去显示PC中标签的值，不会太卡--委托
public void LableValue(Label lb, string strValue)
{
	     lb.BeginInvoke(new Action<string>(msg => lb.Text = msg),
		new object[] { strValue });
}

#### 如何通过循环的方法给label赋值呢？
1.创建对应的label数组 Label[] test = {  Temp1,Temp2,Temp3,Temp4   };Tempx标签名字
2.for (int y = 0; y < tempnum; y++)  循环给test[x]即标签赋值
{
	LableValue(test[y], ((sbyte)(((UInt16)Uart.ReDataBuf[++index]) - 40)).ToString()  ); //ID
}
别的控件一样去处理：BMS_faultx为控件名字，PictureBox为控件类型，test为数组名字
PictureBox []test = { BMS_fault1, BMS_fault2, BMS_fault3 };
for (int i = 0; i < 3; i++)
{
	if ((sbit >> i & 1) == 1) { test[i].BackgroundImage = global::GTDS_485.Properties.Resources.red; }
	else { test[i].BackgroundImage = global::GTDS_485.Properties.Resources.gray1; }
}

#### 数组初始化

string[,]  errItem = {		初始化的二维数组
{},
{},
{}
}

string []  var = {}   初始化一维数组



------------------------------------

不知道变量类型，可以先用var类型来接受，等会再改成对应的类型



想在Form1中用Form3中的变量，在Form3中需要 public static byte g_lecuId = 1; 声明当前变量为Public  static  变量，之后在别的类中直接Form3.var 就可以调用，整个程序运行中此变量的值都在  直到程序结束