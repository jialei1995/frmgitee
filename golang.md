## 新需求->怎么解决

新浪页面 怎么实现局部刷新？

传统网站都是 点击链接整个页面刷新，浪费带宽，有些用户打开网页不会直接往下划，只会看第一个界面。

学习新技术，怎么学？

1.原理 2.基本语法 3.简单案例 4.怎么规范使用 使用陷阱 偏细节

## 语言特点

go=c+python   静态编译语言的安全性能  动态语言的开发维护



fmt.Scanln(&a);  给a赋值 从键盘

go env -w GOPATH=""设置默认搜索路径
go env 查看当前golang环境变量
设置好后在/$GOPATH/src 目录直接创建文件或文件夹都可以

### 类型转换
只能显示转换
var := (int32)var2

### 数据使用
```go
var a int 声明
var a int=45  初始化
var b=67   自动推导初始化
a:=90  自动推导初始化

+：对于字符连说是拼接，对于数据是相加减
```

### 进制转换
```go
FormatInt(num,10)  转10进制
FormatInt.Itoa(intval)   转int
FormatFloat(num,'f',10,64)  保留10位，转为float64类型
FormatBool(val)  转bool
uint64(val)   类型转化，不同类型之前计算 需要类型转换成同一类型
//string -- 》基本类型
strconv包
.FormatInt 返回值boolval,errno  errno不收可以用 _代替
.FormatFloat(str,64)  64位宽
.FormatInt(str,10,64)   10进制，64位
```
### 其他

引入包 概念 ，每个文件都要属于一个包 不能单独存在

+ 语法类似c
+ 引入垃圾回收机制，内存自动释放
+ 天然并发
+ 管道通信机制，go特有的channel
+ 函数返回多个值

  ```
  func getSumSub(n1 int,n2 int)(int ,int)//第二个括号存2个返回值类型
  {
    sum=n1+n2
    sub=n1-n2
    return sum,sub
  }
  ```
+ 切片 延时执行defer

### bin命令的使用

go build test.go  编译go文件  ./test.exe 运行编译好的文件

gofmt -w test.go   自动格式化文档并写入go文件

fmt.Println("hello worldhello worldhello worldhello worldhello worldhello worldhello worldhello worldhello worldhello world")

优化方案: 用 , 将多个字符串链接  避免行过长

```
fmt.Println("hello worldhello worldhello ",
	"worldhello worldhello worldhello",
	"worldhello worldhello worldhello",
	"worldhello world")
```

包与包中的文件 与文件中的函数的关系：

import 包名

//使用包内的函数

包名.函数名()  直接调用包内的某个文件内写好的函数即可  不用在意文件名是怎么写的


### 变量的定义声明

```
//第一种定义方法
var i int  //前面必须有var 表示是个变量  int表示变量类型 为变量名---若不赋值，则i的默认初始值为0
//第2种定义方法--类型推导-自行判定变量类型
var num=10.1  //编译器会自动类型推导 小数整数字符串...
//省略var
name := "tom" //必须用:=，左侧的变量必须是初次定义声明的，否则报错

```

直接可以打印汉字

var n2 int='被'  只能单引号，双引号表示字符串
fmt.Printf("%c type=%T--size=%d\n",n2,n2,unsafe.Sizeof(n2))

打印长方形

```go
  1 package main
  2 import "fmt"
  3
  4 func main(){
  5     for i:=1;i<5;i++{   不需要加括号，也不能加括号--语法要求的
  6         for j:=1;j<6;j++{
  7             fmt.Print("X")  Print打印默认无换行
  8         }
  9         fmt.Println()	Println打印默认有换行
 10     }
 11 }

```

跳转控制语句：goto

### 函数

func 函数名(形参列表)(返回值列表){
  执行语句
  return 返回值
}

```go
package main
import "fmt"

func cal(n1 float64,n2 float64 )float64{
        return n1+n2
}
func main(){
          //var ret float64= cal(10.4,5.3)
          //var ret = cal(10.4,5.3)
          ret:=cal(10.4,5.3) 自行推导类型
          fmt.Print(ret)
}
```

### 包概念

用法
新建utils.go
{
  package utils
  func Cal(){  }  **首字母大写表示该函数可导出**
}

引入包 main.go
{
  package main
  import(
    "fmt"
    "utils"
  )
}

import 1.不能直接从绝对路径开始，默认从$GOPATH的src目录去查找的
2.需要把包放到src目录中。直接放到$GOPATH目录也不行
3.export GOPATH=$GOPATH:/root/go_code
4.建议将文件夹名与文件名与包名一致，便于访问
5.可以给包名取别名去访问,取别名后原来的包名就不能用了
{
  import(
    util "go_code/utilstool"  util就是utilstool的别名
  )
}
6.同一包下，不能有重复的函数名，会报重复定义。也不能有相同的全局变量名
7.如果需要将包编译成可执行文件，需要声明为main（package main），如果写的文件是个库，包名可以自定义
8.编译-在$GOPATH 执行-go build -o bin/mytest.exe go_code/project/main
9.编译main包的时候，import别的包会生成对应的.a 库文件，存放到pkg目录

不同包里面的同一个函数名，会执行对应的函数。

如果希望忽略函数的返回值，可以在对应位置用_ 符号表示占位忽略。
_,sub=getsumAndsub(3,6)

使用函数的注意事项：

```go
1.形参，返回值都可以是多个
2.形参，返回值 类型可以是值类型，也可以是引用类型
3.函数名首字母大写，则该函数可以被本包文件或其他包文件使用-（类似public）
4.数组作为形参传参是值传递，函数内修改，不会修改原来的值 **
5.go函数不支持重载--
{
  c++ 可以通过 函数名一样 但形参不同，达到函数重载的功能。
  go 不支持重载
}
6.函数可以是一种数据类型，可以给该类型赋值，通过变量实现对函数的调用
{
  a:=getsum
  fmt.Println("sum=",a(1,2))  可以直接调用源getsum函数
  fmt.Printf("a的类型是%T",a)  func  函数类型
}
7.函数可以作为形参传入
{  第一个形参就是函数类型的入参
  func myfun(funname func(int,int)int,num1 int,num2 int) int{
    return funname(num1,num2)
  }
}
函数类型可以:type mysum func(int,int) int  这时mysum就成数据类型了
8.type myInt int---声明myInt=int
var num1 myInt;num1=40  定义myInt类型变量 40
var num2 int；num2=num1-->报错，go只看到要把myInt类型变量赋值给int，需要显式转化-->num2=int(num1)
9.可以在函数定义中 对返回值命名，以防接收出错
{
  func getSumAndSub(n1 int,n2 int)(sum int,sub int){
    随便sub sum的计算顺序 都可以。返回值列表已经给顺序了
    sub=n1-n2  返回值中已声明，直接赋值就好
    sum=n1+n2
    return 不需要加返回值了，给sum sub赋值就是返回
  }
}
10.支持可变参数
{
  //支持0-n多个参数
  func sum(arg...int)sum int{  }
  //支持1-n 多个参数
  func sum(n1 int,arg...int)sum int{  }
}
arg是slice切片，支持arg[index]访问各个值
{
  返回值必须用()括起来，“若入参含可变参数，可变参数必须放列表最后”
  func getsum(n1 int,arg...int)(sum int){
        sum=n1 不需要:=
        for i:=0;i<len(arg);i++{
                sum+=arg[i]
        }
        return sum 有无sum都可以
  }
  func main(){
          fmt.Println(getsum(10))
          fmt.Println(getsum(10,20,30))

  }
}
11.入参如果一样可以省略类型
{
  func sum(n1,n2 float32)  这个写法是对的
}
```

### init函数

```go
func init(){
  fmt.Println("first init")
}
程序执行main之前就会自动调用init，做一些初始化工作
1.若文件包含全局变量，init ，main。则执行顺序为全局变量>init>main
测试
{
  以下先打印1 然后是2 最后是3
  func test()int{print(1);return 90}
  var gval=test()
  func init(){print(2)}
  func main(){print(3)}
}
2.如果main.go包含utils.go中都含有全局变量定义，init，执行的流程为：
utils.go的变量定义->utils.go的init->main.go的变量定义->main.go的init
```

### 字符串赋值

可以直接赋值
var name string
name="123432"

### 匿名函数

1. 定义匿名函数的时候直接调用，这种方式匿名函数只能调用一次
2. 将匿名函数赋值给一个函数变量，通过该变量来调用匿名函数，可以多次调用

```go
func main(){
  //只能调用1次的匿名函数--用的多
  val1:=func(n1 int,n2 int)int{
          return n1+n2
    }(10,20)
  fmt.Println(val1)
  //可以多次调用的匿名函数
  val2:=func(n1 int,n2 int)int{
          return n1-n2
  }
  fmt.Println(val2(10,20))
}

```

3. 全局匿名函数

```go
var (
  //Fun1 首字母大写 才能全局被调用
  Fun1=func(n1 int,n2 int)int{
    return n1*n2
  }
)
fmt.Println(Fun1(3,4))  调用
```

### 闭包

就是**一个函数** 与其**相关的引用环境-即变量**组合的一个整体。

配置环境变量：GOPATH  GOROOT 至/etc/profile 修改后只需要exit+login即可

```go
func AddUpper() func (int) int{
  var n int =10
  return func(x int) int{
    n=n+x
    return n
  }
}
f:=AddUpper()  只调用1次，n只初始化1次，返回内部的匿名累加函数
fmt.Println(f(1))   11
fmt.Println(f(2))   13
fmt.Println(f(3))   16
代码分析：
1.AddUpper是个函数，返回值是func (int) int类型
2.返回的是个匿名函数，此匿名函数引用到了函数外的n
共同组成一个整体--闭包
3.调用f的时候 n只会初始化1次
4.调用的时候入参是内部匿名函数的入参，返回值是内部你匿名函数的返回值
观察下面的demo，看str的变化
{
  func AddUpper() func (int) int{
  var n int =10
  var str="hello"
  return func(x int) int{
    n=n+x
        str+=string(x)
        fmt.Println("str=",str)
    return n
  }
}
func main(){
  f:=AddUpper()
  fmt.Println(f(48))
  fmt.Println(f(49))
  fmt.Println(f(50))
}
 输出：
str= hello0
58
str= hello01
107
str= hello012
157
}
```

### defer

