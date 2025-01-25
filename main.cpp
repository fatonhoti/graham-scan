#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

struct Point {
    Point(double x, double y) : x(x), y(y) {}
    double x{0.0}, y{0.0};
};

int main() {

    vector<Point> pts = {
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
    size_t idx = 0;
    for (size_t i = 1; i < pts.size(); i++) {
        if (pts[i].y < pts[idx].y || (pts[i].y == pts[idx].y && pts[i].x < pts[idx].x))
            idx = i;
    }
    Point& ap = pts[idx];
    swap(pts[0], pts[idx]);

    // sort points by polar angle relative to anchor point
    sort(pts.begin() + 1, pts.end(), [&ap](const Point &a, const Point &b) {
        const double pa_a = atan2(a.y - ap.y, a.x - ap.x);
        const double pa_b = atan2(b.y - ap.y, b.x - ap.x);
        if (pa_a == pa_b) {
            const double dist_a = (a.x - ap.x) * (a.x - ap.x) + (a.y - ap.y) * (a.y - ap.y);
            const double dist_b = (b.x - ap.x) * (b.x - ap.x) + (b.y - ap.y) * (b.y - ap.y);
            return dist_a < dist_b;
        }
        return pa_a < pa_b; 
    });

    // generate the hull
    vector<Point> hull;
    for (auto &pt : pts) {
        if (hull.size() < 2) {
            hull.push_back(pt);
            continue;
        }

        while (hull.size() >= 2) {
            const Point& a = hull[hull.size() - 2];
            const Point& b = hull[hull.size() - 1];
            const double cross_product = (b.x - a.x) * (pt.y - a.y) - (b.y - a.y) * (pt.x - a.x);
            // == 0: colinear (optional to have them included or not)
            // > 0: CCW
            // < 0: CW (clockwise turn going A->B->C => B can't be part of hull, A->C)
            if (cross_product > 0) {
                break;
            }
            hull.pop_back();
        }
        hull.push_back(pt);
    }

    for (auto &pt : hull) {
        cout << pt.x << ", " << pt.y << "\n";
    }

    return 0;
}