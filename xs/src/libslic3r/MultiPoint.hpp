#ifndef slic3r_MultiPoint_hpp_
#define slic3r_MultiPoint_hpp_

#include "libslic3r.h"
#include <algorithm>
#include <vector>
#include "Line.hpp"
#include "Point.hpp"

namespace Slic3r {

class BoundingBox;

class MultiPoint
{
    public:
    Points points;
    
    operator Points() const;
    MultiPoint() {};
    MultiPoint(const MultiPoint &other) : points(other.points) {}
    MultiPoint(MultiPoint &&other) : points(std::move(other.points)) {}
    explicit MultiPoint(const Points &_points): points(_points) {}
    MultiPoint& operator=(const MultiPoint &other) { points = other.points; return *this; }
    MultiPoint& operator=(MultiPoint &&other) { points = std::move(other.points); return *this; }
    void scale(double factor);
    void translate(double x, double y);
    void translate(const Point &vector);
    void rotate(double angle);
    void rotate(double angle, const Point &center);
    void reverse();
    Point first_point() const;
    virtual Point last_point() const = 0;
    virtual Lines lines() const = 0;
    double length() const;
    bool is_valid() const { return this->points.size() >= 2; }

    int find_point(const Point &point) const;
    bool has_boundary_point(const Point &point) const;
    BoundingBox bounding_box() const;
    // Return true if there are exact duplicates.
    bool has_duplicate_points() const;
    // Remove exact duplicates, return true if any duplicate has been removed.
    bool remove_duplicate_points();
    void append(const Point &point) { this->points.push_back(point); }
    void append(const Points &src) { this->append(src.begin(), src.end()); }
    void append(const Points::const_iterator &begin, const Points::const_iterator &end) { this->points.insert(this->points.end(), begin, end); }
    void append(Points &&src)
    {
        if (this->points.empty()) {
            this->points = std::move(src);
        } else {
            this->points.insert(this->points.end(), src.begin(), src.end());
            src.clear();
        }
    }

    bool intersection(const Line& line, Point* intersection) const;
    std::string dump_perl() const;
    
    static Points _douglas_peucker(const Points &points, const double tolerance);
};

extern BoundingBox get_extents(const MultiPoint &mp);
extern BoundingBox get_extents_rotated(const std::vector<Point> &points, double angle);
extern BoundingBox get_extents_rotated(const MultiPoint &mp, double angle);

} // namespace Slic3r

#endif