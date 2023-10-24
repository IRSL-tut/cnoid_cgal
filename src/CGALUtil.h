#ifndef __CNOID_CGAL_UTIL_H__
#define __CNOID_CGAL_UTIL_H__

#include "CnoidCGAL.h"

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
//
#include <CGAL/Gmpq.h>
#include <CGAL/Cartesian.h>
//
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/OFF_to_nef_3.h>
//
#include <CGAL/Surface_mesh.h>
#include <CGAL/boost/graph/convert_nef_polyhedron_to_polygon_mesh.h>
// Triangulation
#include <CGAL/mark_domain_in_triangulation.h>
#include <CGAL/Polygon_2.h>
// volume
#include <CGAL/Polygon_mesh_processing/measure.h>

#include <unordered_map>


// double //
//using NTDim = double;
//using CGAL_Kernel = CGAL::Cartesian<NTDim>;
// Gmpq //
#define USE_GMPQ
#ifdef USE_GMPQ
using NTDim = CGAL::Gmpq;
using CGAL_Kernel = CGAL::Cartesian<NTDim>;
#else
using NTDim = CGAL::Exact_predicates_exact_constructions_kernel::FT;
using CGAL_Kernel = CGAL::Exact_predicates_exact_constructions_kernel;
#endif
using CGAL_Nef_polyhedron3 = CGAL::Nef_polyhedron_3<CGAL_Kernel>;
using CGAL_Aff_transformation = CGAL_Nef_polyhedron3::Aff_transformation_3;
using CGAL_Polyhedron = CGAL::Polyhedron_3<CGAL_Kernel>;

//typedef CGAL::Exact_predicates_exact_constructions_kernel   Kernel;
//typedef CGAL::Polyhedron_3<Kernel>                          Polyhedron;
//typedef CGAL::Nef_polyhedron_3<Kernel>                      Nef_polyhedron;

typedef CGAL_Kernel Kernel;
typedef CGAL_Polyhedron Polyhedron;
typedef CGAL_Nef_polyhedron3 Nef_polyhedron;
//typedef CGAL::Surface_mesh<Kernel::Point_3> Surface_mesh_;
typedef CGAL::Surface_mesh<CGAL::Cartesian<double>::Point_3> Surface_mesh_;
//typedef CGAL::Surface_mesh<CGAL::Exact_predicates_exact_constructions_kernel::Point_3> Surface_mesh_;

//// triangulation
typedef CGAL::Triangulation_vertex_base_2<CGAL_Kernel> VertexBase;
typedef CGAL::Constrained_triangulation_face_base_2<CGAL_Kernel> FaceBase;
typedef CGAL::Triangulation_data_structure_2<VertexBase, FaceBase> TDS;
typedef CGAL::Exact_predicates_tag  Itag;
typedef CGAL::Constrained_Delaunay_triangulation_2<CGAL_Kernel, TDS, Itag> CDT;
typedef CDT::Point Point2;
typedef CGAL::Polygon_2<CGAL_Kernel> Polygon_2_;

namespace cnoid {

inline void transformNef(Nef_polyhedron &nef, const Isometry3 &T)
{
    Vector3 pos(T.translation());
    Matrix3 rot(T.linear());
    Nef_polyhedron::Aff_transformation_3 trs(rot(0, 0), rot(0, 1), rot(0, 2), pos.x(),
                                             rot(1, 0), rot(1, 1), rot(1, 2), pos.y(),
                                             rot(2, 0), rot(2, 1), rot(2, 2), pos.z());
    nef.transform(trs);
}

class CGALMesh::CGALObj
{
public:
    CGALObj() : error_count(-1), cgal_obj(nullptr) {}
    CGALObj(const SgMeshDbl &_mesh);
    ~CGALObj() { if (!!cgal_obj) delete cgal_obj; }

    void writeToMesh(SgMeshDbl &_mesh, bool verbose = false);
    int readFromMesh(const SgMeshDbl &_mesh, bool verbose = false);
    void transform(const Isometry3 &T);
    double volume();
    bool isValid() { return !!cgal_obj; }
    int error_count;
    Nef_polyhedron *cgal_obj;
};

}

#endif // __CNOID_CGAL_UTIL_H__