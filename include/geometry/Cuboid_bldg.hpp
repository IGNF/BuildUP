#ifndef GEOMETRY_CUBOID_BLDG_HPP
#define GEOMETRY_CUBOID_BLDG_HPP

#include "geometry/Rectangle_2.hpp"
#include <math.h>
#include <CGAL/number_utils_classes.h>
namespace geometry {

template<class T>
class Cuboid_bldg
{
public:
    typedef typename T::Point_2               Point_2;
    typedef typename T::Vector_2              Vector_2;
    typedef typename T::FT                    FT;

//private:
    typedef typename T::RT                    RT;
    typedef typename T::Line_2                Line_2;
    typedef typename T::Segment_2             Segment_2;
    typedef typename T::Iso_rectangle_2       Iso_rectangle_2;

    Rectangle_2<T> rect_2;
    FT rho;
    FT theta;
    FT h;

public:
    Cuboid_bldg():rect_2(Rectangle_2<T>()),h(0.){}

   // Cuboid_bldg(const Point_2 &p, const Vector_2 &v, const FT &f, const FT &g): rect_2(Rectangle_2<T>(p,v,f)),h(g){}

   // Cuboid_bldg(const Point_2 &p0, const Point_2 &p1, const Point_2 &p2, const FT &g): rect_2(Rectangle_2<T>(p0,p1,p2)),h(g){}

   // Cuboid_bldg(const Segment_2 &s, const FT &g): rect_2(Rectangle_2<T>(s)),h(g){}

  //  Cuboid_bldg(const Point_2 &pmin, const Point_2 &pmax, const FT &g): rect_2(Rectangle_2<T>(pmin,pmax)),h(g){}

  //  Cuboid_bldg(const Rectangle_2<T> &rec, const FT & g): rect_2(rec),h(g) {}

    Cuboid_bldg(const Point_2 &p,const FT &r, const FT &t, const FT&f, const FT &g)
    :rect_2(Rectangle_2<T>(p,Vector_2(r*std::cos(t),r*std::sin(t)),f)),rho(r),theta(t),h(g) {}

    inline bool operator==(const Cuboid_bldg<T> &i) const {return (rect_2==i.rect_2) && (h==i.h);}

    inline bool operator!=(const Cuboid_bldg<T> &i) const {return ! (*this == i);}

    bool hasBadVertex() const
    {
        for(int i=0;i<5;i++)
        {
            if(rect_2.point(i).x()!= rect_2.point(i).x() || rect_2.point(i).y()!= rect_2.point(i).y())
                return true;
            if(isinf(CGAL::to_double(rect_2.point(i).x())) || isinf(CGAL::to_double(rect_2.point(i).y())))
                return true;
            for(int j=i+1;j<4;j++)
                if(rect_2.point(i)[0] == rect_2.point(j)[0] && rect_2.point(i)[1] == rect_2.point(j)[1])
                    return true;
        }
        return false;
    }


   bool hasBadEdge(double minEdgeLen = 2.0) const
    {
        if(rect_2.is_degenerate())
            return true;

        if(rect_2.normal().squared_length()<=minEdgeLen*minEdgeLen)
            return true;

        if(rect_2.ratio()*rect_2.ratio()*rect_2.normal().squared_length()<=minEdgeLen*minEdgeLen)
            return true;

        return false;
    }

//    FT squared_distance_min(const Cuboid_bldg<T> & o) const
//    {
//        FT dMin,d;
//        dMin = (rect_2.point(0)-o.rect_2.point(0)).squared_length();
//        for(int i=0;i<4;i++)
//        {
//            for(int j=0;j<4;j++)
//            {
//                d = (rect_2.point(i)-o.rect_2.point(j)).squared_length();
//                dMin = d<dMin?d:dMin;
//            }
//        }
//        return dMin;
//    }

    //min_sqdist_vertex2edge
    FT squared_distance(const Cuboid_bldg<T> & o) const
    {
        std::vector<FT> sqd;

        for(int i=0;i<4;i++)
        {
            Segment_2 seg(rect_2.point(i),rect_2.point(i+1));
            for(int j=0;j<4;j++)
                sqd.push_back(seg.squared_distance2point(o.rect_2.point(j)));
        }

        FT result = sqd.front();
        for(typename std::vector<FT>::iterator it = sqd.begin();it!=sqd.end();++it)
            if(*it<result)
                result = *it;
        return result;
    }

    //nearest distance of each footprint vetex to the line
    FT distance2line(const Segment_2& s) const
    {
        return distance2line(s.source(),s.target());
    }

