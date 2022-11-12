#pragma once

namespace LG 
{
    class FrameBuffer 
    {
    public:
        FrameBuffer(float width, float height);
        ~FrameBuffer();
        unsigned int GetFrameTexture();
        void RescaleFrameBuffer(float width, float height);
        void Bind() const;
        void Unbind() const;

    private:
        unsigned int m_fbo{};
        unsigned int m_texture{};
        unsigned int m_rbo{};

    };
}