```go
func sum(n1 int,n2 int)int{
  //当执行到defer时，暂时不执行，会将defer后面语句压入defer栈，函数执行完毕才出栈
  defer fmt.Println("ok n1=",n1)
  defer fmt.Println("ok n2=",n2)
  res:=n1+n2
  fmt.Println("ok3 res=",res)
  return res
}
func main(){
  res:=sum(10,20)
  fmt.Println("res=",res)
}
打印结果为：
函数return后，出栈defer中的内容
ok3 res=30
ok n2=20
ok n1=10
函数 defer执行完后 打印函数外的内容
res=30

defer最主要的用处：在函数执行完后，可以即使释放函数中的资源
exp：
func test(){
  file=open()
  defer file.close(file)  //函数执行完就一定会关句柄，不需要放到函数最后就行
  操作...
  操作...
}
```

### 值传递与引用传递

1.值类型
基本数据类型：int float bool string 数组 结构体
2.引用类型
指针 切片 map 管道 interface

### 内置函数

很多，常见的如下：
len：求长度，比如strlng array slice map channel
new：分配内存，主要分配值类型，返回指针
make：分配内存，主要分配引用类型，比如channel slice map。

### 异常处理

1.正常的程序发生错误后，程序会崩溃退出
2.若希望程序出错后，可以捕捉到错误，并保证程序继续运行，就需要 异常处理
```go
var i int8=128   会直接报错
var i int8=127+var2  不会直接报错，运行时会溢出报错
```


```go
package main

import(
        "fmt"
        "time"
)

func test(){
  //使用defer+recover 捕获异常，有异常后执行defer栈中的内容
  defer func(){
    //这里可以简写为 语法没问题
    //if err:=recover();err!=nil{
    err:=recover()
    if err!=nil{
      fmt.Println(err)  //这里可以发邮件给程序员
    }
  }()  // 直接调用无返回值的匿名函数，不用接收返回值
  n1:=10
  n2:=0
  fmt.Println(n1/n2)
}
func main(){
  test()
  for{
    fmt.Println("main xiamian") //不处理异常走不到这
    time.Sleep(time.Second) //休眠1S
  }
}

```

## 数组

1. 定义
   var arr[100] float64
2. 第一个元素的地址 就是数组地址
3. 从终端输入数据赋值给变量fmt.Scanln(&var)
4. 初始化数组的四种方法
{
    var numarr1[3]int=[3]int{1,2,3}
    var numarr2=[3]int{5,6,7}
    var numarr3=[...]int{8,9,10}
    var numarr4=[...]int{1:800,0:900,2:500}
}
5. 打印数组:fmt.Println(numarr1)-与python输出类似
遍历数组：
{
    //:= 定义时赋初值
    for index,val := range numarr{
    ...
    }
}
### 作为函数入参
```go
func testArr01(arr []int) { 
  //形参未指定大小，与切片传入一样
	arr[0]++
}
func testArr02(arr [5]int) { //形参指定大小
	arr[0]++
}
func testArr03(arr *[5]int) { //使用指针方式，也就是引用传递，会改变数组值
	arr[0]++
}
func main() {
	var arr = [5]int{0, 2, 3, 17, 50}
	testArr01(arr[:]) //切片方式传递（常用）
	for i := 0; i < len(arr); i++ {
		fmt.Printf("%d ", arr[i])
	}
	testArr02(arr) //数组名方式传递,此时传递的是副本，并不会改变原数组
	for i := 0; i < len(arr); i++ {
		fmt.Printf("%d ", arr[i])
	}
	testArr03(&arr) //指针方式传递，会改变原数组
	for i := 0; i < len(arr); i++ {
		fmt.Printf("%d ", arr[i])
	}
}
```

## slice
1.切片是数组的引用
2.切片是动态变化的数组
3.切片的定义:var slicename [] int
slice := intarr[1:3]  定义+赋值，引用数组1，2位置
    len(slice)  切片长度
4.切片的容量可以动态变化
cap(slice)--打印切片容量--随着元素增加动态变化
5.slice的第0个元素地址==intarr第1个元素地址
6.其他slice的操作类似array

### slice的使用
```go
//必须make后才能使用，make入参表示类型，长度5，容量10
//无make  不分配空间 不能使用
var slice []float64 = make([]float64,5,10)
之后就可以赋值使用了


其他方式使用slice,这种相当于把数组给slice，不用make
var slice[] int=[] int{1,3,5}

slice := arr[:]  直接将数组整个赋值给切片

切片可以继续切片：
slice2 := slice[1:4]

追加：
slice3 ：= append(slice,1,2,3)   相当于创建了一个数组把新数组添加到新slice，并不会修改原来的slice

一般用的时候直接-slice := append(slice,1,2,3)

copy(slice4,slice3) 使用copy拷贝后，两个slice独立互不影响。
遍历：
{
  for i:=0;i<len(slice);i++{

  }
}
{
  for i,val := range slice{
    i val
  }
}
```

```go
var a[] int=[]int{1,2,3,4,5}
var slice=make([]int,2)
slice[0]=8
slice[1]=9
fmt.Println(slice)
copy(slice,a)
fmt.Println(slice)
拷贝不会出错，slice中的数据会发生变化
[8 9] 源数据
[1 2] 拷贝后
```

```go
func test (slice[] int){
  在函数中作的修改会影响函数外面的slice，因为是引用传递
}
```

```go
//写一个程序返回斐波那契 切片
//返回数组还不好实现，数组大小得用常量初始化，切片可以用变量
func fbnq(n int)([] uint64){
  //创建切片 长度为n
  fbnqslice := make([] uint64,n)
  fbnqslice[0]=1
  fbnqslice[1]=1
  for i:=2;i<n;i++{
    fbnqslice[i]=fbnqslice[i-1]+fbnqslice[i-2]
  }
  return fbnqslice
}
```

## string
```go
fmt.Printf("%v %T",var，var)  格式化打印 %v数据 %T:类型
fmt.Print()      无换行打印
fmt.Println()    换行打印
var n3 int='汉'   存一个字符  必须是单引号
```


## 二维数组

### 初始化

```go
package main
import "fmt"
import _ "strings"  //has "_" can not use

func main(){
    //init ,clare
    var arr[4][6] int;  //row=4  col=6
    //give pos val
    arr[1][2]=100
    fmt.Println(arr)//erwei arr can directly print
    for i:=0;i<4;i++{ //another way to traverse
         for j:=0;j<6;j++{
             fmt.Print(arr[i][j]," ")
         }
         fmt.Println()
     }
    
    //directly init  直接赋值初始化方式
    var arr[2][3]int=[2][3]{{1,2,3},{4,5,6}}
    var arr[2][3]int=[...][3]{{1,2,3},{4,5,6}}  //后面的3不能省，行数可以省。会自动推导行数
    
    var arr=[2][3]{{1,2,3},{4,5,6}}   有个var 则冒号可以省略
    arr := [2][3]{{1,2,3},{4,5,6}} 
}
```

本质上分配的内存空间 是连续的。

arr[2,3] ==== arr[6]  分配的空间是一样的。

二维的访问方式特殊些，更加遍历的去跳着访问。



### 遍历

1. 

```go
fmt.Println(arr)

```

2. 

```go
for i:=0;i<len(arr);i++{
    for j:=0;j<len(arr[i]);j++{ //每个一维数组长度 不一定是一样长度的
        fmt.Print(arr[i][j]," ")
    }
    fmt.Println()
}
```

3. 

```go
for i,v : range(arr){ // i=行号   val=第i行的一维数组
   fmt.Printf("i=%v  v=%v",i,v)
}
for i,v : range(arr){ // i=行号   val=第i行的一维数组
    for j,v2:=range v{  //j=一维数组索引（列号）  v2表示第i行 第j列的数据
        fmt.Printf("arr[%v][%v]=%v",i,j,v)
    }
}
```



4. 

```go
for i:=0;i<3;i++{
    for j:=0;j<5;j++{
        fmt.Printf("input %v banji,%v xuesheng score\n",i,j)
        fmt.Scanln(&arr[i][j])  //从键盘给每个位置赋值
    }
}

```



## MAP

键值对

### 定义

+ var  变量名  map [keytype] valuetype
+ key可以是bool，数字，string，指针，channel，还可以是包含前几种类型的结构体
+ key不能是slice   map   function  ，这几种类型没法用 ==去判断

声明举例：

```c
var  name  map[string]int
var  name  map[string]string
var  name  map[int][string]
var  name  map[string]map[string] string   key是string，值是map
```

 ### 使用方式

+ 先声明，再make

```go
var val map[string] string
val = make(map[string]string,10)   //申请空间
val["no1"]="songjiang"
val["non"]="wuyong"

map[no1:songjiang non:wuyong]
```

+ 直接make---推荐使用
```go
val:=make(map[string]string)   //default size
mymap := map[int]int{}    map初始化 不需要make也可以放数据 能用
//赋值同上
```

+ 声明的时候直接赋值  底层也make分配空间了

```go
val:=map[string]string{
     "key1":"songjia",
     "key2":"lujunyi",  //last one also must has ','
}
```



### 应用案例

```go
name:=""
sex:=""
stu:=make(map[string]string)

for i:=0;i<3;i++{
    fmt.Printf("input %d name\n",i+1);
    fmt.Scanln(&name)
    fmt.Printf("input %d sex\n",i+1);
    fmt.Scanln(&sex)
    //fmt.Scanln(&stu[name])  报错，必须先用临时变量将数据存起来，再去赋值
    stu[name]=sex
}

复杂的：
stuMap := make(map[string] map[string]string)  //map 都得make
stuMap["stu01"] = make(map[string]string)  //每个使用前 都得先make
stuMap["stu01"]["name"]="tom"
stuMap["stu01"]["sex"]="nan"

stuMap["stu02"] = make(map[string]string)  //每个使用前 都得先make
stuMap["stu02"]["name"]="hong"
stuMap["stu02"]["sex"]="nv"
```

### 增删改查

+ map[key] = value  若key不存在，这里就是增加；若key已存在，这里就是修改

+ 删除  `delete(mapname,“keyname”)`   删除key为keyname的节点,若无mapname变量，则不操作，也不报错
+ 若要清空map，需要遍历，一个一个删除

+ 查找   

```go
val,ok := cities["no1"]
if ok{
    有key为“no1”的节点，对应的值为val
}else{
    无key为"no1"的节点
}
```

+ 遍历--只能用for  range去遍历，因为key不是数字

