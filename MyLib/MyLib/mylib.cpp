#include "mylib.h"

MyLib::MyLib()
{}

int MyLib::windowing(int HU_value, int startValue, int windowWidth, int& iGrauwert){
   //0 if ok.-1 if HU_value is out of range. -2 if windowing parameters are out of range
   if(HU_value > 3072 || HU_value < -1024){
       return -1;
   }
   else if(windowWidth < 1 || windowWidth > 4096){
       return -2;
   }
   else{
        //fensterung berechnen
        if(HU_value<startValue){
            iGrauwert = 0;
        }
        else if(HU_value> (startValue+windowWidth)){
            iGrauwert = 255;
        }
        else{
            iGrauwert =  255.0/windowWidth * (HU_value-startValue);
        }
        return 0;
    }
}

 //TODO: scalierung muss berücksichtigig werden,
int MyLib::getSlice(const image3D& image, const Reconstruction& param, image2D& im2D){

    int error_stat = 0; // returns 0 if ok. -1 if input image is incorrect. -2 if output im2D is incorrect.
    if (false){ // if input image is incorrect
        error_stat = -1;
        return error_stat;
    }
    try {
        for (float i = 0.0; i < im2D.width; i++) {
            for (float j = 0.0; j < im2D.height; j++){
                // normalize the param.xdir and param.ydir vectors
                double xdirVecLength = param.xdir.norm();
                double ydirVecLength = param.ydir.norm();
                // calculate the coordinates x,y,z of pixel(i,j) of the slice in global coordinates system
                // the slice is described by two orthogonal vectors xdir & ydir
                double dx = param.pos.x() + (i - im2D.width/2)*param.xdir.x()/xdirVecLength + (j - im2D.height/2)*param.ydir.x()/ydirVecLength;
                double dy = param.pos.y() + (i - im2D.width/2)*param.xdir.y()/xdirVecLength + (j - im2D.height/2)*param.ydir.y()/ydirVecLength;
                double dz = param.pos.z() + (i - im2D.width/2)*param.xdir.z()/xdirVecLength + (j - im2D.height/2)*param.ydir.z()/ydirVecLength;
                // round the x,y,z coordinates TODO: CONSIDER SCALE!!
                int x = (int) (dx + .5);
                int y = (int) (dy + .5);
                int z = (int) ((dz/image.pixelSpacingZ*image.pixelSpacingXY )+.5);
                // find the corresponding voxel index
                int iIndex3DImgae = x+ y*image.width  + z*image.width*image.height;
                // check if the voxel exists / the voxel index is inside the image boundaries
                int iHuVal = 0;
                if (x>=0 && x<image.width && y>=0 && y<image.height && z>=0 && z<image.slices){
                    iHuVal = image.pImage[iIndex3DImgae];
                }
                int iIndex2DImage = (int)i+ (int)j*im2D.height;
                im2D.pImage[iIndex2DImage] = iHuVal;
            }
        }

    } catch (...) {
        error_stat = -2;
    }
    return error_stat;
}

QString MyLib::updatePointslabel(int x, int y, int z){
    QString printable = QStringLiteral("(%1,%2,%3)").arg(x).arg(y).arg(z);
    return printable;
}


