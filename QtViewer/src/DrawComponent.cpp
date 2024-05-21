#include "DrawComponent.h"



#include "gl/freeglut.h"
#include <chrono>
#include <queue>
#include <set>

#include "pmp/io/io.h"
#include "pmp/algorithms/normals.h"

//#define CASE_1_

//#define CASE_2_

//#define CASE_3_

//#define CASE_4_

#define CASE_5_

void DrawComponent::Init()
{
	//1. Load file
	InitLoadFile();

	//2. Add attributes
	AddAttributes();

#ifdef CASE_2_
	CalIdx(0);
#endif

	//3. Remove by faces
#ifdef CASE_3_
	std::set<pmp::Face> removeFaces;
	pmp::Vertex startV(0);

	for (auto f : mesh.faces(startV))
		removeFaces.insert(f);

	for (auto f : removeFaces)
		mesh.delete_face(f);

	mesh.garbage_collection();
#endif

	//4. Remove by vertices
#if defined CASE_4_ || defined CASE_5_
	std::set<pmp::Vertex> removeVertices;
	pmp::Vertex startV(0);

	for (auto v : mesh.vertices(startV))
		removeVertices.insert(v);

	for (auto v : removeVertices)
		mesh.delete_vertex(v);

	mesh.garbage_collection();
#endif
}

void DrawComponent::CalIdx(int idx)
{
#ifdef CASE_2_
	if (!mesh.has_vertex_property("v:index"))
		mesh.add_vertex_property<int>("v:index");
	auto vi = mesh.get_vertex_property<int>("v:index");

	for (auto v : mesh.vertices())
		vi[v] = -1;

	vi[pmp::Vertex(idx)] = 0;

	while (true) {
		bool isChanged = false;
		for (auto v : mesh.vertices()) {
			if (vi[v] >= 0) {
				for (auto nv : mesh.vertices(v)) {
					if (vi[nv] == -1) {
						vi[nv] = vi[v] + 1;
						isChanged = true;
					}
				}
			}
		}

		if (!isChanged)
			break;
	}

	int maxIdx = -1;
	for (auto v : mesh.vertices())
		maxIdx = std::max(maxIdx, vi[v]);

	// colorize vertices based on index (with hsv)
	if (!mesh.has_vertex_property("v:color"))
		mesh.add_vertex_property<pmp::Color>("v:color");
	auto vc = mesh.get_vertex_property<pmp::Color>("v:color");

	for (auto v : mesh.vertices()) {
		vc[v] = GetVertexColor(vi[v], maxIdx);
	}
#endif
}


void DrawComponent::InitLoadFile()
{
	std::string filePath = "obj\\kitten.obj";

	pmp::read(mesh, filePath);
	pmp::vertex_normals(mesh);
	pmp::face_normals(mesh);

	origMesh = mesh;
}

void DrawComponent::Draw()
{
	auto vn = mesh.get_vertex_property<pmp::Normal>("v:normal");
	auto fn = mesh.get_face_property<pmp::Normal>("f:normal");
	auto fc = mesh.get_face_property<pmp::Color>("f:color");
	auto vc = mesh.get_vertex_property<pmp::Color>("v:color");

	glLineWidth(3.0);

#ifdef CASE_1_
	/*for (auto f : mesh.faces()) {
		glNormal3dv(fn[f].data());
		DrawFace(&mesh, f);
	}*/

	for (auto f : mesh.faces()) {
		glBegin(GL_POLYGON);
		for (auto v : mesh.vertices(f)) {
			glNormal3dv(vn[v].data());
			glVertex3dv(mesh.position(v).data());
		}
		glEnd();
	}
#endif

#if defined CASE_3_ || defined CASE_4_
	glColor3f(0, 0, 0);
	glDisable(GL_LIGHTING);
	for (auto f : origMesh.faces()) //Draw with wireframe
		DrawFace(&origMesh, f, true);
	glEnable(GL_LIGHTING);

	glColor3f(1, 1, 1);
	for (auto f : mesh.faces()) {
		glNormal3dv(fn[f].data());
		DrawFace(&mesh, f);
	}
#endif

#ifdef CASE_2_ // Draw with vertex color
	for (auto f : mesh.faces()) {
		glBegin(GL_POLYGON);
		for (auto v : mesh.vertices(f)) {
			glColor3dv(vc[v].data());
			glNormal3dv(vn[v].data());
			glVertex3dv(mesh.position(v).data());
		}
		glEnd();
	}
#endif

#if defined CASE_5_
	for (auto f : mesh.faces()) {
		glNormal3dv(fn[f].data());
		bool flag = false;
		for (auto v : mesh.vertices(f)) {
			if (mesh.is_boundary(v)) {
				flag = true;
				break;
			}
		}

		if (flag) glColor3f(1, 0, 0);
		else glColor3f(1, 1, 1);

		DrawFace(&mesh, f);
	}
	/*for (auto f : mesh.faces()) {
		glNormal3dv(fn[f].data());

		if (mesh.is_boundary(f))
			glColor3f(1, 0, 0);
		else
			glColor3f(1, 1, 1);

		DrawFace(&mesh, f);
	}*/
#endif
}

void DrawComponent::DrawFace(pmp::SurfaceMesh* mesh, pmp::Face f, bool isWire) {
	glBegin(isWire ? GL_LINE_LOOP : GL_POLYGON);
	for (auto v : mesh->vertices(f))
		glVertex3dv(mesh->position(v).data());
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

pmp::Color DrawComponent::GetVertexColor(int vertexIndex, int maxIndex)
{
	HSV hsv;
	hsv.h = (float)vertexIndex / (float)maxIndex;
	hsv.s = 1.0;
	hsv.v = 1.0;

	return HSV2RGB(hsv);
}

pmp::Color DrawComponent::HSV2RGB(HSV hsv)
{
	pmp::Color rgb;
	float hh, p, q, t, ff;
	long i;

	if (hsv.s <= 0.0) {
		rgb[0] = hsv.v;
		rgb[1] = hsv.v;
		rgb[2] = hsv.v;
		return rgb;
	}

	hh = hsv.h;
	if (hh >= 1.0) hh = 0.0;
	hh *= 6.0;
	i = (long)hh;
	ff = hh - i;
	p = hsv.v * (1.0 - hsv.s);
	q = hsv.v * (1.0 - (hsv.s * ff));
	t = hsv.v * (1.0 - (hsv.s * (1.0 - ff)));

	switch (i) {
	case 0:
		rgb[0] = hsv.v;
		rgb[1] = t;
		rgb[2] = p;
		break;
	case 1:
		rgb[0] = q;
		rgb[1] = hsv.v;
		rgb[2] = p;
		break;
	case 2:
		rgb[0] = p;
		rgb[1] = hsv.v;
		rgb[2] = t;
		break;
	case 3:
		rgb[0] = p;
		rgb[1] = q;
		rgb[2] = hsv.v;
		break;
	case 4:
		rgb[0] = t;
		rgb[1] = p;
		rgb[2] = hsv.v;
		break;
	case 5:
	default:
		rgb[0] = hsv.v;
		rgb[1] = p;
		rgb[2] = q;
		break;
	}
	return rgb;
}

