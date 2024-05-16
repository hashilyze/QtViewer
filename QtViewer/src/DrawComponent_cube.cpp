#include "DrawComponent.h"
#include "gl/freeglut.h"

#include "pmp/io/io.h"
#include "pmp/algorithms/normals.h"

void DrawComponent::Init()
{
	/*pmp::read(mesh, "models\\bunny_stanford.obj");

	pmp::vertex_normals(mesh);

	std::cout << "#f " << mesh.n_faces() << "#y " << mesh.n_vertices() << std::endl;*/
}

void DrawComponent::Draw()
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
	// add 6 triangular faces
	mesh.add_quad(v0, v1, v5, v4);
	mesh.add_quad(v3, v2, v1, v0);
	mesh.add_quad(v1, v2, v6, v5);
	mesh.add_quad(v3, v0, v4, v7);
	mesh.add_quad(v2, v3, v7, v6);
	mesh.add_quad(v4, v5, v6, v7);

	std::cout << "vertices: " << mesh.n_vertices() << std::endl;
	std::cout << "edges: " << mesh.n_edges() << std::endl;
	std::cout << "faces: " << mesh.n_faces() << std::endl;

	auto normals = mesh.vertex_property<pmp::Normal>("v:normal");
	for (auto f : mesh.faces()) {
		glBegin(GL_QUADS);
		for (auto v : mesh.vertices(f)) {
			auto p = mesh.position(v);
			auto n = normals[v];
			glNormal3d(n[0], n[1], n[2]);
			glVertex3d(p[0], p[1], p[2]);
		}
		glEnd();
	}
	pmp::write(mesh, "output.obj");
}