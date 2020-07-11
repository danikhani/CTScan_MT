#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <QWidget>
#include <cmath>
//to add files from desktop
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include "applicationdata.h"
#include "mylib.h"
#include <QMouseEvent>
#include <QDebug>
#include <QInputDialog>
#include <Eigen/Geometry>

namespace Ui {
class ImageLoader;
}

class ImageLoader : public QWidget
{
    Q_OBJECT

public:
    explicit ImageLoader(QWidget *parent = nullptr);
    ~ImageLoader();
    void setData(ApplicationData *pData);
signals:
    void LOG(QString str);

private:
    Ui::ImageLoader *ui;
    ApplicationData *m_pData;

    int updatedStart;
    int updatedWidth;
    int currentLayer;
    int updatedThreshold;
    Eigen::Vector3d localPoint_1;
    Eigen::Vector3d localPoint_2;
    Reconstruction reco;
    image2D *reco_im2D;



private slots:
    void ReadFile();
    //static int getSlice(const image3D& image, const Reconstruction& param, image2D& im2D);
    //void updateView(int depth, int startValue, int windowWidth, int threshold, image3D tmp_imageData3D, QImage& image);
    void updateAllViews();
    void updateXYView();
    void updateXZView();
    void updateSliceView();
    void reconstructSlice();
    void visulizeSliceXY(QImage &image);
    void visulizeSliceXZ(QImage &image);



    //void updatedTiefenKarte();
    //void update3DReflection();
    void mousePressEvent(QMouseEvent *event);
    void drawLineXY(QImage &image);
    void drawLineXZ(QImage &image);
    void drawVerticalXZLine(QImage &image, Eigen::Vector3d point, int depth);

    // show values of xy sliders
    void updatedXYWindowingStart(int value);
    void updatedXYWindowingWidth(int value);
    void updatedXYCurrentLayer(int value);
    void updatedXYWindowingThreshold(int value);

    // show values of xz sliders
    void updatedXZWindowingStart(int value);
    void updatedXZWindowingWidth(int value);
    void updatedXZCurrentLayer(int value);
    void updatedXZWindowingThreshold(int value);

    //show values of slice slider
    void updatedSliceWindowingStart(int value);
    void updatedSliceWindowingWidth(int value);
    void updatedSliceCurrentLayer(int value);
    void updatedSliceWindowingThreshold(int value);
    void updatedSliceRotGrade(int value);





};

#endif // IMAGELOADER_H
