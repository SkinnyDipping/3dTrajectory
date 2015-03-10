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

void MainWindow::on_performCasting_clicked()
{
    qDebug() << "Casting";
    CloudPoint centroid = DataContainer::instance().getCloudCentroid();
    cv::Mat_<float> matrix = cv::Mat_<float>::eye(4,4);
    matrix(0,3) = centroid.x;
    matrix(1,3) = centroid.y;
    matrix(2,3) = centroid.z;
    qDebug() << matrix(0,0)<<"\t"<<matrix(0,1)<<"\t"<<matrix(0,2)<<"\t"<<matrix(0,3)<<"\n"<<
                matrix(1,0)<<"\t"<<matrix(1,1)<<"\t"<<matrix(1,2)<<"\t"<<matrix(1,3)<<"\n"<<
                matrix(2,0)<<"\t"<<matrix(2,1)<<"\t"<<matrix(2,2)<<"\t"<<matrix(2,3)<<"\n"<<
                matrix(3,0)<<"\t"<<matrix(3,1)<<"\t"<<matrix(3,2)<<"\t"<<matrix(3,3)<<"\n";
    pointCloudPreview->drawFrame(matrix);
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
