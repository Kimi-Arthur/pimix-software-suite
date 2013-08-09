#include "PFileChooserWidget.h"
#include "ui_PFileChooserWidget.h"

PFileChooserWidget::PFileChooserWidget(QWidget *parent, const QString &caption, const QString &dir, const QString &filter, QString *selectedFilter, QFileDialog::Options options) :
    QWidget(parent),
    ui(new Ui::PFileChooserWidget),
    caption(caption),
    dir(dir),
    filter(filter),
    selectedFilter(selectedFilter),
    options(options)
{
    ui->setupUi(this);
}

PFileChooserWidget::~PFileChooserWidget()
{
    delete ui;
}

QString PFileChooserWidget::getPath()
{
    return filePath;
}

void PFileChooserWidget::on_browseButton_clicked()
{
    QString result = QFileDialog::getOpenFileName(parentWidget(), caption, dir, filter, selectedFilter, options);
    if (!result.isEmpty()) {
        filePath = result;
        ui->pathLine->setText(QDir::toNativeSeparators(filePath));
    }
}

void PFileChooserWidget::on_pathLine_editingFinished()
{
    filePath = QDir::fromNativeSeparators(ui->pathLine->text());
}
