#ifndef ITHERNETCAMERAWIDGET_HPP
#define ITHERNETCAMERAWIDGET_HPP

#include "videowidget.hpp"

#include <QObject>
#include <QWidget>

class IthernetCameraWidget : public VideoWidget
{
public:
    IthernetCameraWidget(int index, QWidget *parent = nullptr);

private:
    QPixmap getPixmap() override;
};

#endif // ITHERNETCAMERAWIDGET_HPP
