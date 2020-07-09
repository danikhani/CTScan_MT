#include "imageloader.h"
#include "ui_imageloader.h"

ImageLoader::ImageLoader(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageLoader)
{

    ui->setupUi(this);
    //give the button
    connect(ui->pushButton_layered, SIGNAL(clicked()), this, SLOT(ReadFile()));
    connect(ui->pushButton_tiefenKarte, SIGNAL(clicked()), this, SLOT(updatedTiefenKarte()));
    connect(ui->pushButton_reflect, SIGNAL(clicked()), this, SLOT(update3DReflection()));

    //xy picture sliders and labels
    connect(ui->slider_xy_start, SIGNAL(valueChanged(int)), this, SLOT(updatedXYWindowingStart(int)));
    connect(ui->slider_xy_width, SIGNAL(valueChanged(int)), this, SLOT(updatedXYWindowingWidth(int)));
    connect(ui->slider_xy_currentLayer, SIGNAL(valueChanged(int)), this, SLOT(updatedXYCurrentLayer(int)));
    connect(ui->slider_xy_threshold, SIGNAL(valueChanged(int)), this, SLOT(updatedXYWindowingThreshold(int)));
    //slider for xy
    connect(ui->slider_xz_start, SIGNAL(valueChanged(int)), this, SLOT(updatedXZWindowingStart(int)));
    connect(ui->slider_xz_width, SIGNAL(valueChanged(int)), this, SLOT(updatedXZWindowingWidth(int)));
    connect(ui->slider_xz_currentLayer, SIGNAL(valueChanged(int)), this, SLOT(updatedXZCurrentLayer(int)));
    connect(ui->slider_xz_threshold, SIGNAL(valueChanged(int)), this, SLOT(updatedXZWindowingThreshold(int)));
    //slider for slicer
    connect(ui->slider_slice_start, SIGNAL(valueChanged(int)), this, SLOT(updatedXZWindowingStart(int)));
    connect(ui->slider_slice_width, SIGNAL(valueChanged(int)), this, SLOT(updatedXZWindowingWidth(int)));
    connect(ui->slider_slice_currentLayer, SIGNAL(valueChanged(int)), this, SLOT(updatedSliceCurrentLayer(int)));
    connect(ui->slider_slice_threshold, SIGNAL(valueChanged(int)), this, SLOT(updatedXZWindowingThreshold(int)));

    localPoint_1.x() = 0;
    localPoint_1.y() = 0;
    localPoint_1.z() = 0;
    localPoint_2.x() = 0;
    localPoint_2.y() = 0;
    localPoint_2.z() = 0;

    reco_im2D =  new image2D(512,512);
}

ImageLoader::~ImageLoader()
{
    delete ui;
}

void ImageLoader::setData(ApplicationData *pData){
   this->m_pData = pData;
}

void ImageLoader::ReadFile(){
    // To open a raw file from a dialog
    QString imagePath = QFileDialog::getOpenFileName(this, "open image", "./", "RAW Image Files (*.raw)");
    // give files to m-pdata
    bool status = m_pData->uploadImage(imagePath);
    //update 3Dview

    if(status == 1){
        updateAllViews();
        emit LOG("Image Loaded");
    }
    else{
        QMessageBox::critical(this, "Cant Load","Try again please");
    }
}
//updateView(int width, int height, int startValue, int windowWidth, image3D tmp_im3D, QImage image);
void ImageLoader::updateAllViews(){
    updateXZView();
    updateXYView();
   }
