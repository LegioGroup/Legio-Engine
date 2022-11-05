#pragma once


#include<vector>
#ifdef LG_VULKAN_API
#include "rendering/vulkan/Vulkan_Vertex.h"
#endif
#ifdef LG_OPENGL_API
#include "rendering/OpenGL/OpenGL_Vertex.h"
#endif

namespace LG
{
    class Primitive
    {
    protected:
        std::vector<Vertex> m_vertices;
        std::vector<uint32_t> m_indices;

    public:
        Primitive() {}
        virtual ~Primitive() {}

        inline const std::vector<Vertex>& GetVertices() const{ return m_vertices; }
        inline const std::vector<uint32_t>& GetIndices() const { return m_indices; }
    };

    class Triangle : public Primitive
    {
    public:
        Triangle()
            : Primitive()
        {
            m_vertices =
            {
                Vertex({0.f, 0.5f, 0.f},       {1.f, 0.f, 0.f},   {0.5f, 1.f},   {0.f, 0.f, 1.f}),
                Vertex({-0.5f, -0.5f, 0.f},    {0.f, 1.f, 0.f},   {0.f, 0.f},    {0.f, 0.f, 1.f}),
                Vertex({0.5f, -0.5f, 0.f},     {0.f, 0.f, 1.f},   {1.f, 0.f},    {0.f, 0.f, 1.f})
            };

            m_indices =
            {
                0, 1, 2//Triangle 1
            };
        }
    };

    class Quad : public Primitive
    {
    public:
        Quad()
            : Primitive()
        {
            m_vertices =
            {
                { {0.5f,  0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
                { {0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
                { {-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
                { {-0.5f,  0.5f, 0.0f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}}
            };
            m_indices =
            {
                0, 1, 2,//Triangle 1
                0, 2, 3 //Triangle 2
            };
        }
    };

    class Pyramid : public Primitive
    {
    public:
        Pyramid()
            : Primitive()
        {
            m_vertices =
            {
                //Triangle front
                Vertex({ 0.0f,  0.5f, 0.0f},    {1.f, 0.f, 0.f},   {0.5f, 1.f},    {0.f, 0.f, 1.f}),
                Vertex({-0.5f, -0.5f, 0.5f},    {0.f, 1.f, 0.f},   {0.f, 0.f},      {0.f, 0.f, 1.f}),
                Vertex({ 0.5f, -0.5f, 0.5f},    {0.f, 0.f, 1.f},   {1.f, 0.f},      {0.f, 0.f, 1.f}),

                //Triangle left
                Vertex({ 0.0f,  0.5f,  0.0f},   {1.f, 1.f, 0.f},   {0.5f, 1.f},     {-1.f, 0.f, 0.f}),
                Vertex({-0.5f, -0.5f, -0.5f},   {0.f, 0.f, 1.f},   {0.f, 0.f},      {-1.f, 0.f, 0.f}),
                Vertex({-0.5f, -0.5f,  0.5f},   {0.f, 0.f, 1.f},   {1.f, 0.f},      {-1.f, 0.f, 0.f}),

                //Triangle back
                Vertex({ 0.0f,  0.5f,  0.0f},   {1.f, 1.f, 0.f},   {0.5f, 1.f},     {0.f, 0.f, -1.f}),
                Vertex({ 0.5f, -0.5f, -0.5f},   {0.f, 0.f, 1.f},   {0.f, 0.f},      {0.f, 0.f, -1.f}),
                Vertex({-0.5f, -0.5f, -0.5f},   {0.f, 0.f, 1.f},   {1.f, 0.f},      {0.f, 0.f, -1.f}),

                //Triangles right
                Vertex({0.0f,  0.5f,  0.0f},    {1.f, 1.f, 0.f},   {0.5f, 1.f},     {1.f, 0.f, 0.f}),
                Vertex({0.5f, -0.5f,  0.5f},    {0.f, 0.f, 1.f},   {0.f, 0.f},      {1.f, 0.f, 0.f}),
                Vertex({0.5f, -0.5f, -0.5f},    {0.f, 0.f, 1.f},   {1.f, 0.f},      {1.f, 0.f, 0.f})
            };
        }
    };

    class Cube : public Primitive
    {
    public:
        Cube()
            : Primitive()
        {
            m_vertices =
            {
                Vertex({-0.5f, 0.5f, 0.5f},     {1.f, 0.f, 0.f},        {0.f, 1.f},     {0.f, 0.f, 1.f}),
                Vertex({-0.5f, -0.5f, 0.5f},    {0.f, 1.f, 0.f},        {0.f, 0.f},     {0.f, 0.f, 1.f}),
                Vertex({0.5f, -0.5f, 0.5f},     {0.f, 0.f, 1.f},        {1.f, 0.f},     {0.f, 0.f, 1.f}),
                Vertex({0.5f, 0.5f, 0.5f},      {1.f, 1.f, 0.f},        {1.f, 1.f},     {0.f, 0.f, 1.f}),

                Vertex({0.5f, 0.5f, -0.5f},     {1.f, 0.f, 0.f},        {0.f, 1.f},     {0.f, 0.f, -1.f}),
                Vertex({0.5f, -0.5f, -0.5f},    {0.f, 1.f, 0.f},        {0.f, 0.f},     {0.f, 0.f, -1.f}),
                Vertex({-0.5f, -0.5f, -0.5f},   {0.f, 0.f, 1.f},        {1.f, 0.f},     {0.f, 0.f, -1.f}),
                Vertex({-0.5f, 0.5f, -0.5f},    {1.f, 1.f, 0.f},        {1.f, 1.f},     {0.f, 0.f, -1.f})
            };

            m_indices =
            {
                0, 1, 2,
                0, 2, 3,

                7, 6, 1,
                7, 1, 0,

                4, 5, 6,
                4, 6, 7,

                3, 2, 5,
                3, 5, 4,

                0, 7, 4,// top
                0, 4, 3,

                1, 6, 5,// bottom
                1, 5, 2
            };
        }
    };
}
