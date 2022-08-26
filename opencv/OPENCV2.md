## ```OPENCV```

### 1图像的膨胀 腐蚀

膨胀和腐蚀都是针对白色部分（高亮部分）而言的。

膨胀就是使高亮部分扩张，效果图比原图拥有更大的高亮区域；

腐蚀就是使高亮部分被蚕食，拥有更少的高亮部分。膨胀是求局部最大值的操作，腐蚀是求局部最小值得操作。

#### 1.2腐蚀

```pyt
cv.erode(img,kernel,iterations)
卷积核：二维数组
iteration：迭代次数  一般为1，腐蚀1次就行
腐蚀次数多了，则图像越来越小
```

```py
cv.dilate(img,kernel,iteration)

```

code

```py
kernel=np.ones((5,5),np.uint8) #卷积核
erosion=cv.erode(img,kernel)  腐蚀
dilate=cv.dilate(ing,kernel)  膨胀

```

#### 1.3开运算 闭运算

*开运算用来消除主图像（高亮图像）之外的噪声（白色）孔洞

*闭运算用来消除主图像（高亮图像）之内的噪声（黑色）孔洞

```py
kernel=np.ones((10,10),np.uint8)
afteropen=cv.morphologyEx(src,cv.MORPH_OPEN,kernel)  开运算后的图像

afterclose=cv.morphologyEx(src,cv.MORPH_CLOSE,kernel)  闭运算后的图像

```

#### 1.4黑帽运算礼帽运算

黑帽运算获取开运算结果与原图像之差

礼帽运算获取闭运算结果与原图像之差

```pyt
kernel=np.ones((10,10),np.uint8)
heimao=cv.morphologyEx(src,cv.MORPH_TOPHAT,kernel)  
limao=cv.morphologyEx(src,cv.MORPH_BLACKHAT,kernel) 
```



### 2噪声

#### 2.1椒盐噪声

随机出现的白点或者黑点

#### 2.2高斯噪声

随机出现的各种颜色的像素点

#### 2.3滤波使图像清晰

方法：去除高频噪声，保留低频信息。实施低通滤波

可分为：均值，高斯，中值，双边滤波

##### 2.3.1均值滤波

优点速度快，缺点去噪同时去除了很多细节部分，使图像模糊

```py
API:
cv.blur(src,ksize,anchor,boardType)
ksize:卷积核大小 (x,x)
anchor：默认（-1，-1）表示核中心
boardType：边界类型	
```

code:

```pyt
cv.blur(img,(5,5))
```

##### 2.3.2高斯滤波

正态分布，带权重的滤波

```pyth
API:
cv.GaussianBlur(src,ksize,sigmax,sigmay,boardType)
ksize:卷积核大小 (x,x)
sigmax:水平方向标准差
sigmay:垂直方向标准差
boardType：边界类型	

code：
dts = cv.GaussianBlur(src,(3,3),1)
plt.figure(figsize=(10,8),dpi=100)
plt.subplot(121),plt.imshow(src[:,:,::-1]),plt.title("原图")
plt.xticks([]),plt.yticks([])

plt.subplot(121),plt.imshow(dts[:,:,::-1]),plt.title("高斯滤波结果")
plt.xticks([]),plt.yticks([])

plt.show()
```

##### 2.3.4中值滤波

专用滤椒盐噪声图像

```pyt
cv.medianBlur(img1,5)

cv.medianBlur(img,ksize) 卷积核大小
```

### 3直方图

#### 3.1直方图

就是图像的某个度（BGR）在多个区间内，每个区间内个数绘制的图像

code

```pyt
 cv.calcHist([zhongzhi],[0],None,[256],[0,256])
 参数：
 img：图像需要用[]括起来
 通道：0 1 2  BGR
 掩码：
 多少组：
 取值范围：
#获取直方图数组
hist = cv.calcHist([gaosi],[0],None,[256],[0,256])
#用plt显示直方图
plt.figure(figsize=(10,8))
plt.plot(hist)
plt.show()
```

## 4人脸检测

### 4.1训练好的xml

opencv中自带训练好的检测器，包括面部、眼睛、猫脸等等，都存在xml文件中，可以通过```cv.__file__```找到.

```py
print(cv.__file__)  #找此模块所在路径
在data文件夹可以看到很多xml文件就是训练好的检测器

```

API：

```py
#实例化级联分类器
classfier = cv.CascaderClassifier("haarcascade_frontalface_default.xml")
#加载分类器
classfier.load("haarcascade_frontalface_default.xml")
rect=classfier.detectMultiScale(gray,scaleFator,minNeighbor,minsize,maxsize)
参数：
gray：要检测的人脸图像
scaleFator：前后2次扫描，搜索窗口的比例系数
minNeighbor：目标最小要检测到minNeighbor次才会被认定为目标
minsize：目标的最大最小尺寸

```

demo

```pyt
import cv2 as cv
import matplotlib.pyplot as plt
import numpy as np

img=cv.imread("C:\\Users\\macbookpro\\PycharmProjects\\pythonProject\\face.jpeg")
gray=cv.cvtColor(img,cv.COLOR_BGR2GRAY)#获取图像的灰度图

#实例化人脸 眼睛识别的分类器
#此文件需要填写绝对路经，在这里我们拷贝此文件到当前文件所在的路径中了
face_cas = cv.CascadeClassifier('haarcascade_frontalface_default.xml')
#加载分类器
face_cas.load('haarcascade_frontalface_default.xml')

eye_cas = cv.CascadeClassifier('haarcascade_eye.xml')
#加载分类器
eye_cas.load('haarcascade_eye.xml')

#调用人脸识别
#识别gray灰度图，目标检测3次才会被认定为人脸，小于minSize的不会被标识为人脸
faceRects=face_cas.detectMultiScale(gray,minNeighbors=3,minSize=(100,100))
for faceRect in faceRects:
    #获取人脸左上角位置与宽度 高度
    x,y,w,h=faceRect
    #框出人脸，用绿色，线条宽度3
    cv.rectangle(img,(x,y),(x+w,y+h),(0,255,0),3)
    #在原图像中找人脸部分 在识别出的人脸中进行眼睛检测
    roi_color=img[y:y+h,x:x+w]  #roi_color roi_gray临时变量遍历总图中所有人脸
    roi_gray=gray[y:y+h,x:x+w]
    eyes=eye_cas.detectMultiScale(roi_gray) #在roi_gray灰度图中检测人脸
    for(ex,ey,ew,eh) in eyes:
        cv.rectangle(roi_color,(ex,ey),(ex+ew,ey+eh),(0,255,0),2)

#检测结果绘制
plt.figure(figsize=(8,6),dpi=100) #dpi在显示器中显示多大 
plt.imshow(img[:,:,::-1]),plt.title('resault')
plt.xticks([]),plt.yticks([])
plt.show()
```



