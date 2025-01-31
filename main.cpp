#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

struct Point
{
    Point(double x, double y) : x(x), y(y) {}
    ~Point() = default;
    double x{0.0}, y{0.0};
};

int main()
{

    std::vector<Point> pts = {
        Point(5.15, -3.07),
        Point(-3.2, 1.8),
        Point(-5.7, -2.75),
        Point(-0.52, -1.28),
        Point(1.1, 5),
        Point(-0.42, -5.06),
        Point(-1.27, -0.93),
        Point(5.9, -1.23)
    };

    // find the anchor point
    auto min_el = std::min_element(pts.begin(), pts.end(), [](const Point &a, const Point &b) {
        // minimum point sorted by y-coordinate, break ties by comparing x-coordinate
        return a.y < b.y || (a.y == b.y && a.x < b.x);
    });
    
    Point &ap = *min_el;
    std::swap(pts[0], ap);

    // sort points by polar angle relative to anchor point
    std::sort(pts.begin() + 1, pts.end(), [&ap](const Point &a, const Point &b) {
        const double pa_a = atan2(a.y - ap.y, a.x - ap.x);
        const double pa_b = atan2(b.y - ap.y, b.x - ap.x);
        if (pa_a == pa_b) {
            const double sdist_a = (a.x - ap.x) * (a.x - ap.x) + (a.y - ap.y) * (a.y - ap.y); // A -> Anchor Point
            const double sdist_b = (b.x - ap.x) * (b.x - ap.x) + (b.y - ap.y) * (b.y - ap.y); // B -> Anchor Point
            return sdist_a < sdist_b;
        }
        return pa_a < pa_b;
    });

    // generate the hull
    std::vector<Point> hull;
    for (const auto &pt : pts)
    {
        while (hull.size() >= 2)
        {
            const Point &a = hull[hull.size() - 2];
            const Point &b = hull[hull.size() - 1];
            const double cross_product = (b.x - a.x) * (pt.y - a.y) - (b.y - a.y) * (pt.x - a.x);
            // < 0: CW (clockwise turn going A->B->C => B can't be part of hull
            // == 0: colinear (optional to have them included or not)
            // > 0: CCW (could be part of hull)
            if (cross_product > 0)
                break;
            hull.pop_back();
        }
        hull.push_back(pt);
    }

    for (const auto &pt : hull)
        std::cout << pt.x << ", " << pt.y << "\n";

    return 0;
}