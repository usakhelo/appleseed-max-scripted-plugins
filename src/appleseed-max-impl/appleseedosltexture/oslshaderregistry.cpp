
// Interface header.
#include "oslshaderregistry.h"

// appleseed-max headers.
#include "genericosltexture.h"

// 3ds Max headers.
#include <iparamb2.h>
#include <maxtypes.h>

// Standard headers.
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

    void fill_shader_info()
    {
        int param_id = 0;

        shader_1.m_shader_name = L"OSL_Noise";
        shader_1.m_class_id = Class_ID(0x467936e2, 0x3f6c32f1);
        shader_1.m_internal_name = L"osl_Noise";

        sh1_params.m_param_name = L"p0";
        sh1_params.m_param_type = ShaderInfo::ParamType::Color;
        sh1_params.m_pid = param_id++;
        sh1_params.m_label_str = L"Color Param";

        shader_1.m_params.push_back(&sh1_params);

        sh2_params.m_param_name = L"p1";
        sh2_params.m_param_type = ShaderInfo::ParamType::Float;
        sh2_params.m_pid = param_id++;
        sh2_params.m_label_str = L"Float Param";

        shader_1.m_params.push_back(&sh2_params);

        param_id = 0;

        shader_2.m_shader_name = L"OSL_Ramp";
        shader_2.m_class_id = Class_ID(0x1aaa49a9, 0x7539328e);
        shader_2.m_internal_name = L"osl_Ramp";

        sh4_params.m_param_name = L"p1";
        sh4_params.m_param_type = ShaderInfo::ParamType::Float;
        sh4_params.m_pid = param_id++;
        sh4_params.m_label_str = L"Float Param";

        shader_2.m_params.push_back(&sh4_params);

        sh3_params.m_param_name = L"p0";
        sh3_params.m_param_type = ShaderInfo::ParamType::Color;
        sh3_params.m_pid = param_id++;
        sh3_params.m_label_str = L"Color Param";

        shader_2.m_params.push_back(&sh3_params);
    }
}

void OSLShaderRegistry::create_class_descriptors()
{
    m_shaders.push_back(&shader_1);
    m_shaders.push_back(&shader_2);

    for (auto sh : m_shaders)
    {
        ClassDesc2* class_descr(new GenericOSLTextureClassDesc(sh));
        ParamBlockDesc2* param_block_descr(new ParamBlockDesc2(
            // --- Required arguments ---
            0,                                          // parameter block's ID
            L"oslTextureMapParams",                     // internal parameter block's name
            0,                                          // ID of the localized name string
            class_descr,                                // class descriptor
            P_AUTO_CONSTRUCT,                           // block flags

                                                        // --- P_AUTO_CONSTRUCT arguments ---
            0,                                          // parameter block's reference number
            p_end
        ));

        int ctrl_id = 100;
        for (auto param_info : sh->m_params)
        {
            add_parameter(param_block_descr, sh, param_info, ctrl_id);
        }

        m_paramblock_descriptors.push_back(MaxSDK::AutoPtr<ParamBlockDesc2>(param_block_descr));
        m_class_descriptors.push_back(MaxSDK::AutoPtr<ClassDesc2>(class_descr));
    }
}


void OSLShaderRegistry::add_parameter(ParamBlockDesc2* pb_desc, ShaderInfo* shader_info, ShaderInfo::ParamInfo* param_info, int& ctrl_id)
{
    /*supports following controls

    TYPE_SPINNER,
    TYPE_RADIO,
    TYPE_SINGLECHEKBOX,
    TYPE_COLORSWATCH,
    TYPE_EDITBOX
    TYPE_CHECKBUTTON,
    TYPE_TEXMAPBUTTON,
    TYPE_MTLBUTTON,
    TYPE_SLIDER,
    TYPE_COLORSWATCH_FRGBA,
    TYPE_INT_COMBOBOX,
    */

    param_info->m_label_res_id = ctrl_id;
    shader_info->m_label_map.insert(std::make_pair(param_info->m_label_res_id, param_info));
    
    param_info->m_ctrl_res_id = ctrl_id++;

    int param_1 = ctrl_id++;
    int param_2 = ctrl_id++;
    int param_3 = ctrl_id++;
    int param_4 = ctrl_id++;
    int param_5 = ctrl_id++;
    int param_6 = ctrl_id++;

    switch (param_info->m_param_type)
    {
    case ShaderInfo::ParamType::Float:
        pb_desc->AddParam(
            param_info->m_pid,           // Parameter ID. We are defining the first parameter here
            param_info->m_param_name,    // Internal name of the parameter
            TYPE_FLOAT,                 // Parameter Type. It will be a float parameter
            P_ANIMATABLE,               // A constant defined in iparamb2.h. Indicates that the parameter is animatable
            param_info->m_label_res_id,  // string table id, e.g. IDS_BASE_COLOR
            p_ui, TYPE_SPINNER, EDITTYPE_FLOAT, param_1, param_2, 1.0f,
            p_end                       // End of the second parameter definition. 'end' is an enumerated value defined in
        );
        break;
    case ShaderInfo::ParamType::Int:
        pb_desc->AddParam(
            param_info->m_pid,
            param_info->m_param_name,
            TYPE_INT,
            P_ANIMATABLE,
            param_info->m_label_res_id,
            p_ui, TYPE_SPINNER, EDITTYPE_INT, param_1, param_2, 1,
            p_end
        );
        break;
    case ShaderInfo::ParamType::Color:
        pb_desc->AddParam(
            param_info->m_pid,
            param_info->m_param_name,
            TYPE_RGBA,
            P_ANIMATABLE,
            param_info->m_label_res_id,
            p_ui, TYPE_COLORSWATCH, param_1,
            p_end
        );
        break;
    case ShaderInfo::ParamType::ColorAlpha:
        pb_desc->AddParam(
            param_info->m_pid,
            param_info->m_param_name,
            TYPE_RGBA,
            P_ANIMATABLE,
            param_info->m_label_res_id,
            p_ui, TYPE_COLORSWATCH, param_1,
            p_end
        );
        break;
    case ShaderInfo::ParamType::TextureColor:
        pb_desc->AddParam(
            param_info->m_pid,
            param_info->m_param_name,
            TYPE_TEXMAP,
            P_ANIMATABLE,
            param_info->m_label_res_id,
            p_ui, TYPE_TEXMAPBUTTON, param_1,
            p_end
        );
        break;
    case ShaderInfo::ParamType::Point:
        pb_desc->AddParam(
            param_info->m_pid,
            param_info->m_param_name,
            TYPE_POINT3,
            P_ANIMATABLE,
            param_info->m_label_res_id,
            p_ui, TYPE_SPINNER, EDITTYPE_FLOAT, param_1, param_2, param_3, param_4, param_5, param_6, 10.0f,
            p_end
        );
        break;
    case ShaderInfo::ParamType::String:
        pb_desc->AddParam(
            param_info->m_pid,
            param_info->m_param_name,
            TYPE_STRING,
            0,
            param_info->m_label_res_id,
            p_ui, TYPE_EDITBOX, param_1,
            p_end
        );
        break;
    default:
        DbgAssert(false);
        break;
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
