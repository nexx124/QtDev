#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    mediaPlayer = NULL;
    ui->setupUi(this);
    ui->Player_StackedWidget->setCurrentIndex(0);
}

void MainWindow::createPlayer()
{
    if (mediaPlayer != NULL) {
        delete mediaPlayer;
    }
    mediaPlayer = new VideoPlayer();
    mediaPlayer->setupUI(ui);
    mediaPlayer->createSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_enter_button_clicked()
{
    if (ui->options_comboBox->currentIndex() == 0) {
        if ( mediaPlayer == NULL) {
            createPlayer();
        }
        ui->Player_StackedWidget->setCurrentWidget(ui->streamingPlayer);
    }
}

void MainWindow::on_openLocal_clicked()
{
    mediaPlayer->openLocal();
}

void MainWindow::on_start_streaming_button_clicked()
{
    mediaPlayer->on_start_streaming_button_clicked();
}

void MainWindow::on_stop_streaming_button_clicked()
{
    mediaPlayer->on_stop_streaming_button_clicked();
}

void MainWindow::on_connect_to_ver_button_clicked()
{
    mediaPlayer->on_connect_to_ver_button_clicked();
}

void MainWindow::on_openUrl_clicked()
{
    mediaPlayer->openUrl();
}

void MainWindow::on_pause_clicked()
{
    mediaPlayer->pauseVideo();
    ui->pause->setChecked(true);
}

void MainWindow::on_stop_clicked()
{
    mediaPlayer->stopVideoPlayer();
    ui->pause->setChecked(false);
    ui->pause->setEnabled(false);
    ui->start_continue_button->setEnabled(true);
    ui->stop->setEnabled(false);
}

void MainWindow::on_start_continue_button_clicked()
{
    mediaPlayer->startVideo();
    ui->pause->setEnabled(true);
    ui->pause->setChecked(false);
    ui->stop->setEnabled(true);
    ui->start_continue_button->setChecked(false);
    ui->start_continue_button->setEnabled(false);
}

void MainWindow::on_stop_stream_to_vr_clicked()
{
    mediaPlayer->stop_streaming_to_VR();
}

void MainWindow::on_send_to_server_clicked()
{
    mediaPlayer->send_to_server();
}

void MainWindow::on_checkBox__enable_coding_stateChanged(int arg1)
{
    mediaPlayer->enableCoding(arg1);
}

void MainWindow::on_comboBox_coding_currentIndexChanged(int index)
{
    mediaPlayer->setCodec(index);
}

void MainWindow::on_pause_streaming_button_clicked()
{
    mediaPlayer->pause_streaming();
}

void MainWindow::on_checkBox_show_hide_stream_stateChanged(int arg1) {

    mediaPlayer->show_hide_stream_stateChanged(arg1);

}

void MainWindow::on_camera_show_button_clicked() {

    mediaPlayer->on_camera_show_button_clicked();

}
