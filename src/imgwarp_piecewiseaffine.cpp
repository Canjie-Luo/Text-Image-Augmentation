#include "imgwarp_piecewiseaffine.h"
#include "delaunay.h"

#include "opencv/highgui.h"

using cv::Point2d;

ImgWarp_PieceWiseAffine::ImgWarp_PieceWiseAffine(void) {
    backGroundFillAlg = BGNone;
}

ImgWarp_PieceWiseAffine::~ImgWarp_PieceWiseAffine(void) {}

Point_<double> ImgWarp_PieceWiseAffine::getMLSDelta(int x, int y) {
    static Point_<double> swq, qstar, newP, tmpP;
    double sw;

    static vector<double> w;
    w.resize(nPoint);

    static Point_<double> swp, pstar, curV, curVJ, Pi, PiJ;
    double miu_s;

    int i = x;
    int j = y;
    int k;

    sw = 0;
    swp.x = swp.y = 0;
    swq.x = swq.y = 0;
    newP.x = newP.y = 0;
    curV.x = i;
    curV.y = j;
    for (k = 0; k < nPoint; k++) {
        if ((i == oldDotL[k].x) && j == oldDotL[k].y) break;
        /* w[k] = pow((i-oldDotL[k].x)*(i-oldDotL[k].x)+
                 (j-oldDotL[k].y)*(j-oldDotL[k].y), -alpha);*/
        w[k] = 1 / ((i - oldDotL[k].x) * (i - oldDotL[k].x) +
                    (j - oldDotL[k].y) * (j - oldDotL[k].y));
        sw = sw + w[k];
        swp = swp + w[k] * oldDotL[k];
        swq = swq + w[k] * newDotL[k];
    }
    if (k == nPoint) {
        pstar = (1 / sw) * swp;
        qstar = 1 / sw * swq;

        // Calc miu_s
        miu_s = 0;
        for (k = 0; k < nPoint; k++) {
            if (i == oldDotL[k].x && j == oldDotL[k].y) continue;

            Pi = oldDotL[k] - pstar;
            miu_s += w[k] * Pi.dot(Pi);
        }

        curV -= pstar;
        curVJ.x = -curV.y, curVJ.y = curV.x;

        for (k = 0; k < nPoint; k++) {
            if (i == oldDotL[k].x && j == oldDotL[k].y) continue;

            Pi = oldDotL[k] - pstar;
            PiJ.x = -Pi.y, PiJ.y = Pi.x;

            tmpP.x = Pi.dot(curV) * newDotL[k].x - PiJ.dot(curV) * newDotL[k].y;
            tmpP.y =
                -Pi.dot(curVJ) * newDotL[k].x + PiJ.dot(curVJ) * newDotL[k].y;
            tmpP *= w[k] / miu_s;
            newP += tmpP;
        }
        newP += qstar;
    } else {
        newP = newDotL[k];
    }

    newP.x -= i;
    newP.y -= j;
    return newP;
}

void ImgWarp_PieceWiseAffine::calcDelta() {
    Mat_<int> imgLabel = Mat_<int>::zeros(tarH, tarW);

    rDx = rDx.zeros(tarH, tarW);
    rDy = rDy.zeros(tarH, tarW);
    for (int i = 0; i < this->nPoint; i++) {
        //! Ignore points outside the target image
        if (oldDotL[i].x < 0) oldDotL[i].x = 0;
        if (oldDotL[i].y < 0) oldDotL[i].y = 0;
        if (oldDotL[i].x >= tarW) oldDotL[i].x = tarW - 1;
        if (oldDotL[i].y >= tarH) oldDotL[i].y = tarH - 1;

        rDx(oldDotL[i]) = newDotL[i].x - oldDotL[i].x;
        rDy(oldDotL[i]) = newDotL[i].y - oldDotL[i].y;
    }
    rDx(0, 0) = rDy(0, 0) = 0;
    rDx(tarH - 1, 0) = rDy(0, tarW - 1) = 0;
    rDy(tarH - 1, 0) = rDy(tarH - 1, tarW - 1) = srcH - tarH;
    rDx(0, tarW - 1) = rDx(tarH - 1, tarW - 1) = srcW - tarW;

    vector<Triangle> V;
    vector<Triangle>::iterator it;
    cv::Rect_<int> boundRect(0, 0, tarW, tarH);
    vector<Point_<double> > oL1 = oldDotL;
    if (backGroundFillAlg == BGPieceWise) {
        oL1.push_back(Point2d(0, 0));
        oL1.push_back(Point2d(0, tarH - 1));
        oL1.push_back(Point2d(tarW - 1, 0));
        oL1.push_back(Point2d(tarW - 1, tarH - 1));
    }
    // In order preserv the background
    V = ::delaunayDiv(oL1, boundRect);

    //     vector< TriangleInID > Vt;
    // //     vector< Triangle >::iterator it;
    // //     cv::Rect_<int> boundRect(0, 0, tarW, tarH);
    //     Vt = ::delaunayDivInID(oldDotL, boundRect);
    Mat_<uchar> imgTmp = Mat_<uchar>::zeros(tarH, tarW);
    for (it = V.begin(); it != V.end(); it++) {
        cv::line(imgTmp, it->v[0], it->v[1], 255, 1, CV_AA);
        cv::line(imgTmp, it->v[0], it->v[2], 255, 1, CV_AA);
        cv::line(imgTmp, it->v[2], it->v[1], 255, 1, CV_AA);

        // Not interested in points outside the region.
        if (!(it->v[0].inside(boundRect) && it->v[1].inside(boundRect) &&
              it->v[2].inside(boundRect)))
            continue;

        cv::fillConvexPoly(imgLabel, it->v, 3,
                           cv::Scalar_<int>(it - V.begin() + 1));
    }
    // imshow("imgTmp", imgTmp);
    // cvWaitKey(10);

    int i, j;

    Point_<int> v1, v2, curV;

    for (i = 0;; i += gridSize) {
        if (i >= tarW && i < tarW + gridSize - 1)
            i = tarW - 1;
        else if (i >= tarW)
            break;
        for (j = 0;; j += gridSize) {
            if (j >= tarH && j < tarH + gridSize - 1)
                j = tarH - 1;
            else if (j >= tarH)
                break;
            int tId = imgLabel(j, i) - 1;
            if (tId < 0) {
                if (backGroundFillAlg == BGMLS) {
                    Point_<double> dV = getMLSDelta(i, j);
                    rDx(j, i) = dV.x;
                    rDy(j, i) = dV.y;
                } else {
                    rDx(j, i) = -i;
                    rDy(j, i) = -j;
                }
                continue;
            }
            v1 = V[tId].v[1] - V[tId].v[0];
            v2 = V[tId].v[2] - V[tId].v[0];
            curV.x = i, curV.y = j;
            curV -= V[tId].v[0];

            double d0, d1, d2;
            d2 = double(v1.x * curV.y - curV.x * v1.y) /
                 (v1.x * v2.y - v2.x * v1.y);
            d1 = double(v2.x * curV.y - curV.x * v2.y) /
                 (v2.x * v1.y - v1.x * v2.y);
            d0 = 1 - d1 - d2;
            rDx(j, i) = d0 * rDx(V[tId].v[0]) + d1 * rDx(V[tId].v[1]) +
                        d2 * rDx(V[tId].v[2]);
            rDy(j, i) = d0 * rDy(V[tId].v[0]) + d1 * rDy(V[tId].v[1]) +
                        d2 * rDy(V[tId].v[2]);
        }
    }
}
