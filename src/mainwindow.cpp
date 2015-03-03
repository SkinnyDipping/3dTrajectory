#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pointCloudPreview = new PointCloudPreview(this);
    sequencePreview = new SequencePreview(this);
    distinctForeground = ui->toggleForeground;
    ui->horizontalLayout->addWidget(sequencePreview);
    ui->horizontalLayout->addWidget(pointCloudPreview);
    ui->horizontalLayout->setSizeConstraint(QLayout::SetMaximumSize);

    cvMOG2 = cv::createBackgroundSubtractorMOG2();

    sequencePreviewOn = false;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startSequencePreview_clicked()
{
    sequencePreviewOn = !sequencePreviewOn;
    if (sequencePreviewOn)
        ui->startSequencePreview->setText("Pause");
    else
        ui->startSequencePreview->setText("Play");
    toggleSequencePreview();
}

void MainWindow::on_rewindSequencePreview_clicked()
{
    rewindSequence();
}

void MainWindow::toggleSequencePreview()
{
    if (sequencePreviewOn){
        sequencePreview->startPlayback();
        cv::Mat frame;
        while (sequencePreviewOn) {
            frame = DataContainer::instance().getNextFrame();
            if (frame.empty()) {
                break;
            }
            if (distinctForeground->isChecked()) {
    //TODO: zrobic w tym samym oknie, kolorowanie takze
                cv::Mat foreground;
                cvMOG2->apply(frame, foreground);
                sequencePreview->viewFrame(frame);
                cv::imshow("goreground", foreground);
//                sequencePreview->viewFrame(foreground);
//                foreground.convertTo(foreground, frame.type());
//                qDebug() << foreground.rows << foreground.cols << frame.rows<<frame.cols;
//                qDebug() << foreground.type() << frame.type();
            } else {
                sequencePreview->viewFrame(frame);
            }
            cv::waitKey(66);
        }
    } else {
        //TODO
    }
}

void MainWindow::rewindSequence() {
    DataContainer::instance().setCurrentFrameIndex(0);
}
