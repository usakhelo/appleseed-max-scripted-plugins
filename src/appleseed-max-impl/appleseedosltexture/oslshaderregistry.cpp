
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
}

void OSLShaderRegistry::instanciate_shader_plugins()
{
    shader_1.m_shader_name = L"shader_1";
    shader_1.m_class_id = Class_ID(0x467936e2, 0x3f6c32f1);

    shader_2.m_shader_name = L"shader_2";
    shader_2.m_class_id = Class_ID(0x1aaa49a9, 0x7539328e);

    m_shaders.push_back(shader_1);
    m_shaders.push_back(shader_2);


    for (auto sh : m_shaders)
    {
        MaxSDK::AutoPtr<ClassDesc2> class_descr(new GenericOSLTextureClassDesc(&sh));
        MaxSDK::AutoPtr<ParamBlockDesc2> param_block_descr(new ParamBlockDesc2(
        //auto param_block_descr = new ParamBlockDesc2(
                // --- Required arguments ---
                0,                                          // parameter block's ID
                L"oslTextureMapParams",                     // internal parameter block's name
                0,                                          // ID of the localized name string
                class_descr.Get(),                          // class descriptor
                P_AUTO_CONSTRUCT,                           // block flags

                                                            // --- P_AUTO_CONSTRUCT arguments ---
                0,                                          // parameter block's reference number

                //0,                   /* Parameter ID. We are defining the first parameter here */
                //L"p0",              /* Internal name of the parameter */
                //TYPE_FLOAT,          /* Parameter Type. It will be a float parameter */
                //P_ANIMATABLE,        /* A constant defined in iparamb2.h. Indicates that the parameter is animatable.*/
                //p_end,               /* End of the first parameter definition.*/
                //1,                   /* Parameter ID. This will be the second parameter*/
                //L"p1",                  /* Internal name of the parameter*/
                //TYPE_FLOAT,          /* Parameter Type. It will be a float parameter*/
                //P_ANIMATABLE,        /* A constant defined in iparamb2.h. Indicates that the parameter is animatable.*/
                //p_end,               /* End of the second parameter definition. 'end' is an enumerated value defined in
                                     /* the 'ParamTags' enumerator.*/

            p_end
            ));

        //param_block_descr->SetClassDesc(class_descr.Get());
        param_block_descr->AddParam(
            0,                   /* Parameter ID. We are defining the first parameter here */
            _M("p0"),              /* Internal name of the parameter */
            TYPE_FLOAT,          /* Parameter Type. It will be a float parameter */
            P_ANIMATABLE,        /* A constant defined in iparamb2.h. Indicates that the parameter is animatable.*/
            0,                  //string table id, e.g. IDS_BASE_COLOR
            p_end               /* End of the first parameter definition.*/
        );
        m_paramblock_descriptors.push_back(MaxSDK::AutoPtr<ParamBlockDesc2>(param_block_descr));
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
    instanciate_shader_plugins();
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
