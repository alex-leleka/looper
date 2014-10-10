#include "recordersettings.h"
#include "ui_recordersettings.h"

#include "audiorecordenginesettings.h"

RecorderSettings::RecorderSettings(AudioRecordEngineLib::AudioRecordEngineSettings * settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecorderSettings),
    m_settings(settings)
{
    ui->setupUi(this);
    connect(ui->buttonBox, SIGNAL(accepted()),
            this, SLOT(saveSettings()));
    // audio input device
    foreach (const QString &device, m_settings->getAudioInputDevices()) {
        ui->audioDeviceBox->addItem(device, QVariant(device));
    }

    //audio codecs
    foreach (const QString &codecName, m_settings->getCodecs()) {
        ui->audioCodecBox->addItem(codecName, QVariant(codecName));
    }

    //containers
    foreach (const QString &containerName, m_settings->getContainers()) {
        ui->containerBox->addItem(containerName, QVariant(containerName));
    }

    //sample rate
    foreach (int sampleRate, m_settings->getSampleRates()) {
        ui->sampleRateBox->addItem(QString::number(sampleRate), QVariant(
                sampleRate));
    }

    //channels
    foreach (int channel, m_settings->getChannelCounts()) {
        ui->channelsBox->addItem(QString::number(channel), QVariant(
                channel));
    }

    //bitrates:
    foreach (int bitrate, m_settings->getBitRates()) {
        ui->bitrateBox->addItem(QString::number(bitrate), QVariant(
                bitrate));
    }
}

RecorderSettings::~RecorderSettings()
{
    delete ui;
}
static QVariant boxValue(const QComboBox *box)
{
    int indx = box->currentIndex();
    if (indx == -1)
        return QVariant();

    return box->itemData(indx);
}

void RecorderSettings::saveSettings()
{
    m_settings->setAudioInputDevice(boxValue(ui->audioDeviceBox).toString());
    m_settings->setCodec(boxValue(ui->audioCodecBox).toString());
    m_settings->setSampleRate(boxValue(ui->sampleRateBox).toInt());
    m_settings->setBitRate(boxValue(ui->bitrateBox).toInt());
    m_settings->setChannelCount(boxValue(ui->channelsBox).toInt());
    m_settings->setContainer(boxValue(ui->containerBox).toString());
}
