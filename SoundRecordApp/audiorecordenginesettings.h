#ifndef AUDIORECORDENGINESETTINGS_H
#define AUDIORECORDENGINESETTINGS_H

#include <QAudioEncoderSettings>
#include <QVector>
class QAudioRecorder;

namespace AudioRecordEngineLib {
    class AudioRecordEngine;
    class AudioRecordEngineSettings
    {
        QAudioEncoderSettings m_settings;
        QString m_container;
        QString m_audioInputDevice;
        const QAudioRecorder * audioRecorder;
    public:
        AudioRecordEngineSettings(const AudioRecordEngine * audioRecorder);
        QAudioEncoderSettings settings() const;
        void setSettings(const QAudioEncoderSettings &settings);
        QString container() const;
        QString audioInputDevice() const;
        void setAudioInputDevice(const QString &);
        void setContainer(const QString &container);
        void setCodec(const QString &);
        void setSampleRate(int);
        void setBitRate(int);
        void setChannelCount(int);
        QVector<QString> getAudioInputDevices() const;
        QVector<QString> getContainers() const;
        QVector<QString> getCodecs() const;
        QVector<int> getSampleRates() const;
        QVector<int> getBitRates() const;
        QVector<int> getChannelCounts() const;
        void applyAudioRecorderSettings(AudioRecordEngine * audioRecorder) const;
    };
}
#endif // AUDIORECORDENGINESETTINGS_H
