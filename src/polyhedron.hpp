#pragma once

#include "face.hpp"
#include "graph.hpp"
#include <fstream>
#include <algorithm>

enum class PolyType
{
    Sphere,
    Cylinder,
    BigSphere,
    VeryBigSphere
};

struct Polyhedron
{
    vector <Face> m_faces;
    Vertex q; // направляющий вектор для поворота
    Vertex center;
    PolyType type;

    explicit Polyhedron(const PolyType &type, const Vertex &moves = Vertex{0, 0, 0}) : type(type)
    {
        switch (type)
        {
            case PolyType::Sphere:
                m_faces = read("sphere.stl");
                q = Vertex(0, 0, 1);
                center = Vertex(0, 0, 0);
                move(moves);
                break;
            case PolyType::Cylinder:
                m_faces = read("cylinder.stl");
                q = Vertex(1, 0, 0);
                center = Vertex(0, 0, 0);
                move(moves);
                break;
            case PolyType::BigSphere:
                m_faces = read("big_sphere.stl");
                q = Vertex(0, 0, 1);
                center = Vertex(0, 0, 0);
                break;
            case PolyType::VeryBigSphere:
                m_faces = read("very_big_sphere.stl");
                q = Vertex(0, 0, 1);
                center = Vertex(0, 0, 0);
                break;
        }
    }

    vector <Face> read(const std::string &filename)
    {
        std::string path = "../figure/" + filename;
        std::ifstream file(path, std::ios_base::in);
        if (!file.is_open()) // если файл не был открыт
            throw std::runtime_error("file isn't open\n");
        std::vector<Face> faces;
        std::string t;
        getline(file, t); // игнорируем имя solid
        while (!file.eof())
        {
            file >> t; // facet
            if (t == "endsolid")
                break;
            file >> t; // normal
            Vertex n;
            file >> n.x >> n.y >> n.z;
            getline(file, t); // ignore /n
            getline(file, t); // outer loop
            vector <Vertex> verts(3);
            for (auto &v: verts)
            {
                file >> t; // vertex
                file >> v.x >> v.y >> v.z;
            }
            faces.emplace_back(Face(verts, n));
            getline(file, t); // ignore /n
            getline(file, t); // endloop
            getline(file, t); // endfacet
        }
        file.close(); // закрываем файл

        return faces;
    }

    void move(const Vertex &v)
    {
        for (auto &face: m_faces)
        {
            for (auto &p: face.vects)
            {
                p.x += v.x;
                p.y += v.y;
                p.z += v.z;
            }
        }
        center.x += v.x;
        center.y += v.y;
        center.z += v.z;
    }

    void rotate(const Vertex &v_)
    {
        Vertex v(v_);
        v.normalize();
        Vertex n = cross(q, v);
        n.normalize();
        double phi = acos(v * q / v.mod());
        for (auto &face: m_faces)
        {
            for (auto &p: face.vects)
                rotate_vertex(p, phi, n);
            rotate_vertex(face.normal, phi, n);
        }
        Vertex center_old = center;
        rotate_vertex(center, phi, n);
        rotate_vertex(q, phi, n);
        move(center_old - center);
    }

    /// Пересчитывает координаты вектора при повороте на угол phi, с осью поворота n
    static void rotate_vertex(Vertex &p, const double phi, const Vertex &n)
    {
        Vertex old = p;
        p.x = old * Vertex(cos(phi) + (1 - cos(phi)) * n.x * n.x,
                           (1 - cos(phi)) * n.x * n.y - sin(phi) * n.z,
                           (1 - cos(phi)) * n.x * n.z + sin(phi) * n.y);

        p.y = old * Vertex((1 - cos(phi)) * n.x * n.y + sin(phi) * n.z,
                           cos(phi) + (1 - cos(phi)) * n.y * n.y,
                           (1 - cos(phi)) * n.y + n.z - sin(phi) * n.x);

        p.z = old * Vertex((1 - cos(phi)) * n.z * n.x - sin(phi) * n.y,
                           (1 - cos(phi)) * n.z * n.y + sin(phi) * n.x,
                           cos(phi) + (1 - cos(phi)) * n.z * n.z);
    }

    void shrink(double t)
    {
        for (auto &face: m_faces)
        {
            for (auto &p: face.vects)
            {
                p = p / t;
            }
        }
        center = center / t;
    }

    void write(const string &filename)
    {
        ofstream file("../figure/" + filename, std::ios_base::trunc);
        file << "solid " << filename.substr(0, filename.size() - 4) << "\n";
        for (auto &face: m_faces)
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
        file << "endsolid " << filename.substr(0, filename.size() - 4) << "\n";
        file.close();
    }

    vector <Vertex> get_Vertex()
    {
        vector <Vertex> a(m_faces.size());
        int size = 0;
        for (auto &face: m_faces)
            a[size++] = (face.vects[0] + face.vects[1] + face.vects[2]) / 3;

        return a;
    }

    ~Polyhedron() = default;
};