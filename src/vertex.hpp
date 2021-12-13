#pragma once

#include <iostream>
#include <cmath>

using namespace std;

struct Vertex
{
    double x = 0.0, y = 0.0, z = 0.0;

    explicit Vertex(double u1 = 0.0, double u2 = 0.0, double u3 = 0.0) : x(u1), y(u2), z(u3)
    {}

    Vertex operator+(const Vertex &a) const
    {
        return Vertex(a.x + x, a.y + y, a.z + z);
    }

    Vertex operator+=(const Vertex &a) const
    {
        return Vertex(a.x + x, a.y + y, a.z + z);
    }

    Vertex operator-(const Vertex &a) const
    {
        return Vertex(x - a.x, y - a.y, z - a.z);
    }

    [[nodiscard]] double mod() const
    {
        return sqrt(x * x + y * y + z * z);
    }

    bool operator==(const Vertex &a) const
    {
        if (a.x != x || a.y != y || a.z != z)
            return false;
        else
            return true;
    }

    bool operator!=(const Vertex &a) const
    {
        if (a.x != x || a.y != y || a.z != z)
            return true;
        else
            return false;
    }

    void normalize()
    {
        double len = mod();
        if (len > 0.0)
        {
            x /= len;
            y /= len;
            z /= len;
        }
    }

    template<class T>
    Vertex operator*(const T &a) const
    {
        return Vertex(x * a, y * a, z * a);
    }

    template<class T>
    Vertex operator/(const T &a) const
    {
        return Vertex(x / a, y / a, z / a);
    }

    double operator*(const Vertex &a) const
    {
        return a.x * x + a.y * y + a.z * z;
    }

    Vertex operator-() const
    {
        return Vertex(-x, -y, -z);
    }

    ~Vertex() = default;
};

inline Vertex cross(const Vertex &a, const Vertex &b)
{
    return Vertex(a.y * b.z - a.z * b.y, -a.x * b.z + a.z * b.x, a.x * b.y - a.y * b.x);
}

inline std::ostream &operator<<(std::ostream &os, const Vertex &a)
{
    os << a.x << " " << a.y << " " << a.z;
    return os;
}

inline double dist(const Vertex &a, const Vertex &b)
{
    return (b - a).mod();
}

inline bool equal(const Vertex &a, const Vertex &b, double eps = 0.0)
{
    return abs(a.x - b.x) < eps && abs(a.y - b.y) < eps && abs(a.z - b.z) < eps;
}