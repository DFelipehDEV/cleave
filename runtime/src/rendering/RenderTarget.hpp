#pragma once
#include "rendering/TextureHandle.hpp"
#include "rendering/RenderTargetHandle.hpp"
#include "math/Vec2.hpp"

namespace Cleave {

class RenderTarget {
public:
    RenderTarget() = default;
    RenderTarget(int width, int height);
    ~RenderTarget() = default;

    RenderTargetHandle GetHandle() const;
    void SetHandle(RenderTargetHandle handle);

    TextureHandle GetTexture() const;
    void SetTexture(TextureHandle texture);

    int GetWidth() const;
    void SetWidth(int width);

    int GetHeight() const;
    void SetHeight(int height);
private:
    int m_width = 1, m_height = 1;
    TextureHandle m_texture = 0;
    RenderTargetHandle m_handle = 0;
};

}  // namespace Cleave