#include "qtqhull.h"

#include <QDebug>

namespace orgQhull {


QtQhull::QtQhull()
{
}

void QtQhull::runQhullPolygonF(const QPolygonF &poly, const char *args)
{
    m_externalPoints = new PointCoordinates(2, "");  //3 = dimension
    std::vector<double> allPoints;
    for (int var = 0; var < poly.size(); ++var) {
        QPointF p = poly.at(var);
        allPoints.push_back(p.x());
        allPoints.push_back(p.y());
        //allPoints.push_back(p.z());
    }

    m_externalPoints->append(allPoints); //convert to vector<double>
    runQhull(*m_externalPoints, args);
}

void QtQhull::runQhull3D(const QVector<QVector3D> &points, const char *args)
{
    m_externalPoints = new PointCoordinates(3, "");  //3 = dimension
    std::vector<double> allPoints;
    for (int var = 0; var < points.size(); ++var) {
        QVector3D p = points.at(var);
        allPoints.push_back(p.x());
        allPoints.push_back(p.y());
        allPoints.push_back(p.z());
    }

    m_externalPoints->append(allPoints); //convert to vector<double>
    runQhull(*m_externalPoints, args);

}

void QtQhull::runQhull4D(const QVector<QVector4D> &points, const char *args)
{
    m_externalPoints = new PointCoordinates(4, "");  //3 = dimension
    std::vector<double> allPoints;
    for (int var = 0; var < points.size(); ++var) {
        QVector4D p = points.at(var);
        allPoints.push_back(p.x());
        allPoints.push_back(p.y());
        allPoints.push_back(p.z());
        allPoints.push_back(p.w());
    }

    m_externalPoints->append(allPoints); //convert to vector<double>
    runQhull(*m_externalPoints, args);
}

void QtQhull::runQhull(const PointCoordinates &points, const char *qhullCommand2)
{
    Qhull::runQhull(points.comment().c_str(), points.dimension(), points.count(), &*points.coordinates(), qhullCommand2);
}

void QtQhull::qconvex_o(const Qhull &qhull)
{
    int dim = qhull.hullDimension();
    int numfacets = qhull.facetList().count();
    int totneighbors = numfacets * dim;  /* incorrect for non-simplicial facets, see qh_countfacets */

    qDebug()<<"dim:"<<dim<<"size:"<<qhull.points().size()
             <<" "<<numfacets<<" "<<totneighbors / 2;

    std::vector<std::vector<double> > points;

    for (orgQhull::QhullPoints::ConstIterator i = qhull.points().begin(); i != qhull.points().end(); ++i) {
        orgQhull::QhullPoint point = *i;
        points.push_back(point.toStdVector());
    }

    for (size_t j = 0; j < points.size(); ++j) {
        std::vector<double> point = points[j];
        size_t n = point.size();
        for (size_t i = 0; i < n; ++i) {
            if (i < n - 1) {
                qDebug()<< point[i]<<" ";
            }
            else {
                qDebug()<< point[i]<<"\n";
            }
        }
    }

    orgQhull::QhullFacetList facets = qhull.facetList();
    std::vector<std::vector<int> > facetVertices;

    orgQhull::QhullFacetListIterator j(facets);
    while (j.hasNext()) {
        orgQhull::QhullFacet f = j.next();
        std::vector<int> vertices;
        if (!f.isGood()) {
            // ignore facet
        }
        else if (!f.isTopOrient() && f.isSimplicial()) { /* 像选项'o'一样定向顶点 */
            orgQhull::QhullVertexSet vs = f.vertices();
            vertices.push_back(vs[1].point().id());
            vertices.push_back(vs[0].point().id());
            for (int i = 2; i < (int)vs.size(); ++i) {
                vertices.push_back(vs[i].point().id());
            }
            facetVertices.push_back(vertices);
        }
        else {  /* note: for non-simplicial facets, this code does not duplicate option 'o', see qh_facet3vertex and qh_printfacetNvertex_nonsimplicial */
            // for(QhullVertex vertex : f.vertices()){
            orgQhull::QhullVertexSetIterator k(f.vertices());
            while (k.hasNext()) {
                orgQhull::QhullVertex vertex = k.next();
                orgQhull::QhullPoint p = vertex.point();
                vertices.push_back(p.id());
            }
            facetVertices.push_back(vertices);
        }
    }
    // for(std::vector<int> vertices : facetVertices)
    for (size_t k = 0; k < facetVertices.size(); ++k) {
        std::vector<int> vertices = facetVertices[k];
        size_t n = vertices.size();
        std::cout << n << " ";
        for (size_t i = 0; i < n; ++i) {
            std::cout << vertices[i] << " ";
        }
        std::cout << "\n";
    }
}

void QtQhull::qdelaunay_o(const Qhull &qhull)
{
    // The Delaunay diagram is equivalent to the convex hull of a paraboloid, one dimension higher
    int hullDimension= qhull.hullDimension();

    // Input sites as a vector of vectors
    std::vector<std::vector<double> > inputSites;
    QhullPoints points= qhull.points();
    // for(QhullPoint point : points)
    QhullPointsIterator j(points);
    while(j.hasNext()){
        QhullPoint point= j.next();
        inputSites.push_back(point.toStdVector());
    }

    // Printer header and Voronoi vertices
    QhullFacetList facets= qhull.facetList();
    int numFacets= facets.count();
    size_t numRidges= numFacets*hullDimension/2;  // only for simplicial facets
    // cout << hullDimension << "\n" << inputSites.size() << " " << numFacets << " " << numRidges << "\n";
    // // for(std::vector<double> site : inputSites)
    // for(size_t k= 0; k < inputSites.size(); ++k){
    //     std::vector<double> site= inputSites[k];
    //     size_t n= site.size();
    //     for(size_t i= 0; i<n; ++i){
    //         cout << site[i] << " ";
    //     }
    //     cout << "\n";
    // }

    // Delaunay regions as a vector of vectors
    std::vector<std::vector<int> > regions;
    // for(QhullFacet f : facets)
    QhullFacetListIterator k(facets);
    while(k.hasNext()){
        QhullFacet f= k.next();
        std::vector<int> vertices;
        if(!f.isUpperDelaunay()){
            if(!f.isTopOrient() && f.isSimplicial()){ /* orient the vertices like option 'o' */
                QhullVertexSet vs= f.vertices();
                vertices.push_back(vs[1].point().id());
                vertices.push_back(vs[0].point().id());
                for(int i= 2; i<(int)vs.size(); ++i){
                    vertices.push_back(vs[i].point().id());
                }
            }else{  /* note: for non-simplicial facets, this code does not duplicate option 'o', see qh_facet3vertex and qh_printfacetNvertex_nonsimplicial */
                // for(QhullVertex vertex : f.vertices()){
                QhullVertexSetIterator i(f.vertices());
                while(i.hasNext()){
                    QhullVertex vertex= i.next();
                    QhullPoint p= vertex.point();
                    vertices.push_back(p.id());
                }
            }
            regions.push_back(vertices);
        }
    }
    // for(std::vector<int> vertices : regions)
    // for(size_t k2= 0; k2 < regions.size(); ++k2){
    //     std::vector<int> vertices= regions[k2];
    //     size_t n= vertices.size();
    //     cout << n << " ";
    //     for(size_t i= 0; i<n; ++i){
    //         cout << vertices[i] << " ";
    //     }
    //     cout << "\n";
    // }
}

void QtQhull::qvoronoi_o(const Qhull &qhull)
{
    int voronoiDimension= qhull.hullDimension() - 1;
    int numfacets= qhull.facetCount();
    size_t numpoints= qhull.points().size();

    // Gather Voronoi vertices
    std::vector<std::vector<double> > voronoiVertices;
    std::vector<double> vertexAtInfinity;
    for(int i= 0; i<voronoiDimension; ++i){
        vertexAtInfinity.push_back(qh_INFINITE);
    }
    voronoiVertices.push_back(vertexAtInfinity);
    // for(QhullFacet facet : qhull.facetList())
    QhullFacetListIterator j(qhull.facetList());
    while(j.hasNext()){
        QhullFacet facet= j.next();
        if(facet.visitId() && facet.visitId()<numfacets){
            voronoiVertices.push_back(facet.getCenter().toStdVector());
        }
    }

    // Printer header and Voronoi vertices
    // cout << voronoiDimension << "\n" << voronoiVertices.size() << " " << numpoints << " 1\n";
    // // for(std::vector<double> voronoiVertex : voronoiVertices)
    // for(size_t k= 0; k < voronoiVertices.size(); ++k){
    //     std::vector<double> voronoiVertex= voronoiVertices[k];
    //     size_t n= voronoiVertex.size();
    //     for(size_t i= 0; i<n; ++i){
    //         cout << voronoiVertex[i] << " ";
    //     }
    //     cout << "\n";
    // }

    // Gather Voronoi regions
    std::vector<std::vector<int> > voronoiRegions(numpoints); // qh_printvoronoi calls qh_pointvertex via qh_markvoronoi
    // for(QhullVertex vertex : qhull.vertexList())
    QhullVertexListIterator j2(qhull.vertexList());
    while(j2.hasNext()){
        QhullVertex vertex= j2.next();
        size_t numinf= 0;
        std::vector<int> voronoiRegion;
        //for(QhullFacet neighbor : vertex.neighborFacets())
        QhullFacetSetIterator k2(vertex.neighborFacets());
        while(k2.hasNext()){
            QhullFacet neighbor= k2.next();
            if(neighbor.visitId()==0){
                if(!numinf){
                    numinf= 1;
                    voronoiRegion.push_back(0); // the voronoiVertex at infinity indicates an unbounded region
                }
            }else if(neighbor.visitId()<numfacets){
                voronoiRegion.push_back(neighbor.visitId());
            }
        }
        if(voronoiRegion.size() > numinf){
            int siteId= vertex.point().id();
            if(siteId>=0 && siteId<int(numpoints)){ // otherwise indicate qh.other_points
                voronoiRegions[siteId]= voronoiRegion;
            }
        }
    }

    // Print Voronoi regions by siteId
    // for(std::vector<int> voronoiRegion : voronoiRegions)
    // for(size_t k3= 0; k3 < voronoiRegions.size(); ++k3){
    //     std::vector<int> voronoiRegion= voronoiRegions[k3];
    //     size_t n= voronoiRegion.size();
    //     cout << n;
    //     for(size_t i= 0; i<n; ++i){
    //         cout << " " << voronoiRegion[i];
    //     }
    //     cout << "\n";
    // }
}

};//orgQhull
