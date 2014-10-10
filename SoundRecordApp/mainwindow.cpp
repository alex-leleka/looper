#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtWidgets>

#include "audiorecordengine.h"
#include "audiorecordenginesettings.h"
#include "recordersettings.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), m_audioRecordEngine(new AudioRecordEngine),
    m_lastSettings(0), modified(false)
{
    ui->setupUi(this);
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    readSettings();

    connect(ui->playButton, SIGNAL(clicked(bool)),
            this, SLOT(tooglePlay(bool)));
    connect(ui->recordButton, SIGNAL(clicked(bool)),
            this, SLOT(toogleRecord(bool)));
    connect(ui->stopButton, SIGNAL(clicked()),
            this, SLOT(pressStop()));
    connect(&statusUpdateTimer,SIGNAL(timeout()), this,
            SLOT(updateRecordingTime()));
    connect(&m_mediaPlayer, SIGNAL(positionChanged(qint64)),
            this, SLOT(playerPositionChanged(qint64)));
    connect(&m_mediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)),
                                   this, SLOT(playerStateChangedHandle(QMediaPlayer::State)));
    connect(ui->playProgresSlider, SIGNAL(valueChanged(int)),
            this, SLOT(playerSliderValueChanged(int)));
    connect(ui->volumeSlider, SIGNAL(valueChanged(int)),
            &m_mediaPlayer, SLOT(setVolume(int)));
    setCurrentFile("");
    setUnifiedTitleAndToolBarOnMac(true);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_audioRecordEngine;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::newFile()
{
    if (maybeSave()) {
        modified = false;
        m_audioRecordEngine->clear();
        setCurrentFile("");
    }
}

