/*
 *  delaunay.cpp
 *  aamlib-opencv
 *
 *  Created by Chen Xing on 10-2-12.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "delaunay.h"
using cv::Point2i;

bool operator<(const Triangle &t1, const Triangle &t2) {
    for (int i = 0; i < 3; i++) {
        if ((t1.v[i].x < t2.v[i].x) ||
            (t1.v[i].x == t2.v[i].x && t1.v[i].y < t2.v[i].y))
            return true;
        else if (t1.v[i].x > t2.v[i].x)
            return false;
        else if (t1.v[i].x == t2.v[i].x && t1.v[i].y > t2.v[i].y)
            return false;
    }
    return false;
}

bool operator<(const TriangleInID &a, const TriangleInID &b) {
    return (a.v[0] < b.v[0]) || (a.v[0] == b.v[0] && a.v[1] < b.v[1]) ||
           (a.v[0] == b.v[0] && a.v[1] == b.v[1] && a.v[2] < b.v[2]);
}

int pComp(const void *p1, const void *p2) {
    if (((Point_<int> *)p1)->x < ((Point_<int> *)p2)->x ||
        (((Point_<int> *)p1)->x == ((Point_<int> *)p2)->x &&
         ((Point_<int> *)p1)->y < ((Point_<int> *)p2)->y))
        return -1;
    else
        return 1;
}

bool FindTriangleFromEdge(CvSubdiv2DEdge e, set<Triangle> &V) {
    CvSubdiv2DEdge t = e;
    Triangle triT;
    int iPointNum = 3;
    int j;

    for (j = 0; j < iPointNum; j++) {
        CvSubdiv2DPoint *pt = cvSubdiv2DEdgeOrg(t);
        if (!pt) break;
        triT.v[j] = cvPoint(cvRound(pt->pt.x), cvRound(pt->pt.y));
        t = cvSubdiv2DGetEdge(t, CV_NEXT_AROUND_LEFT);
    }
    if (j == iPointNum) {
        qsort(triT.v, 3, sizeof(Point_<int>), pComp);
        V.insert(triT);
        return true;
    }

    return false;
}
