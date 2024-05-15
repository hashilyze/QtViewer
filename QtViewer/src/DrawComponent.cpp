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
	// add 4 vertices 
	const auto v0 = mesh.add_vertex(pmp::Point(0, 0, 0));
	const auto v1 = mesh.add_vertex(pmp::Point(1, 0, 0));
	const auto v2 = mesh.add_vertex(pmp::Point(0, 1, 0));
	const auto v3 = mesh.add_vertex(pmp::Point(0, 0, 1));
	// add 4 triangular faces
	mesh.add_triangle(v0, v1, v3);
	mesh.add_triangle(v1, v2, v3);
	mesh.add_triangle(v2, v0, v3);
	mesh.add_triangle(v0, v2, v1);

	std::cout << "vertices: " << mesh.n_vertices() << std::endl;
	std::cout << "edges: " << mesh.n_edges() << std::endl;
	std::cout << "faces: " << mesh.n_faces() << std::endl;

	auto normals = mesh.vertex_property<pmp::Normal>("v:normal");

	for (auto f : mesh.faces()) {
		glBegin(GL_TRIANGLES);
		for (auto v : mesh.vertices(f)) {
			auto p = mesh.position(v);
			auto n = normals[v];
			glNormal3d(n[0], n[1], n[2]);
			glVertex3d(p[0], p[1], p[2]);
		}
		glEnd();
	}
}