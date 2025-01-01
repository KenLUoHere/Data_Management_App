#ifndef RESTOREWINDOW_H
#define RESTOREWINDOW_H

#include <QWidget>
#include <QFileDialog>
#include "RestoreManager.h"

namespace Ui {
class RestoreWindow;
}

class RestoreWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RestoreWindow(QWidget *parent = nullptr);
    ~RestoreWindow();

private slots:
    void onBrowseClicked();
    void onRestoreClicked();
    void updateBackupList();

private:
    Ui::RestoreWindow *ui;
    RestoreManager *restoreManager;
};

#endif // RESTOREWINDOW_H 