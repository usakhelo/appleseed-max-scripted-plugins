
#pragma once
#include <maxtypes.h>
#include <iparamb2.h>

#include <vector>

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

struct ShaderInfo
{
    Class_ID    m_class_id;
    wchar_t*    m_shader_name;
    wchar_t*    m_internal_name;
};

struct ShaderParamInfo
{
    wchar_t*    m_param_name;
    ShaderParamType   m_param_type;
    int         m_pid;
    int         m_str_id;
    int         m_ui_id;
};


class DlgTemplateGenerator
{
  public:
    DWORD* generate_template(IParamBlock2* pblock);
    void release_template();

  private:
    DWORD* buf;
};

class OSLShaderRegistry
{
  public:
    OSLShaderRegistry();
    
    ClassDesc2* get_class_descriptor(int index);
    int get_size();
    void instanciate_shader_plugins();

  private:
    std::vector<ShaderInfo> m_shaders;
    std::vector<MaxSDK::AutoPtr<ClassDesc2>> m_class_descriptors;
    std::vector<MaxSDK::AutoPtr<ParamBlockDesc2>> m_paramblock_descriptors;
};
