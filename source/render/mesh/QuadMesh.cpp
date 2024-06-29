
#include "QuadMesh.h"

#include "app/Application.h"

BEGIN_NS_SCENCE_GRAPH

IMPLEMENT_RUNTIME_CLASS(QuadMesh)

QuadMesh::QuadMesh() : Mesh(gfx::PrimitiveType::TRIANGLE_STRIP) {}
QuadMesh::~QuadMesh() {}
END_NS_SCENCE_GRAPH