```go
for key,val := range mapname{
    fmt.Printf("key=%v,val=%v\n",key,val)
}


//复杂的结构怎么遍历
stuMap := make(map[string] map[string]string)  //map 都得make
stuMap["stu01"] = make(map[string]string)  //每个使用前 都得先make
stuMap["stu01"]["name"]="tom"
stuMap["stu01"]["sex"]="nan"

stuMap["stu02"] = make(map[string]string)  //每个使用前 都得先make
stuMap["stu02"]["name"]="hong"
stuMap["stu02"]["sex"]="nv"

for k1,v1 := range stuMap{ //这里的k1 为 stu01 stu02  v1为对应的值
    当前遍历的key为stu01
    for k2,v2 := range v1{ //本身就是个map，还得用for range去遍历
        
    }
}
```

### map切片 动态的map---数组存map

```go
monsters := make([]map[string]string,2) //slice 需要make

 var monsters []map[string]string  //只声明 后面make
 //切片要make 才能使用   map也 要make才能使用
 monsters = make([]map[string]string ,2)//计划放入两个妖怪
 if(monsters[0]==nil){
     monsters[0]=make(map[string]string)
     monsters[0]["name"]="niiumo"
     monsters[0]["age"]="123"
 }
 if(monsters[1]==nil){
     monsters[1]=make(map[string]string)
     monsters[1]["name"]="huli"
     monsters[1]["age"]="234"
 }
//动态增加 切片 分配的只有2个 
//新建新map  
monster3 := map[string]string{
     "name":"test",
     "age":"890",
}
//直接将新map append到源切片中
monsters=append(monsters,monster3)
```

### map排序

默认key是无序的，但是可以排序遍历

```go
//1. 先将map的key 放入到切片中
//2. 对切片排序
//3. 遍历切片，按照key 输出map的值

var keys []int  //新切片
for k,_ := range map1{
    keys.append(keys,k)  //把遍历的key 放到keys切片中
}
sort.Ints(keys)  //调用sort 包中的keys 进行排序

for _,k := range keys{  //遍历切片，下标没用  值为map1的key
    fmt.Printf("map1[%v]=%v\n",k,map1[k])
}
```

### 使用细节

#### 是引用类型

函数中对入参的修改就是对 源值的修改

```go
func modify(map2 map[int]int){
    map2[1]=100
}
func main(){
    map1 := map[int]int{
        1:1,
        2:2,
    }
    fmt.Println(map1)
    modify(map1)
    fmt.Println(map1)  在函数中的修改会影响函数外面的map的值
}
```



#### 自动动态扩容，不需要append（切片需要）

#### 管理复杂数据类型用struct 比直接用map更好

```go
type Stu struct{
     Name string
     Age int
 }
 func main(){
     students := make(map[string]Stu)
     stu1 := Stu{"tom",18}
     stu2 := Stu{"marry",19}
     students["no1"] = stu1    //直接将定义好的变量插入 map
     students["no2"] = stu2
     fmt.Println(students)
     for k,v := range(students){
         k==学生编号
         v.Name==名字
         v.Age===年龄
     }
 }
```



#### demo 复杂数据类型 函数

```go
func modify(users map[string]map[string]string,name string){
    val,ok := users[name]  若该name存在，修改密码值
    if(ok){
        val["mima"]="88888"
    }else
    {					若不存在，先新建节点  再修改“mima”属性
        users[name] = make(map[string]string)
        users[name]["mima"]="88888"
    }
}

//编写函数 修改map中某个节点的属性值
func main(){
    stuMap := make(map[string] map[string]string)  //map 都得make
    stuMap["stu01"] = make(map[string]string)  //每个使用前 都得先make
    stuMap["stu01"]["name"]="tom"
    stuMap["stu01"]["sex"]="nan"
    stuMap["stu01"]["mima"]="9999"

    stuMap["stu02"] = make(map[string]string)  //每个使用前 都得先make
    stuMap["stu02"]["name"]="marry"
    stuMap["stu02"]["sex"]="nv"
    stuMap["stu02"]["mima"]="0000"
    fmt.Println(stuMap)

    modify(stuMap,"stu01")
    fmt.Println(stuMap)
    modify(stuMap,"stu03")
    fmt.Println(stuMap)
}
map[
    stu01:map[mima:88888 name:tom sex:nan] 
    stu02:map[mima:0000 name:marry sex:nv] 
    stu03:map[mima:88888]
]
数据可看到 stu3新建的属性只有密码，没写别的属性
```



## go的面向对象

+ go里面的面向对象，是支持面向对象，并没有c++  python  c#那么完善
+ 没有class  用的struct实现面向对象特性
+ 很简洁，去掉了重载，构造函数，析构函数，this指针

+ 主要是面向接口编程，go中把接口用到了极致。



### 定义

```go
type Cat struct{
    var1  string
    var2  int
}
var cat1  Cat  定义一个空属性的cat1  cat+‘.’ ：一个个赋值

type ’C‘at struct{
    ’v‘ar1  string
    var2  int
} C大写表示可以在其他包中使用，var1的v大写表示该成员在其他包中可以访问
```



结构体变量在内存分布：

c中，会字节对齐--根据操作系统 默认对齐方式

go中，在64位操作系统中，string 默认分配16字节  int 分配8字节大小空间

+ 字段使用

```go
type Person struct{
    Name string
    Age int
    Score [5]float
    ptr * int
    slice[]int
    map1 map[string]string
}
var p Person
p.Name="name" 可以直接赋值
p.ptr = new(int)    //需要申请空间才能赋值
不申请空间直接 prt=&intval  也可以使用
p.map1 = make(map[string]string) //map需要申请空间
p.slice = make([]int,10)  //slice需要make 大小是10的slice

var v1 *int
v1=new(int)  需要先给int的指针申请空间 才能使用
*v1=100   指针赋值
fmt.Println(*v1)

var v1 *int
v2:=1000
v1=&v2  不申请空间直接取已经申请空间的其他变量地址也能操作
*v1=2000
fmt.Println(*v1)
```

### 赋值

```go
per1:=per2  per1的修改不会影响per2  per1 与 per2 的地址不一定是挨着的
var p1 * Person=&p2   则对p1或p2的修改 指向的时同一块地址 
---------------------------------------
//直接赋值
p2 := Person("name",25)
//new 申请空间
var p3*Person = new(Person)  //p3是指针，标准使用：(*p3).Name  为了方便，也可以p3.Name直接使用，底层会用指针处理 ---go的特点
 
//用&初始化  同p3的使用
var p4 *Person=&Person{"name",476}
var p4 *Person=&Person{}   赋值 使用同上p3

```

### 方法-与函数区别

方法与结构体是绑定的,作用在指定的数据类型上的。

函数入参随意

```go
声明：
type A struct {
    Num int
}
func (a A)test(){  //A 结构体有个test 方法，通过(a A)绑定  test方法与A创建的对象
	//只能作用A 结构体中，作为A的方法
    print(a.Num)
}
//调用
var val A  需要有A类型的对象
val.test()  

在方法中对对象属性的修改不回影响外部的属性，值拷贝  （a A）也作为一个入参 被值拷贝一份到方法中
```



为了提高绑定效率，绑定的时候用指针绑定

```go
func (a* A)test(){  
    print((*a).Num)   这下方法中获取的是对象的地址，不需要把对象拷贝一份
   					  在方法中对属性的修改   会影响对象的属性
}
//调用
(&obj).fun()   --也可以写成 obj.fun() 底层还是（&obj）.fun

方法中a 就是实际obj的地址  所以在方法中的操作，与直接对obj的操作是一样的
```



#### int，float也可以定义自己的方法

```go
type interger  int  重命名
func (i interger)print(){
    fmt.Println("i=",i)
}
则int就有了print方法
var i interger=10
i.print()   调用打印函数
```



#### 重新封装fmt.Println,方便打印

```go
type Stu struct{
    name string
    age int
}
func (stu *Stu)String()string{
    str := fmt.Sprintf("name=%v age=%v",stu.name,stu.age)
    return str
}
func main(){
    stu1 := Stu{
        name:"xiaoming",
        age:20,
    }
    fmt.Println(&stu1)  Println底层 就是调用重新封装的String 
}

```

#### 区别3

```go
函数调用时对入参检查
方法调用时很随意
func test01(p Person){
    fmt.Println(p.name)
}
func test02(p* Person){  必须test02(&p1)  才能调用
    fmt.Println(p.name)
}
-----
func (p Person) test03(){  值拷贝，不会影响源值
    fmt.Println(p.name)
}
p.test03()   值拷贝不会影响源属性
(&p).test03() 重点：仍然是值拷贝，不会影响源属性，不会报错
-----
func (p *Person) test03(){  地址拷贝，会影响源值
    fmt.Println(p.name)
}
p.test03()    重点：仍然是地址拷贝，会影响源属性。不会报错
(&p).test03() 地址拷贝，会影响源属性
```



### 练习

```go
可以定义空类
type myClass struct{
    //...   无任何参数的类，也可以集成方法
}

保留两位有效数字
fmt.Println("sum=",fmt.Sprintf("%.2f",sum))
fmt.Sprintf(".2f",sum)  格式化后传出string
```



### 创建struct变量时直接给指定属性赋值

```go
var stu1 = Stu{"xx",19,"yy"}  {}中数据与结构体中对应起来
stu1 := Stu{"xx",19,"yy"} 

//不依赖字段定义顺序
var stu1 = Stu{
    name:"xx"
    age:20
} 
stu1 := Stu{
    name:"xx"
    age:20
} 

----------------------创建的时候直接返回--指针
var stu5=&Stu{"xx",66} 
stu5:=&Stu{"xx",66}
stu6 :=&stu{
    name:"yy"
    age:99
}
var stu6 =&stu{
    name:"yy"
    age:99
}
直接print的时候会在对象前加&符号，
要去掉&，打印的时候需要*obj去打，*表示取值
```



### 工厂模式

相当于以前构造函数的功能



若某个包里面的sturct首字母小写，别的包无法直接通过包去调用。怎么才能用呢？

```go
package model
type student struct{ //在别的包无法直接使用
    Nmae string    里面的字段还是要大写 ，否则即使传出指针还是无法访问
    Score float32
}
如果字段也是小写怎么办？  通过在model中新增get方法，其他模块中可以获取设置score
func (s * student)getScore()float32{
    return s.score  
}
func (s * student)setScore(s float32){
    s.score=s
}

//别的包可以直接调用此函数去定义该类型的变量指针。
func NewStu(n string,s float32)*student{
    return &student{
        Name:n
        Score:s
    }
}

使用：
var stu = model.NewStu("tom",10)
*stu 可以操作赋值 或 访问
里面的字段还是要大写 ，否则即使传出指针还是无法访问

```



