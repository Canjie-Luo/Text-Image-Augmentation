#include "imgwarp_mls.h"
#include <cstdio>

using cv::Vec3b;

ImgWarp_MLS::ImgWarp_MLS() { gridSize = 100; }

inline double bilinear_interp(double x, double y, double v11, double v12,
                              double v21, double v22) {
    return (v11 * (1 - y) + v12 * y) * (1 - x) + (v21 * (1 - y) + v22 * y) * x;
}

Mat ImgWarp_MLS::setAllAndGenerate(const Mat &oriImg,
                                   const vector<Point_<int> > &qsrc,
                                   const vector<Point_<int> > &qdst,
                                   const int outW, const int outH,
                                   const double transRatio) {
    setSize(oriImg.cols, oriImg.rows);
    setTargetSize(outW, outH);
    setSrcPoints(qsrc);
    setDstPoints(qdst);
    calcDelta();
    return genNewImg(oriImg, transRatio);
}

Mat ImgWarp_MLS::genNewImg(const Mat &oriImg, double transRatio) {
    int i, j;
    double di, dj;
    double nx, ny;
    int nxi, nyi, nxi1, nyi1;
    double deltaX, deltaY;
    double w, h;
    int ni, nj;

    Mat newImg(tarH, tarW, oriImg.type());
    for (i = 0; i < tarH; i += gridSize)
        for (j = 0; j < tarW; j += gridSize) {
            ni = i + gridSize, nj = j + gridSize;
            w = h = gridSize;
            if (ni >= tarH) ni = tarH - 1, h = ni - i + 1;
            if (nj >= tarW) nj = tarW - 1, w = nj - j + 1;
            for (di = 0; di < h; di++)
                for (dj = 0; dj < w; dj++) {
                    deltaX =
                        bilinear_interp(di / h, dj / w, rDx(i, j), rDx(i, nj),
                                        rDx(ni, j), rDx(ni, nj));
                    deltaY =
                        bilinear_interp(di / h, dj / w, rDy(i, j), rDy(i, nj),
                                        rDy(ni, j), rDy(ni, nj));
                    nx = j + dj + deltaX * transRatio;
                    ny = i + di + deltaY * transRatio;
                    if (nx > srcW - 1) nx = srcW - 1;
                    if (ny > srcH - 1) ny = srcH - 1;
                    if (nx < 0) nx = 0;
                    if (ny < 0) ny = 0;
                    nxi = int(nx);
                    nyi = int(ny);
                    nxi1 = ceil(nx);
                    nyi1 = ceil(ny);

                    if (oriImg.channels() == 1)
                        newImg.at<uchar>(i + di, j + dj) = bilinear_interp(
                            ny - nyi, nx - nxi, oriImg.at<uchar>(nyi, nxi),
                            oriImg.at<uchar>(nyi, nxi1),
                            oriImg.at<uchar>(nyi1, nxi),
                            oriImg.at<uchar>(nyi1, nxi1));
                    else {
                        for (int ll = 0; ll < 3; ll++)
                            newImg.at<Vec3b>(i + di, j + dj)[ll] =
                                bilinear_interp(
                                    ny - nyi, nx - nxi,
                                    oriImg.at<Vec3b>(nyi, nxi)[ll],
                                    oriImg.at<Vec3b>(nyi, nxi1)[ll],
                                    oriImg.at<Vec3b>(nyi1, nxi)[ll],
                                    oriImg.at<Vec3b>(nyi1, nxi1)[ll]);
                    }
                }
        }
    return newImg;
}

// Set source points and prepare transformation matrices
void ImgWarp_MLS::setSrcPoints(const vector<Point_<int> > &qsrc) {
    nPoint = qsrc.size();

    newDotL.clear();
    newDotL.reserve(nPoint);

    for (size_t i = 0; i < qsrc.size(); i++) newDotL.push_back(qsrc[i]);
}

void ImgWarp_MLS::setDstPoints(const vector<Point_<int> > &qdst) {
    nPoint = qdst.size();
    oldDotL.clear();
    oldDotL.reserve(nPoint);

    for (size_t i = 0; i < qdst.size(); i++) oldDotL.push_back(qdst[i]);
}
