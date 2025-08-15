#pragma once
#include <glad/glad.h>

struct VertexBuffer{
    uint32_t id = {};

    void create(uint32_t size,float* data);

    void bind()const;
    void unbind()const;

    void destroy();
};

struct IndexBuffer{
    uint32_t id = {};

    void create(uint32_t count,uint32_t* data);

    void bind()const;
    void unbind()const;

    void destroy();
};
