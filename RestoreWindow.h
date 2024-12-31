#ifndef RESTOREWINDOW_H
#define RESTOREWINDOW_H

#include <QMainWindow>

namespace Ui {
class RestoreWindow;
}

class RestoreWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RestoreWindow(QWidget *parent = nullptr);
    ~RestoreWindow();

private:
    Ui::RestoreWindow *ui;
};

#endif // RESTOREWINDOW_H 