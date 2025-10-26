#include "rendering/RenderTarget.hpp"

namespace Cleave {
RenderTarget::RenderTarget(int width, int height)
    : m_width(width), m_height(height) {}

RenderTargetHandle RenderTarget::GetHandle() const { return m_handle; }
void RenderTarget::SetHandle(RenderTargetHandle handle) { m_handle = handle; }

TextureHandle RenderTarget::GetTexture() const { return m_texture; }
void RenderTarget::SetTexture(TextureHandle texture) { m_texture = texture; }

int RenderTarget::GetWidth() const { return m_width; }
void RenderTarget::SetWidth(int width) { m_width = width; }

int RenderTarget::GetHeight() const { return m_height; }
void RenderTarget::SetHeight(int height) { m_height = height; }
} // namespace Cleave
