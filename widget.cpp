#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //give the button
    connect(ui->pushButton_8bit, SIGNAL(clicked()), this, SLOT(ReadFile()));
    //give the button 12 bit
    connect(ui->pushButton_12bit, SIGNAL(clicked()), this, SLOT(ReadFile_12bit()));
    //Speicher der Größe 512*512 reservieren
    m_pImageData = new short[512*512];
}

Widget::~Widget()
{
    delete ui;
    delete[] m_pImageData;
}
void Widget::MalePixel()
{
    QImage image(512,512, QImage::Format_RGB32);
    image.setPixel(50,100,qRgb(255,0,0));
    ui->label_image_2->setPixmap(QPixmap::fromImage(image));
    //ui->pushButton_Pixel->setText("Funktioniert");
}

void Widget::ReadFile()
{
    // To open a raw file from a dialog
    QString imagePath = QFileDialog::getOpenFileName(this, "open image", "./", "RAW Image Files (*.raw)");
    QFile dataFile(imagePath);
    // make it read only
    bool bFileOpen = dataFile.open(QIODevice::ReadOnly);
    // give an error iff the file is already in use or not available
    if (!bFileOpen)
    {
        QMessageBox::critical(this, "Bro","Cannot open it");
        return;
    }
    //now to read the data and save in a slot
    //dataFile.read(imageData,512*512);
    // How many data are read
    int iFileSize = dataFile.size();
    int iNumberBytesRead = dataFile.read(imageData,512*512);

    if (iFileSize != iNumberBytesRead)
    {
        QMessageBox::critical(this, "Bro","The Pixel numerbs are not equal");
        return;
    }
    // Close the data
    dataFile.close();

    //read from the array and make the picture
    QImage image(512,512, QImage::Format_RGB32);
    int imageDataPosition = 0;
    for(int y = 0 ; y < 512; y++){
        for(int x = 0 ; x < 512 ; x++){
            int iGrauWert = imageData[imageDataPosition];
            image.setPixel(x,y,qRgb(iGrauWert,iGrauWert,iGrauWert));
            imageDataPosition++;
        }
    }
    ui->label_image_2->setPixmap(QPixmap::fromImage(image));
}
void Widget::ReadFile_12bit(){
    // To open a raw file from a dialog
    QString imagePath = QFileDialog::getOpenFileName(this, "open image", "./", "RAW Image Files (*.raw)");
    QFile dataFile(imagePath);
    // make it read only
    bool bFileOpen = dataFile.open(QIODevice::ReadOnly);
    // give an error iff the file is already in use or not available
    if (!bFileOpen)
    {
        QMessageBox::critical(this, "Bro","Cannot open it");
        return;
    }
    //now to read the data and save in a slot
    int iFileSize = dataFile.size();
    int iNumberBytesRead = dataFile.read((char*)m_pImageData,512*512*sizeof(short));

    if (iFileSize != iNumberBytesRead)
    {
        QMessageBox::critical(this, "Bro","The Pixel numerbs are not equal");
        return;
    }
    // Close the data
    dataFile.close();

    //read from the array and make the picture
    QImage image(512,512, QImage::Format_RGB32);
    int imageDataPosition = 0;
    for(int y = 0 ; y < 512; y++){
        for(int x = 0 ; x < 512 ; x++){
            int iGrauWert = windowing(m_pImageData[imageDataPosition],800,400);
            image.setPixel(x,y,qRgb(iGrauWert,iGrauWert,iGrauWert));
            imageDataPosition++;
        }
    }
    ui->label_image_2->setPixmap(QPixmap::fromImage(image));
}
int Widget::windowing(int Hu_value, int startValue, int windowWidth){
    int iGrauwert;
    //fensterung berechnen
    if(Hu_value<startValue){
        iGrauwert = 0;
    }
    else if(Hu_value> (startValue+windowWidth)){
        iGrauwert = 255;
    }
    else{
        iGrauwert = (Hu_value-startValue)/windowWidth * 255.0;
        //iGrauwert = 100;
    }
    return iGrauwert;
}

