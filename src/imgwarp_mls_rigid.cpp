#include "imgwarp_mls_rigid.h"

ImgWarp_MLS_Rigid::ImgWarp_MLS_Rigid() { preScale = false; }

double calcArea(const vector<Point_<double> > &V) {
    Point_<double> lt, rb;
    lt.x = lt.y = 1e10;
    rb.x = rb.y = -1e10;
    for (vector<Point_<double> >::const_iterator i = V.begin(); i != V.end();
         i++) {
        if (i->x < lt.x) lt.x = i->x;
        if (i->x > rb.x) rb.x = i->x;
        if (i->y < lt.y) lt.y = i->y;
        if (i->y > rb.y) rb.y = i->y;
    }
    return (rb.x - lt.x) * (rb.y - lt.y);
}

void ImgWarp_MLS_Rigid::calcDelta() {
    int i, j, k;

    Point_<double> swq, qstar, newP, tmpP;
    double sw;

    double ratio;

    if (preScale) {
        ratio = sqrt(calcArea(newDotL) / calcArea(oldDotL));
        for (i = 0; i < nPoint; i++) newDotL[i] *= 1 / ratio;
    }

    double *w = new double[nPoint];

    rDx.create(tarH, tarW);
    rDy.create(tarH, tarW);

    if (nPoint < 2) {
        rDx.setTo(0);
        rDy.setTo(0);
        return;
    }
    Point_<double> swp, pstar, curV, curVJ, Pi, PiJ, Qi;
    double miu_r;

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
            sw = 0;
            swp.x = swp.y = 0;
            swq.x = swq.y = 0;
            newP.x = newP.y = 0;
            curV.x = i;
            curV.y = j;
            for (k = 0; k < nPoint; k++) {
                if ((i == oldDotL[k].x) && j == oldDotL[k].y) break;
                if (alpha == 1)
                    w[k] = 1 / ((i - oldDotL[k].x) * (i - oldDotL[k].x) +
                                (j - oldDotL[k].y) * (j - oldDotL[k].y));
                else
                    w[k] = pow((i - oldDotL[k].x) * (i - oldDotL[k].x) +
                                   (j - oldDotL[k].y) * (j - oldDotL[k].y),
                               -alpha);
                sw = sw + w[k];
                swp = swp + w[k] * oldDotL[k];
                swq = swq + w[k] * newDotL[k];
            }
            if (k == nPoint) {
                pstar = (1 / sw) * swp;
                qstar = 1 / sw * swq;

                // Calc miu_r
                double s1 = 0, s2 = 0;
                for (k = 0; k < nPoint; k++) {
                    if (i == oldDotL[k].x && j == oldDotL[k].y) continue;

                    Pi = oldDotL[k] - pstar;
                    PiJ.x = -Pi.y, PiJ.y = Pi.x;
                    Qi = newDotL[k] - qstar;
                    s1 += w[k] * Qi.dot(Pi);
                    s2 += w[k] * Qi.dot(PiJ);
                }
                miu_r = sqrt(s1 * s1 + s2 * s2);

                curV -= pstar;
                curVJ.x = -curV.y, curVJ.y = curV.x;

                for (k = 0; k < nPoint; k++) {
                    if (i == oldDotL[k].x && j == oldDotL[k].y) continue;

                    Pi = oldDotL[k] - pstar;
                    PiJ.x = -Pi.y, PiJ.y = Pi.x;

                    tmpP.x = Pi.dot(curV) * newDotL[k].x -
                             PiJ.dot(curV) * newDotL[k].y;
                    tmpP.y = -Pi.dot(curVJ) * newDotL[k].x +
                             PiJ.dot(curVJ) * newDotL[k].y;
                    tmpP *= w[k] / miu_r;
                    newP += tmpP;
                }
                newP += qstar;
            } else {
                newP = newDotL[k];
            }

            if (preScale) {
                rDx(j, i) = newP.x * ratio - i;
                rDy(j, i) = newP.y * ratio - j;
            } else {
                rDx(j, i) = newP.x - i;
                rDy(j, i) = newP.y - j;
            }
        }
    }
    delete[] w;

    if (preScale) {
        for (i = 0; i < nPoint; i++) newDotL[i] *= ratio;
    }
}
