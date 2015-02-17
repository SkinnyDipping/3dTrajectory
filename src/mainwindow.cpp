#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pointCloudPreview = new PointCloudPreview(this);
    sequencePreview = new SequencePreview(this);
//    ForegroundExtractorMOG2 foregroundExtractor = ForegroundExtractorMOG2();
    ui->horizontalLayout->addWidget(sequencePreview);
    ui->horizontalLayout->addWidget(pointCloudPreview);

//    cv::Mat frame;
//    while (DataContainer::instance().getNextFrame(frame)) {
//        sequencePreview->viewFrame(frame);
//        char k = cv::waitKey(10);
//        if (k == 'q') break;
//    }


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
