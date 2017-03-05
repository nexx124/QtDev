/*
* VLC-Qt Simple Player
* Copyright (C) 2015 Tadej Novak <tadej@tano.si>
*/

#include <QFileDialog>
#include <QInputDialog>

#include <VLCQtCore/Common.h>
#include <VLCQtCore/Instance.h>
#include <VLCQtCore/Media.h>
#include <VLCQtCore/MediaPlayer.h>

#include "EqualizerDialog.h"

#include "SimplePlayer.h"
#include "ui_SimplePlayer.h"

SimplePlayer::SimplePlayer(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::SimplePlayer),
      _media(0), _streamingMedia(0),
      _equalizerDialog(new EqualizerDialog(this))
{
    ui->setupUi(this);

    _instance = new VlcInstance(VlcCommon::args(), this);
    _player = new VlcMediaPlayer(_instance);
    _streamingPlayer = new VlcMediaPlayer(_instance);
    _player->setVideoWidget(ui->video);
    _equalizerDialog->setMediaPlayer(_player);

    ui->video->setMediaPlayer(_player);
    ui->volume->setMediaPlayer(_player);
    ui->volume->setVolume(50);
    ui->seek->setMediaPlayer(_player);

    connect(ui->actionOpenLocal, &QAction::triggered, this, &SimplePlayer::openLocal);
    connect(ui->actionOpenUrl, &QAction::triggered, this, &SimplePlayer::openUrl);
    connect(ui->actionPause, &QAction::toggled, _player, &VlcMediaPlayer::togglePause);
    connect(ui->actionStop, &QAction::triggered, _player, &VlcMediaPlayer::stop);
    connect(ui->openLocal, &QPushButton::clicked, this, &SimplePlayer::openLocal);
    connect(ui->openUrl, &QPushButton::clicked, this, &SimplePlayer::openUrl);
    connect(ui->pause, &QPushButton::toggled, ui->actionPause, &QAction::toggle);
    connect(ui->stop, &QPushButton::clicked, _player, &VlcMediaPlayer::stop);
    connect(ui->equalizer, &QPushButton::clicked, _equalizerDialog, &EqualizerDialog::show);
}

SimplePlayer::~SimplePlayer()
{
    delete _player;
    delete _streamingPlayer;
    delete _media;
    delete _streamingMedia;
    delete _instance;
    delete ui;
}

void SimplePlayer::openLocal()
{
    QString file =
            QFileDialog::getOpenFileName(this, tr("Open file"),
                                         QDir::homePath(),
                                         tr("Multimedia files(*)"));

    if (file.isEmpty())
        return;

    _streamingMedia = new VlcMedia(file, true, _instance);
    _streamingMedia->setOption(":sout=#transcode{vcodec=h264,vb=0,scale=0,acodec=mpga,ab=128,channels=2,samplerate=44100}"
                               ":rtp{dst=192.168.1.117,port=1234,sdp=rtsp://192.168.1.117/ch1,mux=ts,ttl=1}");

    _streamingPlayer->open(_streamingMedia);
    _streamingPlayer->play();

    //_media = new VlcMedia(file, true, _instance);
    QString url = "rtsp://192.168.1.117/ch1";
    _media = new VlcMedia(url, _instance);
    _player->open(_media);
    _player->play();

}

void SimplePlayer::openUrl()
{
    QString url =
            QInputDialog::getText(this, tr("Open Url"), tr("Enter the URL you want to play"));

    if (url.isEmpty())
        return;

    _media = new VlcMedia(url, _instance);

    _player->open(_media);
}
