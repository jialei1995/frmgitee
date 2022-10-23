#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    //设置单选按钮 男默认选中
    ui->rBtnMan->setChecked(true);

    //选中女后 打印信息
    connect(ui->rBtnWoman,&QRadioButton::clicked,[=](){
        qDebug() << "选中了女了！";
    });

    //多选按钮  2是选中  0是未选中 1是半选
    connect(ui->cBox,&QCheckBox::stateChanged,[=](int state){
        qDebug() << state;
    });


    //利用listWidget写诗 QListWidget成员就是string
//    QListWidgetItem * item = new QListWidgetItem("锄禾日当午");//一次new 1个item
//    //将一行诗放入到listWidget控件中
//    ui->listWidget->addItem(item);//每次add 1个item
//    item->setTextAlignment(Qt::AlignHCenter);//对齐方式


    //QStringList   QList<QString>
    QStringList list ;
    list << "锄禾日当午" << "旱地和下土" << "谁知盘中餐"<< "粒粒皆辛苦";//QStringList赋值方法
    ui->listWidget->addItems(list);//直接add items


}

Widget::~Widget()
{
    delete ui;
}
