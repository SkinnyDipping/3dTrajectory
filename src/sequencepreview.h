#ifndef SEQUENCEPREVIEW_H
#define SEQUENCEPREVIEW_H

#include <QWidget>

namespace Ui {
class SequencePreview;
}

class SequencePreview : public QWidget
{
    Q_OBJECT

public:
    explicit SequencePreview(QWidget *parent = 0);
    ~SequencePreview();

private:
    Ui::SequencePreview *ui;
};

#endif // SEQUENCEPREVIEW_H
