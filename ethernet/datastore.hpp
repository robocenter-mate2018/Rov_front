#ifndef DATASTORE_HPP
#define DATASTORE_HPP

#include <QObject>
#include <QTimer>
#include <QFileDialog>

#include "rov_data_types/rov_data_types.hpp"
#include "tcpconnetor.hpp"
#include "datadebugdialog.hpp"

class DataStore : public QObject
{
    Q_OBJECT
public:
    explicit DataStore(QWidget *parent = nullptr);
    void SetAxisX(int axis);
    void SetAxisY(int axis);
    void SetAxisZ(int axis);
    void SetAxisW(int axis);
    void SetManRotateRigth(int val);
    void SetManRotateLLeft(int val);
    void SetManOpen(int val);
    void SetManClose(int val);
    void SetCamera1RotateRight(int val);
    void SetCamera1RotateLeft(int val);
    void SetCamera2RotateRight(int val);
    void SetCamera2RotateLeft(int val);
    void SetMagnetOn(int val);
    void SetMagnetOff(int val);
    void SetAcousticOn(int val);
    void SetAcousticOff(int val);
    void SetTwisting_motors(int index, int val);

signals:
    void telimetryUpdate(float yaw, float pitch, float roll);
    void tcpConnect();
    void tcpDisconnect();

private:
    void _initTimer();
    void _initConnections();
    void _onTick();
    void _createShortcuts();
    void _getPackage(const std::vector<std::uint8_t> &package);
    void _sendHardwareFirmware(QString fileName);

    TcpConnector _connector;
    QTimer* _timer;
    DataDebugDialog* _debugDialog;
    QFileDialog* _fileDialog;
    rov_types::rov_telimetry _telimetry;
    rov_types::rov_control _control;
    rov_types::rov_hardware_firmware _hardware_firmware;
};

#endif // DATASTORE_HPP
