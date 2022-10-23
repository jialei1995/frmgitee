#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //使用添加Qt资源 ": + 前缀名 + 文件名 "  给工具栏按键添加图标
    ui->actionNew->setIcon(QIcon(":/Image/Luffy.png"));
    ui->actionOpen->setIcon(QIcon(":/Image/LuffyQ.png"));
}

MainWindow::~MainWindow()
{
    delete ui;
}
