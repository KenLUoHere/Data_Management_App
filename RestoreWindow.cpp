#include "RestoreWindow.h"
#include "ui_restorewindow.h"

RestoreWindow::RestoreWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RestoreWindow)
{
    ui->setupUi(this);
}

RestoreWindow::~RestoreWindow()
{
    delete ui;
} 