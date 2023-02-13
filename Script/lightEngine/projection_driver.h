#ifndef PROJECTION_DRIVERR_H
#define PROJECTION_DRIVERR_H

#include <QString>


class ProjectionDriver
{
public:
    ProjectionDriver(QString);
    bool OpenProjection();
    bool CloseProjection();
    bool TurnONLED();
private:
    int lightSensorTypeNumber;
    int lightValue;
    int offset;
};


#endif // PROJECTION_DRIVERR_H
