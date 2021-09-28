## 对enum的学习 struct中的：占位

1. enum ASN{MON=7, TUE, WED=10, THU, FRI};  

2. ASN是个标识符，是一种数据类型（可选项-可有可无）

*enum DAY
{
   MON=1, TUE, WED, THU, FRI, SAT, SUN
};*

enum DAY good_day, bad_day; //变量good_day和bad_day的类型均为枚举型enum DAY

---

enum    //跟第一个定义不同的是，此处的标号DAY省略，这是允许的。
{
    saturday,
    sunday = 0,
    monday,
    tuesday,
    wednesday,
    thursday,
    friday
} workday; //变量workday的类型为枚举型enum DAY，这种无法再定义此类型的变量

---

 typedef enum用法

  typedef enum workday（此处workday是数据类型，通过它可以定义变量）
{
    saturday,
    sunday = 0,
    monday,
    tuesday,
    wednesday,
    thursday,
    friday
};

workday today, tomorrow; //变量today和tomorrow的类型为枚举型workday，即enum workday

-----

*** 对于创建的变量赋值的时候只能赋值括号中的那些数值，默认从0开始。 也可以每个数据都赋别的数值，不然enum就会默认加1.





-----struct中的”：“什么意思

位域：位域出现的原因是由于某些信息的存储表示只需要几个bit位就可以表示而不需要一个完整的字节，同时也是为了节省存储空间和方便处理。

```c
typedef struct  bit_struct

{
    int  bit1:3;

    int  bit2:5;

    int  bit3:7;

}data;
```

bit1、bit2、bit3表示对应的位域

整个位域结构体占用2个字节

bit1占3位，bit2占5位，bit1和bit2共用一个字节

bit3占7位，独占一个字节。

位域必须存储在同一个类型中，不能跨类型，同时也说明位域的长度不会超过所定义类型的长度

