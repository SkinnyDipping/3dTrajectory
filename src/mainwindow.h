#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QCheckBox>

#include "opencv2/opencv.hpp"
#include "opencv2/video.hpp"

#include "pointcloudpreview.h"
#include "sequencepreview.h"
#include "foregroundextractormog2.h"

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

private:
    void toggleSequencePreview();
    void rewindSequence();


private:
    Ui::MainWindow *ui;
    PointCloudPreview *pointCloudPreview;
    SequencePreview *sequencePreview;
    cv::Ptr<cv::BackgroundSubtractorMOG2> cvMOG2;

    QPushButton *loadSequence, *loadPointCloud;
    QCheckBox *distinctForeground;

    bool sequencePreviewOn;
};

#endif // MAINWINDOW_H
