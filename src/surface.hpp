#pragma once

#include "polyhedron.hpp"

struct Surface
{
    vector<Polyhedron> pols;

    explicit Surface(const Graph &graph)
    {
        pols.reserve(graph.n + graph.m);
        for (int i = 0; i < graph.verts.size(); i++)
        {
            for(int j = 0;j<graph.n;j++)
                if(graph.mx[i][j])
                {
                    pols.emplace_back(Polyhedron(PolyType::Sphere, graph.verts[i]));
                    break;
                }
        }

        for (int i = 0; i < graph.n; i++)
            for (int j = i + 1; j < graph.n; ++j)
                if (graph.mx[i][j])
                {
                    Polyhedron cylinder(PolyType::Cylinder, (graph.verts[i] + graph.verts[j]) / 2);
                    cylinder.rotate(graph.verts[j] - graph.verts[i]);
                    double s = dist(graph.verts[j], graph.verts[i]);
                    Vertex n = (graph.verts[j] - graph.verts[i]);
                    n.normalize();
                    cylinder.move(graph.verts[i] - cylinder.center);
                    double step = 0.1;
                    int t = ceil(s / 0.2);
                    for (int g = 0; g < t - 1; g++)
                    {
                        Polyhedron copy_cylinder(cylinder);
                        copy_cylinder.move(graph.verts[i] + n * step - copy_cylinder.center);
                        pols.emplace_back(std::move(copy_cylinder));
                        step += 0.2;
                    }
                    Polyhedron copy_cylinder(cylinder);
                    copy_cylinder.move(graph.verts[j] - n * 0.1 - copy_cylinder.center);
                    pols.emplace_back(std::move(copy_cylinder));
                }
    }

    /// конструктор для пути
    explicit Surface(const vector<Vertex> &verts)
    {
        pols.reserve(verts.size());
        for (int i = 0; i < verts.size() - 1; i++)
        {
            pols.emplace_back(Polyhedron(PolyType::Sphere, verts[i]));

            Polyhedron cylinder(PolyType::Cylinder, (verts[i] + verts[i + 1]) / 2);
            cylinder.rotate(verts[i + 1] - verts[i]);
            double s = dist(verts[i + 1], verts[i]);
            Vertex n = (verts[i + 1] - verts[i]);
            n.normalize();
            cylinder.move(verts[i] - cylinder.center);
            double step = 0.1;
            int t = ceil(s / 0.2);
            for (int g = 0; g < t - 1; g++)
            {
                Polyhedron copy_cylinder(cylinder);
                copy_cylinder.move(verts[i] + n * step - copy_cylinder.center);
                pols.emplace_back(std::move(copy_cylinder));
                step += 0.2;
            }
            Polyhedron copy_cylinder(cylinder);
            copy_cylinder.move(verts[i + 1] - n * 0.1 - copy_cylinder.center);
            pols.emplace_back(std::move(copy_cylinder));
        }

        pols.emplace_back(Polyhedron(PolyType::Sphere, verts.back()));
    }

    explicit Surface(const vector<Polyhedron> &pols) : pols(pols)
    {}

    /// запись в stl
    void write(const string &filename)
    {
        ofstream file("../figure/" + filename, std::ios_base::trunc);
        file << "solid " << filename.substr(0, filename.size() - 4) << "\n";
        for (auto &pol: pols)
        {
            for (auto &face: pol.m_faces)
            {
                file << "facet normal " << face.normal << "\n\t";
                file << "outer loop\n";
                for (auto &v: face.vects)
                {
                    file << "\t\t\t";
                    file << "vertex " << v << "\n";
                }
                file << "\tendloop\n endfacet\n";
            }
        }
        file << "endsolid " << filename.substr(0, filename.size() - 4) << "\n";
        file.close();
    }

    /// для отображения пути
    void write_csv(const string &filename)
    {
        ofstream file("../figure/" + filename, std::ios_base::trunc);
        double scalar = -static_cast<double>(pols.size()) / 2;
        for (auto &pol: pols)
        {
            if (pol.type == PolyType::Sphere)
            {
                for (auto &face: pol.m_faces)
                {
                    for (auto &v: face.vects)
                    {
                        file << v.x << ", " << v.y << ", " << v.z << "\n";
                    }
                }
                scalar += 1.0;
            } else
            {
                for (auto &face: pol.m_faces)
                {
                    for (auto &v: face.vects)
                    {
                        file << v.x << ", " << v.y << ", " << v.z << ", " << 0.0 << "\n";
                    }
                }
            }
        }
        file.close();
    }

    ~Surface() = default;
};