void ImageLoader::updateXYView(){
    //loading data from application data
    const image3D tmp_imageData3D = m_pData->getImage3D();
    //ui elements
    QImage image(tmp_imageData3D.width,tmp_imageData3D.height, QImage::Format_RGB32);
    int startSlider = ui->slider_xy_start->value();
    int widthSlider = ui->slider_xy_width ->value();
    int currentLayerSlider = ui->slider_xy_currentLayer->value();
    int thresholdSlider = ui->slider_xy_threshold ->value();
    // values for the algorithem
    int imageDataPosition = 0;
    int iGrauwert;
    int hu_value;
    int number_HU_OutOfRange = 0;
    int number_windowing_OutOfRange = 0;
    for(int y = 0 ; y < tmp_imageData3D.width; y++){
        for(int x = 0 ; x < tmp_imageData3D.height; x++){
             imageDataPosition = x * tmp_imageData3D.width + y + tmp_imageData3D.width*tmp_imageData3D.height*currentLayerSlider ;
             hu_value = tmp_imageData3D.pImage[imageDataPosition];
             int windowingError = MyLib::windowing(hu_value,startSlider,widthSlider,iGrauwert);
             //0 if ok.-1 if HU_value is out of range. -2 if windowing parameters are out of range
             if(windowingError == -1){
                 number_HU_OutOfRange++;
             }
             if(windowingError == -2){
                 number_windowing_OutOfRange++;
             }
             if(hu_value <= thresholdSlider){
                image.setPixel(y, x,qRgb(iGrauwert, iGrauwert, iGrauwert));
             }else{
                 image.setPixel(y, x, qRgb(255,0,0));
             }
        }
    }

    //draw the line
    if(localPoint_1.x() != 0 && localPoint_1.y() != 0 && localPoint_2.x() != 0 && localPoint_2.y() != 0 ){
        drawLineXY(image);
    }
    if(reco.pos.x()!=0){
        visulizeSliceXY(image);
    }
     ui->label_xy_image->setPixmap(QPixmap::fromImage(image));


    QString outOfRangeNumber;
    outOfRangeNumber.setNum(number_HU_OutOfRange);
    QString messagetext = "times was/were HU_Value out of Range";
    outOfRangeNumber += messagetext;
    //emit LOG(outOfRangeNumber);
}
void ImageLoader::updateXZView(){
    //loading data from application data
    const image3D tmp_imageData3D = m_pData->getImage3D();
    //ui elements
    QImage image(tmp_imageData3D.width,tmp_imageData3D.slices, QImage::Format_RGB32);
    int startSlider = ui->slider_xz_start->value();
    int widthSlider = ui->slider_xz_width ->value();
    int currentLayerSlider = ui->slider_xz_currentLayer->value();
    int thresholdSlider = ui->slider_xz_threshold ->value();
    // values for the algorithem
    int imageDataPosition = 0;
    int iGrauwert;
    int hu_value;
    int number_HU_OutOfRange = 0;
    int number_windowing_OutOfRange = 0;
    for(int y = 0 ; y < tmp_imageData3D.width; y++){
        for(int x = 0 ; x < tmp_imageData3D.slices ; x++){
             imageDataPosition = currentLayerSlider * tmp_imageData3D.width + y + tmp_imageData3D.width*tmp_imageData3D.height*x ;
             hu_value = tmp_imageData3D.pImage[imageDataPosition];
             int windowingError = MyLib::windowing(hu_value,startSlider,widthSlider,iGrauwert);
             //0 if ok.-1 if HU_value is out of range. -2 if windowing parameters are out of range
             if(windowingError == -1){
                 number_HU_OutOfRange++;
             }
             if(windowingError == -2){
                 number_windowing_OutOfRange++;
             }
             if(hu_value <= thresholdSlider){
                image.setPixel(y, x,qRgb(iGrauwert, iGrauwert, iGrauwert));
             }else{
                 image.setPixel(y, x, qRgb(255,0,0));
             }
        }
    }

    if(localPoint_1.x() != 0){
        drawVerticalXZLine(image,localPoint_1, tmp_imageData3D.slices);
    }
    if(localPoint_2.x() != 0){
        drawVerticalXZLine(image,localPoint_2, tmp_imageData3D.slices);
    }
    if(localPoint_1.z()!=0 && localPoint_2.z()!=0){
        drawLineXZ(image);
    }
    visulizeSliceXZ(image);
    ui->label_xz_image->setPixmap(QPixmap::fromImage(image));

    QString outOfRangeNumber;
    outOfRangeNumber.setNum(number_HU_OutOfRange);
    QString messagetext = "times was/were HU_Value out of Range";
    outOfRangeNumber += messagetext;
    //emit LOG(outOfRangeNumber);

}
void ImageLoader::updateSliceView(){
    const image3D tmp_imageData3D = m_pData->getImage3D();
    QImage image(tmp_imageData3D.width,tmp_imageData3D.height, QImage::Format_RGB32);

    int startSlider = ui->slider_slice_start->value();
    int widthSlider = ui->slider_slice_width ->value();
    int thresholdSlider = ui->slider_slice_threshold ->value();
    int iGrauwert;
    int hu_value;
    for (int i = 0; i < tmp_imageData3D.width; i++){
        for (int j = 0; j < tmp_imageData3D.height; j++){
            hu_value = reco_im2D->pImage[i + 512*j];
            int windowingError = MyLib::windowing(hu_value,startSlider,widthSlider,iGrauwert);
            image.setPixel(i,j, qRgb(iGrauwert, iGrauwert, iGrauwert));
        }
    }
    ui->label_slice_image->setPixmap(QPixmap::fromImage(image));
}



