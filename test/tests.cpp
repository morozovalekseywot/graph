#include "gtest/gtest.h"
#include <graph.hpp>
#include <surface.hpp>

TEST(Graph, search)
{
    vector<vector<bool>> mx = {{0, 1, 0, 1, 1, 1},
                               {1, 0, 1, 0, 0, 0},
                               {0, 1, 0, 1, 0, 1},
                               {1, 0, 1, 0, 1, 0},
                               {1, 0, 0, 1, 0, 1},
                               {1, 0, 1, 0, 1, 0}};

    vector<Vertex> verts = {Vertex(0, 2, 0), Vertex(2, 2, 0), Vertex(3, 1, 0), Vertex(2, 0, 0), Vertex(0, 0, 0), Vertex(1, 1, 1)};
    Graph graph(mx, verts);

    vector<double> ans0 = {0.0, 2.0, 2 + sqrt(2), sqrt(8), 2.0, sqrt(3)};
//    print_distance(graph.search(0));
    ASSERT_TRUE(graph.search(0) == ans0);

    vector<double> ans4 = {2.0, 4.0, 2.0 + sqrt(2), 2.0, 0.0, sqrt(3)};
//    print_distance(graph.search(4));
    ASSERT_TRUE(graph.search(4) == ans4);

    ASSERT_EQ(graph.search(4, 1), 4.0);
    ASSERT_EQ(graph.search(5, 1), sqrt(5) + sqrt(2)); // 5 -> 2 -> 1
}

TEST(Graph, search_path)
{
    vector<vector<bool>> mx = {{0, 1, 0, 1, 1, 1},
                               {1, 0, 1, 0, 0, 0},
                               {0, 1, 0, 1, 0, 1},
                               {1, 0, 1, 0, 1, 0},
                               {1, 0, 0, 1, 0, 1},
                               {1, 0, 1, 0, 1, 0}};

    vector<Vertex> verts = {Vertex(0, 2, 0), Vertex(2, 2, 0), Vertex(3, 1, 0), Vertex(2, 0, 0), Vertex(0, 0, 0), Vertex(1, 1, 1)};
    Graph graph(mx, verts);

    vector<int> ans = {4, 3, 2};
    ASSERT_TRUE(graph.search_path(4, 2) == ans);
    vector<int> ans2 = {5, 2, 1};
    ASSERT_TRUE(ans2 == graph.search_path(5, 1));
//    for (int i: ans2)
//        cout << i << ", ";
}

TEST(Graph, visualisation_path)
{
    cout.tie(NULL);
    vector<vector<bool>> mx = {{0, 1, 0, 1, 1, 1},
                               {1, 0, 1, 0, 0, 0},
                               {0, 1, 0, 1, 0, 1},
                               {1, 0, 1, 0, 1, 0},
                               {1, 0, 0, 1, 0, 1},
                               {1, 0, 1, 0, 1, 0}};

    vector<Vertex> verts = {Vertex(0, 2, 0), Vertex(2, 2, 0), Vertex(3, 1, 0), Vertex(2, 0, 0), Vertex(0, 0, 0), Vertex(1, 1, 1)};
    Graph graph(mx, verts);

    vector<int> ans = graph.search_path(5, 1);
    vector<Vertex> path(ans.size());
    for (int i = 0; i < ans.size(); i++)
        path[i] = graph.verts[ans[i]];

    Surface surface(graph);
    surface.write("graph.stl");

    Surface surface_path(path);
    surface_path.write_csv("path.csv");
}

TEST(Graph, add)
{
    cout.tie(NULL);
    vector<vector<bool>> mx = {{0, 1, 0, 1, 1, 1},
                               {1, 0, 1, 0, 0, 0},
                               {0, 1, 0, 1, 0, 1},
                               {1, 0, 1, 0, 1, 0},
                               {1, 0, 0, 1, 0, 1},
                               {1, 0, 1, 0, 1, 0}};

    vector<Vertex> verts = {Vertex(0, 2, 0), Vertex(2, 2, 0), Vertex(3, 1, 0), Vertex(2, 0, 0), Vertex(0, 0, 0), Vertex(1, 1, 1)};
    Graph graph(mx, verts);

    graph.add_vertex(Vertex{2, 1, 0});
    graph.add_edge(6, 2);
    graph.add_edge(6, 1);
    graph.add_edge(6, 4);
    graph.add_edge(6, 3);

    vector<double> ans4 = {2.0, sqrt(5) + 1, sqrt(5) + 1, 2.0, 0.0, sqrt(3), sqrt(5)};
    ASSERT_TRUE(graph.search(4) == ans4);

    Surface surface(graph);
    surface.write("graph_add.stl");

    vector<int> ans = graph.search_path(4, 2);
    vector<Vertex> path(ans.size());
    for (int i = 0; i < ans.size(); i++)
        path[i] = graph.verts[ans[i]];

    Surface surface_path(path);
    surface_path.write_csv("path_add.csv");
}

TEST(Graph, visualistaion)
{
    ios_base::sync_with_stdio(false);
    cout.tie(NULL);
    Graph graph = random_graph(150);
    int start = 0, end = 0;
    double dist = 0.0;
    for (int i = 0; i < graph.n; i++)
    {
        vector<double> ans3 = graph.search(i);
        for (int j = 0; j < ans3.size(); j++)
        {
            if (ans3[j] > dist)
            {
                start = i;
                end = j;
                dist = ans3[j];
            }
        }
    }

    vector<int> ans = graph.search_path(start, end);
    vector<Vertex> path(ans.size());
    for (int i = 0; i < ans.size(); i++)
        path[i] = graph.verts[ans[i]];

    Surface surface_path(path);
    surface_path.write_csv("big_path.csv");

    Surface surface(graph);
    surface.write("big_graph.stl");
}