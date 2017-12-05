
#pragma once
#include <maxtypes.h>
#include <iparamb2.h>

#include <vector>
#include <memory>

struct ShaderInfo
{
    Class_ID m_class_id;
    wchar_t* m_shader_name;
};

std::vector<ClassDesc2*> instanciate_shader_plugins();
