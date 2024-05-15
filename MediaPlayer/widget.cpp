#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QStyle>
#include <QTime>
#include <QDir>
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
    ui->pushButtonMute->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    //Init player

    m_player=new QMediaPlayer();
    m_player->setVolume(0);
    ui->labelVolume->setText(QString("Volume: ").append(QString::number(m_player->volume())));
    ui->horizontalSliderVolume->setValue(m_player->volume());

    connect(ui->pushButtonPlay,&QPushButton::clicked,this->m_player,&QMediaPlayer::play);
    connect(ui->pushButtonStop,&QPushButton::clicked,this->m_player,&QMediaPlayer::stop);
    connect(ui->pushButtonPouse,&QPushButton::clicked,this->m_player,&QMediaPlayer::pause);

    connect(m_player,&QMediaPlayer::durationChanged,this,&Widget::on_durationChanged);
    connect(m_player,&QMediaPlayer::positionChanged,this,&Widget::on_positionChanged);

    //  Init playlist
    m_playlist_model=new QStandardItemModel(this);
    ui->tableViewPlaylist->setModel(m_playlist_model);
    m_playlist_model->setHorizontalHeaderLabels(QStringList()<<"Audio track" << "File");

    ui->tableViewPlaylist->hideColumn(1);
    ui->tableViewPlaylist->header()->setStretchLastSection(true);
    ui->tableViewPlaylist->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_playlist=new QMediaPlaylist(m_player);
    m_player->setPlaylist(m_playlist);

    connect(ui->pushButtonPrev,&QPushButton::clicked,m_playlist,&QMediaPlaylist::previous);
    connect(ui->pushButtonNext,&QPushButton::clicked,m_playlist,&QMediaPlaylist::next);

    connect(m_playlist,&QMediaPlaylist::currentIndexChanged,this,&Widget::on_current_Index_Changed);
}

Widget::~Widget()
{
    delete m_playlist_model;
    delete m_playlist;
    delete m_player;
    delete ui;
}

void Widget::on_pushButtonOpen_clicked()
{
//   QString file=QFileDialog::getOpenFileName(this,"Open file",NULL,"Audio files(*mp3)");
//   /*ui->labeleFile->setText(file);*/

//   QFileInfo fileInfo(file);
//   QString song = fileInfo.baseName();
//   ui->labeleFile->setText(song);

//   m_player->setMedia(QUrl::fromLocalFile(file));
//   m_player->media();
//   this->setWindowTitle(QString("MediaPlayer - ").append(file.split('/').last()));
    QStringList files=QFileDialog::getOpenFileNames(this,"Open files","C:\\Users\\serge\\OneDrive\\Рабочий стол\\Язык программирования С++\\Системное програмирование",
                                                    "Audio file (*mp3 *flac);;mp3(*mp3);; FLAC (*flac)" );
    for(QString file:files)
    {
        QList<QStandardItem*>items;
        items.append(new QStandardItem(QDir(file).dirName()));
        items.append(new QStandardItem(file));
        m_playlist_model->appendRow(items);
        m_playlist->addMedia(QUrl(file));
    }
}

void Widget::on_pushButtonPlay_clicked()
{
   m_player->play();
}

void Widget::on_pushButtonMute_clicked()
{
    m_player->setMuted(!m_player->isMuted());
    ui->pushButtonMute->setIcon(style()->standardIcon(m_player->isMuted()?  QStyle::SP_MediaVolumeMuted : QStyle::SP_MediaVolume));

    ui->horizontalSliderVolume->setValue(0);
    ui->labelVolume->setText(QString("Volume: ").append(QString::number(0)));
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

void Widget::on_positionChanged(qint64 position)
{
   ui->horizontalSliderProgress->setValue(position);
   QTime qt_position=QTime::fromMSecsSinceStartOfDay(position);
   ui->labelProgress->setText(QString("Progress: ").append(qt_position.toString(position<3600000?"mm:ss":"hh:mm:ss")));
}

void Widget::on_horizontalSliderProgress_valueChanged(int value)
{
    if(ui->horizontalSliderProgress->isSliderDown())
        m_player->setPosition(value);
}

void Widget::on_current_Index_Changed(int position)
{
    //ui->tableViewPlaylist->selectRow(position);
    QStandardItem* song = m_playlist_model->item(position,0);
    this->setWindowTitle(QString("MediaPlayer: ").append(song->text()));
    QStandardItem* file= m_playlist_model->item(position,1);
    ui->labeleFile->setText(QString("Song file: ").append(file->text()));
}

