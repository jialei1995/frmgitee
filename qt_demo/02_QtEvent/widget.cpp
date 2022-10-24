#include "widget.h"
#include "ui_widget.h"
#include <QTimer> //定时器类
#include <QMouseEvent>
#include <QDebug>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    //启动定时器 都不需要新建定时器 就直接启动
    id1 = startTimer(1000); //参数1  间隔  单位 毫秒

    id2 = startTimer(2000);


    //定时器第二种方式 新建定时器 并启动
    QTimer * timer = new QTimer(this);
    //启动定时器
    timer->start(500);

    //这个定时器的 超时处理 在connect中实现  最下面的定时器处理 在timerEvent中也可以
    //不知第id 用connect处理  知道id用timerEvent处理
    connect(timer,&QTimer::timeout,[=](){
        static int num = 1;
        //label4 每隔0.5秒+1
        ui->label_4->setText(QString::number(num++));
    });


    //点击暂停按钮 实现停止定时器
    connect(ui->btn,&QPushButton::clicked,[=](){
        timer->stop();
    });

    //给label1 安装事件过滤器
    // 步骤1  安装事件过滤器
    ui->label->installEventFilter(this);


}

 // 步骤2  重写 eventfilter事件
bool Widget::eventFilter(QObject * obj , QEvent * e)
{
    if(obj == ui->label)//对象是当前对象 事件是鼠标的按下事件
    {
        if(e->type() == QEvent::MouseButtonPress)
        {
            //把事件e转化成QMouseEvent c++的类型转换方式
            QMouseEvent * ev  = static_cast<QMouseEvent *>(e);
            //QString str = QString( "事件过滤器中：：鼠标按下了 x = %1   y = %2  globalX = %3 globalY = %4 " ).arg(ev->x()).arg(ev->y()).arg(ev->globalX()).arg(ev->globalY());
            QString str = QString( "事件过滤器中：：鼠标按下了 x = %1   y = %2  globalX = %3 globalY = %4 " ).arg(static_cast<QMouseEvent *>(e)->x()).arg(static_cast<QMouseEvent *>(e)->y()).arg(static_cast<QMouseEvent *>(e)->globalX()).arg(static_cast<QMouseEvent *>(e)->globalY());

            qDebug() << str;
            return true; //true代表用户自己处理这个事件，不向下分发
        }
    }

    //其他默认处理
    return QWidget::eventFilter(obj,e);
}

void Widget::timerEvent(QTimerEvent * ev)
{
    if(ev->timerId() == id1)
    {
        static int num = 1;

        //label2 每隔1秒+1
        ui->label_2->setText(  QString::number(num++));
    }

    if(ev->timerId() == id2)
    {
    //label3  每隔2秒 +1
    static int num2 = 1;
    ui->label_3->setText(  QString::number(num2++));
    }

}

Widget::~Widget()
{
    delete ui;
}
