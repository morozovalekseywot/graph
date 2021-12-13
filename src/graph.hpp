#pragma once

#include "vertex.hpp"
#include "polyhedron.hpp"
#include <chrono>
#include <cmath>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iomanip>

using namespace std;

struct Edge
{
    int a = 0; // индекс первой вершины
    int b = 0; // индекс второй вершины
    double cost = 0.0; // цена пути между ними

    Edge(int a, int b, double cost) : a(a), b(b), cost(cost)
    {};

    ~Edge() = default;
};

class Graph
{
public:
    vector<vector<bool>> mx; // матрица смежности
    vector<Vertex> verts; // вершины
    vector<Edge> edges;
    int n = 0;
    int m = 0; // количество рёбер

    Graph(const vector<vector<bool>> &matrix, const vector<Vertex> &verts) : mx(matrix), verts(verts), n(verts.size())
    {
        for (int i = 0; i < n; ++i)
            for (int j = i + 1; j < n; ++j)
                if (mx[i][j])
                    m++;

        edges.reserve(m);
        for (int i = 0; i < n; ++i)
            for (int j = i; j < n; ++j)
                if (mx[i][j])
                    edges.emplace_back(i, j, dist(verts[i], verts[j]));
    };

    void add_vertex(const Vertex &p)
    {
        // проверка на дубликат
        for (auto &v: verts)
            if (equal(v, p, 0.0001))
                return;

        verts.emplace_back(p);
        for (int i = 0; i < n; ++i)
            mx[i].emplace_back(false);
        mx.emplace_back(vector<bool>(n + 1, false));
        n++;
    }

    void add_edge(const int begin, const int end)
    {
        if (begin < 0 || begin >= n || end < 0 || end >= n)
        {
            cerr << "Error in function add_edge(" << begin << ", " << end << ")\n";
            throw out_of_range("Error in function add_edge");
        }

        // проверка на дубликат
        if (mx[begin][end])
            return;

        m++;
        edges.emplace_back(Edge(begin, end, dist(verts[begin], verts[end])));
        mx[begin][end] = true;
        mx[end][begin] = true;
    }

    /// поиск кратчайшего расстояния между двумя вершинами алгоритмом Форда-Беллмана
    [[nodiscard]] double search(int begin, int end) const
    {
        vector<double> d(n, 1e9);
        d[begin] = 0;
        while (true)
        {
            bool any = false;
            for (int j = 0; j < m; ++j)
                if (d[edges[j].b] > d[edges[j].a] + edges[j].cost)
                {
                    d[edges[j].b] = d[edges[j].a] + edges[j].cost;
                    any = true;
                } else if (d[edges[j].a] > d[edges[j].b] + edges[j].cost) // так как граф не ориентированный и все рёбра хранятся один раз
                {
                    d[edges[j].a] = d[edges[j].b] + edges[j].cost;
                    any = true;
                }
            if (!any) break;
        }

        if (d[end] != 1e9)
            return d[end];
        else
            return -1.0;
    }

    /// поиск кратчайшего расстояния от начальной вершины до всех
    [[nodiscard]] vector<double> search(int begin) const
    {
        vector<double> d(n, 1e9);
        d[begin] = 0;
        while (true)
        {
            bool any = false;
            for (int j = 0; j < m; ++j)
                if (d[edges[j].b] > d[edges[j].a] + edges[j].cost)
                {
                    d[edges[j].b] = d[edges[j].a] + edges[j].cost;
                    any = true;
                } else if (d[edges[j].a] > d[edges[j].b] + edges[j].cost) // так как граф не ориентированный и все рёбра хранятся один раз
                {
                    d[edges[j].a] = d[edges[j].b] + edges[j].cost;
                    any = true;
                }
            if (!any) break;
        }

        for (int i = 0; i < n; i++)
            if (d[i] == 1e9)
                d[i] = -1.0;

        return d;
    }

    /// поиск кратчайшего пути между двумя вершинами
    [[nodiscard]] vector<int> search_path(int begin, int end) const
    {
        vector<double> d(n, 1e9);
        d[begin] = 0;
        vector<int> p(n, -1); // храним предпоследнюю вершину в кратчайшем пути, ведущем в неё
        while (true)
        {
            bool any = false;
            for (int j = 0; j < m; ++j)
                if (d[edges[j].b] > d[edges[j].a] + edges[j].cost)
                {
                    d[edges[j].b] = d[edges[j].a] + edges[j].cost;
                    p[edges[j].b] = edges[j].a;
                    any = true;
                } else if (d[edges[j].a] > d[edges[j].b] + edges[j].cost) // так как граф не ориентированный и все рёбра хранятся один раз
                {
                    d[edges[j].a] = d[edges[j].b] + edges[j].cost;
                    p[edges[j].a] = edges[j].b;
                    any = true;
                }
            if (!any) break;
        }

        if (d[end] == 1e9)
            return {begin};

        vector<int> path;
        path.reserve(n / 2);
        for (int cur = end; cur != -1; cur = p[cur])
            path.push_back(cur);
        reverse(path.begin(), path.end());

        return path;
    }

    ~Graph() = default;
};

Graph random_graph(int n,const vector<Vertex> &points = {})
{
    vector<vector<bool>> mx(n);
    for (int i = 0; i < n; i++)
    {
        mx[i].resize(n, false);
        for (int j = i + 1; j < n; j++)
            if (rand() % (2*n/3) == 3)
                mx[i][j] = true;
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < i; j++)
            if (mx[j][i])
                mx[i][j] = true;
    }
    vector<Vertex> ans;
    if(points.empty())
    {
        Polyhedron sphere(PolyType::VeryBigSphere);
        sphere.shrink(0.2);
        ans = sphere.get_Vertex();
    } else
    {
        ans = points;
    }

    if (n > ans.size())
        throw runtime_error("too large n");

    int step = ans.size() / n, size = 0;
    vector<Vertex> verts(n);
    for (int i = 0; i < ans.size(); i += step)
    {
        if (size >= n)
            break;
        verts[size++] = ans[i];
    }

    Graph graph(mx, verts);
    return graph;
}

inline void print_distance(const vector<double> &ans)
{
    for (int i = 0; i < ans.size(); i++)
        cout << fixed << setprecision(3) << i + 1 << ": " << ans[i] << "\n";
}