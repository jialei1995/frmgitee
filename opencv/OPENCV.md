# ```OPENCV```

## 1. 图像分类--数字图像存储

+ 二值图像
  
  + 图像的二维矩阵由0,1两个值构成，黑色白色
+ 灰度图
  
  + 如果像素点是8位的则每个像素点有256种，如果是16位的，则每个像素点有65535种灰度
+ 彩色图
  + 每个像素由红绿蓝3个分量表示，介于0-255

  ```OPENCV_PYTHON```

| 所有```opencv```数据结构都会转换为```NUmpy```数组

### 1.1部署

安装```opencv```前需要安装```numpy```与```matplotlib```

```c
pip install opencv-python==3.4.2.17  这个版本免费
# 测试是否安装成功
import cv2
pic = cv2.imread("1.jpg")
cv2.imshow("image",pic)
cv2.waitkey(0)
    
问题：在pycharm中找不到cv2库
参开https://www.cnblogs.com/Alliswell-WP/p/Pycharm_cv2_issue.html链接
需要添加opencv-python apply就好了


以上代码报错，因为路径不是绝对路径---
import cv2
pic = cv2.imread("C:\\Users\\macbookpro\\PycharmProjects\\pythonProject\\venv\\pic.jpg")
cv2.imshow("image",pic)
cv2.waitKey(0)执行成功
```

### 1.2特征提取库

```py
pip install opencv-contrib-python
```

## 2.  ```opencv``` 基本操作

```pyt
包含core、highgui、imgproc模块
core：实现最核心的数据结构，基本运算，exp：绘图函数、数组操作相关函数
highgui：视频与图像的读取显示存储等接口
imgproc：图像处理基础方法，包括：图像滤波，几何变换，平滑，阈值分割，形态学处理，边缘检测，目标检测，运动分析，对象跟踪
```

### 2.1 IO操作

#### 2.1.1读取图像

```pyt
cv.imread(绝对路径，读取方式)
读取方式：
cv.IMREAD*COLOR:以彩色方式加载，默认参数    1
cv.IMREAD*GRAYSCALE:以灰度模式加载        0
cv.IMREAD*UNCHANGED:包括alpha通道的加载图像模式    -1
```

#### 2.1.2显示图像

```pyth
cv.imshow()
参数：
+ 要显示的图像的窗口名称，以字符串表示，随便命名即可
+ 要加载的图像，imread的返回值
注意：在调用此函数后，要用cv.waitKey(),给绘制图像留下时间，否则会出现窗口无响应的情况，并且图像无法显示出来
此外：还可以用matplotlib库来对图像进行显示
```

#### 2.1.3保存图像

```pyt
cv.imwrite("要保存路径",img)
```

#### 2.1.4总结

```python
import cv2
import matplotlib.pyplot as plt
import numpy as np
pic = cv2.imread("C:\\Users\\macbookpro\\PycharmProjects\\pythonProject\\venv\\pic.jpg",
                 -1)		#  -1表示彩色显示

# 在matplotlib显示图像
plt.imshow(pic[:,:,::-1])
plt.title('mytitle'),plt.xticks([]),plt.yticks([])
plt.show()

k=cv2.waitKey()
//保存在当前运行程序的路径
cv2.imwrite("save.png",pic)


#灰度图怎么显示
pic = cv2.imread("C:\\Users\\macbookpro\\PycharmProjects\\pythonProject\\venv\\pic.jpg",
                 0)		#  -1表示彩色显示
plt.imshow(pic[cmap=plt.cm.gray])  只有1个通道
```

### 2.2图像上绘制几何图形

#### 2.2.1绘制直线

```py
cv2.line(img,start,end,color,thickness)
参数：
img：要绘制直线的图像
start/end:直线的起点终点
color：线条颜色
thinkness：线条宽度
```

#### 2.2.2绘制圆形

```py
cv.circle(img,centerpoint,r,color,thickness)
centerpoint，r：圆心与半径
```

