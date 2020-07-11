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


int MyLib::getSlice(const image3D& image, const Reconstruction& param, image2D& im2D){
     Eigen::Vector3d directionsVector;
     Eigen::Vector3d xdirNormed =param.xdir.normalized() ;
     Eigen::Vector3d ydirNormed =param.ydir.normalized() ;

    // returns 0 if ok. -1 if input image is incorrect. -2 if output im2D is incorrect.
    if (image.width ==0 ||image.height ==0 ||image.slices ==0){
        return -1;
    }
    try {
        for (float i = 0.0; i < im2D.width; i++) {
            for (float j = 0.0; j < im2D.height; j++){
                directionsVector = param.pos + (i - im2D.width/2)*xdirNormed/param.scale + (j-im2D.height/2)*ydirNormed/param.scale;
                int x = (int) round(directionsVector.x());
                int y = (int) round(directionsVector.y());
                int z = (int) round(directionsVector.z()/image.pixelSpacingZ*image.pixelSpacingXY );
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
        return -2;
    }
    return 0;
}

QString MyLib::getQTextEditString(int numberofErrors, QString errorText){
    QString huOutOfRangeNumber;
    huOutOfRangeNumber.setNum(numberofErrors);
    huOutOfRangeNumber += errorText;
    return huOutOfRangeNumber;
}
//0 if ok.-1 if there is a mathematical problem.
int MyLib::rotateSlice(Eigen::Vector3d normalVector, double rotationGrade, Eigen::Vector3d& rotatedVector){
    try{
        double alpha = 2*M_PI /360 * rotationGrade ;

        Eigen::Vector3d normedNormalVector = normalVector.normalized();

        double x  = normedNormalVector.x();
        double y = normedNormalVector.y();
        double z = normedNormalVector.z();

        double s = sin(alpha);
        double c = cos(alpha);
        double mc = 1 - c;

        //rotation matrix
        //source: https://www.geogebra.org/m/fdmmvvma
        Eigen::Matrix3d rotationMatrix;
        rotationMatrix(0,0) = c + x * x * mc;
        rotationMatrix(0,1) = x * y * mc - z * s;
        rotationMatrix(0,2) =  x * z * mc + y * s;

        rotationMatrix(1,0) = x * y * mc + z * s;
        rotationMatrix(1,1) = c + y * y * mc;
        rotationMatrix(1,2) = y * z * mc - x * s;

        rotationMatrix(2,0) = x * z * mc - y * s;
        rotationMatrix(2,1) = y * z * mc + x * s;
        rotationMatrix(2,2) = c + z * z * mc;

        rotatedVector = rotationMatrix * rotatedVector;
    }
    catch (...) {
        return -1;
    }
    return 0;
}
//0 if ok.-1 if there is a mathematical problem.
int MyLib::calculateBoringLength(Eigen::Vector3d startPoint,Eigen::Vector3d endPoint, double pixelSpacingXY, double pixelSpacingZ, double& boringLength){
    try{
        Eigen::Vector3d boringVector = startPoint - endPoint;
        Eigen::Vector3d scaleVector(pixelSpacingXY,pixelSpacingXY,pixelSpacingZ);

        boringVector = boringVector.cwiseProduct(scaleVector);
        boringLength = boringVector.norm();
    }
    catch(...){
        return -1;
    }

    return 0;
}


