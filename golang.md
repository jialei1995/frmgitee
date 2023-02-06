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
