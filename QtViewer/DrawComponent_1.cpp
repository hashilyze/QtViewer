#include "DrawComponent.h"

#include "gl/freeglut.h"
#include <chrono>
#include <queue>
#include <set>

#include "pmp/io/io.h"
#include "pmp/algorithms/normals.h"

void DrawComponent::Init()
{
    //1. Load file or Create mesh.
    //InitMyMesh();
    InitLoadFile();

    //2. Add attributes.
    AddAttributes();

    //pmp::write(mesh, "output.obj");
}

void DrawComponent::InitLoadFile()
{
    pmp::read(mesh, "obj\\kitten.obj");
    pmp::vertex_normals(mesh);
    pmp::face_normals(mesh);
}

void DrawComponent::InitMyMesh()
{
    // add 8 vertices 
    const auto v0 = mesh.add_vertex(pmp::Point(0, 0, 0));
    const auto v1 = mesh.add_vertex(pmp::Point(2, 0, 0));
    const auto v2 = mesh.add_vertex(pmp::Point(2, 0, 2));
    const auto v3 = mesh.add_vertex(pmp::Point(0, 0, 2));
    const auto v4 = mesh.add_vertex(pmp::Point(0, 2, 0));
    const auto v5 = mesh.add_vertex(pmp::Point(2, 2, 0));
    const auto v6 = mesh.add_vertex(pmp::Point(2, 2, 2));
    const auto v7 = mesh.add_vertex(pmp::Point(0, 2, 2));
    // add 6 quad faces
    mesh.add_quad(v0, v1, v5, v4);
    mesh.add_quad(v3, v2, v1, v0);
    mesh.add_quad(v1, v2, v6, v5);
    mesh.add_quad(v3, v0, v4, v7);
    mesh.add_quad(v2, v3, v7, v6);
    mesh.add_quad(v4, v5, v6, v7);

    std::cout << "vertices: " << mesh.n_vertices() << std::endl;
    std::cout << "edges: " << mesh.n_edges() << std::endl;
    std::cout << "faces: " << mesh.n_faces() << std::endl;

    pmp::vertex_normals(mesh);
    pmp::face_normals(mesh);
}

void DrawComponent::Draw()
{
    auto vn = mesh.get_vertex_property<pmp::Normal>("v:normal");
    auto fn = mesh.get_face_property<pmp::Normal>("f:normal");
    auto fc = mesh.get_face_property<pmp::Color>("f:color");

    //1. Basic drawing
    if (false)
        for (auto f : mesh.faces()) {
            glNormal3dv(fn[f].data());
            DrawFace(f);
        }

    //2. Color with normals
    if (false)
        for (auto f : mesh.faces())
        {
            glColor3dv(fc[f].data());
            DrawFace(f);
        }

    //3. Traverse the mesh
    if (true)
    {
        glLineWidth(1.0);
        glPointSize(20.0);

        //3-1. Draw the mesh with wireframe.
        glColor3f(0, 0, 0);
        for (auto f : mesh.faces()) {
            DrawFace(f, true);
        }

        {
            pmp::Vertex startV(0);
            float ringColors[][3] = { {0, 0, 1}, {1, 1, 0}, {0, 1, 0}, {1, 0, 1} };
            std::set<pmp::Vertex> visitedVertex;
            std::set<pmp::Face> visitedFace;

            visitedVertex.insert(startV);            
            for (int i = 0, e = sizeof(ringColors) / (sizeof(float) * 3); i < e; ++i) {
                float (& ringColor)[3] = ringColors[i];
                glColor3f(ringColor[0], ringColor[1], ringColor[2]);

                for (auto vertex : visitedVertex) {
                    for (auto face : mesh.faces(vertex)) {
                        if (visitedFace.find(face) == visitedFace.end()) {
                            DrawFace(face);
                            visitedFace.insert(face);
                        }
                    }
                }

                std::set<pmp::Vertex> neighbours;
                for (auto vertex : visitedVertex) {
                    for (auto candidate : mesh.vertices(vertex)) {
                        if (visitedVertex.find(candidate) == visitedVertex.end()) {
                            neighbours.insert(candidate);
                        }
                    }
                }
                for (auto vertex : neighbours) {
                    visitedVertex.insert(vertex);
                }
            }

            glColor3f(1, 0, 0);
            glBegin(GL_POINTS);
            glVertex3dv(mesh.position(startV).data());
            glEnd();

            glColor3f(0, 1, 0);
            glBegin(GL_POINTS);
            for (auto v : mesh.vertices(startV)) {
                auto p = mesh.position(v);
                glVertex3dv(p.data());
            }
            glEnd();
        }
    }
}

void DrawComponent::DrawFace(pmp::Face f, bool isWire) {
    glBegin(isWire ? GL_LINE_LOOP : GL_POLYGON);
    for (auto v : mesh.vertices(f))
        glVertex3dv(mesh.position(v).data());
    glEnd();
}

void DrawComponent::AddAttributes()
{
    mesh.add_face_property<pmp::Color>("f:color");

    // compute vertex normals
    pmp::vertex_normals(mesh);

    // compute face normals
    pmp::face_normals(mesh);

    // colorize faces based on normals
    auto fn = mesh.get_face_property<pmp::Normal>("f:normal");
    auto fc = mesh.get_face_property<pmp::Color>("f:color");

    for (auto f : mesh.faces())
    {
        auto n = fn[f];
        pmp::Color c(std::abs(n[0]), std::abs(n[1]), std::abs(n[2]));
        fc[f] = c;
    }
}
