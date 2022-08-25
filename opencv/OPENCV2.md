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
plt.subplot(121),plt.imgshow(src[:,:,::-1]),plt.title("原图")
plt.xticks([]),plt.yticks([])

plt.subplot(121),plt.imgshow(dts[:,:,::-1]),plt.title("高斯滤波结果")
plt.xticks([]),plt.yticks([])

plt.show()
```

##### 2.3.4中值滤波

专用滤椒盐噪声图像