### vscode快捷键

左下角->快捷键  可以自定义


### 封装
+ 多个属性封到一个结构体中
+ 隐藏方法实现细节
+ 通过包实现封装 (属性首字母小写，所有的操作都往外提供方法，类似库文件的使用，其他包不知道自己怎么实现的)
#### 实现步骤
+ 将结构体、字段（属性） 首字母小写
+ 给结构体所在的包提供一个工厂模式的函数，首字母大写（构造函数）
+ 类内实现首字母大写的Set  Get 方法
#### demo
```go
package main
import  "fmt"
import _ "strings"  //has "_" can not use
import _ "unsafe"
import "person"  在GOPATH中找

func main(){
    p:=person.NewPerson("xiaoming")
    p.Setage(15)
    fmt.Println(p.Getage())
    p.Setage(20)
    fmt.Println(p.Getage())
    p.Setage(200)
    fmt.Println(p.Getage())
}

go/src/person/person.go    在person文件夹新建person.go
package person
type person struct{
    Name string
    age  int
}
//factory mode
func NewPerson(name string)*person{
    return &person{
        Name:name,  必须有, 
    }
}

//get set for outside
func (p *person)Getage()int{
    return p.age
}

func (p *person)Setage(age int){
    if(age>0 && age<150){
        p.age=age
    }else
    {
        p.age=999
    }
}

```
### 继承
在已给的类上 继承部分属性
需要给子类增加方法的时候 可以直接通过在父类 增加方法，多个子类都能直接继承此方法
type Pupil struct{  //已有小学生类
  Name string
  Age int
  Socre int
} ShowInfo  SetScore GetScore 等等方法
//怎么根据已有类 创建新的类
1. 创建基类
type Student struct{
  Name string
  Age int
  Score int
} ShowInfo  SetScore GetScore 等等方法
2. Pupil Grauute 都继承这些私有属性
type Pupil{          type Grauute{
  Student              Student//这表示匿名结构体，因为无对象
}                    }
func (p *Pupil)testing(){小学生的考考试}
func (p *Grauute)testing(){大学生的考考试}
3. 使用
```go
pupil := &Pupil{}  //创建空Pupil对象
pupil.Student.Name="tom"  //赋值方式
pupil.Student.Age=8 
//调用方法
//私有方法
pupil.testing()
//继承的方法
pupil.Student.ShowInfo()   需要--子对象.父类 才能打印出来
```
#### 继承的细节
+ 继承的基类的首字母大写的小写的字段，方法 在子类中都可以使用
+ 嵌入结构体字段 访问可以简化  
    ```go
    b.A.Name;b.A.sayhello() ;b.A.age  
    b.Name; b.sayhello(); b.age
    ```
+ 若子类与父类中都有同一属性，采取就近原则访问。子类没有才去父类找
+ 结构体嵌入两个或多个匿名结构体，若两个匿名结构体中有相同的属性或方法（同时本结构体无此名的属性或方法），在访问的时候必须指定匿名结构体的名字，否则编译报错
+ 若嵌套了有名结构体，则这种模式是组合，在组合关系中，访问组合的结构体的字段或方法时，必须带上结构体的名字
    ```go
    type A struct{
      Name string
      Age int
    }
    type C struct{
      a  A  //C结构体中包含 类型为A类型的对象
    }
    var c C
    c.a.Name  只有这样才能成功访问

    ```
+ 嵌套匿名结构体后，可以在创建结构体变量（实例）时，直接指定各个匿名结构体字段的值
    ```go
    type Goods struct{
      Name string
      Price
    }
    type Brand struct{
      Name string
      Addr string
    }
    type Tv struct{ //同时嵌入2个匿名结构体
      Goods
      Brand
    }
    tv:=Tv{  //嵌套结构体 初始化
      Goods{"电视机001",3000},
      Brand{"海尔","山东"},
    }
    tv:=Tv{  //嵌套结构体 初始化  不依赖顺序
      Goods{
        Name:"电视机001",
        Price:3000,
      },
      Brand{
        Name:"海尔",
        Addr:"山东",
      },
    }


    type Tv2 struct{ //同时嵌入2个匿名结构体 指针
      *Goods
      *Brand
    }
    tv:=Tv2{  //嵌套结构体 初始化
      &Goods{"电视机001",3000},
      &Brand{"海尔","山东"},
    }
    tv:=Tv2{  //嵌套结构体 初始化  不依赖顺序
      &Goods{
        Name:"电视机001",
        Price:3000,
      },
      &Brand{
        Name:"海尔",
        Addr:"山东",
      },
    }
    //怎么取值呢？
    fmt.Println("tv",*tv.Goods) 先找到tv.Goods地址，然后*取值
    ```
### 接口 Interface（多态是通过接口 实现的）

```go
//只要一个对象含有接口类型中的所有方法,那么这个对象就实现了这个接口  接口不需要创建对象使用
type Usb interface{
	start()
	stop()
}
type Phone struct{

}
func (p Phone)start(){
	fmt.Println("手机 开始...")
}
func (p Phone)stop(){
	fmt.Println("手机 stop...")
}

type Camera struct{

}
func (p Camera)start(){
	fmt.Println("zhaoxiangji  start...")
}
func (p Camera)stop(){
	fmt.Println("zhaoxiangji stop...")
}

type Computer struct{

}
func (c Computer)Working(usb Usb){
	usb.start()
	usb.stop()
}
func main(){
	var com Computer
	var phone Phone
	var camera Camera
	com.Working(camera)//不需要传Usb类型的对象，传phone，调Phone的start stop

}
总结：
+ 接口就是个结构体 函数结合，本身不需要实现
+ 那个类型的对象想传进来，需要实现对应于接口中的函数
+ 上层调用时，入参是接口类型，传入不同的obj，调用不同的方法
+ 这就是多态
+ 只要一个对象含有接口类型中的所有方法，那么这个对象就实现了这个接口
+ 使用接口的时候，将接口类型作为入参传入，调用的时候将可以实现接口的obj传入，接口本身类型不需要去定义对象
```

#### 特点
+ 一个自定义类型可以实现多个接口
```go
//定义两种接口类型
type Ainterface interface{
	Hello()
}
type Binterface interface{
	Say()
}
//Monster 实现两种接口
type Monster struct{
}
func (m Monster)Hello(){
	fmt.Println("monster hello()")
}
func (m Monster)Say(){
	fmt.Println("monster Say()")
}

func main(){
	var m_monster Monster
	m_monster.Say()
	m_monster.Hello()

	var a Ainterface=m_monster
	var b Binterface=m_monster
	a.Hello()
	b.Say()
}
```
+ 接口中不能有任何变量，只能是方法
type Usb interface{
	start()
	stop()
  var sss int  报错
}
+ 一个接口可以继承多个别的接口，如果要实现A接口，也必须将B C接口的方法全部实现
```go
type Binterface interface{
	test01()
}
type Cinterface interface{
	test02()
}
type Ainterface interface{//要实现A接口，要实现3个方法
	Binterface
	Cinterface
	test03()
}
type Stu struct{
}
//实现A接口的3个方法
func (m Stu)test01(){
	fmt.Println("stu implment test01()")
}
func (m Stu)test02(){
	fmt.Println("stu implment test02()")
}
func (m Stu)test03(){
	fmt.Println("stu implment test03()")
}

func main(){
	var stu Stu
	var a Ainterface=stu  //在Stu中必须将A接口的3个方法实现 才能赋值，否则无法赋值
	a.test01()
	a.test02()
	a.test03()
}
```

+ interface 类型模式是指针（引用类型），必须初始化后才能使用，否则会nil
var a Ainterface=stu   若不赋值stu，则a无法使用

+ 空接口没任何方法，所以所有的对象 都实现了空接口，我们可以把任何变量赋值给空接口
```go
type Tinterface interface{
}
func main(){
	var stu Stu
	var i int
	var t Tinterface=stu //可以将任意对象赋值给空接口
	var m = i
	var t2 interface{} = stu //interface{}===Tinterface 
}
```
+ 接口中不允许有相同的方法名
```go
type Binterface interface{
	test01()
	test03()
}
type Cinterface interface{
	test02()
	test03()
}
type Ainterface interface{//test03() 重复，会报错
	Binterface
	Cinterface
}
```
+ 什么类型实现了接口方法，什么类型才能给接口赋值
```go
type Binterface interface{
	Say()
}
type Stu struct{
}
func (m *Stu)Say(){ //*Stu  实现的Say
	fmt.Println("implment say()")
}

func main(){
	stu := Stu{}
	var usb Binterface=&stu //必须&才行，什么类型实现了Say，什么类型才能给接口赋值
}
```

#### 实践
demo
```go
//结构体 排序
package main
import (
	"fmt" 
	"sort" 
	"math/rand"
)
type Hero struct{
	Name string
	age int
}
type Heroslice []Hero //定义 Heroslice 为 Hero切片类型

//实现接口 调用库函数的时候需要实现 Sort的3个方法：Len() Less() Swap()
//都实现后 则将Heroslice类型的对象传入 sort.Sort。Sort入参就是接口，对Hero切片实现接口需要的函数，即可传入

func (hs Heroslice)Len()int{
	return len(hs)
}
func (hs Heroslice)Less(i int,j int)bool{ //升序还是降序 按年龄排序 升序
	return hs[i].age > hs[j].age  //按照 前面>后面 排序
}
func (hs Heroslice)Swap(i,j int){ //若类型一致 最后一个类型填上就行 前面的类型不需要填
	temp:=hs[i]
	hs[i]=hs[j]
	hs[j]=temp
}
//实现以上3个接口 才能调用 sort.Sort 去排序结构体

func main(){
	//定义切片 或 数组
	var intslice = []int{0,-1,10,7,99}
	//排序
	//1.冒泡
	//2.库方法
	sort.Ints(intslice)
	fmt.Println(intslice)
	//怎么对结构体排序？调用库方法
	var heros Heroslice
	for i:=0;i<10;i++{
		hero:=Hero{
			Name : fmt.Sprintf("英雄%d",rand.Intn(100)),//随机name
			age:rand.Intn(100),                         //随机age
		}
		heros = append(heros,hero)//新建的数据放到切片中
	}
	//排序前的顺序
	for _,v := range heros{
		fmt.Println(v)
	}
	//排序
	sort.Sort(heros)
	fmt.Println("排序后顺序")
	for _,v := range heros{
		fmt.Println(v)
	}
}

[-1 0 7 10 99]
{英雄81 87}
{英雄47 59}
{英雄81 18}
{英雄25 40}
{英雄56 0}
{英雄94 11}
{英雄62 89}
{英雄28 74}
{英雄11 45}
{英雄37 6}
排序后顺序
{英雄62 89}
{英雄81 87}
{英雄28 74}
{英雄47 59}
{英雄11 45}
{英雄25 40}
{英雄81 18}
{英雄94 11}
{英雄37 6}
{英雄56 0}
```

