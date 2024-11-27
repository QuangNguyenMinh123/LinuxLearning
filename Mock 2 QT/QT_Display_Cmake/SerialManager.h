#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H
/**********************************************/
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>

class SerialManager : public QObject
{
    Q_OBJECT
public:
    explicit SerialManager(QObject *parent = nullptr)
        : QObject(parent),
          serialPort(new QSerialPort(this))
    {
        connect(serialPort, &QSerialPort::readyRead, this, &SerialManager::readData);
    }

    ~SerialManager() {
        if (serialPort->isOpen()) {
            serialPort->close();
        }
    }

    // Method to open the serial port
    Q_INVOKABLE void openPort(const QString &portName, int baudRate = QSerialPort::Baud9600) {
        serialPort->setPortName(portName);
        serialPort->setBaudRate(baudRate);
        serialPort->setDataBits(QSerialPort::Data8);
        serialPort->setParity(QSerialPort::NoParity);
        serialPort->setStopBits(QSerialPort::OneStop);
        serialPort->setFlowControl(QSerialPort::NoFlowControl);

        if (serialPort->open(QIODevice::ReadWrite)) {
            emit portOpened();
        } else {
            emit errorOccurred(serialPort->errorString());
        }
    }

    // Method to close the serial port
    Q_INVOKABLE void closePort() {
        if (serialPort->isOpen()) {
            serialPort->close();
            emit portClosed();
        }
    }

    // Method to write data to the serial port
    Q_INVOKABLE void writeData(const QString &data) {
        if (serialPort->isOpen()) {
            serialPort->write(data.toUtf8());
        }
    }

signals:
    void portOpened();
    void portClosed();
    void dataReceived(const QByteArray &data);
    void errorOccurred(const QString &errorString);

private slots:
    // Slot to read incoming data
    void readData() {
        QByteArray data = serialPort->readAll();
        emit dataReceived(data);
    }

private:
    QSerialPort *serialPort;
};

/**********************************************/
#endif // SERIALMANAGER_H
