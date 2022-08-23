# OPENCV

## 1. 图像分类--数字图像存储

+ 二值图像
  + 图像的二维矩阵由0,1两个值构成，黑色白色
+ 灰度图
  + 如果像素点是8位的则每个像素点有256种，如果是16位的，则每个像素点有65535种灰度
+ 彩色图
  + 每个像素由红绿蓝3个分量表示，介于0-255

  OPENCV_PYTHON 

| 所有opencv数据结构都会转换为NUmpy数组

### 1.1部署

安装opencv前需要安装numpy与matplotlib

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

## 2.  opencv 基本操作

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