#### 接口与继承关系
A继承B  则A有B的属性 方法，如果A想学习扩展自己的能力
可以通过接口去学习，接口 是对 继承机制 的补充
```go
package main
import (
	"fmt" 
	_"sort" 
	_"math/rand"
)
type Monkey struct{
	Name string
}
func (this *Monkey)climb(){
	fmt.Println(this.Name,"生来会爬树")
}
type BirdAble interface{
	Flying()
}
type LittleMonkey struct{
	Monkey //继承Monkey的name climb 方法属性
}
//让 LittleMonkey 实现flying
func (this*LittleMonkey)Flying(){
	fmt.Println(this.Name,"通过学习会飞翔了")
}
func main(){
	//定义
	monkey := LittleMonkey{
		Monkey{
			Name:"小猴子",
		},
	}
	monkey.climb()
	//小猴子只能继承Monkey的climb方法 ，想学习游泳 飞翔
	monkey.Flying()
}
```


### 多态-（接口）
1. 多态参数 
    入参(usb Usb),传入不同入参，执行不同方法
2. 多态数组
    给Usb数组中存放Phone Camera 结构体对象
```go
type Usb interface{}
type Phone struct{}
type Camera struct{}
type Xiyiji struct{}
func main(){
	var usbArr [3]Usb   //普通的数组里面只能放一种数据类型
	usbArr[0] = Phone{} //不用接口多态的特定，无法将3种不同的结构体对象传入的
	usbArr[1] = Camera{}
	usbArr[2] = Xiyiji{}
	fmt.Println(usbArr)
}
```

#### 类型断言
```go
type Point struct{
	x int
	y int
}
func main(){
	var Aint interface{}
	var point Point = Point{1,2}
	Aint = point //变量赋值给空接口 
	//如何将 Aint 接口对象 赋值给Point 变量
	var b Point //声明变量
	//b=Aint //报错 无法将接口变量 赋值给Point变量
	b = Aint.(Point) //将接口转换成Point类型，能否成功，能成功就能赋值,类型断言
	fmt.Println(b)
}


y,okflag = x.(float64)  即使转化失败，程序不会panic
if(okflag){
  转换成功 y=
}else{
  转换失败
}
y = x.(float64)  不写flag的话，转化失败 则程序panic
```
最佳实践
```go
package main
import (
        "fmt"
        _"sort"
        _"math/rand"
)

type Usb interface{
        Start()
        Stop()
}
type Phone struct{}
type Camera struct{}
type Xiyiji struct{}
func (p Phone) Start(){
        fmt.Println("手机开始工作")
}
func (p Phone) Call(){
        fmt.Println("手机 在打电话...")
}
func (p Camera) Start(){
        fmt.Println("照相机开始工作")
}
func (p Xiyiji) Start(){
        fmt.Println("洗衣机开始工作")
}


func (p Phone) Stop(){
        fmt.Println("手机stop工作")
}
func (p Camera) Stop(){
        fmt.Println("照相机stop工作")
}
func (p Xiyiji) Stop(){
        fmt.Println("洗衣机stop工作")
}

type Computer struct{}
func (com Computer)Working(usb Usb){
        usb.Start() 如果传进来的是手机，多执行手机的call
        //第1种写法
        if phone,ok := usb.(Phone);ok==true{
          phone.Call()
        }
        //第2种写法
        phone ok := usb.(Phone)
        if(ok==true){
          phone.Call()
        }
        usb.Stop()
}
func main(){
        var usbArr [3]Usb
        usbArr[0] = Phone{}
        usbArr[1] = Camera{}
        usbArr[2] = Xiyiji{}
        var com Computer
        for _,v := range usbArr{
                com.Working(v)
        }
}

```

```go
判断传入参数是什么类型，interface{} 任意类型，泛型 空接口
func TypeJudge(items... interface{}){
  for index,x := range items{
    switch x.(type){//只能这样用.(type) 必须是 接口+switch
      case bool:
      case float64:
      case float32:
      case string:
      case int,int32,int64:
      case default:
    }
  }
}
TypeJudge(n1,n2,n3,name,addr,xx,yy)  不定数量的参数个数
```


### 文件操作
os.File contain all operation about file
os.Open(name string)(fileptr*File,errno)
os.Close(fileptr)

```go  
读文件
func main(){
	/* 
		带缓冲的读取 一次读一行
		fp,errno := os.Open("cat.txt")
		if(errno != nil){
			fmt.Printf("open fail,err=%d",errno)
		}
		defer fp.Close()
		reader := bufio.NewReader(fp)
		
		str,newerr := reader.ReadString('\n')//一次读一行
		for{//死循环
			if(newerr==io.EOF){
				//读到文件末尾
				break
			}
			fmt.Print(str)
			str,newerr = reader.ReadString('o')
		}
	*/
	//若文件比较小 可以一次性读取 返回byte[]
	content,err := ioutil.ReadFile("cat.txt")
	if(err!=nil){
		log.Fatal(err)  //有错误 打印错误
	}
	fmt.Printf("content\n%s",content)
	fmt.Printf("%v",content) // byte 数组打印
  fmt.Printf("%v",string(content)) //byte->string
}
```

写文件
```go
func main(){
	fp, err := os.OpenFile("test.txt",os.O_WRONLY|os.O_CREATE,0777 ) 
	if err != nil {
		log.Fatal(err)
	}
	defer fp.Close()
	//write use * Writer 
	writer := bufio.NewWriter(fp) 
	for i:=0;i<5;i++{
		writer.WriteString("nnnn\n")
	}
	//writer 带缓存的，需要flush去更新 否则不会更新
	writer.Flush()
}

os.O_TRUNC：打开已有文件，清空文件内容
os.O_WRONLY|os.O_APPEND:不改动之前的内容，追加新内容
```


先读后写
```go
func main(){
	//O_TRUNC 清空已有内容
	fp, err := os.OpenFile("test.txt",os.O_RDWR|os.O_APPEND,0777 ) 
	if err != nil {
		log.Fatal(err)
	}
	fmt.Print("open success\n")
	defer fp.Close()
	//先读 后写
	reader := bufio.NewReader(fp)
	str,newerr := reader.ReadString('\n')//一次读一行
	if newerr!=nil{
		log.Fatal(newerr) //读错，显示错误日志
	}
	fmt.Println("read 1 hang",str)
	for{//死循环
		if(newerr==io.EOF){
			//读到文件末尾
			break
		}
		fmt.Print(str)
		str,newerr = reader.ReadString('\n')
	}
	//write use * Writer 
	writer := bufio.NewWriter(fp) 
	for i:=0;i<5;i++{
		writer.WriteString("mmmmmm\n")
	}
	//writer 带缓存的，需要flush去更新 否则不会更新
	writer.Flush()
}
```

copy实现：
```go
func main(){
	fp, err := os.OpenFile("test.txt",os.O_RDONLY,0777 ) 
	if err != nil {
		log.Fatal(err)
	}
	defer fp.Close()

	newfp, newerr := os.OpenFile("cat.txt",os.O_WRONLY,0777 )
	if newerr != nil {
		log.Fatal(newerr)
	} 
	defer newfp.Close()

	//先读 后写 
  //use reader to read writer to write
	reader := bufio.NewReader(fp)
	str,newerr := reader.ReadString('\n')//一次读一行
	if newerr!=nil{
		log.Fatal(newerr) //读错，显示错误日志
	}

	writer := bufio.NewWriter(newfp) 
	fmt.Println("read 1 hang",str)
	for{//死循环
		writer.WriteString(str)
		if(newerr==io.EOF){
			//读到文件末尾
			break
		}
		fmt.Print(str)
		str,newerr = reader.ReadString('\n')
	}
	writer.Flush() //flush 
```

## 命令行参数
```go
func main(){
	fmt.Println("命令行参数有",len(os.Args))  //参数个数
	for i ,v  := range(os.Args){
		fmt.Printf("args[%v]=%v\n",i,v)
	}
}
go run .\test.go 11 22 333 以空格分割
```
### flag包解析命令行参数
```go
//常用，解析命令行参数
func main(){
var user string 
var pwd string
var host string
var port int
flag.StringVar(&user,"u","","用户名，默认为空")
flag.StringVar(&pwd,"pwd","","密码，默认为空")
flag.StringVar(&host,"h","localhost","用户名，默认为空")
flag.IntVar(&port,"port",3366,"用户名，默认3366")

flag.Parse() //解析命令行参数
fmt.Printf("user=%v pwd=%v host=%v port=%v",user,pwd,host,port)
}
go run .\test.go -u jl -pwd 1234 -h hostname -port 8888
user=jl pwd=1234 host=hostname port=8888
go run .\test.go  未传，则保持默认值
user= pwd= host=localhost port=3366
```

## json
web编程中的应用
序列化（转化成json格式）

