#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap pixmap(":/new/prefix1/front_pic.png");
    ui->label->setPixmap(pixmap);

    // Change the Qlabel Size
    ui->label->setScaledContents(true);
    
    // 设置 QLabel 的大小策略
    ui->label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    // 设置最小尺寸以避免图片太小
    ui->label->setMinimumSize(100, 100);  // 可以根据需要调整这个值
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    // 获取原始图片
    QPixmap pixmap(":/new/prefix1/front_pic.png");
    // 根据 label 的新大小缩放图片
    ui->label->setPixmap(pixmap.scaled(ui->label->size(), 
                                     Qt::KeepAspectRatio, 
                                     Qt::SmoothTransformation));
}
