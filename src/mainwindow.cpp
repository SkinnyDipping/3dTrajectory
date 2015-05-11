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
    skeletonization = new Skeletonization(cv::Size(720,400));

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
    float scale = 0.3;
    //TODO casting
    Point3D centroid = DataContainer::instance().getCloudCentroid();
    cv::Mat_<double> matrix = cv::Mat::eye(4,4, CV_64F);
    matrix(0,3) = centroid.x;
    matrix(1,3) = centroid.y;
    matrix(2,3) = centroid.z;
    matrix(0,0) = scale;
    matrix(1,1) = scale;
    matrix(2,2) = scale;

//    cv::Mat transformationMatrix = Caster::cast(DataContainer::instance().getCloudKeypoints(),
//                                                DataContainer::instance().getImageKeypoints());
    cv::Mat_<double> transformationMatrix = Caster::instance().cast();


//    transformationMatrix = matrix*transformationMatrix;

    pointCloudPreview->renderFrame(transformationMatrix);


}

void MainWindow::on_loadCloud_clicked()
{
    pointCloudPreview->renderCloud();
}

void MainWindow::on_analyzeButton_clicked()
{
    cv::Mat_<double> matrix = Caster::instance().cast();
    pointCloudPreview->renderColorizedCloud(matrix);
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
#ifndef CLASS
                cv::Mat foreground;
                cvMOG2->apply(frame, foreground, -1);
                cv::Canny(foreground, foreground, 1, 3);
                cv::cvtColor(foreground, foreground, cv::COLOR_GRAY2BGR);
                sequencePreview->viewFrame(foreground, true);
#else
                cv::Mat foreground;
                skeletonization->pushFrame(frame);
                foreground = skeletonization->getForeground();
                sequencePreview->viewFrame(foreground, true);
#endif
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

void MainWindow::debug(cv::Mat_<double> m) {
    for (int x=0; x<m.rows; x++)
        for (int y=0; y<m.cols; y++)
            qDebug() << m(y, x);
}
