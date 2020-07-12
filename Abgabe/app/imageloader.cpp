#include "imageloader.h"
#include "ui_imageloader.h"

ImageLoader::ImageLoader(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageLoader)
{
    ui->setupUi(this);
    //for connecting the ui elements to the class:
    //the buttons
    connect(ui->pushButton_load, SIGNAL(clicked()), this, SLOT(ReadFile()));
    connect(ui->pushButton_drawPoints, SIGNAL(clicked()), this, SLOT(updatePointCoordinates()));
    connect(ui->pushButton_showSlice, SIGNAL(clicked()), this, SLOT(showTheSlice()));
    //slider for xy window
    connect(ui->slider_xy_start, SIGNAL(valueChanged(int)), this, SLOT(updatedXYWindowingStart(int)));
    connect(ui->slider_xy_width, SIGNAL(valueChanged(int)), this, SLOT(updatedXYWindowingWidth(int)));
    connect(ui->slider_xy_currentLayer, SIGNAL(valueChanged(int)), this, SLOT(updatedXYCurrentLayer(int)));
    connect(ui->slider_xy_threshold, SIGNAL(valueChanged(int)), this, SLOT(updatedXYWindowingThreshold(int)));
    //slider for xz windows
    connect(ui->slider_xz_start, SIGNAL(valueChanged(int)), this, SLOT(updatedXZWindowingStart(int)));
    connect(ui->slider_xz_width, SIGNAL(valueChanged(int)), this, SLOT(updatedXZWindowingWidth(int)));
    connect(ui->slider_xz_currentLayer, SIGNAL(valueChanged(int)), this, SLOT(updatedXZCurrentLayer(int)));
    connect(ui->slider_xz_threshold, SIGNAL(valueChanged(int)), this, SLOT(updatedXZWindowingThreshold(int)));
    //slider for slice window
    connect(ui->slider_slice_start, SIGNAL(valueChanged(int)), this, SLOT(updatedSliceWindowingStart(int)));
    connect(ui->slider_slice_width, SIGNAL(valueChanged(int)), this, SLOT(updatedSliceWindowingWidth(int)));
    connect(ui->slider_slice_currentLayer, SIGNAL(valueChanged(int)), this, SLOT(updatedSliceCurrentLayer(int)));
    connect(ui->slider_slice_threshold, SIGNAL(valueChanged(int)), this, SLOT(updatedSliceWindowingThreshold(int)));
    connect(ui->slider_slice_rotGrade, SIGNAL(valueChanged(int)), this, SLOT(updatedSliceRotGrade(int)));
    connect(ui->slider_slice_scale, SIGNAL(valueChanged(int)), this, SLOT(updatedSliceScale(int)));

    // for avoiding the user clicking on a blank screen.
    imageLoaded = false;
    showSlice = false;
}

ImageLoader::~ImageLoader()
{
    delete ui;
    delete reco_im2D;
    delete m_pData;
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
        imageLoaded = true;
        updateAllViews();
        emit LOG_State("Image Loaded");
        emit LOG_Instructions("Left Click on the XY Plane to choose the X and Y of the first point");
        emit LOG_Instructions("Right Click on the XY Plane to choose the X and Y of the second point");
        emit LOG_Instructions("Left Click on the XZ Plane to choose the Z the first point");
        emit LOG_Instructions("Right Click on the XZ Plane to choose the Z the second point");
        emit LOG_Instructions("Coordinates can be also given by the 'Show the Point' button ");
    }
    else{
        QMessageBox::critical(this, "Cant Load","Try again please");
    }
}
void ImageLoader::updateAllViews(){
    updateXZView();
    updateXYView();
   }
