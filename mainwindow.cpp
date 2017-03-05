#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    mediaPlayer = NULL;
    ui->setupUi(this);
}

void MainWindow::createPlayer()
{
    if (mediaPlayer != NULL) {
        mediaPlayer->close();
        delete mediaPlayer;
    }
    mediaPlayer = new SimplePlayer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_enter_button_clicked()
{
    if (ui->options_comboBox->currentIndex() == 0)
        createPlayer();
    if (mediaPlayer != NULL)
        mediaPlayer->show();
}
