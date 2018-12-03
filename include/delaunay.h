/*
 *  delaunay.h
 *  aamlib-opencv
 *
 *  Created by Chen Xing on 10-2-12.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "opencv/cv.h"
#include "opencv2/legacy/legacy.hpp"
#include <algorithm>
#include <cstdlib>
#include <map>
#include <set>
#include <vector>
using cv::Point_;
using cv::Mat_;
using std::vector;
using std::set;
using std::map;
using std::sort;

class Delaunay;

struct Triangle {
    Point_<int> v[3];
};

struct TriangleInID {
    int v[3];
};

int pComp(const void *p1, const void *p2);

bool FindTriangleFromEdge(CvSubdiv2DEdge e, set<Triangle> &V);

//! Find the Delaunay division for given points(Return in int coordinates).
template <class T>
vector<Triangle> delaunayDiv(const vector<Point_<T> > &vP, cv::Rect boundRect) {
    CvSubdiv2D *subdiv;

    CvMemStorage *storage;
    storage = cvCreateMemStorage(0);
    subdiv = cvCreateSubdivDelaunay2D(boundRect, storage);
    for (size_t e = 0; e < vP.size(); e++) {
        cvSubdivDelaunay2DInsert(subdiv, vP[e]);
    }

    CvSeqReader reader;
    int i, total = subdiv->edges->total;
    int elem_size = subdiv->edges->elem_size;

    cvStartReadSeq((CvSeq *)(subdiv->edges), &reader, 0);

    set<Triangle> V;

    for (i = 0; i < total; i++) {
        CvQuadEdge2D *edge = (CvQuadEdge2D *)(reader.ptr);

        if (CV_IS_SET_ELEM(edge)) {
            CvSubdiv2DEdge e = (CvSubdiv2DEdge)edge;
            FindTriangleFromEdge(e, V);

            CvSubdiv2DEdge e1 = (CvSubdiv2DEdge)edge + 2;  //=next[2]
            FindTriangleFromEdge(e1, V);
        }
        CV_NEXT_SEQ_ELEM(elem_size, reader);
    }
    cvReleaseMemStorage(&storage);
    vector<Triangle> ans;
    ans.resize(V.size());
    std::copy(V.begin(), V.end(), ans.begin());
    return ans;
}

template <class T>
struct PointLess {
    bool operator()(const Point_<T> &pa, const Point_<T> &pb) const {
        return (pa.x < pb.x) || (pa.x == pb.x && pa.y < pb.y);
    }
};

bool operator<(const TriangleInID &a, const TriangleInID &b);

template <typename T>
bool FindTriangleIDFromEdge(CvSubdiv2DEdge e, set<TriangleInID> &V,
                            map<Point_<T>, int, PointLess<T> > &pMap) {
    CvSubdiv2DEdge t = e;
    TriangleInID triT;
    int iPointNum = 3;
    int j;

    for (j = 0; j < iPointNum; j++) {
        CvSubdiv2DPoint *pt = cvSubdiv2DEdgeOrg(t);
        if (!pt) break;
        if (pMap.find(Point_<T>(pt->pt.x, pt->pt.y)) != pMap.end())
            triT.v[j] = pMap.find(Point_<T>(pt->pt.x, pt->pt.y))->second;
        else
            return false;
        t = cvSubdiv2DGetEdge(t, CV_NEXT_AROUND_LEFT);
    }
    if (j == iPointNum) {
        sort(triT.v, triT.v + 3);
        V.insert(triT);
        return true;
    }

    return false;
}

//! Find the Delaunay division for given points(Return in point id).
template <class T>
vector<TriangleInID> delaunayDivInID(const vector<Point_<T> > &vP,
                                     cv::Rect boundRect) {
    map<Point_<T>, int, PointLess<T> > pMap;

    CvSubdiv2D *subdiv;

    CvMemStorage *storage;
    storage = cvCreateMemStorage(0);
    subdiv = cvCreateSubdivDelaunay2D(boundRect, storage);
    for (size_t e = 0; e < vP.size(); e++) {
        pMap[vP[e]] = e;
        cvSubdivDelaunay2DInsert(subdiv, vP[e]);
    }

    CvSeqReader reader;
    int i, total = subdiv->edges->total;
    int elem_size = subdiv->edges->elem_size;

    cvStartReadSeq((CvSeq *)(subdiv->edges), &reader, 0);

    set<TriangleInID> V;

    for (i = 0; i < total; i++) {
        CvQuadEdge2D *edge = (CvQuadEdge2D *)(reader.ptr);

        if (CV_IS_SET_ELEM(edge)) {
            CvSubdiv2DEdge e = (CvSubdiv2DEdge)edge;
            FindTriangleIDFromEdge(e, V, pMap);

            CvSubdiv2DEdge e1 = (CvSubdiv2DEdge)edge + 2;
            FindTriangleIDFromEdge(e1, V, pMap);
        }
        CV_NEXT_SEQ_ELEM(elem_size, reader);
    }
    cvReleaseMemStorage(&storage);
    vector<TriangleInID> ans;
    ans.resize(V.size());
    std::copy(V.begin(), V.end(), ans.begin());
    return ans;
}

template <typename T>
void labelMatByTriInID(const vector<Point_<T> > &vP,
                       vector<TriangleInID> &triList, Mat_<int> &mapMat,
                       cv::Size labelSize) {
    mapMat.create(labelSize);
    mapMat.setTo(triList.size());

    vector<TriangleInID>::iterator it;
    Point_<T> v[3];
    for (it = triList.begin(); it != triList.end(); it++) {
        // Not interested in points outside the region.
        v[0] = vP[it->v[0]];
        v[1] = vP[it->v[1]];
        v[2] = vP[it->v[2]];

        cv::fillConvexPoly(mapMat, v, 3, it - triList.begin());
    }
}
