#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
//to add files from desktop
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include "applicationdata.h"
#include "mylib.h"
namespace Ui {
class Widget;
}

class ImageLoader : public QWidget
{
    Q_OBJECT

public:
    explicit ImageLoader(QWidget *parent = nullptr);
    void setData(ApplicationData* pData);
    ~ImageLoader();

private:
    Ui::Widget *ui;
    int updatedStart;
    int updatedWidth;
    int currentLayer;
    int updatedThreshold;
    ApplicationData *m_pData;

private slots:
    void ReadFile();
    void update3DView();
    //int windowing( int HU_value, int startValue, int windowWidth);
    void updatedWindowingStart(int value);
    void updatedWindowingWidth(int value);
    void updatedCurrentLayer(int value);
    void updatedWindowingThreshold(int value);
    void updatedTiefenKarte();
    void update3DReflection();
};


#endif // WIDGET_H