QString ImageLoader::updatePointlabel(int x, int y, int z){
    auto printable = QStringLiteral("(%1,%2,%3)").arg(x).arg(y).arg(z);
    return printable;
};
void ImageLoader::mousePressEvent(QMouseEvent *event)
{
    const image3D tmp_imageData3D = m_pData->getImage3D();
    int x = event->x();
    int y = event->y();
    QPoint globalPos;
    globalPos = event->pos();
    QPoint localPosXY;
    QPoint localPosXZ;
    localPosXY = ui->label_xy_image->mapFromParent(globalPos);
    localPosXZ = ui->label_xz_image->mapFromParent(globalPos);
    if (ui->label_xy_image->rect().contains(localPosXY)){
        // check for risks. for choosing a point
        if(event->button() == Qt::LeftButton){
            localPoint_1.x() = localPosXY.x();
            localPoint_1.y() = localPosXY.y();
            ui ->label_point1Coordinate->setText(updatePointlabel(localPoint_1.x(),localPoint_1.y(),localPoint_1.z()));
            updateAllViews();
        }
        else if(event->button() == Qt::RightButton){
            localPoint_2.x() = localPosXY.x();
            localPoint_2.y() = localPosXY.y();
            ui ->label_point2Coordinate->setText(updatePointlabel(localPoint_2.x(),localPoint_2.y(),localPoint_2.z()));
            updateAllViews();
        }
    }
    if (ui->label_xz_image->rect().contains(localPosXZ)){
        // check for risks. for choosing a point
        if(event->button() == Qt::LeftButton){
            localPoint_1.z() = localPosXZ.y();
            ui ->label_point1Coordinate->setText(updatePointlabel(localPoint_1.x(),localPoint_1.y(),localPoint_1.z()));
            updateAllViews();
        }
        else if(event->button() == Qt::RightButton){
            localPoint_2.z() = localPosXZ.y();
            ui ->label_point2Coordinate->setText(updatePointlabel(localPoint_2.x(),localPoint_2.y(),localPoint_2.z()));
            updateAllViews();
        }
     }
}
void ImageLoader::drawVerticalXZLine(QImage &image, Eigen::Vector3d point, int depth){
    for (int z = 0; z < depth ; z++){
        image.setPixel(point.x(),z, qRgb(255, 0, 0));
     }
}

