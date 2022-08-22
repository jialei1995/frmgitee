# OPENCV

### 图像分类--数字图像存储

+ 二值图像
  + 图像的二维矩阵由0,1两个值构成，黑色白色
+ 灰度图
  + 如果像素点是8位的则每个像素点有256种，如果是16位的，则每个像素点有65535种灰度
+ 彩色图
  + 每个像素由红绿蓝3个分量表示，介于0-255

###  OPENCV_PYTHON 

| 所有opencv数据结构都会转换为NUmpy数组

## 部署

安装opencv前需要安装numpy与matplotlib

```c
pip install opencv-python==3.4.2.17  这个版本免费
测试是否安装成功
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

