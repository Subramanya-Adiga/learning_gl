#pragma once
#include <cstdint>
#include <string_view>
#include <glad/glad.h>

struct Shader{
    uint32_t shader_id;

    void load_from_file(std::string_view veretex,std::string_view fragement);

    void use_shader() const;

    void delete_shader();

    static uint32_t process_shader(const char* source,GLenum shader_type);
};
