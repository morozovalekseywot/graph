#include "graph.hpp"
#include "surface.hpp"

#define CIN                           \
    ios_base::sync_with_stdio(false); \
    cin.tie(NULL);                    \
    cout.tie(NULL)

void test_graph()
{
    int n = 5;
    vector<vector<bool>> mx = {{0, 1, 0, 1, 1, 1},
                               {1, 0, 1, 0, 0, 0},
                               {0, 1, 0, 1, 0, 1},
                               {1, 0, 1, 0, 1, 0},
                               {1, 0, 0, 1, 0, 1},
                               {1, 0, 1, 0, 1, 0}};

    vector<Vertex> verts = {Vertex(0, 2, 0), Vertex(2, 2, 0), Vertex(3, 1, 0), Vertex(2, 0, 0), Vertex(0, 0, 0), Vertex(1, 1, 1)};
    Graph graph(mx, verts);
    vector<double> ans = graph.search(0);
    for (int i = 0; i < n; i++)
        cout << fixed << setprecision(3) << i + 1 << ": " << ans[i] << "\n";

    auto ans2 = graph.search_path(4, 2);
    for (int i: ans2)
        cout << i << ", ";
    cout << "\n";
    cout << graph.search(4, 2);
}

void test_graph_visualisation()
{
    vector<vector<bool>> mx = {{0, 1, 0, 1, 1, 1},
                               {1, 0, 1, 0, 0, 0},
                               {0, 1, 0, 1, 0, 1},
                               {1, 0, 1, 0, 1, 0},
                               {1, 0, 0, 1, 0, 1},
                               {1, 0, 1, 0, 1, 0}};

    vector<Vertex> verts = {Vertex(0, 2, 0), Vertex(2, 2, 0), Vertex(3, 1, 0), Vertex(2, 0, 0), Vertex(0, 0, 0), Vertex(1, 1, 1)};
    Graph graph(mx, verts);
    vector<int> ans = graph.search_path(0, 2);
    vector<Vertex> path(ans.size());
    for (int i = 0; i < ans.size(); i++)
        path[i] = graph.verts[ans[i]];

    Surface surface(graph);
    surface.write("graph.stl");

    Surface surface_path(path);
    surface_path.write_csv("path.csv");
}

void bench_search(int n)
{
    vector<Vertex> points(120050);
    ifstream in("../figure/vertices.txt", ios_base::in);
    if(!in.is_open())
        throw runtime_error("vertices.txt didn't open");
    cout<<"Begin read\n";
    int size = 0;
    while (!in.eof())
        in >> points[size].x >> points[size].y >> points[size++].z;
    cout<<"End read\n";
    in.close();
    points.resize(size);
    ofstream file("../graphics/bench_search.txt", ios_base::trunc);
    for (int i = n / 50; i <= n; i += n / 50)
    {
        Graph graph = random_graph(i, points);
        auto begin = chrono::steady_clock::now();
        vector<double> ans = graph.search(0);
        auto end = chrono::steady_clock::now();
        auto time = chrono::duration_cast<chrono::microseconds>(end - begin);
        cout << 50 * i / n << "%\n";
        file << i << " " << time.count() << "\n";
    }
}

int main()
{
//    test_graph();
//    test_graph_visualisation();
//    CIN;
    bench_search(100000);

//    Polyhedron sphere1(PolyType::Cylinder);
//    sphere1.write("big_sphere1.stl");
//    sphere1.shrink(0.1);
//    sphere1.write("sphere1.stl");
//
//    Polyhedron sphere2(PolyType::Sphere);
//    sphere2.move(Vertex(1, 0, 0));
//
//    Polyhedron cylinder(PolyType::Cylinder);
//    cylinder.rotate(sphere2.center-sphere1.center);
//
//    Surface surface({sphere1, sphere2, cylinder});
//    surface.write("output.stl");

    return 0;
}
