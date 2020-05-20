#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
//to add files from desktop
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include "applicationdata.h"
namespace Ui {
class Widget;
}

class ImageLoader : public QWidget
{
    Q_OBJECT

public:
    explicit ImageLoader(QWidget *parent = nullptr, ApplicationData* pData = nullptr);
    ~ImageLoader();
    void setData(ApplicationData* pData);

private:
    Ui::Widget *ui;
    short* m_pImageData_130;
    int updatedStart;
    int updatedWidth;
    int currentLayer;
    int updatedThreshold;
    short* m_pTiefenkarte;
    ApplicationData *m_pData;

private slots:
    void ReadFile();
    void update3DView();
    int windowing( int HU_value, int startValue, int windowWidth);
    void updatedWindowingStart(int value);
    void updatedWindowingWidth(int value);
    void updatedCurrentLayer(int value);
    void updatedWindowingThreshold(int value);
    void updatedTiefenKarte();
    void update3DReflection();
};


#endif // WIDGET_H
