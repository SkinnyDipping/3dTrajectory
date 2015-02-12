#include "sequencepreview.h"
#include "ui_sequencepreview.h"

SequencePreview::SequencePreview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SequencePreview)
{
    ui->setupUi(this);
}

SequencePreview::~SequencePreview()
{
    delete ui;
}
