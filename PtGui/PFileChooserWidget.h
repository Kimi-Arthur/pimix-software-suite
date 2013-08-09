#ifndef PFILECHOOSERWIDGET_H
#define PFILECHOOSERWIDGET_H

#include "PtGui_global.h"
#include <QWidget>
#include <QFileDialog>

namespace Ui {
class PFileChooserWidget;
}

class PTGUISHARED_EXPORT PFileChooserWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PFileChooserWidget(QWidget *parent = 0, const QString & caption = QString(), const QString & dir = QString(), const QString & filter = QString(), QString * selectedFilter = 0, QFileDialog::Options options = 0);
    ~PFileChooserWidget();
    QString getPath();

private slots:
    void on_browseButton_clicked();

    void on_pathLine_editingFinished();

private:
    Ui::PFileChooserWidget *ui;
    QString filePath;
    // Parameters
    QString caption;
    QString dir;
    QString filter;
    QString *selectedFilter;
    QFileDialog::Options options;
};

#endif // FILECHOOSERWIDGET_H
