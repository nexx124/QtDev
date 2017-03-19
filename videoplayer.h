#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QWidget>
#include <QtNetwork/QTcpSocket>
#include "ui_mainwindow.h"

namespace Ui {
    class MainWindow;
}

class VlcInstance;
class VlcMedia;
class VlcMediaPlayer;

class VideoPlayer : public QWidget
{
    Q_OBJECT
public:
    explicit VideoPlayer(QWidget *parent = 0);
    ~VideoPlayer();

    void setupUI(Ui::MainWindow *ui) {parent_ui = ui;}
    void createSettings();
    void setCodec(int);
    void enableCoding(int);

public slots:
    void openLocal();

    void openUrl();

    void on_start_streaming_button_clicked();

    void on_connect_to_ver_button_clicked();

    void readData();

    void on_stop_streaming_button_clicked();

    void pauseVideo();

    void stopVideoPlayer();

    void startVideo();

    void stop_streaming_to_VR();

    void send_to_server();

    void pause_streaming();

    void show_hide_stream_stateChanged(int arg1);

private:

    Ui::MainWindow *parent_ui;

    VlcInstance *_instance;
    VlcMedia *_media;
    VlcMediaPlayer *_player;
    VlcMedia *_streamingMedia;
    VlcMediaPlayer *_streamingPlayer;

    QTcpSocket *sock;

    QString codec;
    QString file;
};


#endif // VIDEOPLAYER_H
