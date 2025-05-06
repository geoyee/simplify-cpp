/*
 (c) 2017, Vladimir Agafonkin
 Simplify.js, a high-performance JS polyline simplification library
 mourner.github.io/simplify-js
*/

#pragma once

// T can be float, double, long double, etc.

#include <cmath>
#include <vector>

namespace simplify_cpp
{
    template <typename T>
    inline bool equal(T a, T b, T eps)
    {
        return std::abs(a - b) <= eps;
    }

    template <typename T>
    struct Point
    {
        T x, y;

        inline bool operator==(const Point<T> &p) const
        {
            constexpr T eps = 1e-12;
            return equal(x, p.x, eps) && equal(y, p.y, eps);
        }

        inline bool operator!=(const Point<T> &p) const
        {
            return !operator==(p);
        }
    };

    template <typename T>
    using Points = std::vector<Point<T>>;

    // square distance between 2 points
    template <typename T>
    inline T getSqDist(Point<T> p1, Point<T> p2)
    {
        auto dx = p1.x - p2.x,
             dy = p1.y - p2.y;
        return dx * dx + dy * dy;
    }

    // square distance from a point to a segment
    template <typename T>
    inline T getSqSegDist(Point<T> p, Point<T> p1, Point<T> p2)
    {
        auto x = p1.x,
             y = p1.y,
             dx = p2.x - x,
             dy = p2.y - y;
        constexpr T eps = 1e-12;
        if (!equal(dx, 0.0, eps) || !equal(dy, 0.0, eps))
        {
            auto t = ((p.x - x) * dx + (p.y - y) * dy) / (dx * dx + dy * dy);
            if (t > 1)
            {
                x = p2.x;
                y = p2.y;
            }
            else if (t > 0)
            {
                x += dx * t;
                y += dy * t;
            }
        }
        dx = p.x - x;
        dy = p.y - y;
        return dx * dx + dy * dy;
    }
    // rest of the code doesn't care about point format

    // basic distance-based simplification
    template <typename T>
    inline Points<T> simplifyRadialDist(Points<T> points, T sqTolerance)
    {
        auto prevPoint = points[0];
        Points<T> newPoints = {prevPoint};
        Point<T> point;
        size_t len = points.size();
        for (size_t i = 1; i < len; ++i)
        {
            point = points[i];
            if (getSqDist(point, prevPoint) > sqTolerance)
            {
                newPoints.push_back(point);
                prevPoint = point;
            }
        }
        if (prevPoint != point)
        {
            newPoints.push_back(point);
        }
        return newPoints;
    }

    template <typename T>
    inline void simplifyDPStep(const Points<T> &points, size_t first, size_t last, T sqTolerance, Points<T> &simplified)
    {
        auto maxSqDist = sqTolerance;
        size_t index = 0;
        for (size_t i = first + 1; i < last; ++i)
        {
            auto sqDist = getSqSegDist(points[i], points[first], points[last]);
            if (sqDist > maxSqDist)
            {
                index = i;
                maxSqDist = sqDist;
            }
        }
        if (maxSqDist > sqTolerance)
        {
            if (index - first > 1)
                simplifyDPStep(points, first, index, sqTolerance, simplified);
            simplified.push_back(points[index]);
            if (last - index > 1)
                simplifyDPStep(points, index, last, sqTolerance, simplified);
        }
    }

    // simplification using Ramer-Douglas-Peucker algorithm
    template <typename T>
    inline Points<T> simplifyDouglasPeucker(const Points<T> &points, T sqTolerance)
    {
        size_t last = points.size() - 1;
        Points<T> simplified = {points[0]};
        simplifyDPStep(points, 0, last, sqTolerance, simplified);
        simplified.push_back(points[last]);
        return simplified;
    }

    // both algorithms combined for awesome performance
    template <typename T>
    inline Points<T> simplify(Points<T> points, T tolerance, bool highestQuality = true)
    {
        if (points.size() <= 2)
        {
            return points;
        }
        auto sqTolerance = tolerance * tolerance;
        points = highestQuality ? points : simplifyRadialDist(points, sqTolerance);
        points = simplifyDouglasPeucker(points, sqTolerance);
        return points;
    }
} // namespace simplify_cpp