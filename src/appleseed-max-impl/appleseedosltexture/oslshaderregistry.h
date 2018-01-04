
#pragma once

// 3ds Max headers.
#include <maxtypes.h>
#include <iparamb2.h>

// Standard headers.
#include <vector>
#include <map>

struct ShaderInfo
{
    enum ParamType
    {
        Float,
        Int,
        Color,
        ColorAlpha,
        TextureColor,
        Point,
        String
    };
    
    struct ParamInfo
    {
        wchar_t*        m_param_name;
        ShaderInfo::ParamType m_param_type;
        int             m_pid;
        wchar_t*        m_label_str;
        int             m_label_res_id;
        int             m_ctrl_res_id;
        int             m_num_of_ctrls;
    };
    
    Class_ID                        m_class_id;
    wchar_t*                        m_shader_name;
    wchar_t*                        m_internal_name;
    std::vector<ShaderInfo::ParamInfo*>    m_params;
};

class OSLShaderRegistry
{
  public:
    OSLShaderRegistry();
    
    ClassDesc2* get_class_descriptor(int index);
    int get_size();
    void create_class_descriptors();
    void add_parameter(ParamBlockDesc2* pb_desc, ShaderInfo::ParamInfo* param_info, int& ctrl_id);

  private:
    std::vector<ShaderInfo*>                        m_shaders;
    std::vector<MaxSDK::AutoPtr<ClassDesc2>>        m_class_descriptors;
    std::map<int, const ShaderInfo::ParamInfo*>     m_label_map;
    std::vector<MaxSDK::AutoPtr<ParamBlockDesc2>>   m_paramblock_descriptors;
};
