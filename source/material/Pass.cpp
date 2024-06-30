#include "Pass.h"

BEGIN_NS_SCENCE_GRAPH
IMPLEMENT_RUNTIME_CLASS(Pass)

Pass::Pass(const std::string& name, Effect* effect) : m_name(name), m_effect(effect) {
    m_effect->addRef();
}
Pass::~Pass() { m_effect->delRef(); }

END_NS_SCENCE_GRAPH