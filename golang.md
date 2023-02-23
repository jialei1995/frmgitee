## 新需求->怎么解决

新浪页面 怎么实现局部刷新？

传统网站都是 点击链接整个页面刷新，浪费带宽，有些用户打开网页不会直接往下划，只会看第一个界面。

学习新技术，怎么学？

1.原理 2.基本语法 3.简单案例 4.怎么规范使用 使用陷阱 偏细节

## 语言特点

go=c+python   静态编译语言的安全性能  动态语言的开发维护



fmt.Scanln(&a);  给a赋值 从键盘



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



## map

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

#### map的value 

管理复杂数据类型用struct 比直接用map更好

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

