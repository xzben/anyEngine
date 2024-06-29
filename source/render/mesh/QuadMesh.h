#pragma once

#include "../Mesh.h"

BEGIN_NS_SCENCE_GRAPH
class QuadMesh : public Mesh {
    DECLARE_RUNTIME_CLASS(QuadMesh)
public:
    QuadMesh();
    ~QuadMesh();
};

END_NS_SCENCE_GRAPH