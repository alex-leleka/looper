#ifndef AUDIORECORDENGINE_H
#define AUDIORECORDENGINE_H

#include <QObject>
namespace AudioRecordEngineLib {

    class AudioRecordEngine : public QObject
    {
        Q_OBJECT
    public:
        explicit AudioRecordEngine(QObject *parent = 0);

    signals:

    public slots:

    };
}
#endif // AUDIORECORDENGINE_H
