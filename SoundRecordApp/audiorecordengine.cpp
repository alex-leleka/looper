#include <QAudioProbe>
#include <QAudioRecorder>
#include <QDir>
#include <QMediaRecorder>

#include "audiorecordengine.h"
#include "audiorecordenginesettings.h"
namespace AudioRecordEngineLib {
    AudioRecordEngine::AudioRecordEngine(QObject *parent) :
        QObject(parent)
    {
        // TODO: add volume sensitivity adjuster
        m_audioRecorder = new QAudioRecorder(this);
        m_probe = new QAudioProbe;
        m_probe->setSource(m_audioRecorder);
        connect(m_audioRecorder, SIGNAL(durationChanged(qint64)), this,
                SLOT(updateProgress(qint64)));
        m_file.open();
        m_audioRecorder->setOutputLocation(QUrl::fromLocalFile(m_file.fileName()));
    }
    AudioRecordEngine::~AudioRecordEngine()
    {
        delete m_audioRecorder;
        delete m_probe;
    }
    void AudioRecordEngine::updateProgress(qint64 duration)
    {
        Q_UNUSED(duration);
        if (m_audioRecorder->error() != QMediaRecorder::NoError)
            raiseErrorMessage(m_audioRecorder->errorString());
    }
    void AudioRecordEngine::startRecording()
    {
        m_audioRecorder->record();
    }

    void AudioRecordEngine::pauseRecording()
    {
        if (m_audioRecorder->state() != QMediaRecorder::PausedState)
            m_audioRecorder->pause();
        else
            m_audioRecorder->record();
    }
    bool AudioRecordEngine::isRecording() const
    {
        return !(m_audioRecorder->state() == QMediaRecorder::StoppedState);
    }

    void AudioRecordEngine::stopRecording()
    {
        m_audioRecorder->stop();
    }

    void AudioRecordEngine::clear()
    {
        if(m_audioRecorder->state() == QMediaRecorder::StoppedState)
        {
            m_file.resize(qint64(0));
        }
    }

    bool AudioRecordEngine::saveRecord(const QString& newName)
    {
        QFileInfo fileInfo(newName);
        return m_file.copy(fileInfo.absoluteFilePath());//newName);
    }
    qint64 AudioRecordEngine::getRecordSize() const
    {
        return m_file.size();
    }

    qint64 AudioRecordEngine::duration() const
    {
        return m_audioRecorder->duration();
    }

    QString AudioRecordEngine::getAudioFileName() const
    {
        return m_file.fileName();
    }

    void AudioRecordEngine::applyAudioRecorderSettings(const AudioRecordEngineSettings & settings)
    {
        m_audioRecorder->setAudioInput(settings.audioInputDevice());
        m_audioRecorder->setEncodingSettings(settings.settings(), QVideoEncoderSettings(), settings.container());
    }
}
