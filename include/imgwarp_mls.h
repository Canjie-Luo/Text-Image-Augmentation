#ifndef IMGTRANS_MLS_H
#define IMGTRANS_MLS_H

#include "opencv/cv.h"
#include <vector>
using std::vector;

using cv::Mat;
using cv::Mat_;
using cv::Point_;
using cv::Point;

//! The base class for Moving Least Square image warping.
/*!
 * Choose one of the subclasses, the easiest interface to generate
 * an output is to use setAllAndGenerate function.
 */
class ImgWarp_MLS {
   public:
    ImgWarp_MLS();
    virtual ~ImgWarp_MLS() {}

    //! Set all and generate an output.
    /*!
      \param oriImg the image to be warped.
      \param qsrc A list of "from" points.
      \param qdst A list of "target" points.
      \param outW The width of the output image.
      \param outH The height of the output image.
      \param transRatio 1 means warp to target points, 0 means no warping

      This will do all the initialization and generate a warped image.
      After calling this, one can later call genNewImg with different
      transRatios to generate a warping animation.
    */
    Mat setAllAndGenerate(const Mat &oriImg, const vector<Point_<int> > &qsrc,
                          const vector<Point_<int> > &qdst, const int outW,
                          const int outH, const double transRatio = 1);

    //! Generate the warped image.
    /*! This function generate a warped image using PRE-CALCULATED data.
     *  DO NOT CALL THIS AT FIRST! Call this after at least one call of
     *  setAllAndGenerate.
     */
    Mat genNewImg(const Mat &oriImg, double transRatio);

    //! Calculate delta value which will be used for generating the warped
    //image.
    virtual void calcDelta() = 0;

    //! Parameter for MLS.
    double alpha;

    //! Parameter for MLS.
    int gridSize;

    //! Set the list of target points
    void setDstPoints(const vector<Point_<int> > &qdst);

    //! Set the list of source points
    void setSrcPoints(const vector<Point_<int> > &qsrc);

    //! The size of the original image. For precalculation.
    void setSize(int w, int h) { srcW = w, srcH = h; }

    //! The size of output image
    void setTargetSize(const int outW, const int outH) {
        tarW = outW;
        tarH = outH;
    }

   protected:
    vector<Point_<double> > oldDotL, newDotL;

    int nPoint;

    Mat_<double> /*! \brief delta_x */ rDx, /*! \brief delta_y */ rDy;

    int srcW, srcH;
    int tarW, tarH;
};

#endif  // IMGTRANS_MLS_H
