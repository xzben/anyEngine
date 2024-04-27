#include "VulkanCommandBuffer.h"

#include "VulkanBuffer.h"
#include "VulkanCommandPool.h"
#include "VulkanInputAssembler.h"
#include "VulkanPipeline.h"
#include "VulkanTexture.h"

BEGIN_GFX_NAMESPACE

static VkCommandBufferLevel s_vk_levels[] = {VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                                             VK_COMMAND_BUFFER_LEVEL_SECONDARY};

inline VkCommandBufferUsageFlags map_vk_command_buffer_usage_flags(
    CommandBufferUsage flags) {
    VkCommandBufferUsageFlags result = 0;

    if (hasFlag(flags, CommandBufferUsage::ONE_TIME_SUBMIT))
        result |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    if (hasFlag(flags, CommandBufferUsage::RENDER_PASS_CONTINUE))
        result |= VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;

    if (hasFlag(flags, CommandBufferUsage::SIMULTANEOUS_USE))
        result |= VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

    return result;
}

VulkanCommandBuffer::VulkanCommandBuffer(VulkanCommandPool& pool,
                                         CommandBufferLevel level)
    : m_pool(pool), m_level(level) {
    VkCommandBufferAllocateInfo allocate_info{
        VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
    allocate_info.commandPool        = pool;
    allocate_info.commandBufferCount = 1;
    allocate_info.level              = s_vk_levels[(int)level];

    if (vkAllocateCommandBuffers(pool.getLogicDevice(), &allocate_info,
                                 &m_handle)
        != VK_SUCCESS) {
        m_handle = VK_NULL_HANDLE;
    }
}

VulkanCommandBuffer::~VulkanCommandBuffer() {
    if (m_handle != VK_NULL_HANDLE) {
        vkFreeCommandBuffers(m_pool.getLogicDevice(), m_pool, 1, &m_handle);
    }
}

void VulkanCommandBuffer::begin(CommandBufferUsage usage) {
    VkCommandBufferBeginInfo info{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
    info.flags = map_vk_command_buffer_usage_flags(usage);
    vkBeginCommandBuffer(m_handle, &info);
}

void VulkanCommandBuffer::beginRendPass(
    RenderPass* renderpass, const std::vector<DrawSurface*>& attachments,
    const std::vector<ClearValue>& clearValues) {}

void VulkanCommandBuffer::bindPipeline(Pipeline* pipeline) {
    CCASSERT(pipeline != nullptr, "pipeline to been nullptr");

    m_curPipeline = pipeline;
    vkCmdBindPipeline(m_handle, VK_PIPELINE_BIND_POINT_GRAPHICS,
                      pipeline->getHandle<VkPipeline>());
}

void VulkanCommandBuffer::bindInputAssembler(InputAssembler* input) {
    m_curInput      = input;
    auto pVertexBuf = input->getVertexBuffer();
    auto pIndexBuf  = input->getIndexBuffer();

    CCASSERT(pVertexBuf != nullptr,
             "bind input vertex buffer must not been nullptr");
    VkBuffer buf = pVertexBuf->getHandle<VkBuffer>();
    vkCmdBindVertexBuffers(m_handle, 0, 1, &buf, nullptr);

    if (pIndexBuf != nullptr) {
        VkBuffer indexBuf      = pIndexBuf->getHandle<VkBuffer>();
        uint32_t indexItemSize = input->getIndexItemSize();
        VkIndexType type       = VK_INDEX_TYPE_UINT32;
        switch (indexItemSize) {
            case 32:
                type = VK_INDEX_TYPE_UINT32;
                break;
            case 16:
                type = VK_INDEX_TYPE_UINT16;
                break;
            default: {
                CCASSERT(false, "invliad index Item size:%d", indexItemSize);
                break;
            }
        }
        vkCmdBindIndexBuffer(m_handle, indexBuf, 0, type);
    }
}

void VulkanCommandBuffer::bindTexture(const std::string& name, Texture* tex) {}
void VulkanCommandBuffer::bindUniformBuffer(const std::string& name,
                                            Buffer* buf) {}

void VulkanCommandBuffer::draw() {
    uint32_t indexCount  = m_curInput->getIndexCount();
    uint32_t vertexCount = m_curInput->getVertexCount();
    if (indexCount > 0) {
        vkCmdDrawIndexed(m_handle, indexCount, 1, 0, 0, 0);
    } else {
        vkCmdDraw(m_handle, vertexCount, 1, 0, 0);
    }
}

void VulkanCommandBuffer::enable(RenderState state) {}
void VulkanCommandBuffer::disable(RenderState state) {}
void VulkanCommandBuffer::draw(const DrawMeshInfo& info) {}
void VulkanCommandBuffer::compute(const ComputeInfo& info) {}

void VulkanCommandBuffer::nextSubPass() {
    vkCmdNextSubpass(m_handle, VK_SUBPASS_CONTENTS_INLINE);
}

void VulkanCommandBuffer::endRendPass() { vkCmdEndRenderPass(m_handle); }
void VulkanCommandBuffer::end() { vkEndCommandBuffer(m_handle); }

void VulkanCommandBuffer::setViewport(float x, float y, float width,
                                      float height) {
    VkViewport viewport{x, y, width, height, 0.f, 1.f};
    vkCmdSetViewport(m_handle, 0, 1, &viewport);
}

void VulkanCommandBuffer::setScissor(float x, float y, float width,
                                     float height) {
    VkRect2D rect{{x, y}, {width, height}};
    vkCmdSetScissor(m_handle, 0, 1, &rect);
}

void VulkanCommandBuffer::copyBuffer(Buffer* src, Buffer* dst, uint32_t size,
                                     uint32_t srcOffset, uint32_t dstOffset) {
    VkBufferCopy copy_region = {};
    copy_region.size = size == 0 ? std::min(src->size(), dst->size()) : size;
    copy_region.srcOffset = srcOffset;
    copy_region.dstOffset = dstOffset;

    vkCmdCopyBuffer(*this, src->getHandle<VkBuffer>(),
                    dst->getHandle<VkBuffer>(), 1, &copy_region);
}

void VulkanCommandBuffer::copyTexture(Texture* src, Texture* dst,
                                      const TextureCopyInfo& info) {
    // VkImage srcImg = src->getHandle<VkImage>();
    // VkImage dstImg = dst->getHandle<VkImage>();
    // VkImageBlit blit;
    // blit.srcOffsets[0] = {info.srcOffset.x, info.srcOffset.y,
    // info.srcOffset.z}; blit.srcOffsets[1] = {info.range.x, info.range.y,
    // info.range.z}; blit.srcSubresource.aspectMask     =
    // VK_IMAGE_ASPECT_COLOR_BIT; blit.srcSubresource.mipLevel       = 1;
    // blit.srcSubresource.baseArrayLayer = 0;
    // blit.srcSubresource.layerCount     = 1;
    // blit.dstOffsets[0] = {info.dstOffset.x, info.dstOffset.y,
    // info.dstOffset.z}; blit.dstOffsets[1] = {info.range.x, info.range.y,
    // info.range.z}; blit.dstSubresource.aspectMask     =
    // VK_IMAGE_ASPECT_COLOR_BIT; blit.dstSubresource.mipLevel       = 1;
    // blit.dstSubresource.baseArrayLayer = 0;
    // blit.dstSubresource.layerCount     = 1;

    // vkCmdBlitImage(m_handle, srcImg, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
    //                dstImg, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit,
    //                VK_FILTER_LINEAR);
}

void VulkanCommandBuffer::blitTexture(Texture* src, Texture* dst,
                                      const TextureBliteInfo& info) {}
void VulkanCommandBuffer::generateMipmaps(Texture* textre, uint32_t mipLevels) {
    VkImage imgHandle              = textre->getHandle<VkImage>();
    VkCommandBuffer command_buffer = m_handle;

    VkImageMemoryBarrier barrier{};
    barrier.sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.image               = imgHandle;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount     = 1;
    barrier.subresourceRange.levelCount     = 1;

    auto& info        = textre->getInfo();
    int32_t mipWidth  = info.width;
    int32_t mipHeight = info.height;

    for (uint32_t i = 1; i < mipLevels; i++) {
        barrier.subresourceRange.baseMipLevel = i - 1;
        barrier.oldLayout     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout     = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

        vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
                             VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0,
                             nullptr, 1, &barrier);

        VkImageBlit blit{};
        blit.srcOffsets[0]                 = {0, 0, 0};
        blit.srcOffsets[1]                 = {mipWidth, mipHeight, 1};
        blit.srcSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.srcSubresource.mipLevel       = i - 1;
        blit.srcSubresource.baseArrayLayer = 0;
        blit.srcSubresource.layerCount     = 1;
        blit.dstOffsets[0]                 = {0, 0, 0};
        blit.dstOffsets[1]                 = {mipWidth > 1 ? mipWidth / 2 : 1,
                              mipHeight > 1 ? mipHeight / 2 : 1, 1};
        blit.dstSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.dstSubresource.mipLevel       = i;
        blit.dstSubresource.baseArrayLayer = 0;
        blit.dstSubresource.layerCount     = 1;

        vkCmdBlitImage(command_buffer, imgHandle,
                       VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, imgHandle,
                       VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit,
                       VK_FILTER_LINEAR);

        barrier.oldLayout     = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.newLayout     = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
                             VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0,
                             nullptr, 0, nullptr, 1, &barrier);

        if (mipWidth > 1) mipWidth /= 2;
        if (mipHeight > 1) mipHeight /= 2;
    }

    barrier.subresourceRange.baseMipLevel = mipLevels - 1;
    barrier.oldLayout     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout     = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
                         VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr,
                         0, nullptr, 1, &barrier);
}
END_GFX_NAMESPACE