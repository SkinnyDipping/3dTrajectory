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

    cvMOG2 = cv::createBackgroundSubtractorMOG2(500, 16, true);
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

    //        cv::Mat transformationMatrix = Caster::cast(DataContainer::instance().getCloudKeypoints(),
    //                                                    DataContainer::instance().getImageKeypoints());


    pointCloudPreview->renderFrame(matrix);
}

void MainWindow::on_loadCloud_clicked()
{
    pointCloudPreview->renderCloud();
}

void MainWindow::on_analyzeButton_clicked()
{
}

void MainWindow::on_casterCast_clicked()
{

    std::vector<Point2D> image_keypoints;
    std::vector<Point3D> cloud_keypoints;
//    image_keypoints.push_back(Point2D(454, 132));
//    image_keypoints.push_back(Point2D(367, 127));
//    image_keypoints.push_back(Point2D(270, 199));
//    image_keypoints.push_back(Point2D(55, 110));
//    image_keypoints.push_back(Point2D(544, 364));
//    image_keypoints.push_back(Point2D(497, 230));
//    image_keypoints.push_back(Point2D(367, 320));
//    image_keypoints.push_back(Point2D(443, 321));
//    image_keypoints.push_back(Point2D(440, 196));
//    image_keypoints.push_back(Point2D(441, 229));
//    image_keypoints.push_back(Point2D(366, 137));
//    image_keypoints.push_back(Point2D(365, 202));
//    cloud_keypoints.push_back(Point3D(0.0463, 6.5282, 133.215897));
//    cloud_keypoints.push_back(Point3D(-0.8515, 6.1534, 133.221603));
//    cloud_keypoints.push_back(Point3D(-1.775, 5.7682, 132.409393));
//    cloud_keypoints.push_back(Point3D(-3.6192, 4.9853, 133.212799));
//    cloud_keypoints.push_back(Point3D(0.8383, 5.1496, 131.091095));
//    cloud_keypoints.push_back(Point3D(0.556, 6.7573, 132.14));
//    cloud_keypoints.push_back(Point3D(-0.8112, 6.1722, 131.1138));
//    cloud_keypoints.push_back(Point3D(-0.0135, 6.4959, 131.105896));
//    cloud_keypoints.push_back(Point3D(-0.0762, 6.4742, 132.492706));
//    cloud_keypoints.push_back(Point3D(-0.617, 6.4595, 132.135));
//    cloud_keypoints.push_back(Point3D(-0.8596, 6.1433, 133.112503));
//    cloud_keypoints.push_back(Point3D(-0.8529, 6.1302, 132.419));

//    image_keypoints.clear();
//    cloud_keypoints.clear();

    image_keypoints.push_back(Point2D(368, 127));
    image_keypoints.push_back(Point2D(367, 136));
    image_keypoints.push_back(Point2D(374, 134));
    image_keypoints.push_back(Point2D(454, 132));
    image_keypoints.push_back(Point2D(448, 138));
    image_keypoints.push_back(Point2D(364, 202));
    image_keypoints.push_back(Point2D(270, 199));
    image_keypoints.push_back(Point2D(498, 229));
    image_keypoints.push_back(Point2D(440, 197));
//    image_keypoints.push_back(Point2D(303, 378));
//    image_keypoints.push_back(Point2D(431, 369));
//    image_keypoints.push_back(Point2D(546, 364));
//    image_keypoints.push_back(Point2D(610, 207));
//    image_keypoints.push_back(Point2D(538, 62));
//    image_keypoints.push_back(Point2D(55, 110));

    cloud_keypoints.push_back(Point3D(-0.85150000, 6.15340000, 133.22160000));
    cloud_keypoints.push_back(Point3D(-0.86040000, 6.14910000, 133.11830000));
    cloud_keypoints.push_back(Point3D(-0.78370000, 6.17650000, 133.13580000));
    cloud_keypoints.push_back(Point3D(0.05170000, 6.52890000 ,133.21090000));
    cloud_keypoints.push_back(Point3D(-0.00980000, 6.49730000, 133.13410000));
    cloud_keypoints.push_back(Point3D(-0.85290000, 6.13020000, 132.41910000));
    cloud_keypoints.push_back(Point3D(-1.77060000, 5.73810000, 132.42180000));
    cloud_keypoints.push_back(Point3D(0.56140000 ,6.75780000 ,132.14000000));
    cloud_keypoints.push_back(Point3D(-0.07120000, 6.47120000, 132.49750000));
//    cloud_keypoints.push_back(Point3D(-0.97200000, 4.28410000, 131.08200000));
//    cloud_keypoints.push_back(Point3D(-0.10230000, 4.73800000, 131.08500000));
//    cloud_keypoints.push_back(Point3D(0.84990000 ,5.14390000 ,131.09180000));
//    cloud_keypoints.push_back(Point3D(1.06440000 ,4.19220000 ,132.42920000));
//    cloud_keypoints.push_back(Point3D(0.90020000 ,6.37770000 ,133.94240000));
//    cloud_keypoints.push_back(Point3D(-3.61530000, 4.98000000, 133.20190000));



    Caster::instance().cast(image_keypoints, cloud_keypoints);

}

void MainWindow::toggleSequencePreview()
{
    if (sequencePreviewOn) {
        sequencePreview->startPlayback();
        while (sequencePreviewOn) {
            cv::Mat frame = DataContainer::instance().getNextFrame();
            if (frame.empty()) {
                // end of preview
                sequencePreviewOn = false;
                ui->startSequencePreview->setText("Play");
                rewindSequence();
                break;
            }
            if (distinctForeground->isChecked()) {

                if(skeletonization->apply(frame))
                    // Choosing viewing of particular joints is available in this method
                    sequencePreview->viewFrame(skeletonization->getForeground(), true, skeletonization->getJoints());

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
