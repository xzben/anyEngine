#pragma once

#include "sceneGraphDefine.h"

BEGIN_NS_SCENCE_GRAPH

template <class T>
class InstanceObject {
public:
    static T* getInstance() {
        static T s_instance;

        return &s_instance;
    }
};

END_NS_SCENCE_GRAPH