void ImageLoader::updateXYView(){
    if(imageLoaded){
        //loading data from application data
        const image3D tmp_imageData3D = m_pData->getImage3D();
        //ui elements
        int numberHUOutOfRange = 0;
        int numberWindowingOutOfRange = 0;
        QImage image(tmp_imageData3D.width,tmp_imageData3D.height, QImage::Format_RGB32);
        // draw the pixels
        drawXYPixels(tmp_imageData3D,image,numberHUOutOfRange,numberWindowingOutOfRange);

        //draw the xy projection of the normalvector
        drawNormalVectorXY(image);

        //draw the orthogonal line to the normalvector
        if(showSlice){
            visualizeSliceXY(tmp_imageData3D,image);
        }

        ui->label_xy_image->setPixmap(QPixmap::fromImage(image));
        //error number of HU or Windowing out of range
        if(numberHUOutOfRange!=0 ||numberWindowingOutOfRange!= 0){
            emit LOG_State(MyLib::getQTextEditString(numberHUOutOfRange," times was/were HU_Value out of Range while loading XY view"));
            emit LOG_State(MyLib::getQTextEditString(numberWindowingOutOfRange," times was/were Windowing out of Range while loading XY view"));
        }
    }
}
void ImageLoader::updateXZView(){
    if(imageLoaded){
        //loading data from application data
        const image3D tmp_imageData3D = m_pData->getImage3D();
        //ui elements
        QImage image(tmp_imageData3D.width,tmp_imageData3D.slices, QImage::Format_RGB32);
        int numberHUOutOfRange = 0;
        int numberWindowingOutOfRange = 0;
        // draw the pixels
        drawXZPixels(tmp_imageData3D,image,numberHUOutOfRange,numberWindowingOutOfRange);

        //draw the xz projection of the normalvector
        drawNormalVectorXZ(image);

        //draw the vertical lines
        if(m_pData->point_1.x() != 0){
            drawVerticalXZLine(image,m_pData->point_1, tmp_imageData3D.slices);
        }
        if(m_pData->point_2.x() != 0){
            drawVerticalXZLine(image,m_pData->point_2, tmp_imageData3D.slices);
        }

        //draw the orthogonal line to the normalvector
        if(showSlice){
            visualizeSliceXZ(tmp_imageData3D,image);
        }


        ui->label_xz_image->setPixmap(QPixmap::fromImage(image));
        //error number of HU or Windowing out of range
        if(numberHUOutOfRange!=0 || numberWindowingOutOfRange!= 0){
            emit LOG_State(MyLib::getQTextEditString(numberHUOutOfRange," times was/were HU_Value out of Range while loading XZ view"));
            emit LOG_State(MyLib::getQTextEditString(numberWindowingOutOfRange," times was/were Windowing out of Range while loading XZ view"));
        }
    }

}
void ImageLoader::updateSliceView(){
    const image3D tmp_imageData3D = m_pData->getImage3D();
    QImage image(tmp_imageData3D.width,tmp_imageData3D.height, QImage::Format_RGB32);
    int numberHUOutOfRange = 0;
    int numberWindowingOutOfRange = 0;
    int error_stat = 0;

    if(!showSlice){
        emit LOG_State("Click on 'Show the Slice' to view the slice!");
    }
    // draw the pixels
    drawSlicePixels(tmp_imageData3D,image,numberHUOutOfRange,numberWindowingOutOfRange);

    if(ui->checkBox_showBoring->checkState()){
        drawBoringCircle(image);
        double boringLength;
        error_stat = MyLib::calculateBoringLength(m_pData->point_1,m_pData->point_2,m_pData->getImage3D().pixelSpacingXY,m_pData->getImage3D().pixelSpacingZ,boringLength);
        ui->lineEdit_boringlength->setValue(boringLength);
    }
    else{
        ui->lineEdit_boringlength->setValue(0);
    }


    ui->label_slice_image->setPixmap(QPixmap::fromImage(image));
    //error number of HU or Windowing out of range
    if(numberHUOutOfRange!=0 ||numberWindowingOutOfRange!= 0){
        emit LOG_State(MyLib::getQTextEditString(numberHUOutOfRange," times was/were HU_Value out of Range while loading Slice view"));
        emit LOG_State(MyLib::getQTextEditString(numberWindowingOutOfRange," times was/were Windowing out of Range while loading Slice view"));
    }
    if(error_stat!=0){
        emit LOG_State("Exception catched while calculating the boring length");
    }

}
void ImageLoader::drawXYPixels(image3D tmp_imageData3D, QImage &image,int &numberHUOutOfRange, int &numberWindowingOutOfRange){
    int startSlider = ui->slider_xy_start->value();
    int widthSlider = ui->slider_xy_width ->value();
    int currentLayerSlider = ui->slider_xy_currentLayer->value();
    int thresholdSlider = ui->slider_xy_threshold ->value();
    // values for the algorithem
    int imageDataPosition = 0;
    int iGrauwert;
    int hu_value;
    for(int y = 0 ; y < tmp_imageData3D.width; y++){
        for(int x = 0 ; x < tmp_imageData3D.height; x++){
             imageDataPosition = x * tmp_imageData3D.width + y + tmp_imageData3D.width*tmp_imageData3D.height*currentLayerSlider ;
             hu_value = tmp_imageData3D.pImage[imageDataPosition];
             int windowingError = MyLib::windowing(hu_value,startSlider,widthSlider,iGrauwert);
             //0 if ok.-1 if HU_value is out of range. -2 if windowing parameters are out of range
             if(windowingError == -1){
                 numberHUOutOfRange++;
             }
             if(windowingError == -2){
                 numberWindowingOutOfRange++;
             }
             if(hu_value <= thresholdSlider){
                image.setPixel(y, x,qRgb(iGrauwert, iGrauwert, iGrauwert));
             }else{
                 image.setPixel(y, x, qRgb(255,0,0));
             }
        }
    }

}
void ImageLoader::drawXZPixels(image3D tmp_imageData3D, QImage &image,int &numberHUOutOfRange, int &numberWindowingOutOfRange){
    int startSlider = ui->slider_xz_start->value();
    int widthSlider = ui->slider_xz_width ->value();
    int currentLayerSlider = ui->slider_xz_currentLayer->value();
    int thresholdSlider = ui->slider_xz_threshold ->value();
    // values for the algorithem
    int imageDataPosition = 0;
    int iGrauwert;
    int hu_value;
    for(int y = 0 ; y < tmp_imageData3D.width; y++){
        for(int x = 0 ; x < tmp_imageData3D.slices ; x++){
             imageDataPosition = currentLayerSlider * tmp_imageData3D.width + y + tmp_imageData3D.width*tmp_imageData3D.height*x ;
             hu_value = tmp_imageData3D.pImage[imageDataPosition];
             int windowingError = MyLib::windowing(hu_value,startSlider,widthSlider,iGrauwert);
             //0 if ok.-1 if HU_value is out of range. -2 if windowing parameters are out of range
             if(windowingError == -1){
                 numberHUOutOfRange++;
             }
             if(windowingError == -2){
                 numberWindowingOutOfRange++;
             }
             if(hu_value <= thresholdSlider){
                image.setPixel(y, x,qRgb(iGrauwert, iGrauwert, iGrauwert));
             }else{
                 image.setPixel(y, x, qRgb(255,0,0));
             }
        }
    }
}
void ImageLoader::drawSlicePixels(image3D tmp_imageData3D, QImage &image,int &numberHUOutOfRange, int &numberWindowingOutOfRange){
    int startSlider = ui->slider_slice_start->value();
    int widthSlider = ui->slider_slice_width ->value();
    int thresholdSlider = ui->slider_slice_threshold ->value();
    int iGrauwert;
    int hu_value;
    for (int y = 0; y < tmp_imageData3D.width; y++){
        for (int x = 0; x < tmp_imageData3D.height; x++){
            hu_value = reco_im2D->pImage[y + tmp_imageData3D.height*x];
            int windowingError = MyLib::windowing(hu_value,startSlider,widthSlider,iGrauwert);
            image.setPixel(y,x, qRgb(iGrauwert, iGrauwert, iGrauwert));
            //0 if ok.-1 if HU_value is out of range. -2 if windowing parameters are out of range
            if(windowingError == -1){
                numberHUOutOfRange++;
            }
            if(windowingError == -2){
                numberWindowingOutOfRange++;
            }
            if(hu_value <= thresholdSlider){
               image.setPixel(y, x,qRgb(iGrauwert, iGrauwert, iGrauwert));
            }else{
               image.setPixel(y, x, qRgb(255,0,0));
            }
        }
    }

}
void ImageLoader::drawNormalVectorXY(QImage &image)
{
    Eigen::Vector3d tanLine;
    Eigen::Vector3d line;
    tanLine = m_pData->getnormalVector();
    double norm = tanLine.norm();
    tanLine.normalize();
    for (double i = 0; i < norm; i++) {
        line = tanLine*i + m_pData->point_1;
        image.setPixel(line.x(),line.y(),qRgb(0, 255, 0));
    }
}
void ImageLoader::drawNormalVectorXZ(QImage &image)
{
    Eigen::Vector3d tanLine;
    Eigen::Vector3d line;
    tanLine = m_pData->getnormalVector();
    double norm = tanLine.norm();
    tanLine.normalize();
    for (double i = 0; i < norm; i++) {
        line = tanLine*i + m_pData->point_1;
        image.setPixel(line.x(),line.z(),qRgb(0, 255, 0));
    }
}
void ImageLoader::visualizeSliceXZ(image3D tmp_imageData3D,QImage &image){
    Eigen::Vector3d tanLine = m_pData->getnormalVector();
    Eigen::Vector3d line;
    tanLine.normalize();
    Eigen::Vector3d orthogonalVector(-tanLine.z(),0,tanLine.x());


    for (int l=-100; l<=100; l++){
        line = param.pos + orthogonalVector*l;
        if (line.x()>=0 && line.x()<tmp_imageData3D.width && line.z()>=0 && line.z()<tmp_imageData3D.slices){
            image.setPixel(line.x(),line.z(), qRgb(255, 255, 0));
        }
    }
}
void ImageLoader::visualizeSliceXY(image3D tmp_imageData3D,QImage &image){
    Eigen::Vector3d tanLine = m_pData->getnormalVector();
    Eigen::Vector3d line;
    tanLine.normalize();
    Eigen::Vector3d orthogonalVector(-tanLine.y(),tanLine.x(),0);

    for (int l=-100; l<=100; l++){
        line = param.pos + orthogonalVector*l;
        if (line.x()>=0 && line.x()<tmp_imageData3D.width && line.y()>=0 && line.y()<tmp_imageData3D.height){
            image.setPixel(line.x(),line.y(), qRgb(255, 255, 0));
        }
    }
}
void ImageLoader::updatePointCoordinates(){
   m_pData->point_1.x() = ui->lineEdit_P1_X->value();
   m_pData->point_1.y() = ui->lineEdit_P1_Y->value();
   m_pData->point_1.z() = ui->lineEdit_P1_Z->value();

   m_pData->point_2.x() = ui->lineEdit_P2_X->value();
   m_pData->point_2.y() = ui->lineEdit_P2_Y->value();
   m_pData->point_2.z() = ui->lineEdit_P2_Z->value();

   // update the views to see the new lines
   showSlice = false;
   updateXZView();
   updateXYView();
}
void ImageLoader::showTheSlice(){
    showSlice = true;
    updateXYView();
    updateXZView();
    makeSliceData();
    updateSliceView();
}

