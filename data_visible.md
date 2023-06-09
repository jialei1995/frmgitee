# 数据可视化

## 1```numpy```

### 1.1构造

```pyt
构造方式：
numpy中的类型强制统一：str>float>int
np.array([])  直接传进列表即可
快捷生成数组方式：
np.ones(shape)  指定n*n生成全是1的二维数组
np.zeros((2,3))  指定n*n生成全是0的二维数组
np.full(shape,fill_value)  指定形状，要填的数值
np.eye(3)     生成3*3数组，对角线全为1，其他位置全为0
等差数列
np.linspace(2.0, 3.0, num=5)  start end nums分几份
array([2.  , 2.25, 2.5 , 2.75, 3.  ]


np.arange(3,7,2)   start  end  step步长
array([3, 5])
随机数生成
np.random.randint() 	整数
np.random.random()		小数
np.random.randn()		正态分布数
```

### 1.2访问

```arr[index1,index2,...,indexn]```

访问元素：```arr[row,colum]```

访问行：arr[row]

访问列：arr[:,column]

切片：```arr[row1:row2,column1:column2]```

### 1.3属性

arr.shape	形状

arr.ndim 	维度

arr.size		大小

arr.dtyte 	 数据类型

### 1.4运算

+ 聚合运算：

​		sum	求和

​		mean

​		std

​		max min最大最小

​		```np.meidan() ``` 中位数

​		使用axis来控制轴方向

+ 广播运算

​		因为两个集合要运算，形状要一致，广播就是为了保障运算的正常运行，处理形状的特性。

## 2```pandas```

### 2.1数据类型

### Series：类字典的，一维的数组对象

+ 构造

  Series(data,index)  

  Series(data=dict)

+ 属性
  + shape  
  + size
  + dtype  类型
  + index 相当于key
  + values

+ 访问

  + 兼容numpy的访问方式

  + 兼容字典的访问方式

  + s.loc[显示索引]    s.iloc[隐式索引/列表]

    标签的切片是闭区间，索引的切片是开区间

+ 运算

  + s+numpy  广播

  + s+s   索引对齐

  + 常用方法：s.head()  s.tail()  查看开头结尾的5个原素，本质是切片函数，一般用于查看结构，主要用的DataFrame对象中

    s.sort_values()  根据值排序

    s.sort_index()   根据索引排序

    s.value_count（）  统计列表中相同元素个数

    s.unique()	去重

    s.map()         映射函数

### DataFrame:Series的容器，是字典对象

+ 构造
  + DataFrame(data,index,columns)
  + pd.read_csv()   pd.read_excel()  pd.read_table()



## 3数据

### 折线图显示

```pyt
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from pandas import Series,DataFrame


#准备要显示的数据  纵坐标数据就是随机的int  横坐标是ABCDE字符
score=Series(data=np.random.randint(0,100,size=5),index=list('ABCDE'))
#score=Series(data=np.random.randint(0,100,size=5),index=[1,2,3,4,5])
#绘制要显示的折线图
score.plot()
plt.show()#显示折线图
#------------------------------------------------------
heros=pd.read_excel('test.xlsx') #读取excel表格数据
heros['id'].plot()  #用表格中的id绘制表格
plt.show()
```

读取excel表格数据 遇到问题：Excel xlsx file； not supported(xlrd版本不兼容问题，完美解决！)

```py
需要先卸载自己安装的xlrd重新安装1.2.0的版本才行
pip uninstall xlrd
pip install xlrd==1.2.0 -i https://mirrors.aliyun.com/pypi/simple/
```

用DataFrame做的折线图表示

```py
#DataFrame生成10个人的成绩，每个人3科目成绩 size=10*3   列里面放每科目名称
score_df=DataFrame(data=np.random.randint(0,100,size=(10,3)),columns=['pyth','java','php'])
score_df['java'].plot()
plt.show()


直接score_df.plot() 生成的线型图就是3个科目都在一起的线型图
```

### 柱状图

```
series.plot(kind='bar')
dataframe.plot(kind='bar')

#index表示5个开发商  columns表示3个楼盘
score_df=DataFrame(data=np.random.randint(0,100,size=(5,3)),index=list("ABCDE"),columns=['碧桂园','万科','万达'])


plot(kind='barh')   柱子变成横向的柱子，有时易于观察
```

