#include "imageloader.h"
#include "ui_imageloader.h"

ImageLoader::ImageLoader(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //give the button
    connect(ui->pushButton_8bit, SIGNAL(clicked()), this, SLOT(ReadFile()));
    //give the button 12 bit
    connect(ui->pushButton_12bit, SIGNAL(clicked()), this, SLOT(ReadFile_12bit()));
    //update slider numbers
    connect(ui->slider_start, SIGNAL(valueChanged(int)), this, SLOT(updatedWindowingStart(int)));
    connect(ui->slider_width, SIGNAL(valueChanged(int)), this, SLOT(updatedWindowingWidth(int)));
    //Speicher der Größe 512*512 reservieren
    m_pImageData = new short[512*512];
    updatedStart = 800;
    updatedWidth = 400;
}

ImageLoader::~ImageLoader()
{
    delete ui;
    delete[] m_pImageData;
}
void ImageLoader::MalePixel()
{
    QImage image(512,512, QImage::Format_RGB32);
    image.setPixel(50,100,qRgb(255,0,0));
    ui->label_image_2->setPixmap(QPixmap::fromImage(image));
    //ui->pushButton_Pixel->setText("Funktioniert");
}

void ImageLoader::ReadFile()
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
void ImageLoader::ReadFile_12bit(){
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
    update2DView(800,400);
}
int ImageLoader::windowing(int Hu_value, int startValue, int windowWidth){
    int iGrauwert;
    //fensterung berechnen
    if(Hu_value<startValue){
        iGrauwert = 0;
    }
    else if(Hu_value> (startValue+windowWidth)){
        iGrauwert = 255;
    }
    else{
        iGrauwert =  255.0/windowWidth * (Hu_value-startValue);
    }
    return iGrauwert;
}

void ImageLoader::updatedWindowingStart(int value)
{
    ui ->label_start->setText("Start:" + QString::number(value));
    updatedStart = value;
    update2DView(updatedStart,updatedWidth);
}
void ImageLoader::updatedWindowingWidth(int value)
{
    ui ->label_width->setText("Width:" + QString::number(value));
    updatedWidth = value;
    update2DView(updatedStart,updatedWidth);
}

void ImageLoader::update2DView(int start, int width)
{
    //read from the array and make the picture
    QImage image(512,512, QImage::Format_RGB32);
    int imageDataPosition = 0;
    for(int y = 0 ; y < 512; y++){
        for(int x = 0 ; x < 512 ; x++){
            int iGrauWert = windowing(m_pImageData[imageDataPosition],start,width);
            image.setPixel(x,y,qRgb(iGrauWert,iGrauWert,iGrauWert));
            imageDataPosition++;
        }
    }
    ui->label_image_2->setPixmap(QPixmap::fromImage(image));
}