bool MainWindow::save()
{
    if (curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool MainWindow::saveAs()
{
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.exec();
    QStringList files = dialog.selectedFiles();

    if (files.isEmpty())
        return false;

    return saveFile(files.at(0));
}
void MainWindow::about()
{
   QMessageBox::about(this, tr("About SoundRecordApp"),
            tr("The <b>SoundRecordApp</b> software is a simple program "
               "for recording audio and saving it without"
               "compression using cross-platform Qt liberaries."));
}

void MainWindow::createActions()
{
    newAct = new QAction(QIcon(":/images/new.png"), tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the recording to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(tr("Save &As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the recording under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the SoundRecordApp"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
    settingsAct = new QAction(tr("Settings"), this);
    settingsAct->setStatusTip(tr("Select recording quality and sound source"));
    connect(settingsAct, SIGNAL(triggered()), this, SLOT(settingsWindow()));
}

void MainWindow::settingsWindow()
{
    AudioRecordEngineSettings settings(m_audioRecordEngine);
    RecorderSettings w(&settings, this);
    w.exec();
    m_audioRecordEngine->applyAudioRecorderSettings(settings);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(settingsAct);
    helpMenu->addAction(aboutAct);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(saveAct);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::readSettings()
{
    QSettings settings("QtProject", "SoundRecordApp");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    ui->volumeSlider->setValue(settings.value("playerVolume", 50).toInt());

    int bitRate = settings.value("audiorecordsettings_bitRate", -1).toInt();
    int channelCount = settings.value("audiorecordsettings_channelCount", -1).toInt();
    QString  codec = settings.value("audiorecordsettings_codec", QString("audio/PCM")).toString();
    int sampleRate = settings.value("audiorecordsettings_sampleRate", 0).toInt();
    QString input = settings.value("audioinputdevice", QString("")).toString();
    QString container = settings.value("audiorecordcontainer", QString("wav")).toString();
    AudioRecordEngineSettings engineSettings(m_audioRecordEngine);
    engineSettings.setBitRate(bitRate);
    engineSettings.setChannelCount(channelCount);
    engineSettings.setCodec(codec);
    engineSettings.setSampleRate(sampleRate);
    engineSettings.setAudioInputDevice(input);
    engineSettings.setContainer(container);
    m_audioRecordEngine->applyAudioRecorderSettings(engineSettings);
    resize(size);
    move(pos);
}

void MainWindow::writeSettings()
{
    modified = false;
    QSettings settings("QtProject", "SoundRecordApp");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
    settings.setValue("playerVolume", ui->volumeSlider->value());
    if(m_lastSettings)
    {
        settings.setValue("audiorecordsettings_bitRate", QVariant(m_lastSettings->settings().bitRate()));
        settings.setValue("audiorecordsettings_channelCount", QVariant(m_lastSettings->settings().channelCount()));
        settings.setValue("audiorecordsettings_codec", QVariant(m_lastSettings->settings().codec()));
        settings.setValue("audiorecordsettings_sampleRate", QVariant(m_lastSettings->settings().sampleRate()));
        settings.setValue("audioinputdevice", QVariant(m_lastSettings->audioInputDevice()));
        settings.setValue("audiorecordcontainer", QVariant(m_lastSettings->container()));
    }
}

bool MainWindow::maybeSave()
{
    if (modified && (m_audioRecordEngine->getRecordSize() != qint64(0))) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("SoundRecordApp"),
                     tr("Do you want to save your recording?"),
                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

bool MainWindow::saveFile(const QString &fileName)
{
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    if (!m_audioRecordEngine->saveRecord(fileName)) {
        QMessageBox::warning(this, tr("SoundRecordApp"),
                             tr("Cannot write file %1.")
                             .arg(fileName));
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
        return false;
    }
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    modified = false;
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}
void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    setWindowModified(false);
    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "untitled";
    setWindowFilePath(shownName);
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::toogleRecord(bool state)
{
    // rec or pause action
    if(state)
    {
        if(!m_audioRecordEngine->isRecording())
            maybeSave();
        m_audioRecordEngine->startRecording();
        startStatusUpdate();
    }
    else
    {
        m_audioRecordEngine->pauseRecording();
        stopStatusUpdate();
    }
}

void MainWindow::tooglePlay(bool state)
{
    if(m_audioRecordEngine->isRecording() || (m_audioRecordEngine->getRecordSize() < 1))
    {
        ui->playButton->setChecked(false);
        return;
    }
    // play or pause action
    if(state)
    {
        if(m_mediaPlayer.state() == QMediaPlayer::StoppedState)
        {
            m_mediaPlayer.setMedia(QUrl::fromLocalFile(m_audioRecordEngine->getAudioFileName()));
            m_mediaPlayer.play();
        } else
        if(m_mediaPlayer.state() == QMediaPlayer::PausedState)
        {
            m_mediaPlayer.play();
        }
    }
    else
    {
        if(m_mediaPlayer.state() == QMediaPlayer::PlayingState)
        {
            m_mediaPlayer.pause();
        }
    }
}

void MainWindow::pressStop()
{
    if(m_audioRecordEngine->isRecording())
    {
        m_audioRecordEngine->stopRecording();
        stopStatusUpdate();
    }
    else
    {// stop playing
        if((m_mediaPlayer.state() == QMediaPlayer::PlayingState)||(m_mediaPlayer.state() == QMediaPlayer::PausedState))
        {
            m_mediaPlayer.stop();
        }
    }
    ui->recordButton->setChecked(false);
    ui->playButton->setChecked(false);
}

void MainWindow::startStatusUpdate()
{
    statusUpdateTimer.start(100);
}

void MainWindow::stopStatusUpdate()
{
    statusUpdateTimer.stop();
}

QString MainWindow::convertToTimeString(qint64 len)
{
    QString time;
    const int SeconLen = 1000; //ms
    if(len > 3600 * SeconLen)
    {
        time += QString::number(int(len / (3600 * SeconLen))) + QString(":");
        len = len % (3600 * SeconLen);
    }
    time += QString::number(int(len / (60 * SeconLen))) + QString(":");
    len = len % (60 * SeconLen);
    time += QString::number(int(len / SeconLen)) + QString(".");
    len = len % SeconLen;
    time += QString::number(int(len / 10));

    return time;
}

void MainWindow::playerSliderValueChanged(int val)
{
    qint64 newPosition = qint64(double(val) / ui->playProgresSlider->maximum() * m_mediaPlayer.duration());
    m_mediaPlayer.setPosition(newPosition);
}

void MainWindow::updateRecordingTime()
{
    QString time = convertToTimeString(m_audioRecordEngine->duration());
    statusBar()->showMessage(tr("Recording: ") + time);
}

void MainWindow::playerPositionChanged(qint64 val)
{
    // update playing progres slider
    double percentageProgres = double(val)/ m_mediaPlayer.duration();
    int progresVal = ui->playProgresSlider->maximum() * percentageProgres;
    bool prevSliderSignalsState = ui->playProgresSlider->blockSignals(true);
    ui->playProgresSlider->setValue(progresVal);
    ui->playProgresSlider->blockSignals(prevSliderSignalsState);
    // update status bar
    QString time = convertToTimeString(val);
    statusBar()->showMessage(tr("Playing: ") + time);
}

void MainWindow::playerStateChangedHandle(QMediaPlayer::State state)
{
    if(QMediaPlayer::StoppedState == state)
    {
        pressStop();
        ui->playProgresSlider->setEnabled(false);
    }
    if(QMediaPlayer::PlayingState == state)
    {
        ui->playProgresSlider->setEnabled(true);
    }
}