plt.figure用法：

```py
score_df=Series(data=np.random.randint(30,100,size=30))
plt.figure(figsize=(5,20))   #显示比例 直接显示会太挤了，设置之后 则图像垂直方向像素拉长，显得不挤
score_df.plot(kind='barh')
plt.show()

plt.figure(figsize=(1,2))
plt.figure(figsize=(100,200))
这两个比例都是1：2，第一个里面每个平方cm，有1个像素，第二个里面每平方cm，像素100个
```

### groupby

```py
heros=pd.read_excel('test.xlsx') #读取excel表格数据
#将数据按照攻击分类，分类之后对比分类后的类别中age的平均差异
print(heros.groupby('attack')['age'].mean())   mean求平均值
```

### 直方图

```py
heros['age'].plot(kind='hist')   #根据年龄段，分段显示每段中的频数
heros['age'].plot(kind='hist'，bins=6)   #根据年龄段，分6段显示每段中的频数
```

### 散列图

```py
data1=np.random.randn(1000)
data2=np.random.randn(1000)
df = DataFrame(data={'A':data1,
                     'B':data2})  #A里面有个数组，B里面一个数组
df.plot(kind='scatter',x='A',y='B')   #需要指定散列图的x y是什么
plt.show()


#指定画布形状,有没有都可以
plt.figure(figsize=(5,5))
ax=plt.subplot()
df.plot(kind='scatter',x='A',y='B',ax=ax)
```



## scipy

```py
#滤波原理
arr1 = np.array([1,22,33,24,35,26])
print(arr1>25)			返回[False False  True False  True  True]列表
print(arr1[arr1>25])	[33 35 26]
```

```py
import random
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from pandas import Series,DataFrame
import matplotlib
from scipy.fftpack import fft2,ifft2
#fftpack.ftt2  时域--》频域
#fttpack.iftt2  频域--》时域
pic = plt.imread('test.jpeg')
plt.figure(figsize=(10,10))  #图像显示大一些
plt.imshow(pic,cmap='gray')
plt.show()
#时域-->频域,转化图片后做处理
f_moon = fft2(pic)
print (np.abs(f_moon).max())
print(np.abs(f_moon).min())
print(np.mean(np.abs(f_moon)))

threshold = 350
f_moon[f_moon>threshold]=0  #将过大的阈值清0

#频域-->时域  再将域转化回去
if_moon=ifft2(f_moon)
plt.imshow(np.real(if_moon))
plt.imshow(pic,cmap='gray')
plt.show()

```















































## 

## 

-------------------

# 数据分析

## 过程

### 采集

1. 埋点  

​			程序中加日志收集，固定类型的打印

2. 传感器采集

## 存储

1. 各种sql server

## 分析数据的目的

+ 及时发现异常，纠正错误
+ 找到数据之间的因果关系

## excel操作

拿到原数据必须要备份一下再去操作：在sheet上右键->隐藏工作表。

+ 直接在数据页-->插入-->数据透视表
+ 根据字段拖入右侧不同字段到行列，筛选器
+ 最上面-->分析-->插入切片器
+ 最上面-->分析-->数据透视图，将对应的数值列可以进行不同数据显示

视图-->新建窗口：把当前窗口拆分出来，便于新sheet对当前窗口的操作



sum(可以跨工作表)   两段不同的数据中间可以用“,”隔开

SUMIF(判断条件数据列,要对比的数据格,要求和的数据)



##### MATCH(B118,$B$112:$B$126,0)  

参数： 要找的数据

​			在哪个区域找

​			0：精确匹配

​			返回：此数据所在的索引

##### INDEX(B95:C103,4,1)

参数：查找区域范围

​			在查找区域要查的行

​			在查找区域要查的列

​			返回：对应的行列的数据

##### index与match混合操作

INDEX(元数据!$A:$X,MATCH($B25,元数据!$I:$I,0),MATCH(F$17,元数据!$1:$1,0))

+ 这里查找数据的范围需要固定，不然向下向右拉的时候会出问题
+ 第一个match计算的是需要在总范围中取数据的行
+ 第2个match计算的是需要在总范围中取数据的列

##### 条件判断赋值

在空白处新建下拉框,在赋值位置,根据下拉框选项来给当前位置赋值

