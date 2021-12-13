#pragma once

#include "vertex.hpp"
#include <vector>

struct Face
{
    const int n_verts = 3;

    vector<Vertex> vects; /// массив вершин
    Vertex normal;        /// единичная внешняя нормаль

    Face(const vector<Vertex> &a, const Vertex &norm);

    explicit Face(const std::vector<Vertex> &a);

    Face &operator=(const Face &a);

    ~Face() = default;
};

Face::Face(const vector<Vertex> &a, const Vertex &norm) : vects(a)
{
    normal = norm;
    normal.normalize();
}

Face::Face(const std::vector<Vertex> &a)
{
    if (a.size() != n_verts)
    {
        throw std::runtime_error("Wrong number of vertices");
    }

    vects = {a[0], a[1], a[2]};

    normal = cross(vects[1] - vects[0], vects[2] - vects[0]);
    normal.normalize();
    if (normal == Vertex(0, 0, 0))
    {
        std::cerr << "\n" << vects[0] << "|" << vects[1] << "|" << vects[2] << "\n";
        Vertex vec1 = vects[1] - vects[0];
        Vertex vec2 = vects[2] - vects[0];
        vec1.normalize();
        vec2.normalize();
        std::cerr << vec1 << "|" << vec2;
        throw std::runtime_error("Wrong number of difficult Vertex in face");
    }
}

Face &Face::operator=(const Face &a)
{
    vects = a.vects;
    normal = a.normal;
    return *this;
}