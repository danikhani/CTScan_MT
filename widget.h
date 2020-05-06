#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
//to add files from desktop
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
    char imageData[512*512];
    short* m_pImageData;
    int windowing( int HU_value, int startValue, int windowWidth);

private slots:
    void MalePixel();
    void ReadFile();
    void ReadFile_12bit();
    //void SetPicture();
};


#endif // WIDGET_H
