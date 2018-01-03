
#pragma once
#include <maxtypes.h>
#include <iparamb2.h>

#include <vector>

struct ShaderParamInfo;

struct ShaderInfo
{
    enum ShaderParamType
    {
        Float,
        Int,
        Color,
        ColorAlpha,
        TextureColor,
        Point,
        String
    };

    Class_ID                        m_class_id;
    wchar_t*                        m_shader_name;
    wchar_t*                        m_internal_name;
    std::vector<ShaderParamInfo>    m_params;
};

struct ShaderParamInfo
{
    wchar_t*        m_param_name;
    ShaderInfo::ShaderParamType m_param_type;
    int             m_pid;
    wchar_t*        m_label_str;
    int             m_label_res_id;
    int             m_ctrl_res_id;
};

class OSLShaderRegistry
{
  public:
    OSLShaderRegistry();
    
    ClassDesc2* get_class_descriptor(int index);
    int get_size();
    void create_class_descriptors();

  private:
    std::vector<ShaderInfo> m_shaders;
    std::vector<MaxSDK::AutoPtr<ClassDesc2>> m_class_descriptors;
    std::vector<MaxSDK::AutoPtr<ParamBlockDesc2>> m_paramblock_descriptors;
};
