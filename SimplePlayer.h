/*
* VLC-Qt Simple Player
* Copyright (C) 2015 Tadej Novak <tadej@tano.si>
*/

#ifndef SIMPLEPLAYER_H_
#define SIMPLEPLAYER_H_

#include <QMainWindow>
#include <QtNetwork/QTcpSocket>

namespace Ui {
    class SimplePlayer;
}

class VlcInstance;
class VlcMedia;
class VlcMediaPlayer;

class EqualizerDialog;

class SimplePlayer : public QMainWindow
{
    Q_OBJECT
public:
    explicit SimplePlayer(QWidget *parent = 0);
    ~SimplePlayer();

private slots:
    void openLocal();
    void openUrl();

    void on_start_streaming_button_clicked();

    void on_connect_to_ver_button_clicked();
    void readData();

private:
    Ui::SimplePlayer *ui;

    VlcInstance *_instance;
    VlcMedia *_media;
    VlcMediaPlayer *_player;
    VlcMedia *_streamingMedia;
    VlcMediaPlayer *_streamingPlayer;

    EqualizerDialog *_equalizerDialog;

    QTcpSocket *sock;
};

#endif // SIMPLEPLAYER_H_
