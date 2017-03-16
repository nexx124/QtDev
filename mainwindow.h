#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "videoplayer.h"

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

    void on_openLocal_clicked();

    void on_start_streaming_button_clicked();

    void on_stop_streaming_button_clicked();

    void on_connect_to_ver_button_clicked();

    void on_openUrl_clicked();

    void on_pause_clicked();

    void on_stop_clicked();

    void on_start_continue_button_clicked();

    void on_stop_stream_to_vr_clicked();

    void on_send_to_server_clicked();

    void on_checkBox__enable_coding_stateChanged(int arg1);

    void on_comboBox_coding_currentIndexChanged(int index);

    void on_pause_streaming_button_clicked();

private:
    Ui::MainWindow *ui;
    VideoPlayer *mediaPlayer;
};

#endif // MAINWINDOW_H
