
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

    std::vector<ShaderInfo*> shaders;
}

void OSLShaderRegistry::instanciate_shader_plugins()
{
    shader_1.m_shader_name = L"shader_1";
    shader_1.m_class_id = Class_ID(0x467936e2, 0x3f6c32f1);

    shader_2.m_shader_name = L"shader_2";
    shader_2.m_class_id = Class_ID(0x1aaa49a9, 0x7539328e);

    shaders.push_back(&shader_1);
    shaders.push_back(&shader_2);


    for (auto sh : shaders)
    {
        auto class_descr = new GenericOSLTextureClassDesc(sh);

        auto param_block_descr = new ParamBlockDesc2(
                // --- Required arguments ---
                0,                                          // parameter block's ID
                L"oslTextureMapParams",                     // internal parameter block's name
                0,                                          // ID of the localized name string
                class_descr,                                // class descriptor
                P_AUTO_CONSTRUCT,                           // block flags

                                                            // --- P_AUTO_CONSTRUCT arguments ---
                0,                                          // parameter block's reference number

                //0,                   // Parameter ID. We are defining the first parameter here
                //_T("p0"),            // Internal name of the parameter
                //TYPE_FLOAT,          // Parameter Type. It will be a float parameter
                //P_ANIMATABLE,        // A constant defined in iparamb2.h. Indicates that the parameter is animatable

            p_end
            );

        class_descr->AddParamBlockDesc(param_block_descr);
        m_class_descriptors.push_back(class_descr);
    }
}

OSLShaderRegistry::OSLShaderRegistry()
{
    m_class_descriptors.clear();
    // parse shaders in the directory
    // create shader_info for each of them
    // create class descriptor for each of shader_info
    instanciate_shader_plugins();
}

ClassDesc2* OSLShaderRegistry::get_class_descriptor(int index)
{
    if (index < m_class_descriptors.size())
        return static_cast<ClassDesc2*>(m_class_descriptors[index]);
    return nullptr;
}

int OSLShaderRegistry::get_size()
{
    return static_cast<int>(m_class_descriptors.size());
}
