#ifndef JOYSTICKMEDIATOR_HPP
#define JOYSTICKMEDIATOR_HPP

#include <QObject>
#include <QSettings>
#include <QMap>

#include "joystickmanager.hpp"
#include "joystickconfigdialog.hpp"

class JoystickMediator : public QObject
{
    Q_OBJECT

public:
    explicit JoystickMediator(QSettings* settings, QWidget *parent = nullptr);
    void showConfigDialog();

signals:
    void ManRotateRight(int);
    void ManRotateLeft(int);
    void ManOpen(int);
    void ManClose(int);
    void AxisXChange(int);
    void AxisYChange(int);
    void AxisZChange(int);
    void AxisWChange(int);
    void Camera1RotateRight(int);
    void Camera1RotateLeft(int);
    void Camera2RotateRight(int);
    void Camera2RotateLeft(int);
    void MagnetOn(int);
    void MagnetOff(int);
    void AcousticOn(int);
    void AcousticOff(int);
    void JoystickConnect();
    void JoystickDisconnect();

private slots:
    void _manRotateRight(int val, int step);
    void _manRotateLeft(int val, int step);
    void _manOpen(int val, int step);
    void _manClose(int val, int step);
    void _axisXChange(int val, int step);
    void _axisYChange(int val, int step);
    void _axisZChange(int val, int step);
    void _axisWChange(int val, int step);
    void _camera1RotateRight(int val, int step);
    void _camera1RotateLeft(int val, int step);
    void _camera2RotateRight(int val, int step);
    void _camera2RotateLeft(int val, int step);
    void _magnetOn(int val, int step);
    void _magnetOff(int val, int step);
    void _acousticOn(int val, int step);
    void _acousticOff(int val, int step);

private:
    void _changeConnection(QString signal, QString slot, bool connect = true);
    void _updateSettings();
    void _initConnections();

    QMap<QString, QString> _connects;
    QMap<QString, bool> _inverts;
    QSettings* _settings;
    JoystickManager* _joysticManager;
    JoystickConfigDialog* _configDialog;
};

#endif // JOYSTICKMEDIATOR_HPP
