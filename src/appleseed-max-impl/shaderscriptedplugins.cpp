
#include "shaderscriptedplugins.h"

#include "genericosltexture.h"

#include <iparamb2.h>
#include <maxtypes.h>

#include <vector>
#include <memory>

namespace
{
    ShaderInfo shader_1;
    ShaderInfo shader_2;

    std::vector<ShaderInfo> shaders;
    std::vector<std::shared_ptr<ParamBlockDesc2>> param_block_descrpts;
    std::vector<std::shared_ptr<GenericOSLTextureClassDesc>> shader_class_descrpts;
}

std::vector<std::shared_ptr<GenericOSLTextureClassDesc>> registerShaderPlugins()
{
    shader_1.m_shader_name = L"shader_1";
    shader_1.m_class_id = Class_ID(0x467936e2, 0x3f6c32f1);

    shader_2.m_shader_name = L"shader_2";
    shader_2.m_class_id = Class_ID(0x1aaa49a9, 0x7539328e);

    shaders.push_back(shader_1);
    shaders.push_back(shader_2);


    for (auto sh : shaders)
    {
        auto class_descr = std::make_shared<GenericOSLTextureClassDesc>(new GenericOSLTextureClassDesc(&sh));

        auto param_block_descr = std::make_shared<ParamBlockDesc2>(
            new ParamBlockDesc2(
                // --- Required arguments ---
                0,                                          // parameter block's ID
                L"appleseedEnvironmentMapParams",           // internal parameter block's name
                0,                                          // ID of the localized name string
                class_descr.get(),                          // class descriptor
                P_AUTO_CONSTRUCT,                           // block flags

                                                            // --- P_AUTO_CONSTRUCT arguments ---
                0                                           // parameter block's reference number
            ));

        class_descr->AddParamBlockDesc(param_block_descr.get());
        shader_class_descrpts.push_back(class_descr);
    }

    return shader_class_descrpts;
}
