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
      _media(NULL), _streamingMedia(NULL),
      _instance(NULL), _player(NULL),
      _streamingPlayer(NULL), sock(NULL), codec(""),
      file("")
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

    sock->disconnect();
    delete sock;
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
    parent_ui->start_streaming_button->setEnabled(false);
    parent_ui->stop_stream_to_vr->setEnabled(false);
    parent_ui->comboBox_coding->setEnabled(false);
    parent_ui->pause_streaming_button->setEnabled(false);

    sock = new QTcpSocket();
    connect(sock, SIGNAL(readyRead()), this, SLOT(readData()));

    codec = "#transcode{vcodec=h264,vb=2000,venc=x264{profile=baseline},width=1280,height=720,acodec=mp3,ab=192,channels=2,samplerate=44100}";
}

void VideoPlayer::openLocal()
{
    file = QFileDialog::getOpenFileName(this, tr("Open file"),
                                         QDir::currentPath(),
                                         tr("Multimedia files(*)"));

    if (file.isEmpty())
        return;
    _media = new VlcMedia(file, true, _instance);
    _player->open(_media);

    parent_ui->start_continue_button->setEnabled(false);
    parent_ui->pause->setEnabled(true);
    parent_ui->stop->setEnabled(true);
    parent_ui->start_streaming_button->setEnabled(true);

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

    _streamingMedia = new VlcMedia(_media->core());

    QString options = ":sout=" + codec + ":http{mux=ffmpeg{mux=flv},dst=:8088/ch1}";
    _streamingMedia->setOption(options);

    _streamingPlayer->setPosition(_player->position());
    _streamingPlayer->open(_streamingMedia);

    parent_ui->connect_to_ver_button->setEnabled(true);
    parent_ui->stop_streaming_button->setEnabled(true);
    parent_ui->start_streaming_button->setEnabled(false);
    parent_ui->comboBox_coding->setEnabled(false);
    parent_ui->checkBox__enable_coding->setEnabled(false);
    parent_ui->pause_streaming_button->setEnabled(true);

}

void VideoPlayer::on_stop_streaming_button_clicked()
{
     _streamingPlayer->stop();

     if (parent_ui->stop_stream_to_vr->isEnabled())
         this->stop_streaming_to_VR();

     parent_ui->stop_streaming_button->setEnabled(false);
     parent_ui->start_streaming_button->setEnabled(true);
     parent_ui->checkBox__enable_coding->setEnabled(true);
     parent_ui->checkBox__enable_coding->setChecked(false);
     parent_ui->pause_streaming_button->setEnabled(false);
}

void VideoPlayer::stop_streaming_to_VR() {
    if (sock->isOpen()) {
        QString mes = "@@stop";
        sock->write(mes.toStdString().c_str());
        sock->waitForBytesWritten(1000);
        sock->waitForReadyRead(1000);
    }
    parent_ui->stop_stream_to_vr->setEnabled(false);
    parent_ui->connect_to_ver_button->setEnabled(true);
}

void VideoPlayer::readData()
{
    QString reply = QString::fromUtf8(sock->readAll().toStdString().c_str());
    if (reply != "")
        QMessageBox().information(this, tr("Reply from host"), reply);
    else
        QMessageBox().information(this, tr("Reply from host"), "Host is unreacheable. Try again");
}

void VideoPlayer::on_connect_to_ver_button_clicked()
{
    QString url =
            QInputDialog::getText(this, tr("Connect to host"), tr("Enter the address you want to connect"));

    if (url.isEmpty())
        return;

    if (!_streamingMedia || _streamingPlayer->state() == Vlc::Stopped) {
        QMessageBox().warning(this, tr("Warning!"),"Stream is not set. Set it first.");
        return;
    }

    QString mes = "http://192.168.1.117:8088/ch1";

    if (!sock->isOpen()) {

        sock->connectToHost(url, 8080);

        if(sock->waitForConnected(2000)) {
          sock->write(mes.toStdString().c_str());
          sock->waitForBytesWritten(1000);
          sock->waitForReadyRead(1000);
        }
    } else {
        sock->connectToHost(url, 8080);

        if(sock->waitForConnected(2000)) {
          sock->write(mes.toStdString().c_str());
          sock->waitForBytesWritten(1000);
          sock->waitForReadyRead(3000);
        }
    }
    parent_ui->stop_stream_to_vr->setEnabled(true);
    parent_ui->connect_to_ver_button->setEnabled(false);
}

void VideoPlayer::pauseVideo() {
    if (_player == NULL)
        return;
    _player->pause();
    parent_ui->pause->setEnabled(false);
    parent_ui->start_continue_button->setEnabled(true);
}

void VideoPlayer::stopVideoPlayer() {
    if (_player == NULL)
        return;
    _player->stop();
    parent_ui->start_continue_button->setEnabled(true);
    parent_ui->pause->setEnabled(false);
}

void VideoPlayer::startVideo() {
    if (_player == NULL)
        return;
    _player->play();
    parent_ui->start_continue_button->setEnabled(false);
    parent_ui->pause->setEnabled(true);
    parent_ui->stop->setEnabled(true);
}

void VideoPlayer::send_to_server() {
    QString command =
            QInputDialog::getText(this, tr("Command to host"), tr("Enter the command"));

    if (command.isEmpty())
        return;

    if (sock == NULL)
        return;

    sock->write(command.toStdString().c_str());
    sock->waitForBytesWritten(1000);
    sock->waitForReadyRead(2000);
}

void VideoPlayer::setCodec(int index) {
    switch (index) {
        case 0:
            codec = "#transcode{vcodec=h264,acodec=mpga,ab=128,channels=2,samplerate=44100}";
        break;
        case 1:
            codec = "#transcode{vcodec=h264,vb=800,acodec=mpga,ab=128,channels=2,samplerate=44100}";
        break;
        case 2:
            codec = "#transcode{vcodec=hevc,acodec=mpga,ab=128,channels=2,samplerate=44100}";
        break;
        case 3:
            codec = "#transcode{vcodec=mp2v,vb=800,acodec=mpga,ab=128,channels=2,samplerate=44100}";
        break;
        case 4:
            codec = "#transcode{vcodec=none,acodec=mp3,ab=128,channels=2,samplerate=44100}";
        break;
        case 5:
            codec = "#transcode{vcodec=h264,vb=2000,venc=x264{profile=baseline},width=1280,height=720,acodec=mp3,ab=192,channels=2,samplerate=44100}";
        break;
        default:
            codec = "#transcode{vcodec=h264,acodec=mpga,ab=128,channels=2,samplerate=44100}";
    }
}

void VideoPlayer::enableCoding(int arg) {
    if (arg)
        parent_ui->comboBox_coding->setEnabled(true);
    else
        parent_ui->comboBox_coding->setEnabled(false);
}

void VideoPlayer::pause_streaming() {
    if (_streamingPlayer->state() == Vlc::Playing) {
        _streamingPlayer->pause();
    }
    if (_streamingPlayer->state() == Vlc::Paused) {
        _streamingPlayer->play();
    }

    if (sock == NULL)
        return;
    QString command = "@@pause_stream";
    sock->write(command.toStdString().c_str());
    sock->waitForBytesWritten(1000);
    sock->waitForReadyRead(2000);
}

void VideoPlayer::show_hide_stream_stateChanged(int arg1)
{
    if (arg1) {
        parent_ui->videoSurface->setVisible(true);
    } else {
        parent_ui->videoSurface->setVisible(false);
    }
}
