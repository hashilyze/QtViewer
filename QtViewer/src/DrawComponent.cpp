#include "DrawComponent.h"
#include "gl/freeglut.h"

#include "pmp/io/io.h"
#include "pmp/algorithms/normals.h"

void DrawComponent::Init()
{
	pmp::read(mesh, "models\\bunny_stanford.obj");

	pmp::vertex_normals(mesh);

	std::cout << "#f " << mesh.n_faces() << "#y " << mesh.n_vertices() << std::endl;
}

void DrawComponent::Draw()
{
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