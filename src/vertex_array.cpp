#include "vertex_array.hpp"
#include <glad/glad.h>

void VertexArray::create() { glCreateVertexArrays(1, &id); }

void VertexArray::bind() { glBindVertexArray(id); }

void VertexArray::unbind() { glBindVertexArray(0); }

void VertexArray::destroy() { glDeleteVertexArrays(1, &id); }
