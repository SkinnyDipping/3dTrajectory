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
    analysisOn = false;

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
    std::vector<Point3DRGB> _ = std::vector<Point3DRGB>();
//    _.push_back(Point3D(0,0,0));
    pointCloudPreview->renderCloud(_);
}

void MainWindow::on_analyzeButton_clicked()
{
    analysisOn = true;
    on_startSequencePreview_clicked();
    setTrajectory();
//    pointCloudPreview->renderCloud(m_trajectory);
}

void MainWindow::on_casterCast_clicked()
{
    qDebug()<<"So we cast..";

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

//    image_keypoints.push_back(Point2D(368, 127));
//    image_keypoints.push_back(Point2D(367, 136));
//    image_keypoints.push_back(Point2D(374, 134));
//    image_keypoints.push_back(Point2D(454, 132));
//    image_keypoints.push_back(Point2D(448, 138));
//    image_keypoints.push_back(Point2D(364, 202));
//    image_keypoints.push_back(Point2D(270, 199));
//    image_keypoints.push_back(Point2D(498, 229));
//    image_keypoints.push_back(Point2D(440, 197));
//    image_keypoints.push_back(Point2D(303, 378));
//    image_keypoints.push_back(Point2D(431, 369));
//    image_keypoints.push_back(Point2D(546, 364));
//    image_keypoints.push_back(Point2D(610, 207));
//    image_keypoints.push_back(Point2D(538, 62));
//    image_keypoints.push_back(Point2D(55, 110));

//    cloud_keypoints.push_back(Point3D(-0.85150000, 6.15340000, 133.22160000));
//    cloud_keypoints.push_back(Point3D(-0.86040000, 6.14910000, 133.11830000));
//    cloud_keypoints.push_back(Point3D(-0.78370000, 6.17650000, 133.13580000));
//    cloud_keypoints.push_back(Point3D(0.05170000, 6.52890000 ,133.21090000));
//    cloud_keypoints.push_back(Point3D(-0.00980000, 6.49730000, 133.13410000));
//    cloud_keypoints.push_back(Point3D(-0.85290000, 6.13020000, 132.41910000));
//    cloud_keypoints.push_back(Point3D(-1.77060000, 5.73810000, 132.42180000));
//    cloud_keypoints.push_back(Point3D(0.56140000 ,6.75780000 ,132.14000000));
//    cloud_keypoints.push_back(Point3D(-0.07120000, 6.47120000, 132.49750000));
//    cloud_keypoints.push_back(Point3D(-0.97200000, 4.28410000, 131.08200000));
//    cloud_keypoints.push_back(Point3D(-0.10230000, 4.73800000, 131.08500000));
//    cloud_keypoints.push_back(Point3D(0.84990000 ,5.14390000 ,131.09180000));
//    cloud_keypoints.push_back(Point3D(1.06440000 ,4.19220000 ,132.42920000));
//    cloud_keypoints.push_back(Point3D(0.90020000 ,6.37770000 ,133.94240000));
//    cloud_keypoints.push_back(Point3D(-3.61530000, 4.98000000, 133.20190000));


    image_keypoints.push_back(Point2D(158, 333));
    image_keypoints.push_back(Point2D(184, 306));
    image_keypoints.push_back(Point2D(205, 279));
    image_keypoints.push_back(Point2D(222, 256));
//    image_keypoints.push_back(Point2D(239, 235));
    image_keypoints.push_back(Point2D(255, 216));
    image_keypoints.push_back(Point2D(269, 199));
    image_keypoints.push_back(Point2D(282, 184));
    image_keypoints.push_back(Point2D(294, 170));
    image_keypoints.push_back(Point2D(87, 157));
    image_keypoints.push_back(Point2D(124, 41));
    image_keypoints.push_back(Point2D(172, 117));
    image_keypoints.push_back(Point2D(179, 112));
    image_keypoints.push_back(Point2D(296, 27));
    image_keypoints.push_back(Point2D(304, 32));
    image_keypoints.push_back(Point2D(320, 298));
    image_keypoints.push_back(Point2D(335, 300));
    image_keypoints.push_back(Point2D(330, 375));
    image_keypoints.push_back(Point2D(483, 367));
    image_keypoints.push_back(Point2D(489, 379));
    image_keypoints.push_back(Point2D(490, 348));
    image_keypoints.push_back(Point2D(516, 351));
    image_keypoints.push_back(Point2D(542, 355));
    image_keypoints.push_back(Point2D(567, 359));
    image_keypoints.push_back(Point2D(593, 362));
    image_keypoints.push_back(Point2D(478, 309));
//    image_keypoints.push_back(Point2D(483, 280));
    image_keypoints.push_back(Point2D(488, 255));
    image_keypoints.push_back(Point2D(490, 233));
//    image_keypoints.push_back(Point2D(500, 201));
    image_keypoints.push_back(Point2D(525, 202));
    image_keypoints.push_back(Point2D(551, 204));
    image_keypoints.push_back(Point2D(578, 206));
    image_keypoints.push_back(Point2D(605, 207));


    cloud_keypoints.push_back(Point3D(1.51570000, 4.08810000, 133.02970000));
    cloud_keypoints.push_back(Point3D(1.81080000, 4.19850000, 133.16140000));
    cloud_keypoints.push_back(Point3D(2.08380000, 4.31660000, 133.32100000));
    cloud_keypoints.push_back(Point3D(2.34030000, 4.44020000, 133.48450000));
//    cloud_keypoints.push_back(Point3D(2.62590000, 4.56720000, 133.64590000));
    cloud_keypoints.push_back(Point3D(2.91310000, 4.67620000, 133.80760000));
    cloud_keypoints.push_back(Point3D(3.17970000, 4.78630000, 133.96270000));
    cloud_keypoints.push_back(Point3D(3.46040000, 4.90610000, 134.12690000));
    cloud_keypoints.push_back(Point3D(3.74910000, 5.02950000, 134.29190000));
    cloud_keypoints.push_back(Point3D(1.05880000, 4.19720000, 134.36260000));
    cloud_keypoints.push_back(Point3D(1.22720000, 4.05470000, 135.16810000));
    cloud_keypoints.push_back(Point3D(4.05120000, 8.15860000, 135.00940000));
    cloud_keypoints.push_back(Point3D(4.27910000, 8.32650000, 135.12910000));
    cloud_keypoints.push_back(Point3D(3.86880000, 5.16990000, 136.00180000));
    cloud_keypoints.push_back(Point3D(4.09280000, 5.26950000, 135.99280000));
    cloud_keypoints.push_back(Point3D(2.40990000, 3.25310000, 133.30330000));
    cloud_keypoints.push_back(Point3D(2.47440000, 3.12700000, 133.30100000));
    cloud_keypoints.push_back(Point3D(1.81960000, 2.55040000, 133.01400000));
    cloud_keypoints.push_back(Point3D(2.66700000, 1.97930000, 133.03500000));
    cloud_keypoints.push_back(Point3D(2.48120000, 1.82530000, 133.01310000));
    cloud_keypoints.push_back(Point3D(2.50100000, 1.83360000, 133.22190000));
    cloud_keypoints.push_back(Point3D(2.56790000, 1.69120000, 133.22210000));
    cloud_keypoints.push_back(Point3D(2.63020000, 1.54780000, 133.21640000));
    cloud_keypoints.push_back(Point3D(2.68570000, 1.42380000, 133.21800000));
    cloud_keypoints.push_back(Point3D(2.74870000, 1.28340000, 133.21260000));
    cloud_keypoints.push_back(Point3D(2.97900000, 2.19750000, 133.31620000));
//    cloud_keypoints.push_back(Point3D(3.16990000, 2.25350000, 133.47120000));
    cloud_keypoints.push_back(Point3D(3.44850000, 2.35260000, 133.62180000));
    cloud_keypoints.push_back(Point3D(3.71520000, 2.48000000, 133.78330000));
//    cloud_keypoints.push_back(Point3D(4.67570000, 8.48540000, 134.39470000));
    cloud_keypoints.push_back(Point3D(2.58490000, 1.67900000, 134.15940000));
    cloud_keypoints.push_back(Point3D(2.64360000, 1.54400000, 134.15940000));
    cloud_keypoints.push_back(Point3D(2.70390000, 1.40320000, 134.15940000));
    cloud_keypoints.push_back(Point3D(2.75950000, 1.27390000, 134.15750000));


    Caster::instance().cast(DataContainer::instance().getCloud(), image_keypoints, cloud_keypoints, Point2D(720,400));

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
//                    sequencePreview->viewFrame(skeletonization->getForeground(), true, skeletonization->getJoints());
                    sequencePreview->viewFrame(frame);

                if (analysisOn)
                    //TODO zrobic zeby nie zrwalao vectora tylko 1 punkt
                {
                    Point2D point = skeletonization->getFeet().first;
                    if (point.x < 0 || point.y < 0 )
                        continue;
                    std::vector<Point3D> trajectory_points = Caster::instance().getPoint(point);
                    qDebug()<<"Trajectory: "<<trajectory_points.size()<<"pts";
                    for (Point3D p : trajectory_points) {
                        m_trajectory.push_back(Point3DRGB(p.x,p.y,p.z,0,0,255));
                    }
                    pointCloudPreview->renderCloud(m_trajectory);
                }

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

void MainWindow::setTrajectory()
{
    m_trajectory.push_back(Point3DRGB(0,0,0,0,0,255));
}

void MainWindow::debug(cv::Mat_<double> m) {
    for (int x=0; x<m.rows; x++)
        for (int y=0; y<m.cols; y++)
            qDebug() << m(y, x);
}
