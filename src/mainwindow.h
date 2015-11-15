#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"

#include "caster.h"
#include "pointcloudpreview.h"
#include "sequencepreview.h"
#include "foregroundextractormog2.h"
#include "skeletonization.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_startSequencePreview_clicked();
    void on_rewindSequencePreview_clicked();
    void on_performCasting_clicked();
    void on_loadCloud_clicked();
    void on_analyzeButton_clicked();
    void on_casterCast_clicked();
    void on_loadSequenceButton_clicked();

private:
    void toggleSequencePreview();
    void rewindSequence();
    int decodePreviewMode(std::string mode);

    void setTrajectory();

    //TEMP
    void debug(cv::Mat_<double> m);


private:
    Ui::MainWindow *ui;
    PointCloudPreview *pointCloudPreview;
    SequencePreview *sequencePreview;
    cv::Ptr<cv::BackgroundSubtractorMOG2> cvMOG2;

    Skeletonization *skeletonization;
//    Caster m_caster;

    QPushButton *loadSequence, *loadPointCloud;
    QCheckBox *distinctForeground;
    QLineEdit *m_sequencePath;
    QComboBox *modeSelectionBox;

    bool sequencePreviewOn;
    bool analysisOn;

    std::vector<Point3D> m_trajectory;

    //TEMP
    cv::Mat m_transformationMatrix;
};

#endif // MAINWINDOW_H
