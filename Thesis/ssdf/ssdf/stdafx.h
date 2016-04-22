#ifndef STDAFX_
#define STDAFX

//OpenCV
#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS

//OpenCV
#include <cv.hpp>
#include <cvaux.hpp>
#include <cxcore.hpp>
#include <opencv2/opencv.hpp>

//STL
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <list>
#include <memory>
#include <string>

//CGAL
#include <CGAL/Cartesian.h>
#include <CGAL/Point_2.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_euclidean_traits_2.h>
#include <CGAL/nearest_neighbor_delaunay_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

namespace CGAL{
    typedef Exact_predicates_exact_constructions_kernel						K;
    typedef K::Point_2														Point;
    typedef std::vector<CGAL::Point>										Points;
    typedef CGAL::Triangulation_euclidean_traits_2<K>						TriangulationTrait;
    typedef CGAL::Triangulation_vertex_base_with_info_2<int, K>				Vb;
    typedef CGAL::Triangulation_data_structure_2<Vb>						Tds;
    typedef CGAL::Delaunay_triangulation_2<TriangulationTrait, Tds>			Dt;    
    typedef std::vector<Dt::Vertex_handle>                                  Vertices;
}

//OpenCV definitions
using cv::Mat;
using cv::Vec2f;
using cv::Vec3d;
using cv::Vec3f;
using cv::Vec2d;
using cv::Vec3b;
using cv::Vec4b;
using cv::Vec4f;
typedef cv::Rect_<float>    Rect;
typedef cv::Rect_<int>      RectI;
typedef cv::Point_<float>   Point;
typedef cv::Point_<int>     PointI;
typedef std::vector<Point>  Points;
typedef std::vector<PointI> PointsI;

//STL definitions
using std::cout;
using std::endl;
using std::string;

//general definitions
typedef std::vector<int> Ints;
typedef std::vector<float> Floats;

//namespace notation
#define BEGIN_NAMESPACE(ns) namespace ns {
#define END_NAMESPACE }
#define BEGIN_NAMESPACE_2(ns1, ns2) namespace ns1 { namespace ns2 {
#define END_NAMESPACE_2 }} 

#endif
