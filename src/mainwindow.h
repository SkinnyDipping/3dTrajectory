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

    void setPalette(QPalette p) {
        palette_main_background = p;
    }

private slots:
    void on_playButton_clicked();
    void on_rewindButton_clicked();
    void on_castButton_clicked();
    void on_loadCloudButton_clicked();
    void on_analyzeButton_clicked();
private:
    void toggleSequencePreview();
    void rewindSequence();
    int decodePreviewMode(std::string mode);

    void setTrajectory();

    void initialize_ui();

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
    QComboBox *modeSelectionBox;

    bool sequencePreviewOn;
    bool analysisOn;

    std::vector<Point3DRGB> m_trajectory;

    //TEMP
    cv::Mat m_transformationMatrix;

    QColor color_main_backrground_1 = QColor(31, 61, 77);
    QColor color_main_backrground_2 = QColor(62,62,72);
    QColor color_dark_background = QColor(37,37,37);
    QColor color_light_background = QColor(201,194,187);
    QColor color_dark_text = QColor(37,37,37);
    QColor color_light_text = QColor(201,194,187);

    QLinearGradient gradient_main_background;

    QPalette palette_main_background;
    QPalette palette_sec_background;
    QPalette palette_total_background;

};

#endif // MAINWINDOW_H