```go
package main
import (
	"fmt" 
	"encoding/json"
)
type  Monster struct{
	Name string;
	Age int;
	Birthday string;
	Sal int;
	Skill string;
}
//结构体 序列化
func test(){
	mons := Monster{
		Name:"牛魔王",  首字母大写 才能在json包中使用，否则无法使用
		Age:20,
		Birthday:"2022-2-2",
		Sal:9000,
		Skill:"牛魔掌",
	}
	data,err :=  json.Marshal(&mons) //返回byte 数组
	if(err != nil){
		fmt.Println("序列号错误")
	}
	fmt.Println(string(data)) //byte数组 转成string 打印
}
//map 序列化
func testMap(){
	var a map[string]interface{}
	a = make(map[string]interface{}) 空接口可以传任何数据类型进去
	var arr [5]int = [5]int{1,2,3,4,5}
	a["name"]=arr
	a["age"] = 90
	a["skill"]=arr
	fmt.Println(a)
	data,err :=  json.Marshal(&a) //map传入的时候有无& 都可以 返回byte 数组
	if(err != nil){
		fmt.Println("序列号错误")
	}
	fmt.Println(string(data)) //byte数组 转成string 打印
}
//切片序列化
func testSlice(){
	var slice[] map[string]interface{} //切片中 包含多个map
	var m1 map[string]interface{}
	m1 = make(map[string]interface{})  //map使用要make
	m1["name"] = "jack"
	m1["age"] = 999
	m1["addr"] = "上海"
	slice = append(slice,m1) //将m1 添加到slice中
	m2 := make(map[string]interface{})  //map使用要make
	m2["name"] = "tom"
	m2["age"] = 888
	m2["addr"] = "nanjing"
	slice = append(slice,m2) //将m1 添加到slice中
	data,err :=  json.Marshal(&slice) //map传入的时候有无& 都可以 返回byte 数组
	if(err != nil){
		fmt.Println("序列号错误")
	}
	fmt.Println(string(data)) //byte数组 转成string 打印
}
func main(){
	testMap()
}

```

反序列化（将json 字符串 转成...）
```go
type  Monster struct{
	Name string;
	Age int;
	Birthday string;
	Sal int;
	Skill string;
}
func unmashal(){
	//模拟获取到次字符串
	str := "{\"Name\":\"牛魔王\",\"Age\":20,\"Birthday\":\"2022-2-2\",\"Sal\":9000,\"Skill\":\"牛魔掌\"}"
	fmt.Println(str)
	var mons Monster
	err:=json.Unmarshal([]byte(str),&mons)  先转成字节数组传入
	if(err!=nil){
		fmt.Println("re marshal fail")
	}
	fmt.Println(mons)
}
将字符串 反序列化成 Monster类型的对象
```

## 单元测试
cal.go
```go
package main
import (
	"fmt" 
)

//在工作中怎么测试确认 函数的执行结果是否正确
func AppUpper(n int)int {
	res:=0
	for i:=0;i<=n;i++{
		res+=i
	}
	return res
}
//传统方法就是调用 测试
func test(){
	res:=AppUpper(10)  //check AppUpper
	if(res==55){
		fmt.Println("res is right")
	}else{
		fmt.Println("res is wrong")
	}
}
//新的测试函数的方法，testing 测试框架，可以做单元测试 性能测试
//测试函数需要写 测试用例 testcase
func main(){
	test()
}

```

cal_test.go
```go
//专用测试test.go中的appupper函数的程序
package main
import (
	"fmt" 
	"testing" //框架包
)
//implement Test TestXxx  第一个字母必须大写
func TestAppUpper(t *testing.T){
	res:=AddUpper(10)
	if(res!=55){
		t.Fatalf("执行出错") //程序自动退出
	}
	t.Logf("执行正确")
}

func TestHello(t *testing.T){
	fmt.Println("TestHello 被调用")
}
```

## goroutine
go 的主线程(即进程) 中可以跑很多协程--goroutine（轻松起上万个）--协程就类似 linuxc 中说的线程
```go
import (
	"fmt" 
	"strconv"
	"time"
)
func test(){
	for i:=0;i<10;i++{
		fmt.Println("test() hello world"+strconv.Itoa(i))
		time.Sleep(time.Second)
	}
}
func main(){
	go test()  //new goroutine exec test
	for i:=0;i<10;i++{
		fmt.Println("main() hello world"+strconv.Itoa(i))
		time.Sleep(time.Second)
	}
}
//main and test 同时执行
```

### 细节
#### 执行顺序
主线程若结束，则整个程序结束
### MPG模式
m:主线程  p：上下文  g：协程

### 其他
NumCPU()  返回当前机器物理cpu个数
GOMAXPROCS(n)  设置同时执行的最大cpu个数，并返回先前的设置
```go
func main(){
	cnt:=runtime.NumCPU()
	fmt.Println("current PC cpu num=",cnt)
	runtime.GOMAXPROCS(cnt-1)  //设置占用几个cpu跑go程序
}
```


## 管道
不同 goroutine之前 如何通讯

```go
package main
import (
	"fmt" 
	"time"
)
//需求：计算1-200 所有数据的阶乘，结构放map
//全局 每个协程都能访问
var(
	myMap = make(map[int]int,10)
)
func test(n int){
	res:=1
	for i:=1;i<=n;i++{
		res=res*i
	}
	myMap[n]=res //问题1：不允许并发写map
}
func main(){
	for i:=1;i<=200;i++{
		go test(i)
	}
	//问题2：到底怎么处理 到底休眠几秒
	time.Sleep(time.Second*10) //休眠10s
	fmt.Println(myMap)
}
```
> go build -race .\xiecheng.go   -race 编译选项 可以查看exe中 发生竞争 的个数
---
PS E:\jl\04-golang\src\go_coed> .\xiecheng.exe
==================  在0x00c00006e4b0位置两个协程想同时写入 出错了
WARNING: DATA RACE
Write at 0x00c00006e4b0 by goroutine 8:
  runtime.mapassign_fast64()
      C:/Program Files/Go/src/runtime/map_fast64.go:93 +0x0
  main.test()
      E:/jl/04-golang/src/go_coed/xiecheng.go:18 +0x70
  main.main.func1()
      E:/jl/04-golang/src/go_coed/xiecheng.go:22 +0x39

Previous write at 0x00c00006e4b0 by goroutine 19:

====================================
WARNING: DATA RACE
Read at 0x00c000320830 by main goroutine:

Previous write at 0x00c000320830 by goroutine 98:
  main.test()
      E:/jl/04-golang/src/go_coed/xiecheng.go:18 +0x7c
  main.main.func1()
      E:/jl/04-golang/src/go_coed/xiecheng.go:22 +0x39

Goroutine 98 (finished) created at:
  main.main()
      E:/jl/04-golang/src/go_coed/xiecheng.go:22 +0x77
==================
Found 2 data race(s)
---

优化上面的程序
1.全局变量 加锁--低水平的做法
2.管道
```go
package main
import (
	"fmt" 
	"time"
	"sync"  //同步
)
var(
	myMap = make(map[int]int,1)
	//互斥锁
	lock sync.Mutex
)
func test(n int){
	res:=1
	for i:=1;i<=n;i++{
		res=res*i
	}
	lock.Lock() //写之前加锁
	myMap[n]=res //问题1：不允许并发写map
	lock.Unlock() //写之后解锁
}
func main(){
	for i:=1;i<=200;i++{
		go test(i)
	}
	//问题2：到底怎么处理 到底休眠几秒
	time.Sleep(time.Second*10) //休眠10s
	lock.Lock() //du之前加锁
	fmt.Println(myMap)
	lock.Unlock() //du之后解锁
}
读也得加锁  否则读到某个位置的时候，另一个线程在写此位置
当前线程在读此位置

按理来说：10s 足够所有的协程执行完了，读的时候不应该出现竞争，实际运行中，会出现竟态--我们设计程序知道10s 协程会执行完，主线程并不知道。底层还是会出现资源争夺
```

以上用 锁，并没有解决问题2
```go
//使用管道 解决 不知道该 主线程延时多久

```

### 管道特点
1. 本质是个队列，数据先进先出
2. 线程安全，多goroutine 访问，无需加锁。go底层决定
3. 有类型，string类型的管道，只能存string类型的数据
4. map可以自动扩容，但是管道不可以
5. len(intChan)  当前管道有几个数据
6. cap(intChan)  总容量是多少
7. 关闭后只能读不能写
#### 声明
```go
var varname chan datatype
exp:
var intChan chan int  (存int数据)
var mapChan chan map[int]string  (存map 数据)
var perChan chan Person  (存 Person 数据)
声明成空接口类型后，取出时 需要var：=(<-intchan).(Cat)  断言转化 才可以
```

#### 关闭-遍历
channel 关闭后，无法再向其中写数据，但是仍然可以读
遍历 for-range：
遍历时如果channel没有关闭，则报deadlock
遍历时如果已经关闭，则正常遍历

```go
func main(){
	intChan := make(chan int,100)
	for i:=0;i<100;i++{
		intChan<-i*2
	}
	close(intChan)//关闭管道
	// outChan1 := <-intChan  先进先出
	// outChan2 := <-intChan
	// outChan3 := <-intChan
	// fmt.Println(outChan1,outChan2,outChan3)
	//for range 遍历
	//1. 不能使用  因为intChan长度会一直减小 无法完整遍历
	// for i:=0;i<len(intChan);i++{
	// }
	//2. for range 遍历---正常遍历方法
	for v:= range intChan{
		fmt.Println(v)
	}
}
```

#### 协程与管道配合
```go
//一个协程读 一个协程写
//两个管道  一个放数据 一个放bool
package main
import (
	"fmt" 
	"time"
	_"sync"  //同步
)

func writeData(intChan chan int){
	for i:=1;i<=50;i++{
		intChan<-i //放入数据
		fmt.Println("writedata=%v",i)
		time.Sleep(time.Second)
	}
	close(intChan)//写完就关闭  不影响读数据
}
func readData(intChan chan int,exitChan chan bool){
	for{
		v,ok:=<-intChan  //读不出来阻塞 读取失败则退出
		if !ok{
			break
		}
		fmt.Println("readdata=%v",v)
		//time.Sleep(time.Second)
	}
	//读完了
	exitChan<-true
}
func main(){
	intChan := make(chan int,50)
	exitChan := make(chan bool,1)
	go writeData(intChan)
	go readData(intChan,exitChan)
	//方法1，阻塞主线程  不好确定到底延时多久
	//time.Sleep(50*time.Second)
	//方法2：阻塞
	for{
		_,ok := <-exitChan
		if !ok{
			break
		}
	}
}

```

在主线程中直接使用管道，写入直接读
```go
func main(){
	intChan := make(chan int,50)
	intChan<-3
  close(intChan)  //关闭之后才能读
	for {
		v,ok := <-intChan
		if !ok{
			fmt.Println("read fail")  所有的都读完 才会进入此分支
			break
		}
		fmt.Println("v=",v)
	}
}

```

