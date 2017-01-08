#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <exception>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initialize_ui();

    pointCloudPreview = new PointCloudPreview(this);
    sequencePreview = new SequencePreview(this);
    modeSelectionBox = ui->modeSelectionBox;
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

void MainWindow::on_playButton_clicked()
{
    sequencePreviewOn = !sequencePreviewOn;
    if (sequencePreviewOn)
        ui->playButton->setText("Pause");
    else
        ui->playButton->setText("Play");
    toggleSequencePreview();
}

void MainWindow::on_rewindButton_clicked()
{
    rewindSequence();
}

/*
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
*/

void MainWindow::on_loadCloudButton_clicked()
{
    std::vector<Point3DRGB> _ = std::vector<Point3DRGB>();
    //    _.push_back(Point3D(0,0,0));
    pointCloudPreview->renderCloud(_);
}

void MainWindow::on_analyzeButton_clicked()
{
    analysisOn = true;
    on_playButton_clicked();
    setTrajectory();
    //    pointCloudPreview->renderCloud(m_trajectory);
}

void MainWindow::on_castButton_clicked()
{
    qDebug()<<"Casting..";

    std::vector<Point2D> image_keypoints = std::vector<Point2D>();
    std::vector<Point3D> cloud_keypoints = std::vector<Point3D>();

    image_keypoints = DataContainer::instance().getImageKeypoints();
    cloud_keypoints = DataContainer::instance().getCloudKeypoints();

    //#define POINTS_FROM_HARDCODE
#ifdef POINTS_FROM_HARDCODE
    image_keypoints.clear();
    cloud_keypoints.clear();

    image_keypoints.push_back(Point2D(158, 333));
    image_keypoints.push_back(Point2D(184, 306));
    image_keypoints.push_back(Point2D(205, 279));
    image_keypoints.push_back(Point2D(222, 256));
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
    image_keypoints.push_back(Point2D(488, 255));
    image_keypoints.push_back(Point2D(490, 233));
    image_keypoints.push_back(Point2D(525, 202));
    image_keypoints.push_back(Point2D(551, 204));
    image_keypoints.push_back(Point2D(578, 206));
    image_keypoints.push_back(Point2D(605, 207));
    cloud_keypoints.push_back(Point3D(1.51570000, 4.08810000, 133.02970000));
    cloud_keypoints.push_back(Point3D(1.81080000, 4.19850000, 133.16140000));
    cloud_keypoints.push_back(Point3D(2.08380000, 4.31660000, 133.32100000));
    cloud_keypoints.push_back(Point3D(2.34030000, 4.44020000, 133.48450000));
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
    cloud_keypoints.push_back(Point3D(3.44850000, 2.35260000, 133.62180000));
    cloud_keypoints.push_back(Point3D(3.71520000, 2.48000000, 133.78330000));
    cloud_keypoints.push_back(Point3D(2.58490000, 1.67900000, 134.15940000));
    cloud_keypoints.push_back(Point3D(2.64360000, 1.54400000, 134.15940000));
    cloud_keypoints.push_back(Point3D(2.70390000, 1.40320000, 134.15940000));
    cloud_keypoints.push_back(Point3D(2.75950000, 1.27390000, 134.15750000));
#endif

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
                ui->playButton->setText("Play");
                rewindSequence();
                break;
            }

            if (analysisOn)
            {
                skeletonization->apply(frame);
                std::vector<User> users = skeletonization->getUsers();
                User usr = users[0];
                int usr_idx = usr.idx;
                Point2D usr_point = usr.foot;
                if (usr_point.x < 0 || usr_point.y < 0 )
                    continue;
                std::vector<Point3D> trajectory_points = Caster::instance().getPoint(usr_point);
                for (Point3D p : trajectory_points) {
                    int r,g,b;
                    r=0;
                    g=((usr_idx+1)%2)*255;
                    b=usr_idx*255;
                    m_trajectory.push_back(Point3DRGB(p.x,p.y,p.z,r,g,b));
                }

                pointCloudPreview->renderCloud(m_trajectory);
                sequencePreview->viewFrame(frame);

            } else {
                skeletonization->apply(frame, decodePreviewMode(modeSelectionBox->currentText().toStdString()));
                sequencePreview->viewFrame(skeletonization->getForeground(), true, skeletonization->getJoints());
            }

            cv::waitKey(66);
        }
    }
}