void ImageLoader::drawLineXY(QImage &image)
{
    Eigen::Vector3d tanLine;
    Eigen::Vector3d line;
    tanLine = localPoint_2 - localPoint_1;
    double norm = tanLine.norm();
    tanLine.normalize();
    for (double i = 0; i < norm; i++) {
        line = tanLine*i + localPoint_1;
        image.setPixel(line.x(),line.y(),qRgb(255, 255, 0));
    }
}
void ImageLoader::drawLineXZ(QImage &image)
{
    Eigen::Vector3d tanLine;
    Eigen::Vector3d line;
    tanLine = localPoint_2 - localPoint_1;
    double norm = tanLine.norm();
    tanLine.normalize();
    for (double i = 0; i < norm; i++) {
        line = tanLine*i + localPoint_1;
        image.setPixel(line.x(),line.z(),qRgb(255, 255, 0));
    }
}
void ImageLoader::reconstructSlice(){
    double percentage = ui->slider_slice_currentLayer->value();
    Eigen::Vector3d normalVector = localPoint_2 - localPoint_1;
    Eigen::Vector3d notRounded = (percentage/100.0)*normalVector + localPoint_1;

    reco.pos.x() = (int)round(notRounded.x());
    reco.pos.y() = (int)round(notRounded.y());
    reco.pos.z() = (int)round(notRounded.z());

    ui ->label_slicerCoordinate->setText(updatePointlabel(reco.pos.x(),reco.pos.y(),reco.pos.z()));

    double d = normalVector.x()*reco.pos.x() + normalVector.y()*reco.pos.y() + normalVector.z()*reco.pos.z();

    // equation on plane: ax + by + cz = d
    // also we know that the point reco.pos is place on the plan
    //double d = a*reco.pos.x + b*reco.pos.y + c*reco.pos.z;
    // consider xdir=(1, y1, 0) ; ydir=(1, y2, z2)
    // xdir and ydir are orthogonal, e.a (xdir.ydir = 0)
    // and both xdir and ydir are placed on the plan and should satisfy the plane equation
    // so the result is the following:
    reco.xdir.x() = 1;
    reco.xdir.y() = (-normalVector.x())/normalVector.y();
    reco.xdir.z() = 0;

    reco.ydir.x() = 1;
    reco.ydir.y() = normalVector.y()/normalVector.x();
    reco.ydir.z() = (d-normalVector.x() + pow(normalVector.y(),2)/(d-normalVector.x()))/normalVector.z();

    const image3D tmp_imageData3D = m_pData->getImage3D();

    //image2D reco_im2D =  image2D(512,512);
    int err_stat = MyLib::getSlice(tmp_imageData3D, reco, *reco_im2D);
    //drawDrillTrajectory = true;
    //updateView();
}
void ImageLoader::visulizeSliceXZ(QImage &image){
    Eigen::Vector3d normalVector = localPoint_2 - localPoint_1;
    double tanLine = -normalVector.x()/normalVector.z();

    // draw a circle around the pos point in both views
    for (int l=-100; l<=100; l++){
        double dx = reco.pos.x() + (-normalVector.z())*l/(pow(pow(normalVector.x(),2)+pow(normalVector.y(),2),.5));
        double dz = reco.pos.z() + (normalVector.x()) *l/(pow(pow(normalVector.x(),2)+pow(normalVector.z(),2),.5));
        int x = (int)round(dx);
        int z = (int)round(dz);
        if (x>=0 && x<512 && z>=0 && z<256){
            image.setPixel(x,z, qRgb(255, 255, 0));
        }
    }
}
void ImageLoader::visulizeSliceXY(QImage &image){
    Eigen::Vector3d normalVector = localPoint_2 - localPoint_1;
    double tanLine = -normalVector.x()/normalVector.y();


    for (int l=-100; l<=100; l++){
        double dx = reco.pos.x() + (-normalVector.y())*l/(pow(pow(normalVector.x(),2)+pow(normalVector.y(),2),.5));
        double dy = reco.pos.y() + (normalVector.x()) *l/(pow(pow(normalVector.x(),2)+pow(normalVector.y(),2),.5));
        int x = (int)round(dx);
        int y = (int)round(dy);
        if (x>=0 && x<512 && y>=0 && y<512){
            image.setPixel(x,y, qRgb(255, 255, 0));
        }
    }

}


//---------------------------------Slidersupdate------------------------
//XY window
void ImageLoader::updatedXYWindowingStart(int value)
{
    ui ->label_xy_start->setText("Start:" + QString::number(value));
    updateAllViews();
}
void ImageLoader::updatedXYWindowingWidth(int value)
{
    ui ->label_xy_width->setText("Width:" + QString::number(value));
    updateAllViews();
}
void ImageLoader::updatedXYCurrentLayer(int value)
{
    ui ->label_xy_currentLayer->setText("Layer" + QString::number(value));
    updateAllViews();
}
void ImageLoader::updatedXYWindowingThreshold(int value)
{
    ui ->label_xy_threshold->setText("Threshold:" + QString::number(value));
    updateAllViews();
}
//update xz slider values
void ImageLoader::updatedXZWindowingStart(int value)
{
    ui ->label_xz_start->setText("Start:" + QString::number(value));
    updateAllViews();
}
void ImageLoader::updatedXZWindowingWidth(int value)
{
    ui ->label_xz_width->setText("Width:" + QString::number(value));
    updateAllViews();
}
void ImageLoader::updatedXZCurrentLayer(int value)
{
    ui ->label_xz_currentLayer->setText("Layer" + QString::number(value));
    updateAllViews();
}
void ImageLoader::updatedXZWindowingThreshold(int value)
{
    ui ->label_xz_threshold->setText("Threshold:" + QString::number(value));
    updateAllViews();
}

//Slice
void ImageLoader::updatedSliceWindowingStart(int value)
{
    ui ->label_slice_start->setText("Start:" + QString::number(value));
    updateSliceView();
}
void ImageLoader::updatedSliceWindowingWidth(int value)
{
    ui ->label_slice_width->setText("Width:" + QString::number(value));
    updateSliceView();
}
void ImageLoader::updatedSliceCurrentLayer(int value)
{
    ui ->label_slice_currentLayer->setText("% " + QString::number(value));
    reconstructSlice();
    updateAllViews();
}
void ImageLoader::updatedSliceWindowingThreshold(int value)
{
    ui ->label_slice_threshold->setText("Threshold:" + QString::number(value));
    updateSliceView();
}

