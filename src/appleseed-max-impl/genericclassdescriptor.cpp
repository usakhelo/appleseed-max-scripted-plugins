

// Interface header.
#include "genericclassdescriptor.h"

// appleseed-max headers.
#include "main.h"
#include "utilities.h"
#include "version.h"

// appleseed.renderer headers.
#include "renderer/api/bsdf.h"
#include "renderer/api/bssrdf.h"
#include "renderer/api/material.h"
#include "renderer/api/scene.h"
#include "renderer/api/utility.h"

// appleseed.foundation headers.
#include "foundation/image/colorspace.h"
#include "foundation/utility/searchpaths.h"

// 3ds Max headers.
#include <AssetManagement/AssetUser.h>
#include <color.h>
#include <iparamm2.h>
#include <stdmat.h>
#include <strclass.h>

// Windows headers.
#include <tchar.h>

namespace asf = foundation;
namespace asr = renderer;

//
// AppleseedShaderBrowserEntryInfo class implementation.
//

const MCHAR* AppleseedShaderBrowserEntryInfo::GetEntryName() const
{
    return AppleseedSSSMtlFriendlyClassName;
}

const MCHAR* AppleseedShaderBrowserEntryInfo::GetEntryCategory() const
{
    return L"Materials\\appleseed";
}

Bitmap* AppleseedShaderBrowserEntryInfo::GetEntryThumbnail() const
{
    // todo: implement.
    return nullptr;
}


//
// AppleseedShaderClassDesc class implementation.
//

AppleseedShaderClassDesc::AppleseedShaderClassDesc()
{
    IMtlRender_Compatibility_MtlBase::Init(*this);
}

int AppleseedShaderClassDesc::IsPublic()
{
    return TRUE;
}

void* AppleseedShaderClassDesc::Create(BOOL loading)
{
    return new AppleseedSSSMtl();
}

const MCHAR* AppleseedShaderClassDesc::ClassName()
{
    return AppleseedSSSMtlFriendlyClassName;
}

SClass_ID AppleseedShaderClassDesc::SuperClassID()
{
    return MATERIAL_CLASS_ID;
}

Class_ID AppleseedShaderClassDesc::ClassID()
{
    return AppleseedSSSMtl::get_class_id();
}

const MCHAR* AppleseedShaderClassDesc::Category()
{
    return L"";
}

const MCHAR* AppleseedShaderClassDesc::InternalName()
{
    // Parsable name used by MAXScript.
    return L"appleseedSSSMtl";
}

FPInterface* AppleseedShaderClassDesc::GetInterface(Interface_ID id)
{
    if (id == IMATERIAL_BROWSER_ENTRY_INFO_INTERFACE)
        return &m_browser_entry_info;

    return ClassDesc2::GetInterface(id);
}

HINSTANCE AppleseedShaderClassDesc::HInstance()
{
    return g_module;
}

bool AppleseedShaderClassDesc::IsCompatibleWithRenderer(ClassDesc& renderer_class_desc)
{
    // Before 3ds Max 2017, Class_ID::operator==() returned an int.
    return renderer_class_desc.ClassID() == AppleseedRenderer::get_class_id() ? true : false;
}
