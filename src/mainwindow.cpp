#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QVBoxLayout *l = new QVBoxLayout;
    pc = new PointCloudPreview(this);
    pc2 = new PointCloudPreview(this);
    ui->horizontalLayout->addWidget(pc);
    ui->horizontalLayout->addWidget(pc2);

}

MainWindow::~MainWindow()
{
    delete ui;
}