//协程中写数据  主线程读管道数据  不会直接退出
```go
func writeData(intChan chan int){
	for i:=1;i<=50;i++{
		intChan<-i //放入数据
		fmt.Println("writedata=%v",i)
		time.Sleep(time.Second)
	}
	close(intChan)//写完就关闭  不影响读数据
}
func main(){
	intChan := make(chan int,3)
	go writeData(intChan)
	for{
		v,ok:=<-intChan  //读不出来阻塞 读取失败则退出
		if !ok{
			fmt.Println("read fail")
			break
		}
		fmt.Println("v=%v",v)
	}
}
```

//怎么实现一个协程放数据 多个协程处理数据,以下代码有问题  
```go
package main
import (
	"fmt" 
)
func putThread(numChan chan int){
	for i:=1;i<=100;i++{
		numChan<-i
	}
	close(numChan)
	fmt.Println("close numChannel")
}
func CalThread(numChan chan int,resChan chan map[int]uint64){
	var res uint64
	res=0
	//从numChan取数 
	for{
		v,ok :=<-numChan
		if !ok{
			fmt.Println("in calthread read numchannel err")
			break
		}
		for i:=1;i<=v;i++{
			res+=uint64(i)
		}
		val:=map[int]uint64{
			v:res,
	    }
		resChan<-(val)
	}	
	
}

func main(){
	numChan := make(chan int,10000)
	resChan := make(chan map[int]uint64,10000)
	go putThread(numChan)
	for i:=0;i<1;i++{
		go CalThread(numChan,resChan)
	}

	for{
		v,ok := <-resChan
		if !ok{
			fmt.Println("read res channel err")
			close(resChan)
			break
		}
		fmt.Println(v)
	}
}
```

再优化后 正常的程序
```go
//借助exitChan 管道，知道多会再去关闭 resChan，不关闭一直读 就会出错
package main
import (
	"fmt" 
)
func putThread(numChan chan int){
	for i:=1;i<=100;i++{
		numChan<-i
	}
	close(numChan)
	fmt.Println("close numChannel")
}
func CalThread(numChan chan int,resChan chan map[int]uint64,exitChan chan bool){
	var res uint64
	res=0
	//从numChan取数 
	for{
		v,ok :=<-numChan
		if !ok{
			fmt.Println("in calthread read numchannel err")
			break
		}
		for i:=1;i<=v;i++{
			res+=uint64(i)
		}
		val:=map[int]uint64{
			v:res,
	    }
		resChan<-(val)
	}
	exitChan<-true
}

func main(){
	numChan := make(chan int,100)
	resChan := make(chan map[int]uint64,100)
	exitChan := make(chan bool,4)
	go putThread(numChan)
	for i:=0;i<4;i++{
		go CalThread(numChan,resChan,exitChan)
	}
	//阻塞程序
	for i:=0;i<4;i++{
		<-exitChan
	}
	//4个线程都退出，关闭存结果的 channel
	close(resChan)
	//遍历
	for{
		v,ok := <-resChan
		if !ok{
			fmt.Println("read res channel over")
			break
		}
		fmt.Println(v)
	}
}
```

#### 管道的阻塞机制
写死锁：
给的管道大小10，一直写20个数据，则会阻塞 导致deadlock
若有协程读 就不会deadlock了，底层发现有人读（即使2s读一次），就会等别人读。不会deadlock---异步机制

```go
func writeData(intChan chan int){
	for i:=1;i<=50;i++{
		intChan<-i //放入数据
		fmt.Println("writedata=%v",i)
	}
	close(intChan)//写完就关闭  不影响读数据
}
func readData(intChan chan int,exitChan chan bool){
	for{
		v,ok:=<-intChan  //读不出来阻塞 读取失败则退出
		if !ok{
			break
		}
		fmt.Println("readdata=%v",v)
		time.Sleep(time.Second)
	}
	//读完了
	exitChan<-true
}
func main(){
	intChan := make(chan int,10)
	exitChan := make(chan bool,1)
	go writeData(intChan)
	go readData(intChan,exitChan)
	for{
		_,ok := <-exitChan
		if !ok{
			break
		}
	}
}
writedata=%v 1
writedata=%v 2
writedata=%v 3
writedata=%v 4
writedata=%v 5
writedata=%v 6
writedata=%v 7
writedata=%v 8
writedata=%v 9
writedata=%v 10
writedata=%v 11   阻塞，channel 大小10 发现有人读 等读完一个 再去写一个
readdata=%v 1     阻塞若发现没人读，直接deadlock
readdata=%v 2
writedata=%v 12
readdata=%v 3
writedata=%v 13
readdata=%v 4
writedata=%v 14
readdata=%v 5
writedata=%v 15
readdata=%v 6
writedata=%v 16
```

求1-1000之内的素数,一个线程放数据  别的4个线程取数据
```go
func putNum(numChan chan int){
        for i:=1;i<=1000;i++{
                numChan<-i
        }
        close(numChan)
        fmt.Println("close numChannel")
}
func primeNum(intChan chan int,primeChan chan int,exitChan chan bool){
    var flag bool
    for{
        num,ok:=<-intChan
        if !ok{
            break //intChan is empty
        }
        flag=true
        //judge if num is primenum
        for i:=2;i<num;i++{
            if(num%i == 0){
                //num not primenum
                flag=false
                break;
            }
        }
        if flag{
            //put num to primeChan
            primeChan<-num
        }
    }
    fmt.Println("one thread exit,can not get intChan")
    //can not directly close primtChan--other thread may can put data
    exitChan<-true
}
func main(){
    intChan := make(chan int,1000)
    //save prime num
    primeChan := make(chan int,2000)
    //标识退出的管道
    exitChan := make(chan bool,4)

    //open thread put 1-8000
    go putNum(intChan)

    //open 4 thread ,get num and judge if is primenum
    for i:=0;i<4;i++{
        go primeNum(intChan,primeChan,exitChan)
    }

    //阻塞 main thread
    for i:=0;i<4;i++{
        <-exitChan
    }
    //all primeNum thread exit,can close primeChan
    close(primeChan)

    //遍历
    for{
        v,ok:=<-primeChan
        if !ok{
            fmt.Println("travers over")
            break
        }
        fmt.Printf("%v ",v)
    }
}

```

#### 管道特点
1. 可以声明成只读/只写
```go
只读只写 不是类型，是个属性，一般用作函数入参 限制函数权限
//只能进行写操作 chanx
func send(chanx chan<-){
  chanx<-m
}
//只能进行 读 操作 chanx
func send(chanx chan<-){
  n:=<-chanx
}
func main(){
	//1.default declare chennel is read write,2 directions
	//var chan1 chan int //can read and write
	//2.only write
	var chan2 chan<- int //chan<-
	chan2 = make(chan int,n)
	//3.only read
	var chan3 <-chan int //<-chan
	//4 useful
	send(chanx)
	recv(chanx)
}
```
2. select 
可以解决从管道取数据的阻塞问题
```go

func main(){
	intChan := make(chan int,10)
	for i:=0;i<10;i++{
		intChan<-i
	}
	stringChan := make(chan string,5)
	for i:=0;i<5;i++{
		stringChan<-"hello"+fmt.Sprintf("%d",i)
	}
	//不知道管道多会才能关闭 而遍历 又需要在 管道关闭之后

	//所以 遍历的时候 会阻塞

	//则可以使用select 解决,不需要关闭 也可以遍历。会把数据取出来
	//for range 遍历的时候没有<- 取出动作 只是遍历
	for{
		select{
		case v:= <-intChan://若intchan一直不关闭，则先取后面的case，不会阻塞到这里
			fmt.Println("from intChan read %v",v)
		case v:= <-stringChan:
			fmt.Println("from stringchan read %v",v)
		default:
			fmt.Println("nothing can be read")
			return
		}
	}
}
```

3. 线程退出不要影响主程序
```go
package main
import (
	"fmt" 
	"time"
)
func sayHello(){
	for i:=0;i<10;i++{
		time.Sleep(time.Second)
		fmt.Println("hello world")
	}
}

func test(){
	defer func(){
		//捕获panic
		if err:=recover(); err != nil {
			fmt.Println("test 发生错误",err)
		}
	}()  //直接调用匿名函数
	var mymap map[int]string
	mymap[0]="golang"//error
}

func main(){
	go sayHello()
	go test() //线程出现问题 整个程序退出,优化线程 不要影响主程序

	for i:=0;i<10;i++{
		time.Sleep(time.Second *2)
		fmt.Println("in main ")
	}
}
```

### 统计程序运行时间
start := time.Now().Unix()  
...
	end := time.Now().Unix() 


## 反射
适配器函数
写框架的时候 会用到

## 网络编程

客户端服务器通讯
ser.go
```go
package main
import "fmt"
import "net"
import "io"
func threadForClient(conn net.Conn){
    //处理某个客户端的数据
    defer conn.Close() //函数退出 一定要退出
    for{
        buf:=make([]byte,1024)
        //阻塞 等待客户端发来数据。若cli退出，也会收到返回值 io.EOF
        n,err:=conn.Read(buf) //读clinet传来的数据
        if err==io.EOF{ //只有客户端退出 才会收到此 EOF标志
            fmt.Println("client had  exit")
            break
        }
        //show client meg,只显示到n 否则buf中后面的数据全部显示
        fmt.Print(string(buf[:n])) //不用换行，客户端发来就是带换行的
    }
}
func main(){
    fmt.Println("ser正在监听...")
    listen,err := net.Listen("tcp","0.0.0.0:8888")
    if err != nil{
        fmt.Println("listen fail")
        return
    }
    //listen is a socket
    //in case forget colse
    defer listen.Close()
    //for wait cli connect
    for{
        fmt.Println("wait for connect...")
        conn,err:=listen.Accept()  //阻塞直到cli链接
        if err!=nil{
            fmt.Println("Accept err")
        }
        //get client ip and port
        fmt.Printf("accept success con=%v  ip=%v",conn,conn.RemoteAddr().String())
        go threadForClient(conn) //每次有客户端链接 建立一个 协程处理
    }
}
```

cli.go
```go
package main
import "fmt"
import "net"
import "bufio"
import "os"
func main(){
    conn,err:=net.Dial("tcp","0.0.0.0:8888")
    if err!=nil{
        return
    }
    //function1:发送单行数据 创建句柄 来自stdin
    reader:=bufio.NewReader(os.Stdin)
    for{
    //from stdin get data  get一行
    line,err := reader.ReadString('\n')
    if err!=nil{
        return
    }

    //send data to ser ,先转成byte 切片再转发
    n,err:=conn.Write([]byte(line))
    if err!=nil{
        return
    }
    fmt.Println("client send ok len=",n)

    }
}
```


