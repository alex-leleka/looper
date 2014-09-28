#ifndef RECORDERSETTINGS_H
#define RECORDERSETTINGS_H

#include <QDialog>

namespace Ui {
class RecorderSettings;
}
namespace AudioRecordEngineLib {
    class AudioRecordEngineSettings;
}
class RecorderSettings : public QDialog
{
    Q_OBJECT

public:
    explicit RecorderSettings(AudioRecordEngineLib::AudioRecordEngineSettings * s,QWidget *parent = 0);
    ~RecorderSettings();

private:
    Ui::RecorderSettings *ui;
    AudioRecordEngineLib::AudioRecordEngineSettings * m_settings;
private slots:
    void saveSettings();
};

#endif // RECORDERSETTINGS_H
