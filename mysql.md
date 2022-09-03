### select

select与字段名顺序有关去返回数据

`SELECT name, continent, population FROM world`

返回的字段名显示成中文： 无as也可以正常运行

`SELECT name as 国家名, continent 大洲, population 人口 FROM world`



select continent,name  from world

对结果去重：在select 后加distinct--是整行一样才去重，只有某个字段一样不去重

`select distinct continent,name   from world`



计算人均gdp，增加新列：

`select name,population,gdp,gdp/population 人均gdp from world`

不写`人均gdp`的列名也可以获取结果，但是新增列就没有字段名了

`select name,population,gdp,gdp/population  from world`

### where

where筛选条件放到语句最后部分

`select name,population,gdp,gdp/population 人均gdp from world where population>=1000000000`

支持的符号：

```c
=    	> 		< 		>= 		<=
<>或!=  ：不等于
between...and...
in:条件筛选范围  not in：不在条件筛选范围
is null--is not null：为空  为非空
and  or  not:逻辑非
```

查找人口在此两个数值之间的数据：

`select name,population,gdp  from world where population  between 32890170 and 102890172`

文本必须并且只能用英文的单引号包裹才可以

`select name,population,gdp  from world where name not in ('Afghanistan','Algeria')`

sql的列表就是用（）括起来的以逗号分隔的数据。

查找国家名以C开头na结尾的国家数据，中间以%分割。  like关键字

`select name,population,gdp  from world where name like 'C%na'`

_表示占位，表示筛选只要第二个字母是t的字母都可以。%表示其他随便是啥都行

`select name,population,gdp  from world where name like '_h%'`      `__t__`：表示只要第3位是t其他位不关心

`select name,population,gdp  from world where name like '%h%'` 这个表示只要名字中含有h即可匹配成功

like '%a%a%a%'  只要名字中含有3个字母a就能被匹配。

and的优先级高于or，但是还是建议筛选的时候用括号括起来，便于阅读。

---

筛选名称中包含所有元音字母并且不包含空格的国家

`SELECT name from world where name like '%a%' and name like '%e%' and name like '%i%' and name like '%o%' and name like '%u%' and name not like '% %'`

### order by

`SELECT * from nobel order by yr,winner desc`

默认是asc升序，order by name (升序/降序)



查询面积排行前3的国家：排序后用limit

`SELECT * from world order by area desc limit 3`

查询面积排行4-8的国家

`SELECT * from worlod order by area desc limit 3,4`

### group by 与聚合函数

`SELECT sum(population)  as 总公亚洲人口 from world where continent='Asia'`

```c
avg()   max()  min() sum()
```

计算表格行数

`SELECT count(*)  as 表格行数 from world `

计算各个大洲国家数量：按洲分组后，计算每个周的国家的数量

`SELECT continent,count(name) 各州国家数量 from world group by continent `

将各个大洲分组后，去重 -- 与select distinct 的去重底层不一样

`SELECT continent from world group by continent `