#### 2.2.3绘制矩形

```py
cv.rectangle(img,leftupper,rightdown,color,thickness)
参数：
leftupper,rightdown：矩形的左上角与右下角坐标
```

#### 2.2.3向图像中添加文字

```pyt
cv.putText(img,text,station,font,fontsize,color,thickness,cv.LINE_AA)
参数：
test：要写入的文本
station:文本放置位置
font：字体

```

#### 2.2.4总结

```py
import cv2 as cv
import matplotlib.pyplot as plt
import numpy as np

# 创建空白图像
img = np.zeros((512,512,3),np.uint8)   # 512 512表示大小 3表示是彩色图像

# 绘制图像
cv.line(img,(0,0),(511,511),(255,0,0),5) # 起始点，结束点，颜色（蓝绿红），粗细
cv.rectangle(img,(384,0),(510,128),(0,255,0),3) #左上点，右下点，绿，粗细
cv.circle(img,(447,63),63,(0,0,255),-1)  # 圆心，半径 红 -1内部填满

font = cv.FONT_HERSHEY_SIMPLEX
cv.putText(img,"test",(10,500),font,4,(255,255,255),2,cv.LINE_AA) # 白色

# 图像显示
plt.imshow(img[:,:,::-1])
plt.title('mytitle'),plt.xticks([]),plt.yticks([])
plt.show()
```

---

![](C:\jl\git\study_txt\typora_pic\Figure_1.png)

### 2.3图像基本操作

#### 2.3.1像素操作

```py
px = img[100,100]  		#获取某点像素，返回3维数组
blue = img[100,100,2]	#获取100 100位置的0 1 2表示BGR3个通道的强度
img[100,100] = [255,255,255]	#修改100 100位置的颜色
```

#### 2.3.2通道合并分解

```py
有时需要在bgr单独通道工作，需要对图像进行通道的拆分合并
b,g,r=cv.split(img)
img2=cv.merge(b,g,r)

plt.imshow(g,cmap=plt.cm.gray)
plt.imshow(r,cmap=plt.cm.gray)  显示一个通道的灰度图
```

#### 2.3.4色彩空间的转换

```py
gray=cv.cvtColor(img,flag)
参数flag：
cv.COLOR_BGR2GRAY
cv.COLOR_BGR2HSV
plt.imshow(gray,cmap=plt.cm.gray)  灰度图的显示

```

#### 2.3.4图像的属性

```pyt
img.shape   (512, 512)
img.size    262144
img.dtype   数据类型
```

#### 2.3.5图像的加法

```py
#即图像的合成，两张大小一致的图像，相加，背景色就会合成进去
img3=cv.add(img1,img2)   opencv的加法
img4=img1+img2			 numpy中的加法

exp：
img1 = cv.imread("C:\\jl\\testpic\\43_1.jpg")
img2 = cv.imread("C:\\jl\\testpic\\43_2.jpg")

img3=cv.add(img1,img2)#图像大小像素要一致  效果更好
img4=img1+img2  #numpy的加法

plt.imshow(img4[:,:,::-1])
plt.title('mytitle'),plt.xticks([]),plt.yticks([])
plt.show()
```

#### 2.3.6图像的混合（特殊的加法）

```pyt
img=cv.addWeighted(img1,0.7,img2,(1-0.7),0)
图像1占权重0.7 图像2占0.3 最后加系数0
权重的值肯定在（0-1）之间
调整权重可以达到不同的效果
```

### 2.4图像放大缩小-线性变换

```py
newimg=cv.resize(src,dsize,fx=0,fy=0,interpolation=cv.INTER_LINEAR)
参数：
src：输入图像
dsize：绝对尺寸，直接指定调整后图像大小
fx,fy:相对尺寸，需要将dsize设置None，再去设置fx fy比例因子
interpolation：插值方法 可取值--要放大图像插入位置的像素点怎么取值
```

