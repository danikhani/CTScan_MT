#ifndef MYLIB_H
#define MYLIB_H

#include "MyLib_global.h"
#include "Eigen/Core"


struct image3D{
    int width;
    int height;
    int slices;
    double pixelSpacingXY;
    double pixelSpacingZ;
    short *pImage;
};
struct image2D{
public:
    image2D(int w,int h)
    {
        pImage=new short[w*h];
        width=w ;height=h;
        /**
         * @brief void *memset(void *adres, int zeichen, size_t n);
         * Mit der Funktion memset() wird der Wert von zeichen in jedes der ersten n Zeichen des Speicherbereichs mit der Adresse adres geschrieben
         */
        memset(pImage,0,w*h*sizeof(short));
    };
    ~image2D(){delete pImage;};
    int width;
    int height;
    short *pImage;
};
struct Reconstruction{
    Eigen::Vector3d pos;
    Eigen::Vector3d xdir;
    Eigen::Vector3d ydir;
    double scale;
};
class MYLIB_EXPORT MyLib
{
public:
    MyLib();
    /**
     * @brief Calculates a windowed piexel
     *
     * Calculates a windowed pixel from the HU_values of the original dataset based on the windowsing parameters.
     * @param HU_value Hu value to e windowed
     * @param startValue start value for windowing [-1024..3072]
     * @param windowWidth Width value for windowing [1..4096]
     * @return 0 if ok. -1 if HU_Value is out of range. -2 if Widnowing paramters are out of range.
     */
    static int windowing(int HU_value, int startValue, int windowWidth, int& iGrauwert);
    /**
     * @brief Calculates an arbitrary slice
     * Calculates and arbitrary slice from within the original dataset image with the reconstruction paramters param.
     * @param image HU original dataset
     * @param param reconstruction parameters
     * @param im2D reconstructed 2d image
     * @return
     */
    static int getSlice(const image3D& image, const Reconstruction& param, image2D& im2D);
private:

};

#endif // MYLIB_H
