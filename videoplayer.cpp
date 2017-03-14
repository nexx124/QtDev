#include <QFileDialog>
#include <QInputDialog>
#include <VLCQtCore/Common.h>
#include <VLCQtCore/Instance.h>
#include <VLCQtCore/Media.h>
#include <VLCQtCore/MediaPlayer.h>
#include <QMessageBox>

#include "videoplayer.h"
#include "ui_mainwindow.h"

VideoPlayer::VideoPlayer(QWidget *parent)
    : QWidget(parent),
      parent_ui(new Ui::MainWindow),
      _media(0), _streamingMedia(0),
      _instance(0), _player(0), _streamingPlayer(0)
{

}

VideoPlayer::~VideoPlayer()
{
    delete _player;
    delete _streamingPlayer;
    delete _media;
    delete _streamingMedia;
    delete _instance;
    delete parent_ui;
}

void VideoPlayer::createSettings() {
    if (_instance != NULL)
        delete _instance;
    if (_player != NULL)
        delete _player;
    if (_streamingPlayer != NULL)
        delete _streamingPlayer;

    _instance = new VlcInstance(VlcCommon::args(), this);
    _player = new VlcMediaPlayer(_instance);
    _streamingPlayer = new VlcMediaPlayer(_instance);

    _player->setVideoWidget(parent_ui->videoSurface);

    parent_ui->videoSurface->setMediaPlayer(_player);
    parent_ui->volume->setMediaPlayer(_player);
    parent_ui->volume->setVolume(50);
    parent_ui->seek->setMediaPlayer(_player);

    parent_ui->stop_streaming_button->setEnabled(false);
    parent_ui->stop->setEnabled(false);
    parent_ui->start_continue_button->setEnabled(false);
    parent_ui->pause->setEnabled(false);
}

void VideoPlayer::openLocal()
{
    QString file =
            QFileDialog::getOpenFileName(this, tr("Open file"),
                                         QDir::homePath(),
                                         tr("Multimedia files(*)"));

    if (file.isEmpty())
        return;
    _media = new VlcMedia(file, true, _instance);
    _player->open(_media);
    _player->play();
    parent_ui->start_continue_button->setEnabled(false);
    parent_ui->pause->setEnabled(true);
    parent_ui->stop->setEnabled(true);

}

void VideoPlayer::openUrl()
{
    QString url =
            QInputDialog::getText(this, tr("Open Url"), tr("Enter the URL you want to play"));

    if (url.isEmpty())
        return;

    _media = new VlcMedia(url, _instance);

    _player->open(_media);
}

void VideoPlayer::on_start_streaming_button_clicked()
{
    QString file =
            QFileDialog::getOpenFileName(this, tr("Open file"),
                                         QDir::homePath(),
                                         tr("Multimedia files(*)"));

    if (file.isEmpty())
        return;

    _streamingMedia = new VlcMedia(file, true, _instance);
    _streamingMedia->setOption(":sout=#transcode{vcodec=h264,vb=800,acodec=mpga,ab=128,channels=2,samplerate=44100}"
                               ":rtp{dst=192.168.1.117,port=4444,sdp=rtsp://192.168.1.117:4444/ch1,mux=ts,ttl=1}");

    _streamingPlayer->open(_streamingMedia);
    _streamingPlayer->play();

    parent_ui->connect_to_ver_button->setEnabled(true);
    parent_ui->stop_streaming_button->setEnabled(true);

}

void VideoPlayer::readData()
{
    QString reply = QString::fromUtf8(sock->readAll().toStdString().c_str());
    if (!reply.isEmpty())
        QMessageBox().information(this, tr("Reply from host"), reply);
}

void VideoPlayer::on_connect_to_ver_button_clicked()
{
    QString url =
            QInputDialog::getText(this, tr("Connect to host"), tr("Enter the address you want to connect"));

    if (url.isEmpty())
        return;

    if (!_streamingMedia) {
        QMessageBox().warning(this, tr("Warning!"),"Stream is not set. Set it first.");
        return;
    }
    sock = new QTcpSocket();
    sock->connectToHost(url, 8080);
        QString mes = "rtsp://192.168.1.117:4444/ch1";
        sock->write(mes.toStdString().c_str());
        connect(sock, SIGNAL(readyRead()), this, SLOT(readData()));
}

void VideoPlayer::on_stop_streaming_button_clicked()
{
    if (_streamingPlayer != NULL) {
        _streamingPlayer->stop();
        parent_ui->stop_streaming_button->setEnabled(false);
    }
}

void VideoPlayer::pauseVideo() {
    if (_player == NULL)
        return;
    _player->pause();
}

void VideoPlayer::stopVideoPlayer() {
    if (_player == NULL)
        return;
    _player->stop();
}

void VideoPlayer::startVideo() {
    if (_player == NULL)
        return;
    _player->play();
}
