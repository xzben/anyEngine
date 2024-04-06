#include "pipeline_common.h"

BEGIN_PIPELINE_NAMESPACE

struct ShaderCode {
    std::vector<uint8_t> code;
    gfx::ShaderStage stage;
    std::string entryName{"main"};
};

END_PIPELINE_NAMESPACE