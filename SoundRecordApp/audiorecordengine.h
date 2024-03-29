#ifndef AUDIORECORDENGINE_H
#define AUDIORECORDENGINE_H

#include <QMediaRecorder>
#include <QUrl>
#include <QObject>
#include <QString>
#include <QTemporaryFile>

QT_BEGIN_NAMESPACE
class QAudioRecorder;
class QAudioProbe;
QT_END_NAMESPACE

namespace AudioRecordEngineLib {
    class AudioRecordEngineSettings;

    class AudioRecordEngine : public QObject
    {
        friend class AudioRecordEngineSettings;
        Q_OBJECT
    public:
        explicit AudioRecordEngine(QObject *parent = 0);
        ~AudioRecordEngine();
        AudioRecordEngine & operator=(const AudioRecordEngine &) = delete;
        qint64 getRecordSize() const;
        qint64 duration() const;
        void applyAudioRecorderSettings(const AudioRecordEngineSettings &settings);
    signals:
        void raiseErrorMessage(QString);
    public slots:
        void updateProgress(qint64 duration);
        void startRecording();
        void pauseRecording();
        void stopRecording();
        bool saveRecord(const QString&);
        QString getAudioFileName() const;
        void clear();
        bool isRecording() const;

    private:
        QAudioRecorder *m_audioRecorder;
        QAudioProbe *m_probe;
        QTemporaryFile m_file;
    };
}

#endif // AUDIORECORDENGINE_H
