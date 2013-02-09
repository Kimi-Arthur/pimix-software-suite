#ifndef FILECHOOSERWIDGET_H
#define FILECHOOSERWIDGET_H

#include "PtGui_global.h"
#include <QWidget>
#include <QFileDialog>

namespace Ui {
class FileChooserWidget;
}

class PTGUISHARED_EXPORT FileChooserWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit FileChooserWidget(QWidget *parent = 0, const QString & caption = QString(), const QString & dir = QString(), const QString & filter = QString(), QString * selectedFilter = 0, QFileDialog::Options options = 0);
    ~FileChooserWidget();
    QString getPath();

private slots:
    void on_browseButton_clicked();

    void on_pathLine_editingFinished();

private:
    Ui::FileChooserWidget *ui;
    QString filePath;
    // Parameters
    QString caption;
    QString dir;
    QString filter;
    QString *selectedFilter;
    QFileDialog::Options options;
};

#endif // FILECHOOSERWIDGET_H
