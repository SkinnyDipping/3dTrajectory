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

    modeSelectionBox = ui->modeSelectionBox;

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
    cv::Mat_<double> transformationMatrix = Caster::instance().cast();


    pointCloudPreview->renderFrame(transformationMatrix);
}

void MainWindow::on_loadCloud_clicked()
{
    std::vector<Point3D> _ = std::vector<Point3D>();
//    _.push_back(Point3D(0,0,0));
    pointCloudPreview->renderCloud(_);
}

void MainWindow::on_analyzeButton_clicked()
{
    setTrajectory();
    pointCloudPreview->renderCloud(m_trajectory);
}

void MainWindow::on_casterCast_clicked()
{

    std::vector<Point2D> image_keypoints;
    std::vector<Point3D> cloud_keypoints;
    image_keypoints.push_back(Point2D(454, 132));
    image_keypoints.push_back(Point2D(367, 127));
    image_keypoints.push_back(Point2D(270, 199));
    image_keypoints.push_back(Point2D(55, 110));
    image_keypoints.push_back(Point2D(544, 364));
    image_keypoints.push_back(Point2D(497, 230));
    image_keypoints.push_back(Point2D(367, 320));
    image_keypoints.push_back(Point2D(443, 321));
    image_keypoints.push_back(Point2D(440, 196));
    image_keypoints.push_back(Point2D(441, 229));
    image_keypoints.push_back(Point2D(366, 137));
    image_keypoints.push_back(Point2D(365, 202));
    cloud_keypoints.push_back(Point3D(0.0463, 6.5282, 133.215897));
    cloud_keypoints.push_back(Point3D(-0.8515, 6.1534, 133.221603));
    cloud_keypoints.push_back(Point3D(-1.775, 5.7682, 132.409393));
    cloud_keypoints.push_back(Point3D(-3.6192, 4.9853, 133.212799));
    cloud_keypoints.push_back(Point3D(0.8383, 5.1496, 131.091095));
    cloud_keypoints.push_back(Point3D(0.556, 6.7573, 132.14));
    cloud_keypoints.push_back(Point3D(-0.8112, 6.1722, 131.1138));
    cloud_keypoints.push_back(Point3D(-0.0135, 6.4959, 131.105896));
    cloud_keypoints.push_back(Point3D(-0.0762, 6.4742, 132.492706));
    cloud_keypoints.push_back(Point3D(-0.617, 6.4595, 132.135));
    cloud_keypoints.push_back(Point3D(-0.8596, 6.1433, 133.112503));
    cloud_keypoints.push_back(Point3D(-0.8529, 6.1302, 132.419));

    m_transformationMatrix = Caster::instance().cast(cloud_keypoints, image_keypoints);

    std::vector<Point3D> transImg;
    for (int i=0; i<image_keypoints.size(); i++)
        transImg.push_back(Point3D(image_keypoints[i].x, image_keypoints[i].y, 0));


#define t_I (transImg)
    cv::Mat_<double> t_M = m_transformationMatrix;
    for (int i=0; i<transImg.size(); i++)
    {
        Point3D p = Point3D();
        p.x = t_M(0,0)*t_I[i].x + t_M(0,1)*t_I[i].y + t_M(0,2)*t_I[i].z + t_M(0,3);
        p.y = t_M(1,0)*t_I[i].x + t_M(1,1)*t_I[i].y + t_M(1,2)*t_I[i].z + t_M(1,3);
        p.z = t_M(2,0)*t_I[i].x + t_M(2,1)*t_I[i].y + t_M(2,2)*t_I[i].z + t_M(2,3);
        transImg[i] = p;
    }
#undef t_I

    qDebug() << "IMAGE:";
    for (int i=0; i<image_keypoints.size(); i++)
        qDebug() << transImg[i];

    Point3D normalVector = Point3D(0,0,1);
    qDebug()<<"normalVector"<<normalVector<<normalVector*m_transformationMatrix;

    pointCloudPreview->renderFrame(m_transformationMatrix);

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

//            if (distinctForeground->isChecked()) {

//                if(skeletonization->apply(frame))
//                    // Choosing viewing of particular joints is available in this method
//                    sequencePreview->viewFrame(skeletonization->getForeground(), true, skeletonization->getJoints());

//            } else {
//                sequencePreview->viewFrame(frame);
//            }

            if (modeSelectionBox->currentText() == "Preview") {
                sequencePreview->viewFrame(frame);

            } else if (modeSelectionBox->currentText() == "Foreground") {
                if(skeletonization->apply(frame, -1))
                    // Choosing viewing of particular joint is available in this method
                    sequencePreview->viewFrame(skeletonization->getForeground(), true, skeletonization->getJoints());

            } else if (modeSelectionBox->currentText() == "Thresholding") {
                if(skeletonization->apply(frame, 0))
                    // Choosing viewing of particular joint is available in this method
                    sequencePreview->viewFrame(skeletonization->getForeground(), true, skeletonization->getJoints());

            } else if (modeSelectionBox->currentText() == "Median Filtering") {
                if(skeletonization->apply(frame, 1))
                    // Choosing viewing of particular joint is available in this method
                    sequencePreview->viewFrame(skeletonization->getForeground(), true, skeletonization->getJoints());

            } else if (modeSelectionBox->currentText() == "Erosion & Dilatation") {
                if(skeletonization->apply(frame, 2))
                    // Choosing viewing of particular joint is available in this method
                    sequencePreview->viewFrame(skeletonization->getForeground(), true, skeletonization->getJoints());

            } else if (modeSelectionBox->currentText() == "Canny") {
                if(skeletonization->apply(frame, 3))
                    // Choosing viewing of particular joint is available in this method
                    sequencePreview->viewFrame(skeletonization->getForeground(), true, skeletonization->getJoints());

            } else if (modeSelectionBox->currentText() == "Skeletonization") {
                if(skeletonization->apply(frame, 4))
                    // Choosing viewing of particular joint is available in this method
                    sequencePreview->viewFrame(skeletonization->getForeground(), true, skeletonization->getJoints());

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
    m_trajectory.push_back(Point3D(0,0,0));
}

void MainWindow::debug(cv::Mat_<double> m) {
    for (int x=0; x<m.rows; x++)
        for (int y=0; y<m.cols; y++)
            qDebug() << m(y, x);
}
