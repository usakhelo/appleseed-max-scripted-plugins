
#pragma once
#include <maxtypes.h>
#include <iparamb2.h>

#include <vector>
#include <memory>

class GenericOSLTextureClassDesc;

struct ShaderInfo
{
    Class_ID m_class_id;
    wchar_t* m_shader_name;
};

std::vector<std::shared_ptr<GenericOSLTextureClassDesc>> registerShaderPlugins();
