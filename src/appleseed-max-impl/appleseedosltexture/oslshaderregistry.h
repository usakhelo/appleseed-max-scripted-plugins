
#pragma once
#include <maxtypes.h>
#include <iparamb2.h>

#include <vector>

struct ShaderInfo
{
    Class_ID m_class_id;
    wchar_t* m_shader_name;
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
