#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QMediaPlayer>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
QT_END_NAMESPACE

namespace Ui {
class MainWindow;
}
namespace AudioRecordEngineLib
{
    class AudioRecordEngine;
    class AudioRecordEngineSettings;
}

using namespace AudioRecordEngineLib;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    AudioRecordEngine * m_audioRecordEngine;
    const AudioRecordEngineSettings * m_lastSettings;
    QMediaPlayer m_mediaPlayer;
protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void newFile();
    bool save();
    bool saveAs();
    void about();
    void toogleRecord(bool state);
    void tooglePlay(bool state);
    void pressStop();
    void settingsWindow();
    void updateRecordingTime();
    void playerPositionChanged(qint64);
    void playerStateChangedHandle(QMediaPlayer::State);
    void playerSliderValueChanged(int);
private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool maybeSave();
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    void startStatusUpdate();
    void stopStatusUpdate();
    QString convertToTimeString(qint64 len);
    QString curFile;
    bool modified;
    QTimer statusUpdateTimer;
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QAction *newAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *settingsAct;
    QAction *aboutAct;
};

#endif // MAINWINDOW_H