void ImageLoader::mousePressEvent(QMouseEvent *event)
{
    //int x = event->x();
    //int y = event->y();
    QPoint globalPos;
    globalPos = event->pos();
    QPoint localPosXY;
    QPoint localPosXZ;
    localPosXY = ui->label_xy_image->mapFromParent(globalPos);
    localPosXZ = ui->label_xz_image->mapFromParent(globalPos);
    if (ui->label_xy_image->rect().contains(localPosXY)){
        showSlice = false;
        // check for risks. for choosing a point
        if(event->button() == Qt::LeftButton){
            m_pData->point_1.x() = localPosXY.x();
            m_pData->point_1.y() = localPosXY.y();
            ui->lineEdit_P1_X->setValue(m_pData->point_1.x());
            ui->lineEdit_P1_Y->setValue(m_pData->point_1.y());
            updateAllViews();
        }
        else if(event->button() == Qt::RightButton){
            m_pData->point_2.x() = localPosXY.x();
            m_pData->point_2.y() = localPosXY.y();
            ui->lineEdit_P2_X->setValue(m_pData->point_2.x());
            ui->lineEdit_P2_Y->setValue(m_pData->point_2.y());
            updateAllViews();
        }
    }
    if (ui->label_xz_image->rect().contains(localPosXZ)){
        showSlice = false;
        // check for risks. for choosing a point
        if(event->button() == Qt::LeftButton){
            m_pData->point_1.z() = localPosXZ.y();
            ui->lineEdit_P1_Z->setValue(m_pData->point_1.z());
            updateAllViews();
        }
        else if(event->button() == Qt::RightButton){
            m_pData->point_2.z() = localPosXZ.y();
            ui->lineEdit_P2_Z->setValue(m_pData->point_2.z());
            updateAllViews();
        }
     }
}
void ImageLoader::drawVerticalXZLine(QImage &image, Eigen::Vector3d point, int depth){
    for (int z = 0; z < depth ; z++){
        image.setPixel(point.x(),z, qRgb(255, 0, 0));
     }
}
void ImageLoader::drawBoringCircle(QImage &image){
double boringD = ui->doubleSpinBox_boring_Diam ->value();
double middlePointI = reco_im2D->width/2;
    double middlePointJ = reco_im2D->height/2;
    for (float runParameterI = 0; runParameterI < reco_im2D->width; runParameterI = runParameterI+0.1) {
        // circle equation: (x-x0)^2 + (y-y0)^2 = r^2
        double scaledRadius = boringD/2*param.scale;
        double deltaJ2 = pow(scaledRadius,2) - pow(runParameterI-middlePointI,2);
        if (deltaJ2<0)
            continue;
        double dRunParameterJ1 = pow(deltaJ2,.5) + middlePointJ;
        int runParameterJ1 = (int)dRunParameterJ1;
        double dRunParameterJ2 = -pow(deltaJ2,.5) + middlePointJ;
        int runParameterJ2 = (int)dRunParameterJ2;
        if (runParameterJ1> 0 && runParameterJ2>0 && runParameterJ1<reco_im2D->height && runParameterJ2<reco_im2D->height){
            image.setPixel(runParameterI,runParameterJ1, qRgb(255, 0, 0));
            image.setPixel(runParameterI,runParameterJ2, qRgb(255, 0, 0));
        }
    }
    //sho
};


