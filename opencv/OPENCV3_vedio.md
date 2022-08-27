# ```opencv```

## 1视频操作

### 1.1视频读取

code

```pyt
import cv2 as cv
import numpy as np

'''
cv2.CAP_PROP_POS_MSEC  视频文件的当前位置
cv2.CAP_PROP_POS_FRAMES  视频文件的当前帧位置
cv2.CAP_PROP_POS_AVI_RATIO  视频文件的相对位置0：开始  1：结束
cv2.CAP_PROP_FRAME_WIDTH  视频文件的帧宽度
cv2.CAP_PROP_FRAME_HEIGHT  视频文件的帧高度
cv2.CAP_PROP_FPS  视频文件的帧率
cv2.CAP_PROP_FOURCC  编解码器4字符代码
cv2.CAP_PROP_FRAME_COUNT  视频文件总帧数
'''
# 获取视频对象
cap = cv.VideoCapture("C:\\Users\\jl\\PycharmProjects\\pythontest\\vedio.mp4")
# 判断是否读取成功
while (cap.isOpened()):
    # 获取每一帧图像
    ret, frame = cap.read()  # ret为true表示获取成功 frame表示获取的帧图像
    # 获取成功显示图像
    if (ret == True):
        cv.imshow('frame', frame) #展示图像
    # 每帧间隔25ms
    if cv.waitKey(25) & 0xff == ord('q'): #按下q退出
        break
# 释放视频对象资源
cap.release()
cv.destroyAllWindows()

```

视频保存：

```pyt
import cv2 as cv
import numpy as np
# 获取视频对象
cap = cv.VideoCapture("C:\\Users\\jl\\PycharmProjects\\pythontest\\vedio.mp4")
#获取图像的属性，宽和高，并显示为整数
frame_width=int(cap.get(3))
frame_height = int(cap.get(4))
#创建保存视频的对象，设置编码格式，帧率=10 图像的宽高
out=cv.VideoWriter('out.avi',cv.VideoWriter_fourcc('M','J','P','G'),10,(frame_width,frame_height))

while(True):
    #获取视频中的每一帧图像
    ret,frame=cap.read()
    if(ret==True):
        #将每一帧写入到输出文件
        out.write(frame)
    else:
        break
# 释放视频对象资源
cap.release()
out.release()
cv.destroyAllWindows()
```

