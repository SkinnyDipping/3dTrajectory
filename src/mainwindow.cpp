#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pointCloudPreview = new PointCloudPreview(this);
    sequencePreview = new SequencePreview(this);
    ui->horizontalLayout->addWidget(sequencePreview);
    ui->horizontalLayout->addWidget(pointCloudPreview);

    ui->horizontalLayout->setSizeConstraint(QLayout::SetMaximumSize);

    loadSequence = ui->loadSequence;
    loadPointCloud = ui->loadPointCloud;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loadSequence_clicked() {

}

void MainWindow::on_loadPointCloud_clicked() {

}
