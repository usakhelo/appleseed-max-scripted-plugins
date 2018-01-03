
#include "oslshaderregistry.h"

#include "genericosltexture.h"

#include <iparamb2.h>
#include <maxtypes.h>

#include <vector>
#include <memory>

namespace
{
    ShaderInfo shader_1;
    ShaderInfo shader_2;

    ShaderInfo::ParamInfo sh1_params;
    ShaderInfo::ParamInfo sh2_params;
    ShaderInfo::ParamInfo sh3_params;
    ShaderInfo::ParamInfo sh4_params;

    int param_id = 0;

    int get_param_id()
    {
        return param_id++;
    }

    void fill_shader_info()
    {
        shader_1.m_shader_name = L"OSL_Noise";
        shader_1.m_class_id = Class_ID(0x467936e2, 0x3f6c32f1);
        shader_1.m_internal_name = L"osl_Noise";

        sh1_params.m_param_name = L"p0";
        sh1_params.m_param_type = ShaderInfo::ParamType::Color;
        sh1_params.m_pid = get_param_id();
        sh1_params.m_label_str = L"Color Param";

        shader_1.m_params.push_back(&sh1_params);

        sh2_params.m_param_name = L"p1";
        sh2_params.m_param_type = ShaderInfo::ParamType::Float;
        sh2_params.m_pid = get_param_id();
        sh2_params.m_label_str = L"Float Param";

        shader_1.m_params.push_back(&sh2_params);


        shader_2.m_shader_name = L"OSL_Ramp";
        shader_2.m_class_id = Class_ID(0x1aaa49a9, 0x7539328e);
        shader_2.m_internal_name = L"osl_Ramp";

        sh3_params.m_param_name = L"p0";
        sh3_params.m_param_type = ShaderInfo::ParamType::Color;
        sh3_params.m_pid = get_param_id();
        sh3_params.m_label_str = L"Color Param";

        shader_2.m_params.push_back(&sh3_params);

        sh4_params.m_param_name = L"p1";
        sh4_params.m_param_type = ShaderInfo::ParamType::Float;
        sh4_params.m_pid = get_param_id();
        sh4_params.m_label_str = L"Float Param";

        shader_2.m_params.push_back(&sh4_params);
    }
}

void OSLShaderRegistry::create_class_descriptors()
{
    m_shaders.push_back(&shader_1);
    m_shaders.push_back(&shader_2);

    for (auto sh : m_shaders)
    {
        MaxSDK::AutoPtr<ClassDesc2> class_descr(new GenericOSLTextureClassDesc(sh));

        m_class_descriptors.push_back(MaxSDK::AutoPtr<ClassDesc2>(class_descr));
    }
}

OSLShaderRegistry::OSLShaderRegistry()
{
    m_class_descriptors.clear();
    m_shaders.clear();
    // parse shaders in the directory
    // create shader_info for each of them
    // create class descriptor for each of shader_info
    fill_shader_info();
    create_class_descriptors();
}

ClassDesc2* OSLShaderRegistry::get_class_descriptor(int index)
{
    if (index < m_class_descriptors.size())
        return m_class_descriptors[index].Get();
    return nullptr;
}

int OSLShaderRegistry::get_size()
{
    return static_cast<int>(m_class_descriptors.size());
}