    FT distance2line(const Point_2& p1, const Point_2& p2) const
    {
        //(y-y1)/(y2-y1) = (x-x1)/(x2-x1)
        //(y2-y1)x + (x1-x2)y -(x1-x2)y1-(y2-y1)x1 = 0
        // a = y2-y1; b=x1-x2; c=-by1-ax1; ax+by+c=0
        FT a,b,c;
        a=p2.y()-p1.y();
        b=p1.x()-p2.x();
        c=-b*p1.y()-a*p1.x();

        //distance of a point to line Ax+By+C=0 : |Ax0+By0+C|/|a||b|
        Point_2 p0(rect_2.point(0));
        FT dMin =  std::abs(a*p0.x()+b*p0.y()+c)/sqrt(a*a+b*b);
        for(int i=1;i<4;i++)
        {
            Point_2 p(rect_2.point(i));
            double d=std::abs(a*p.x()+b*p.y()+c)/sqrt(a*a+b*b);
            dMin = d<dMin? d:dMin;
        }
        return dMin;
    }

    //object distance(its center) to line
    FT distance2line_obj(const Segment_2& s ) const
    {
        return distance2line_obj(s.source(),s.target());
    }

    FT distance2line_obj(const Point_2& p1, const Point_2& p2) const
    {
        //(y-y1)/(y2-y1) = (x-x1)/(x2-x1)
        //(y2-y1)x + (x1-x2)y -(x1-x2)y1-(y2-y1)x1 = 0
        // a = y2-y1; b=x1-x2; c=-by1-ax1; ax+by+c=0
        FT a,b,c;
        a=p2.y()-p1.y();
        b=p1.x()-p2.x();
        c=-b*p1.y()-a*p1.x();

        //distance of a point to line Ax+By+C=0 : |Ax0+By0+C|/|a||b|
        return abs(a*rect_2.center().x()+b*rect_2.center().y()+c)/sqrt(a*a+b*b);
    }

    FT angle2vector_radian(const Vector_2 & a) const
    {
        // arcos (a*b /|a||b|)
        Vector_2& b = rect_2.normal();
        return std::acos(a*b/a.length()*b.length());
    }

/* Modifiers */
    inline void center(const Point_2& p ) { rect_2.c=p; }
    inline void normal(const Vector_2& v) { rect_2.n=v; }
    inline void ratio (FT f             ) { rect_2.r=f; }
    inline void height(FT g             ) { h=g;        }


///* Convenience Modifiers */
    inline void translate       (const Vector_2& v ) { rect_2.translate(v); }
//    inline void scale_and_rotate(const Vector_2& v, const FT &g ) { rect_2.scale_and_rotate(v); h=g*h; }
//    inline void scale           (const FT &f, const FT &g ) { rect_2.scale(f); h=g*h; }
//    inline void reverse_orientation () { rect_2.reverse_orientation(); }
//
//    inline Cuboid_bldg rotate(int i) {return Cuboid_bldg(rect_2.rotate(i),h);}
//    inline Cuboid_bldg translated(const Vector_2& v ) const { return Cuboid_bldg(rect_2.translated(v),h);}
//    inline Cuboid_bldg scaled_and_rotated(const Vector_2& v, const FT &g ) const { return Cuboid_bldg(rect_2.scaled_and_rotated(v),g*h);}
//    inline Cuboid_bldg opposite() const { return Cuboid_bldg(rect_2.opposite(),h); }
//    inline Cuboid_bldg reversed_orientation () const { return opposite(); }
//    inline Cuboid_bldg scaled(const FT &f, const FT &g) const { return Cuboid(rect_2.scaled(f),g*h); }
//
//    Cuboid_bldg scaled_center(int i, const FT &f) const {return Cuboid_bldg(rect_2.scaled_center(i,f),h);}
//    Cuboid_bldg scaled_edge(int i, const FT &f) const {return Cuboid_bldg(rect_2.scaled_edge(i,f),h);}
//    Cuboid_bldg rotation_scaled_corner(int i, const Vector_2& v) const {return Cuboid_bldg(rect_2.rotation_scaled_corner(i,v),h);}


    inline bool is_degenerate() const {return rect_2.is_degenerate(); }

    inline const Point_2&  center() const { return rect_2.center(); }
    inline const Vector_2& normal() const { return rect_2.normal(); }
    inline const FT&       ratio () const { return rect_2.ratio(); }

    inline const FT area() const
    {
        FT l1 = 2*sqrt(this->normal().squared_length());
        FT l2 = l1*this->ratio();
        return l1*l2;
    }

    Point_2 point(int i) const
    {
        return rect_2.point(i);
    }
   // point(i) aliases
    inline Point_2 operator[](int i) const { return point(i); }

    Segment_2 segment(int i) const
    {
        return rect_2.segment(i);
    }

     Line_2 line(int i) const
    {
        return rect_2.line(i);
     }

}; // class template Cuboid_bldg

} //namespace geometry


#endif