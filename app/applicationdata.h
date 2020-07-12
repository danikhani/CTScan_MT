#ifndef APPLICATIONDATA_H
#define APPLICATIONDATA_H
#include <cmath>
#include <QString>
#include <QWidget>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include "Eigen/Core"
#include "mylib.h"


class ApplicationData
{
public:
    ApplicationData();
    ~ApplicationData();
    bool uploadImage(QString path);
    const image3D getImage3D();
    /**
     * @brief point_1
     * @brief point_2
     * @brief getnormalVector()
     * To save the information about the starting point and endpoint of drilling.
     * getnormalVector() returns a Eigen::Vector3D which is the drill trajoketrie.
     */
    Eigen::Vector3d point_1;
    Eigen::Vector3d point_2;
    Eigen::Vector3d getnormalVector();

private:
    image3D imageData3D;
    Eigen::Vector3d normalVector;
};

#endif // APPLICATIONDATA_H
