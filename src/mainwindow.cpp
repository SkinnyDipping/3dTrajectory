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

    caster = Caster();

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
    //TODO casting
    Point3D centroid = DataContainer::instance().getCloudCentroid();
    cv::Mat_<float> matrix = cv::Mat_<float>::eye(4,4);
    matrix(0,3) = centroid.x;
    matrix(1,3) = centroid.y;
    matrix(2,3) = centroid.z;

    std::vector<Point2D> testPoints;
    for (float x=0; x<=320; x++)
        for (float y=0; y<=210; y++)
            testPoints.push_back(Point2D(x, y));

//    PointCloud image;
//    for (int x=0; x<DataContainer::instance().getReferenceFrame().cols; x++)
//        for (int y=0; y<DataContainer::instance().getReferenceFrame().rows; y++)
//            image.push_back(Point3D(x/DataContainer::instance().getReferenceFrame().cols-1, y/DataContainer::instance().getReferenceFrame().rows-1, 0));
//    qDebug()<<image.size();

//    pointCloudPreview->renderFrame(&image[0], image.size(), matrix);
//    pointCloudPreview->renderPoint(&image[0], image.size());
//    qDebug() << centroid;

    OUTPUT pts = caster.castCloudToImage(DataContainer::instance().getCloudKeypoints(), DataContainer::instance().getImageKeypoints());

    pointCloudPreview->renderPoint(pts.first, pts.second);

//    Point3D* p = new Point3D(15.06, 0.17, 132.57);
//    pointCloudPreview->renderPoint(p, 1);
}

void MainWindow::on_loadCloud_clicked()
{
    pointCloudPreview->showCloud(DataContainer::instance().getCloudKeypoints());
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
