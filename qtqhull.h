#ifndef QTQHULL_H
#define QTQHULL_H

#include "QtQhull_global.h"
#include <QObject>
#include <QVector3D>
#include <QVector4D>
#include <QPolygonF>

#include <libqhullcpp/Qhull.h>
#include <libqhullcpp/PointCoordinates.h>
#include <libqhullcpp/QhullVertex.h>
#include <libqhullcpp/QhullVertexSet.h>
#include <libqhullcpp/QhullFacetList.h>
#include <libqhullcpp/QhullError.h>
#include <libqhullcpp/QhullFacet.h>
#include <libqhullcpp/QhullIterator.h>
#include <libqhullcpp/QhullFacetSet.h>

namespace orgQhull {

class QTQHULL_EXPORT QtQhull:public QObject,Qhull
{
    Q_OBJECT
public:
    QtQhull();
public:
    void runQhullPolygonF(const QPolygonF& poly, const char* args);
    void runQhull3D(const QVector<QVector3D> &points, const char* args);
    void runQhull4D(const QVector<QVector4D> &points, const char* args);
    void runQhull(const PointCoordinates &points, const char *qhullCommand2);

public:
    //!凸面算法
    static void qconvex_o(const Qhull& qhull);//qconvex_o
    //! Delaunay三角剖分
    static void qdelaunay_o(const Qhull &qhull);//qdelaunay_o
    //! Voronoi图
    static void qvoronoi_o(const Qhull &qhull);//qvoronoi_o

private:
    PointCoordinates* m_externalPoints;
};

};//orgQhull
#endif // QTQHULL_H