| ```cv.INTER_LINEAR```  | 双线性插值             |
| ---------------------- | ---------------------- |
| ```cv.INTER_NEAREST``` | 最邻近插值             |
| ```cv.INTER_AREA```    | 像素区域重采样（默认） |
| ```cv.INTER_CUBIC```   | 双3次插值              |

实例：

```py
#相对尺寸
newimg=cv.resize(img2,None,fx=5,fy=5,interpolation=cv.INTER_AREA)
print(newimg.shape) #打印扩大的shape
#绝对尺寸
r,c = img1.shape[:2]  #shape返回行数 列数 彩色图还是灰度图  :2表示取值行数列数
res.resize(img1,(2*c,2*r)) #直接调整绝对尺寸
```

#### 2.4.1图像的平移

```py
cv.warpAffine(img,M,dsize)
```

参数：

+ M 2*3的矩阵 对于（x,y）坐标，要把它移动到(x+tx,y+ty)

  typora中矩阵怎么表示呢？
  $$
  M = \left[
  \matrix{
    \alpha_1 & test1\\
    \alpha_2 & test2\\
    \alpha_3 & test3 
  }
  \right]
  $$
  
  $$
  M=\left[
  	\matrix{
  	1&0&tx\\
  	0&1&ty
  	}
  \right]
  $$
  

+ M必须是```np.float32```类型的```Numpy```数组

+ ```dsize```:输出图像的大小-应该是（宽度，高度）形式，width=列数，height=行数

```py
img1 = cv.imread("C:\\jl\\testpic\\43_1.jpg")
img2 = cv.imread("C:\\jl\\testpic\\43_2.jpg")

#newimg=img2
newimg=cv.resize(img2,None,fx=5,fy=5,interpolation=cv.INTER_AREA)

r,c = newimg.shape[:2]  #获取原图行 列
M=np.float32([[1,0,100],[0,1,50]])  #新建移动矩阵
dts = cv.warpAffine(newimg,M,(c,r))  #得到新图像
# 只显示平移后的图像
plt.imshow(dts[:,:,::-1])
plt.show()

#两个一起图像显示
fig,axes=plt.subplots(nrows=1,ncols=2,figsize=(10,8),dpi=100)
axes[0].imshow(newimg[:,:,::-1])
axes[0].set_title("before momve")
axes[1].imshow(dts[:,:,::-1])
axes[1].set_title("after move")

plt.show()

```

#### 2.4.2图像的旋转

```py
cv.getRotationMatrix2D(center,angle,scale)
```

参数：

+ center：旋转中心
+ angle：角度 0-360
+ scale：缩放比例--1不缩放   0.5 缩放一倍

返回：

+ M：旋转矩阵

```py
newimg=cv.resize(img2,None,fx=5,fy=5,interpolation=cv.INTER_AREA)
r,c = newimg.shape[:2]
#生成旋转矩阵
M=cv.getRotationMatrix2D((c/2,r/2),90,1)#以中心点为原点旋转
dts=cv.warpAffine(newimg,M,(c,r))	#根据获得的矩阵平移得到旋转后的图像
plt.imshow(dts[:,:,::-1])
plt.show()
```

#### 2.4.3投射变换

```py
c, r = newimg.shape[:2]
M1 = np.float32([[56, 65], [368, 52], [28, 378], [389, 390]])
M2 = np.float32([[100, 65], [200, 100], [80, 290], [310, 300]])
T = cv.getPerspectiveTransform(M1, M2)

dts = cv.warpPerspective(newimg, T, (c, r))  #变换后的图像
```

#### 2.4.5金字塔图

```py
upimg=cv.pyrUp(newimg)		#在原图基础上获取更大的图
downimg=cv.pyrDown(newimg)  #在原图基础上获取更小的图
#显示
cv.imshow("biger",upimg)
cv.imshow("origin",newimg)
cv.imshow("small",downimg)
cv.waitKey()
cv.destroyAllWindows()
```

