#pragma once
#include "pmp/surface_mesh.h"


typedef struct {
    float h;
    float s;
    float v;
} HSV;

class DrawComponent
{
public:
    void Init();
    void InitMyMesh();
    void InitLoadFile();

    void Draw();
    void DrawFace(pmp::SurfaceMesh* mesh, pmp::Face f, bool isWire = false);
    void AddAttributes();

    void CalIdx(int idx);

    pmp::Color HSV2RGB(HSV hsv);

    pmp::Color GetVertexColor(int vertexIndex, int maxIndex);

    pmp::SurfaceMesh origMesh, mesh;

    int arrowIdx = 0;
};

