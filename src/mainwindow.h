#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFileDialog>
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
    void on_loadSequence_clicked();
    void on_loadPointCloud_clicked();

private:
    Ui::MainWindow *ui;
    PointCloudPreview *pointCloudPreview;
    SequencePreview *sequencePreview;
    ForegroundExtractorMOG2 foregroundExtractor;

    QPushButton *loadSequence, *loadPointCloud;
};

#endif // MAINWINDOW_H
