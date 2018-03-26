#include "camerascontrolwidget.hpp"
#include <iostream>
#include <QCameraInfo>
#include <QVideoWidget>

CamerasControlWidget::CamerasControlWidget(QWidget *parent) :
      QWidget(parent),
      _detector(new ImageDetector(this)),
      _timer(new QTimer(this))
{
    _initCameras();
    _createLayout();
    _initConnections();
    _timer->start(100);
}

void CamerasControlWidget::swapCameras(int index)
{
    _mainCameraLayout->removeWidget(_cameras[_mainCameraIndex]);
    _subCamerasLayout->replaceWidget(_cameras[index], _cameras[_mainCameraIndex]);
    _mainCameraLayout->addWidget(_cameras[index]);

    _cameras[_mainCameraIndex]->setPriority(CameraPriority::Sub);
    _cameras[index]->setPriority(CameraPriority::Main);

    _mainCameraIndex = index;
}

void CamerasControlWidget::refreshCamerasInfo()
{
    QList<QCameraInfo> camerasInfo = QCameraInfo::availableCameras();

    for (int i = 0; i < 2; i++)
    {
       if (_cameras[i]->isEnabled())
       {
           auto camera = dynamic_cast<USBCameraWidget*>(_cameras[i]);
           auto it = std::find_if(camerasInfo.begin(), camerasInfo.end(),
                                  [&camera](QCameraInfo info) { return info.deviceName() == (camera)->getDeviceName(); }
                   );
           if (it == camerasInfo.end()){
               _cameras[i]->setEnabled(false);
               _cameras[i]->repaint();
           }
           else
               camerasInfo.erase(it);
       }
    }

    for (int i = 0; i < 2; i++){
        if (camerasInfo.size() == 0) break;
        if (!_cameras[i]->isEnabled())
        {
            dynamic_cast<USBCameraWidget*>(_cameras[i])->setCamera(camerasInfo.front());
            camerasInfo.pop_front();
        }
    }
}

void CamerasControlWidget::_initCameras()
{
    _initFfmpeg();

    _cameras[0] = new USBCameraWidget(0, this);
    _cameras[1] = new USBCameraWidget(1, this);
    _cameras[2] = new EthernetCameraWidget(2, "udp://127.0.0.1:" + QString::number(CAMERA_PORT_1), this);
    _cameras[3] = new EthernetCameraWidget(3, "udp://127.0.0.1:" + QString::number(CAMERA_PORT_2), this);

    _cameras[0]->setPriority(CameraPriority::Main);
}

void CamerasControlWidget::_createLayout()
{
    QVBoxLayout* camerasLayout = new QVBoxLayout;
    _mainCameraLayout = new QVBoxLayout;
    _subCamerasLayout = new QHBoxLayout;

    _mainCameraLayout->addWidget(_cameras[0]);
    _subCamerasLayout->addWidget(_cameras[1]);
    _subCamerasLayout->addWidget(_cameras[2]);
    _subCamerasLayout->addWidget(_cameras[3]);

    camerasLayout->addLayout(_mainCameraLayout);
    camerasLayout->addLayout(_subCamerasLayout);

    this->setLayout(camerasLayout);
}

void CamerasControlWidget::_initConnections()
{
    connect(_timer, &QTimer::timeout, this, &CamerasControlWidget::refreshCamerasInfo);
}

void CamerasControlWidget::_initFfmpeg()
{
    av_register_all();
    avformat_network_init();
    avcodec_register_all();
}