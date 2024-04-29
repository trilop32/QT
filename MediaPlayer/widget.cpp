#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QStyle>
#include <QTime>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    // Buttons style
    ui->pushButtonPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->pushButtonPouse->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    ui->pushButtonStop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    ui->pushButtonPrev->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    ui->pushButtonNext->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    //Init player

    m_player=new QMediaPlayer();
    ui->labelVolume->setText(QString("Volume: ").append(QString::number(m_player->volume())));
    ui->horizontalSliderVolume->setValue(m_player->volume());

    connect(ui->pushButtonPlay,&QPushButton::clicked,this->m_player,&QMediaPlayer::play);
    connect(ui->pushButtonStop,&QPushButton::clicked,this->m_player,&QMediaPlayer::stop);
    connect(ui->pushButtonPouse,&QPushButton::clicked,this->m_player,&QMediaPlayer::pause);
    connect(m_player,&QMediaPlayer::durationChanged,this,&Widget::on_durationChanged);
}

Widget::~Widget()
{
    delete m_player;
    delete ui;
}


void Widget::on_pushButtonOpen_clicked()
{
   QString file=QFileDialog::getOpenFileName(this,"Open file",NULL,"Audio files()");
   ui->labeleFile->setText(file);
   m_player->setMedia(QUrl::fromLocalFile(file));
   m_player->play();
}


void Widget::on_horizontalSliderVolume_valueChanged(int value)
{
    m_player->setVolume(value);
    ui->labelVolume->setText(QString("Volume: ").append(QString::number(m_player->volume())));
}

void Widget::on_durationChanged(quint64 duration)
{
    ui->horizontalSliderProgress->setMaximum(duration);
    QTime qt_duration=QTime::fromMSecsSinceStartOfDay(duration);
    ui->labelDuration->setText(QString("Duration: ").append(qt_duration.toString(duration<3600000?"mm:ss":"hh:mm:ss")));

}

