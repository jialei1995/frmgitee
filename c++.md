1、c和c++的区别
2、对面向过程和面向对象的理解
3、什么是类
4、什么是封装
5、const的作用
6、static的作用
7、构造函数、拷贝构造函数
8、深拷贝、浅拷贝、赋值运算符的重载 
9、四种类型转换函数的作用
10、finanl  explicit overrid的作用
11、重载、覆盖、重写的区别
12、什么是多态（调用同一接口，实现 不同结果，早绑定  晚绑定）
13、虚函数的实现原理
14、虚析构函数的作用
15、为什么使用new、delete而不使用malloc和free
16、c中为什么没有重载
17、extern “C”
18、抽象类  纯虚函数的作用
19、static的作用
20、sizeof求对象大小
21、指针和引用的区别

std::function<void(int)> f_display = print_num;
std::function<void(int)> 相当于数据类型，这里是个void返回值，int入参的函数类型，赋值print_num。

std::function<void()> f_display_42 = []() { std::cout << 42<< '\n'; }; lamda表达式，没入参

### 引用：
	1. 给变量取别名--堆区或栈区的变量  不能直接int &a=10; 10是常量区  可以const int & a=10;
	2. 引用定义的时候必须初始化--不能int&b;  不初始化是错误的
	3. 引用一旦初始化，不可以修改	 int &b=a;  b=c; 这里只是将c的值赋值给b，并不是更改引用，引用只能定义的时候去赋值。
	 	int a=10;
	 	int &b=a;   则b与a都可以操作同一数据
	4. 引用做函数返回值-- 接收返回值需要  int &ret=fun();
	  	1. 不要返回局部变量的引用---可以返回 局部的static变量--因为它在全局区  函数执行完，他也不会被释放，程序结束才会被释放
	  	2. 函数的调用可以作为左值（=左边的值）
	      		int & test(){static a=10;return a;}
	      		int &ref=test();//test的内部的局部静态变量返回给ref 则ref值就是test函数返回值
	      		test()=1000；   则ref的值就会变成1000  这句话相当于给a的别名赋值，会直接修改a的值，则也会修改ref的值
	5. 引用的本质
	   	int a=10;
	   	int &ref=a;  //相当于 int *const ref=&a;指针指向不可更改--也说明了为啥引用不可以再修改指向
	   	ref=20;    //内部发现ref是引用 自动转化成*ref=20;
	
	6. 常量引用
	   	const int &ref=10;//ref变为只读状态，不可修改
	   	用处：
	   	int a=100;
void print(const int&value){cout<<value;}  //保证函数中用ref用的只是只读状态 并不想更改入参的值

函数重载：函数名相同，提高复用性
	满足条件：
	1.同一个作用域下--类内
	2.函数名称相同
	3.函数入参  类型不同或  个数不同或 （类型的）顺序不同
	4.引用作为函数重载条件+函数重载碰到默认参数  fun(int a,int b=10)  fun(int a)  fun(10)---产生二义性  这两fun不能重载
	
##### 访问权限
公共	类内可以访问  类外可以访问
保护	类内可以访问  类外不可以访问	儿子可以访问父亲的保护区域属性方法
私有	类内可以访问  类外不可以访问	朋友可以访问

#### 怎么修改类内的私有属性呢？
obj.setName("lisi");//通过给对象的public函数传参即可  因为私有属性在类内是可以访问的
怎么访问类内私有属性？
obj.getNmae(){return m_name}   对象不可以直接通过.访问私有属性。可以通过public方法获取该属性


只要写一个类：系统就会提供3个默认函数-默认构造 析构 拷贝构造 

拷贝构造函数：  复制p2的属性 给p  Person p=Person(p2);
	Person(const Person &p)  
	{
		m_age=p.m_age;
	}
	编译器默认提供的拷贝是浅拷贝--值value 的简单复制
	深拷贝：各自对象 创建各自内部的 堆，各对象操作自己的堆数据  互不影响

	对于有指针的类，浅拷贝的时候 指针简单复制，表示两个对象指向同一堆区，因此当析构的时候 指针就会被重复释放 ---产生问题
	Person(const Person & p)
	{
		m_height=new int(*p.m_height);//深拷贝核心代码-要申请空间+给申请的空间赋值（赋值原对象的对应的堆的数据）
		m_height=p.m_height;		 //若是浅拷贝就是直接的赋值   与堆毫无关系
	}


	int * var_int = new int(value);//相当于先申请空间  然后给空间写数据value 

初始化列表：只能构造函数 初始化类内的属性 
Test():m_a(1),m_b(2),m_c(3)
{

}



####  c++特点之一

```
直接定义的时候（）初始化
string s("nihao");
int i(100);
char buf('b');
cout<<s<<" "<<i<<" "<<buf<<endl;
```

#### 命名空间作用

1. 比如C++ 标准库里面定义了 vector 容器，您自己也写了个 vector 类，这样名字就冲突了。于是标准库里的名字都加上 std:: 的前缀，您必须用 std::vector 来引用。同理，您自己的类也可以加
   个自定义的前缀。但是经常写全名会很繁琐，所以在没有冲突的情况下您可以偷懒，写一句
   using namespace std;，接下去的代码就可以不用写前缀直接写 vector 了

```c
using namespace std;
namespace A
{
    int x = 1;
    void fun() {
        cout<<"A namespace"<<endl;
    }
}
using namespace A;  一段程序可以引用多个using namespace
int main()
{
    fun();  调用的就是A 里面的fun
    x = 3;  修改的就是A里面的变量x
    cout<<x<<endl;
    fun();
    return 0;
}
```



#### 类

实例化对象有两种方式：

```c++
Dog dog1;  栈中实例化
dog1.name = "旺财";  访问方式
dog1.age = 2;
dog1.run();
Dog *dog2 = new Dog();  堆中实例化
dog2->age = 1;	访问成员方式
dog2->run();
delete dog2;  堆中实例化后 此实例用完要释放  防止内存泄漏
```

##### this指针

1. 一个类中的不同对象调用成员函数的时候调的都是同一个函数，怎么直到要访问的是那个对象的成员呢
2. 每个对象都有自己的this指针，记录的当前对象的内存地址
   1. this指针只能在成员函数中使用，全局、静态函数都不能使用this。实际上，成员函数默认第一个参数为T*const this。例如类里面的某个成员函数int func(int p),func的原型在编译器看来 应该是int func(T* *const this,int p).
   2. this在成员函数的开始前构造，在成员函数结束后清除
   3. this指针因编译器不用 有不同的存放位置，可能是栈 寄存器 甚至是全局变量

##### 抽象类

如果类中至少有一个函数被声明为纯虚函数，则这个类就是抽象类

设计抽象类（通常称为 ABC）的目的，是为了给其他类提供一个可以继承的适当的基类。
抽象类不能被用于实例化对象，它只能作为接口使用，试图实例化一个抽象类的对象，会
导致编译错误



如果子类继承抽象类，则必须实现抽象类中的所有的虚函数，否则子类实例化的时候会报错