void ImageLoader::makeSliceData(){
    double slicePercentage = ui->slider_slice_currentLayer->value();
    double rotGrade = ui->slider_slice_rotGrade->value();
    param.scale = ui->slider_slice_scale->value();
    Eigen::Vector3d normalVector = m_pData->getnormalVector();
    Eigen::Vector3d normedNormalVector = normalVector.normalized();
    Eigen::Vector3d notRoundedLine = (slicePercentage/100.0)*normalVector + m_pData->point_1;
    int numberofRotatingErrors = 0;

    param.pos.x() = (int)round(notRoundedLine.x());
    param.pos.y() = (int)round(notRoundedLine.y());
    param.pos.z() = (int)round(notRoundedLine.z());
    ui->lineEdit_Slicer_X->setValue(param.pos.x());
    ui->lineEdit_Slicer_Y->setValue(param.pos.y());
    ui->lineEdit_Slicer_Z->setValue(param.pos.z());

    //First direction vector of the surface estimated using the normalvector.
    //using euler rotation matrix. the first direction vector can get rotated using the normalvector as a rotation axis.
    //Crossproduct of the normalVector and the first directionvector .
    if (normalVector.x()==0 && normalVector.y()==0){
        Eigen::Vector3d tempVector(1,0,0);
        //to rotate the vector:
        numberofRotatingErrors += MyLib::rotateSlice(normalVector,rotGrade,tempVector);
        param.xdir = tempVector;
        //crossproduct of the normalvector and one direction vector
        param.ydir = normedNormalVector.cross(param.xdir);
    }
    else if(normalVector.x()==0 && normalVector.z()==0){
        Eigen::Vector3d tempVector(1,0,0);
        //to rotate the vector:
        numberofRotatingErrors += MyLib::rotateSlice(normalVector,rotGrade,tempVector);
        param.xdir = tempVector;
        //crossproduct of the normalvector and one direction vector
        param.ydir = normedNormalVector.cross(param.xdir);
    }
    else if(normalVector.y()==0 && normalVector.z()==0){
        Eigen::Vector3d tempVector(0,1,0);
        //to rotate the vector:
        numberofRotatingErrors += MyLib::rotateSlice(normalVector,rotGrade,tempVector);
        param.xdir = tempVector;
        //crossproduct of the normalvector and one direction vector
        param.ydir = normedNormalVector.cross(param.xdir);
    }
    else{
        Eigen::Vector3d tempVector(normalVector.y(),-normalVector.x(),0);
        //to rotate the vector:
        numberofRotatingErrors += MyLib::rotateSlice(normalVector,rotGrade,tempVector);
        param.xdir = tempVector;
        //crossproduct of the normalvector and one direction vector
        param.ydir = normedNormalVector.cross(param.xdir);
    }
    param.xdir.normalize();
    param.ydir.normalize();

    //loading data from application data
    const image3D tmp_imageData3D = m_pData->getImage3D();
    reco_im2D =  new image2D(tmp_imageData3D.height,tmp_imageData3D.width);

    // Error handling :return 0 if ok. -1 if input image is incorrect. -2 if output im2D is incorrect.
    int err_stat = MyLib::getSlice(tmp_imageData3D, param, *reco_im2D);

    if (err_stat ==-1){
        emit LOG_State("Input image is incorrect. Cant determine slices");
    }
    else if(err_stat == -2){
        emit LOG_State("output im2D is incorrect. Cant determine slices");
    }

    if(numberofRotatingErrors !=0 ){
         emit LOG_State(MyLib::getQTextEditString(numberofRotatingErrors," times exceptions catched while while rotating the sliceview"));
    }

}
//---------------------------------Sliders update------------------------
//XY window
void ImageLoader::updatedXYWindowingStart(int value)
{
    ui ->label_xy_start->setText("Start:" + QString::number(value));
    updateXYView();
}
void ImageLoader::updatedXYWindowingWidth(int value)
{
    ui ->label_xy_width->setText("Width:" + QString::number(value));
    updateXYView();
}
void ImageLoader::updatedXYCurrentLayer(int value)
{
    ui ->label_xy_currentLayer->setText("Z = " + QString::number(value+1));
    updateXYView();
}
void ImageLoader::updatedXYWindowingThreshold(int value)
{
    ui ->label_xy_threshold->setText("Threshold:" + QString::number(value));
    updateXYView();
}
//update xz slider values
void ImageLoader::updatedXZWindowingStart(int value)
{
    ui ->label_xz_start->setText("Start:" + QString::number(value));
    updateXZView();
}
void ImageLoader::updatedXZWindowingWidth(int value)
{
    ui ->label_xz_width->setText("Width:" + QString::number(value));
    updateXZView();
}
void ImageLoader::updatedXZCurrentLayer(int value)
{
    ui ->label_xz_currentLayer->setText("Y= " + QString::number(value+1));
    updateXZView();
}
void ImageLoader::updatedXZWindowingThreshold(int value)
{
    ui ->label_xz_threshold->setText("Threshold:" + QString::number(value));
    updateXZView();
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
    makeSliceData();
    updateAllViews();
    updateSliceView();
}
void ImageLoader::updatedSliceWindowingThreshold(int value)
{
    ui ->label_slice_threshold->setText("Threshold:" + QString::number(value));
    updateSliceView();
}

void ImageLoader::updatedSliceRotGrade(int value)
{
    ui ->label_slice_rotationAngel->setText(QString::number(value)+ "°" );
    makeSliceData();
    updateSliceView();
}

void ImageLoader::updatedSliceScale(int value)
{
    ui ->label_slice_scale->setText("Scale: " + QString::number(value));
    makeSliceData();
    updateSliceView();
}

