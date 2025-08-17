#pragma once 
#include "buffer.hpp"

struct VertexArray{
    uint32_t id = {};
    VertexBuffer vbo = {};
    IndexBuffer ibo = {};

    void create();
    
    void bind();
    void unbind();

    void destroy();
};