if(H5="全部",sum(),sum())



## ```jupyter```

```anaconda需要先安装```

```jupyter_contrib  需要安装```



```py
cpc = np.read_csv(..)  #获取csv文件句柄
#根据条件('字段>7.0&字段<8.0')筛选 [['字段1','字段2']]数据
cpc.query('字段>7.0&字段<8.0')[['字段1','字段2']]
#另一种快捷的查询方法-简单的查询条件可以这样用
cpc[['字段1','字段2']][cpc.gmvroi.isin([7.0,8.0])]
#模糊查询 查找名称带有宝山的门店
pd.Series(cpc['平台门店名称'][cpc.平台门店名称.str.contains('宝山')].unique())
```





## flask

快捷创建服务器方法

```pycharm 专业版可以直接创建flask工程
社区版不能直接创建，可以直接创建普通的纯py工程，然后再工程的根目录创建static文件夹  templates文件夹   app.py文件

同样可以创建可以运行的flask工程
```



```py
from flask import Flask
app=Flask(__name__)

@app.route('/')  #/表示当前网站是127.0.0.1/5000/
def hello_world():
    return 'hello world'

if __name__=="__main__":
    app.run()
```



```py
from flask import Flask,render_template

@app.route('/test') #这个访问的时候就要加 /test
def hello_world():
    return render_template("test.html")
#自己写好的html网页 也可以直接通过render_template渲染后在网页显示
#注意 html文件需要放到templates文件夹 render_template才能加载
```



给网页填变量

```c
def hello_world():
    time=datetime.date.today()
    # 传参 var（任意命名都可以）
    return render_template("test.html",var=time)
#网页写法
welcom {{var}},欢迎光临  //双大括号将var包起来
```

给网页传列表

```py
def hello_world():
    ltime=datetime.date.today()
    lname=["xiaoli","xiaoming","xiaoli"]
    return render_template("test.html",time=ltime,name=lname)

# html 写法
welcom {{time}},欢迎光临 <br/>  #换行符
今天值班的有：<br/>
{% for var in name %}
{{var}}
<li>{{var}}</li>  # 格式化打印
{% endfor %}
```



给网页传字典

```py
task={"task":"clean","time":"3hour"}
    return render_template("test.html",time=ltime,name=lname,task=task)
    
    <!--了解如何在页面打印表格--->
    <table border='1'> <!---表格最外层括号--加border='1'就有框了 -->
        <tr>   <!----第一行---->
            <td> 测试1 </td>  <!----第1列---->
            <td> 测试2 </td>  <!----第2列---->
        </tr>
        <tr>   <!----第2行---->
            <td> 测试1 </td>
            <td> 测试2 </td>
        </tr>
        <tr>   <!----第2行---->
            <td> 测试1 </td>
            <td> 测试2 </td>
        </tr>
    </table>
    <!---表格 处理字典-->
    <table border='1'> <!---表格最外层括号--加border='1'就有框了 -->
        {% for key,value in task.items() %}  #遍历字典
        <tr>   <!----第一行---->
            <td> {{key}} </td>  <!----第1列---->
            <td> {{value}} </td>  <!----第2列---->
        </tr>
        {%endfor%}
    </table>
```

表单提交，通过网页1 跳转网页2

```pyt	
from flask import Flask,render_template,request 
@app.route("/regit") #默认的都是get请求，这个表示post请求也允许
def regit():
    return render_template("test.html")

//目的网页
@app.route("/result",methods=['POST','GET']) #默认的都是get请求，这个表示post请求也允许
def result():
	if(request.method=='POST'):
		表单提交的键值对=request.form----》可以拿到表单的各个键值对
    	return render_template("mmm.html",)
    if(request.method=='GET'):
		var=request.form----》可以拿到表单的各个键值对
    	return render_template("nnn.html",var)
    
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Title</title>
</head>
<body>
#要跳转的目的地址 方式post
<form action="http://localhost:/5000/result" method="post">
    <p>姓名：<input type="text" name="姓名"></p>
    <p>年龄：<input type="text" name="年龄"></p>
    <p>性别：<input type="text" name="性别"></p>
    <p>地址：<input type="text" name="地址"></p>
    <p>姓名：<input type="submit" value="提交"></p>
</form>

</body>
</html>
```

