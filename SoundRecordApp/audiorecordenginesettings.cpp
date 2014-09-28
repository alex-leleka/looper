#include "audiorecordenginesettings.h"
#include "audiorecordengine.h"
#include <QAudioRecorder>
#include <QMediaRecorder>

namespace AudioRecordEngineLib {
    AudioRecordEngineSettings::AudioRecordEngineSettings(const AudioRecordEngine * audioRecorderEng)
    {
        audioRecorder = audioRecorderEng->m_audioRecorder;
    }

    QAudioEncoderSettings AudioRecordEngineSettings::settings() const
    {
        return m_settings;
    }

    void AudioRecordEngineSettings::setSettings(const QAudioEncoderSettings &settings)
    {
        m_settings = settings;
    }

    QString AudioRecordEngineSettings::container() const
    {
        return m_container;
    }

    void AudioRecordEngineSettings::setContainer(const QString &container)
    {
        m_container = container;
    }

    void AudioRecordEngineSettings::setCodec(const QString & codec)
    {
        m_settings.setCodec(codec);
    }

    void AudioRecordEngineSettings::setSampleRate(int sRate)
    {
        m_settings.setSampleRate(sRate);
    }

    void AudioRecordEngineSettings::setBitRate(int bitRate)
    {
        m_settings.setBitRate(bitRate);
    }

    void AudioRecordEngineSettings::setChannelCount(int chCount)
    {
        m_settings.setChannelCount(chCount);
    }

    QVector<QString> AudioRecordEngineSettings::getContainers() const
    {
        QVector<QString> vectContainers;
        foreach (const QString &containerName, audioRecorder->supportedContainers())
        {
            vectContainers.push_back(containerName);
        }
        return vectContainers;
    }
    QVector<QString> AudioRecordEngineSettings::getCodecs() const
    {
        QVector<QString> vectCodecs;
        foreach (const QString &codecName, audioRecorder->supportedAudioCodecs())
        {
            vectCodecs.push_back(codecName);
        }
        return vectCodecs;
    }
    QVector<int> AudioRecordEngineSettings::getSampleRates() const
    {
        QVector<int> vectSampleRates {-1, 8000, 11025, 16000, 32000, 48000, 96000};
        return vectSampleRates;
    }
    QVector<int> AudioRecordEngineSettings::getChannelCounts() const
    {
        QVector<int> vectChannelCounts {-1, 1, 2, 4};
        return vectChannelCounts;
    }
    QVector<int> AudioRecordEngineSettings::getBitRates() const
    {
        QVector<int> vectBitRates {0, 32000, 64000, 96000, 128000};
        return vectBitRates;
    }
    QString AudioRecordEngineSettings::audioInputDevice() const
    {
        return m_audioInputDevice;
    }
    void AudioRecordEngineSettings::setAudioInputDevice(const QString & device)
    {
        m_audioInputDevice = device;
    }

    QVector<QString> AudioRecordEngineSettings::getAudioInputDevices() const
    {
        QVector<QString> vectDevices;
        vectDevices.push_back(QString());
        foreach (const QString &device, audioRecorder->audioInputs())
        {
            vectDevices.push_back(device);
        }
        return vectDevices;
    }
    void AudioRecordEngineSettings::applyAudioRecorderSettings(AudioRecordEngine * audioRecorder) const
    {
        audioRecorder->m_audioRecorder->setAudioInput(m_audioInputDevice);
        audioRecorder->m_audioRecorder->setEncodingSettings(m_settings, QVideoEncoderSettings(), m_container);
    }
}