void MainWindow::rewindSequence() {
    DataContainer::instance().setCurrentFrameIndex(0);
}

void MainWindow::setTrajectory()
{
    m_trajectory.push_back(Point3DRGB(0,0,0,0,0,255));
}

int MainWindow::decodePreviewMode(std::string mode) {
    if (mode == "Preview")
        return 0;
    if (mode == "Foreground")
        return 1;
    if (mode == "Thresholding")
        return 2;
    if (mode == "Median filtering")
        return 3;
    if (mode == "Erosion & Dilatation")
        return 4;
    if (mode == "Canny")
        return 5;
    if (mode == "Skeletonization")
        return 6;
}

void MainWindow::debug(cv::Mat_<double> m) {
    for (int x=0; x<m.rows; x++)
        for (int y=0; y<m.cols; y++)
            qDebug() << m(y, x);
}

void MainWindow::initialize_ui() {
    gradient_main_background = QLinearGradient(0,0,0,ui->widget->height());
    gradient_main_background.setColorAt(0, color_main_backrground_1);
    gradient_main_background.setColorAt(1, color_main_backrground_2);

    palette_main_background.setBrush(QPalette::Base, gradient_main_background);
    palette_main_background.setBrush(QPalette::Highlight, gradient_main_background);
    palette_main_background.setBrush(QPalette::Background, gradient_main_background);
    palette_sec_background.setBrush(QPalette::Base, color_light_background);
    palette_sec_background.setBrush(QPalette::Highlight, color_light_background);
    palette_sec_background.setBrush(QPalette::Background, color_light_background);
    palette_total_background.setBrush(QPalette::Base, color_dark_background);
    palette_total_background.setBrush(QPalette::Highlight, color_dark_background);
    palette_total_background.setBrush(QPalette::Window, color_dark_background);
    ui->widget->setPalette(palette_main_background);
    ui->widget->show();
    ui->widget2->setPalette(palette_sec_background);
    ui->widget3->setPalette(palette_total_background);

    std::string style_sheet = "QPushButton { color: #C9C2BB; font-family: roboto; font-size: 23px;} "
                              "QPushButton:pressed { color: #C4AD3A; background-color: #252525; }";

    std::string style_sheet_label = "QLabel { font-family:roboto; font-size:23px; }";

    ui->playButton->setStyleSheet(QString::fromStdString(style_sheet));
    ui->rewindButton->setStyleSheet(QString::fromStdString(style_sheet));
    ui->castButton->setStyleSheet(QString::fromStdString(style_sheet));
    ui->loadCloudButton->setStyleSheet(QString::fromStdString(style_sheet));
    ui->analyzeButton->setStyleSheet(QString::fromStdString(style_sheet));

    ui->label->setStyleSheet(QString::fromStdString(style_sheet_label));
    ui->label_2->setStyleSheet(QString::fromStdString(style_sheet_label));

    ui->label_3->setStyleSheet("QLabel { font-family:roboto; font-size:30px; color: #C9C2BB;}");

    auto box_ss = file2string(":/stylesheets/qcombobox.qss");
    ui->modeSelectionBox->setStyleSheet(box_ss.c_str());

}

std::string MainWindow::file2string(std::string filename) {


    QFile file(filename.c_str());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            throw std::exception();

    std::stringstream ss;
    QTextStream in(&file);
    QString line = in.readLine();
    while (!line.isNull()) {
        ss << line.toStdString();
        line = in.readLine();
    }
    return ss.str();
}