## redis数据库
remote directory server 服务器
下载redis-server
直接执行redis-server 启动服务端
netstat -anp |grep redis 可以看到6379号端口已处于listen状态
直接执行redis-cli 运行客户端
```c
127.0.0.1:6379> set key1  hello  往数据库存入key1:"hello"
OK
127.0.0.1:6379> get key1    取值
"hello"
//redis 默认有16个数据库 默认打开的0号数据库
//切换数据库 1号
127.0.0.1:6379>  select 1
OK
127.0.0.1:6379[1]> get key1  1号中无key1
(nil)
127.0.0.1:6379[1]>  select 0
OK
127.0.0.1:6379> get key1
"hello"
dbsize ：查看当前数据库有几对数据（key-val）
flushdb:清空当前所在的数据库
flushall：清空16个数据库
```

redis支持的数据类型：
string(字符串) Hash(哈希) List(列表) Set(集合) zset(有序集合)
存在于内存中，一旦服务器停止就没了。

其他命令：
mset 同时设置一至多对数据
mget 同时获取多对数据

存放 哈希 数据（结构体）---键 不可重复
`hset user1 name smith`
`hset user1 age 30`
`hset user1 job golangcoder`
`hget user1 name`  获取name
`hgetall user1`   一次性获取所有的数据
`hdel user1`  从库删除
`hmset user2 name jerry age 20 job "java coder"`  有空格用双引号包起来
`hmget user2 name age job`  一次性获取三个属性
`hexists key field`  查看key的field属性是否存在  返回1/0 存在/不存在

List 操作：
lpush citys jiagang xiangfen fencheng
（lpush key value [value ...]）
lrange citys 0 -1 (全部取出)
127.0.0.1:6379> lrange citys 0 -1  取出来是相反的顺序
1) "fencheng"
2) "xiangfen"
3) "jiagang"
lpush：左边插入
rpush：右边插入
lrange：从左边取出
lpop：从左边弹出一个数据  List中会少一个
rpop: 从右边弹出一个数据 
del citys  删除List 对象
llen  查看长度

Set 操作：
元素是无序，且不能重复的
127.0.0.1:6379> sadd email 642@qq.com  插入
127.0.0.1:6379> smembers email  查看
srem 删除指定值

### golang 操作redis
下载库（golang 操作redis的库）
在 /root/go（GOPATH）:执行 go get github.com/garyburd/redigo/redis
则可以看到 /root/go/src/github.com 表示库安装成功

```go
//操作string 类型redis
package main
import (
    "fmt"
    "github.com/garyburd/redigo/redis" //import redis package
)
func main(){
    //connect
    coon,err := redis.Dial("tcp","127.0.0.1:6397")
    if err!=nil{
       fmt.Println("Dial fial",err)
       return
    }
	defer coon.Close() //记住最后一定关闭库 
	_,err = coon.Do("set","name","tom")
	if err!=nil{
		fmt.Println("set err",err)
	}
	//返回的res是空接口，需要转换,怎么转换呢
	res,err := coon.Do("get","name")
	if err!=nil{
		fmt.Println("get err",err)
	}
	//namestring := res.(string) 类型断言 不能用
	//redis库提供了方法  直接将get 结果转成 string
	res,err := redis.String(coon.Do("get","name"))
	if err!=nil{
		fmt.Println("get err",err)
	}
    fmt.Println("connect success",coon)
}

```

```go
//operation hash type data
_,err = coon.Do("hset","user01","name","jack")
_,err = coon.Do("hset","user01","age",18)  //18 写成string 也可以，因为入参是空接口类型 
//get data
r1,err = redis.String(coon.Do("hget","user01","name"))
r2,err = redis.Int(coon.Do("hget","user01","age"))  //结果是年龄，数字

//batch operation set get data
_,err = coon.Do("Mset","name","user01","addr","beijing")
res,err = redis.Strings(coon.Do("Mget","name","age"))
redis.Strings  
res = 切片 
for i,v := range res{  
}
```

//List 操作
查表

## 数据结构 算法 
复杂数据转成稀疏数组
怎么存
```go
package main
import (
    "fmt"
)

type ValNode struct{
    row int
    col int
    val int
}

func main(){
    var chessMap[11][12]int
    chessMap[1][2]=1 //black
    chessMap[2][3]=2 //white

    for _,v:=range chessMap{
        for _,v2 :=range v{
            fmt.Printf("%d\t",v2)
        }
        fmt.Println()
    }
    //how to save above data
    //1 trave chessMap ,if cur pos not equl 0,save to node
    var sparseArr []ValNode
    //标准的稀疏数组 首先要表示原始行列规模,表示其他位置 都为0
    valnode := ValNode{
        row:11,
        col:12,
        val:0,
    }
    sparseArr=append(sparseArr,valnode)

    for i,v:=range chessMap{
        for j,v2:=range v{
            if v2!=0{
                //create ValNode
                valnode := ValNode{
                    row:i,
                    col:j,
                    val:v2,
                }
                sparseArr=append(sparseArr,valnode)//append data to res
            }
        }
    }
    //output sparseArr
    for _,v2:= range sparseArr{
        fmt.Printf("row=%d,col=%d,val=%d\n",v2.row,v2.col,v2.val)
    }
}

```

队列实现： 环形数组
```go
package main
import (
    "fmt"
    "os"
    "errors"
)

type Queue struct{
    maxsize int
    array [5]int
    front int //head
    rear int  //tail
    length int //num of data 没有这个也行，计算太复杂了
}

func (this *Queue)AddQueue(val int)(err error){
    if(this.length==this.maxsize){
        return errors.New("queue full")
    }
    this.array[this.rear]=val
    this.rear=((this.rear+1)%this.maxsize)
    this.length++
    return //直接return  相当于 return err  因为err是 nil
}
func (this *Queue)GetQueue()(val int,err error){
    if(this.length==0){
        return -1,errors.New("queue empty") //err 默认为空，如果发生错误 这里就赋值上去了
    }
    this.length--
    val=this.array[this.front]
    this.front=((this.front+1)%this.maxsize)
    return val,err//不给err赋值，就是空的
}

//this.front do not contains head val
func (this*Queue)showQueue(){
    for i:= 0;i<this.length;i++{
        fmt.Printf("arrayy[%d]=%d\t",this.front,this.array[this.front])
        this.front++
        this.front=this.front%this.maxsize
    }
    fmt.Println()
}

func main(){
    queue := &Queue{
        maxsize:5,
        front:0,
        rear:0,
        length:0,
    }
    var key string
    var val int
    for{
        fmt.Println("1.add 2.get 3.show 4.exit")
        fmt.Scanln(&key)
        switch key{  //case后不需要break 也可以跳出，与c 不一样
        case "add":
            fmt.Println("input data to Queue")
            fmt.Scanln(&val)
            err:=queue.AddQueue(val)
            if err!=nil{
                fmt.Println(err.Error()) //注意返回的是error类型数据，需要.Error()  才可以将错误字符串取出 注意有() 才可以
            }
        case "get":
            val,err:=queue.GetQueue()
            if err!=nil{
                fmt.Println(err.Error())
            }
            fmt.Printf(" get val=%d\n ",val)
        case "show":
            queue.showQueue()
        case "exit":
            os.Exit(0)
        }
    }
}

```

剑指 Offer 46. 把数字翻译成字符串--类似青蛙跳台阶问题
f(n) = f(n-1) + f(n-2)
只需要知道dp[0]和dp[1]，然后递推就好了。这里的变化是，能不能跳两个需要判断一下。
又是动态规划
```c
int translateNum(int num) {
    vector<int> dp(100);
    dp[0]=1;
    string src=to_string(num);
    string subst=src.substr(0,2);
    if(subst>="10" && subst<="25")//dp[1]的计算  到1位置有几种方法 能转字符串
    {
        dp[1]=2; //是否能组成有效子串 能组成 则有两种方法
    }else
    {
        dp[1]=1;//无法组成有效字串  只有一种方法转化
    }
    for (int i = 2; i < src.size(); ++i) {
        subst=src.substr(i-1,2);//获取字串  从什么位置开始  数多少个组成子串
        if(subst>="10" && subst<="25")
        {
            dp[i]=dp[i-1]+dp[i-2];
        }else
        {
            dp[i]=dp[i-1];
        }
    }
    return dp[src.size()-1];
}
```

剑指 Offer 42. 连续子数组的最大和
```c
int maxSubArray(vector<int>& nums) { //dp[i] 保存`到i位置`为止得`连续`最大值，如果后面碰到负数，别担心算出来得最大值已经被存在maxans中了
      if(nums.size()==0)
      return 0;
      vector<int> dp(nums.size());
      dp[0]=nums[0];
      int maxans=dp[0];
      for(int i=1;i<nums.size();i++)
      {
          dp[i]=max(dp[i-1]+nums[i],nums[i]);//如果之前dp[i-1]是负值，则重新从当前位置开始计算dp[i] 
          maxans=max(maxans,dp[i]);//需要借助额外变量 maxans 去存截止 i位置得最大值，不能破坏dp[i] 因为计算dp[i+1]得时候要用dp[i]
      }
      return maxans;    //maxans 不一定以第i位为止 中间有最大值就提前被保存了
  }
```

剑指 Offer 38. 字符串的排列
回溯法--循环中调用dfs 递归
还是不怎么理解
```c
class Solution {
public:
    vector<string> permutation(string s) {
        dfs(s, 0);
        return res;
    }
private:
    vector<string> res;//类内的private 变量，类内都可以访问
    void dfs(string s, int x) { //x 表示第x 位置 放什么数据
        if(x == s.size() - 1) {
            res.push_back(s);                       // 添加排列方案
            return;
        }
        set<int> st;
        for(int i = x; i < s.size(); i++) {
            if(st.find(s[i]) != st.end()) continue; // 重复，因此剪枝
            st.insert(s[i]);        //将当前字母放入set 则后面不会再出现当前字母
            swap(s[i], s[x]);                       // 交换，将 s[i] 固定在第 x 位
            dfs(s, x + 1);                          // 开始 固定第 x + 1 位字符
            swap(s[i], s[x]);                       // 恢复交换
        }
    }
};

全程图是怎样的？参考此链接
https://leetcode.cn/problems/zi-fu-chuan-de-pai-lie-lcof/solutions/178988/mian-shi-ti-38-zi-fu-chuan-de-pai-lie-hui-su-fa-by/
```