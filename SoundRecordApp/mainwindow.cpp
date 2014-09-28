#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtWidgets>

#include "audiorecordengine.h"
#include "audiorecordenginesettings.h"
#include "recordersettings.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), m_audioRecordEngine(new AudioRecordEngine),
    modified(false)
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

    setCurrentFile("");
    setUnifiedTitleAndToolBarOnMac(true);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_audioRecordEngine;
}

void MainWindow::closeEvent(QCloseEvent *event)
//! [3] //! [4]
{
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}
//! [4]

//! [5]
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
            tr("The <b>SoundRecordApp</b> software is a simple program"
               "for recording audio and saving it wothout"
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
//! [20]
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
//! [20]
    exitAct->setStatusTip(tr("Exit the SoundRecordApp"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

//! [21]

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
    settings.applyAudioRecorderSettings(m_audioRecordEngine);
}

void MainWindow::createMenus()
//! [25] //! [27]
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
//! [28]
    fileMenu->addAction(saveAct);
//! [26]
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(settingsAct);
    helpMenu->addAction(aboutAct);
}
//! [27]

//! [29] //! [30]
void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAct);
//! [31]
    fileToolBar->addAction(saveAct);
}
//! [30]

//! [32]
void MainWindow::createStatusBar()
//! [32] //! [33]
{
    statusBar()->showMessage(tr("Ready"));
}
//! [33]

//! [34] //! [35]
void MainWindow::readSettings()
//! [34] //! [36]
{
    QSettings settings("QtProject", "SoundRecordApp");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    resize(size);
    move(pos);
}
//! [35] //! [36]

//! [37] //! [38]
void MainWindow::writeSettings()
//! [37] //! [39]
{    modified = false;
    QSettings settings("QtProject", "SoundRecordApp");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}
//! [38] //! [39]

//! [40]
bool MainWindow::maybeSave()
//! [40] //! [41]
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
//! [48] //! [49]
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::toogleRecord(bool state)
{
    // rec or pause action
    if(state)
    {
        m_audioRecordEngine->startRecording();
    }else
    {
        m_audioRecordEngine->pauseRecording();
    }
}

void MainWindow::tooglePlay(bool state)
{
    if(m_audioRecordEngine->isRecording())
        return;
    // play or pause action
    if(state)
    {

    }else
    {

    }
}

void MainWindow::pressStop()
{
    if(m_audioRecordEngine->isRecording())
        m_audioRecordEngine->stopRecording();
    else
    {// stop playing
    }
}
