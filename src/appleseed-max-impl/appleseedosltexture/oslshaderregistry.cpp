
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

    int numID = 100;
    int ctrlID() 
    { 
        return numID++;
    }
}

void OSLShaderRegistry::instanciate_shader_plugins()
{
    shader_1.m_shader_name = L"shader_1";
    shader_1.m_class_id = Class_ID(0x467936e2, 0x3f6c32f1);
    shader_1.m_internal_name = L"OSL_Noise";

    shader_2.m_shader_name = L"shader_2";
    shader_2.m_class_id = Class_ID(0x1aaa49a9, 0x7539328e);
    shader_2.m_internal_name = L"OSL_Ramp";

    m_shaders.push_back(shader_1);
    m_shaders.push_back(shader_2);


    for (auto sh : m_shaders)
    {
        MaxSDK::AutoPtr<ClassDesc2> class_descr(new GenericOSLTextureClassDesc(&sh));

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

DWORD* DlgTemplateGenerator::generate_template(IParamBlock2* pblock)
{
    //parse pblock or get info on elements from parser
    //build elements, count them, decide size of template buffer
    //build header and allocate buffer
    //fillout header and elements
    //return buffer pointer

    //buffer structure
    //typedef struct {
//    DWORD style; WS_VISIBLE          0x10000000L; WS_CHILD            0x40000000L; DS_SETFONT          0x40L = 0x50000040
//    DWORD dwExtendedStyle; 0x50000040
//    WORD  cdit; 0x0000
//    short x; 0x0000
//    short y; 0x0000
//    short cx; 0x00D9
//    short cy; 0x0060
//    } DLGTEMPLATE, *LPDLGTEMPLATE;
    //menu array. if 0x0000 then no menu.
    //class array if 0x0000 then predefined dialog box class. if 0xFFFF then one additional element that specifies the ordinal value of a predefined system window class.
    //if any other value then array is treated as a control class name array.
    //title array. if 0x0000 then no title.
    //16bits font point size
    //font name array
    //typedef struct {
    //    DWORD style;
    //    DWORD dwExtendedStyle;
    //    short x;
    //    short y;
    //    short cx;
    //    short cy;
    //    WORD  id;
    //} DLGITEMTEMPLATE, *PDLGITEMTEMPLATE;

    auto desc = pblock->GetDesc();
    USHORT count = desc->Count();
    for (int i = 0; i < count; ++i)
    {
        ParamDef& def = desc->GetParamDef(desc->IndextoID(i));
        auto param_type = def.type;
        auto param_id = def.ID;
        auto param_ctrl_type = def.ctrl_type;
        auto param_ctrl_id = def.ctrl_IDs;

    }
    return buf;
}

void DlgTemplateGenerator::release_template()
{
}
