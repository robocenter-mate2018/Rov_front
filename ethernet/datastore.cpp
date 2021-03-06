#include "datastore.hpp"
#include <QShortcut>

DataStore::DataStore(QWidget *parent) :
    QObject(parent),
    _debugDialog(new DataDebugDialog(&_telimetry, &_control, parent)),
    _fileDialog(new QFileDialog(parent))
{
    _initTimer();
    _initConnections();
    _createShortcuts();
}

void DataStore::SetAxisX(int axis)
{
    _control.axis_x = static_cast<std::int8_t>(axis);
}

void DataStore::SetAxisY(int axis)
{
    _control.axis_y = static_cast<std::int8_t>(axis);
}

void DataStore::SetAxisZ(int axis)
{
    _control.axis_z = static_cast<std::int8_t>(axis);
}

void DataStore::SetAxisW(int axis)
{
    _control.axis_w = static_cast<std::int8_t>(axis);
}

void DataStore::SetManRotateRigth(int val)
{
    _control.manipulator_rotate = static_cast<char>(val);
}

void DataStore::SetManRotateLLeft(int val)
{
    _control.manipulator_rotate = static_cast<char>(-val);
}

void DataStore::SetManOpen(int val)
{
    _control.manipulator_open_close = static_cast<char>(val);
}

void DataStore::SetManClose(int val)
{
    _control.manipulator_open_close = static_cast<char>(-val);
}

void DataStore::SetCamera1RotateRight(int val)
{
    _control.camera_rotate[0] = static_cast<char>(val);
}

void DataStore::SetCamera1RotateLeft(int val)
{
    _control.camera_rotate[0] = static_cast<char>(-val);
}

void DataStore::SetCamera2RotateRight(int val)
{
    _control.camera_rotate[1] = static_cast<char>(val);
}

void DataStore::SetCamera2RotateLeft(int val)
{
    _control.camera_rotate[1] = static_cast<char>(-val);
}

void DataStore::SetMagnetOn(int val)
{
    _control.magnet = static_cast<char>(val);
}

void DataStore::SetMagnetOff(int val)
{
    _control.magnet = static_cast<char>(-val);
}

void DataStore::SetAcousticOn(int val)
{
    _control.acoustic = static_cast<char>(val);
}

void DataStore::SetAcousticOff(int val)
{
    _control.acoustic = static_cast<char>(-val);
}

void DataStore::SetTwisting_motors(int index, int val)
{
    _control.twisting_motors[index] = static_cast<char>(val);
}

void DataStore::_initTimer()
{
    _timer = new QTimer(this);
    _timer->start(20);
}

void DataStore::_initConnections()
{
    connect(_timer, &QTimer::timeout, this, &DataStore::_onTick);
    connect(&_connector, &TcpConnector::dataUpdate, this, &DataStore::_getPackage);
    connect(&_connector, &TcpConnector::Connect, [this](){emit tcpConnect();});
    connect(&_connector, &TcpConnector::Disconnect, [this](){emit tcpDisconnect();});
    connect(_fileDialog, &QFileDialog::fileSelected, this, &DataStore::_sendHardwareFirmware);
}

void DataStore::_onTick()
{
    _debugDialog->Update();
    _connector.Send(_control.serialize());
}

void DataStore::_createShortcuts()
{
    connect(new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_1), static_cast<QWidget*>(parent())), &QShortcut::activated, [this](){_debugDialog->show();});
    connect(new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_F), static_cast<QWidget*>(parent())), &QShortcut::activated, [this](){_fileDialog->show();});
}

void DataStore::_getPackage(const std::vector<uint8_t> &package)
{
    switch (package[0]) {
        case rov_types::rov_telimetry::meta().packet_id:
            _telimetry.deserialize(package);
            emit(telimetryUpdate(_telimetry.yaw, _telimetry.pitch, _telimetry.roll));
            break;
    }
}

void DataStore::_sendHardwareFirmware(QString fileName)
{
    std::cout << fileName.toStdString() << std::endl;
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)){
        std::cout << "Bad File" << std::endl;
        return;
    }
    QTextStream in(&file);
    _hardware_firmware.firmware = in.readAll().toStdString();

    _connector.Send(_hardware_firmware.serialize());
    std::cout << "Poslan!" << std::endl;
}
