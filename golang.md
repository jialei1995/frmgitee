## 新需求->怎么解决

新浪页面 怎么实现局部刷新？

传统网站都是 点击链接整个页面刷新，浪费带宽，有些用户打开网页不会直接往下划，只会看第一个界面。

学习新技术，怎么学？

1.原理 2.基本语法 3.简单案例 4.怎么规范使用 使用陷阱 偏细节

## 语言特点

go=c+python   静态编译语言的安全性能  动态语言的开发维护

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
  func myfun(funname func(int,int)int,num1 int,num2 int){
    return funname(num1,num2)
  }
}
```