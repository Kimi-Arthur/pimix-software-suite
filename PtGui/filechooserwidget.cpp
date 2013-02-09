#include "filechooserwidget.h"
#include "ui_filechooserwidget.h"

FileChooserWidget::FileChooserWidget(QWidget *parent, const QString &caption, const QString &dir, const QString &filter, QString *selectedFilter, QFileDialog::Options options) :
    QWidget(parent),
    ui(new Ui::FileChooserWidget),
    caption(caption),
    dir(dir),
    filter(filter),
    selectedFilter(selectedFilter),
    options(options)
{
    ui->setupUi(this);
}

FileChooserWidget::~FileChooserWidget()
{
    delete ui;
}

QString FileChooserWidget::getPath()
{
    return filePath;
}

void FileChooserWidget::on_browseButton_clicked()
{
    QString result = QFileDialog::getOpenFileName(parentWidget(), caption, dir, filter, selectedFilter, options);
    if (!result.isEmpty()) {
        filePath = result;
        ui->pathLine->setText(QDir::toNativeSeparators(filePath));
    }
}

void FileChooserWidget::on_pathLine_editingFinished()
{
    filePath = QDir::fromNativeSeparators(ui->pathLine->text());
}
