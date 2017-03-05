#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "SimplePlayer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void createPlayer();

private slots:
    void on_enter_button_clicked();

private:
    Ui::MainWindow *ui;
    SimplePlayer *mediaPlayer;
};

#endif // MAINWINDOW_H
