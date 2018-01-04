
// Interface header.
#include "genericosltexture.h"

// appleseed-max headers.
#include "appleseedrenderer/appleseedrenderer.h"
#include "main.h"
#include "templategenerator.h"
#include "utilities.h"
#include "version.h"

// 3ds Max headers.
#include <iparamm2.h>
#include <maxscript\maxscript.h>
#include <maxscript\util\listener.h>

namespace asf = foundation;
namespace asr = renderer;

//
// Changing the values of these constants WILL break compatibility
// with 3ds Max files saved with older versions of the plugin.
//

const USHORT ChunkFileFormatVersion = 0x0001;

const USHORT ChunkMtlBase = 0x1000;

namespace
{
    const wchar_t* GenericOSLTextureFriendlyClassName = L"appleseed osl texture";
}


namespace
{
    class OSLTextureParamDlg : public ParamDlg 
    {
      public:


        OSLTextureParamDlg(HWND hwMtlEdit, IMtlParams* imp, GenericOSLTexture* map, ShaderInfo* shader_info)
            : m_hmedit(hwMtlEdit)
            , m_imp(imp)
            , m_texture(map)
            , m_shader_info(shader_info)
            , m_y_pos(6)
        {
            m_class_id = m_texture->ClassID();
            create_dialog();
        }

        Class_ID ClassID() override
        { 
            return m_class_id;
        }

        ReferenceTarget* GetThing() override
        { 
            return m_texture;
        }

        void SetThing(ReferenceTarget *m) override
        {
            assert(m->ClassID() == m_texture->ClassID());
            m_texture = static_cast<GenericOSLTexture*>(m);
            m_pmap->SetParamBlock(m_texture->m_pblock);
        }

        void DeleteThis() override
        {
            if (m_pmap) DestroyMParamMap2(m_pmap);
            m_pmap = NULL;
            delete this;
        }

        void SetTime(TimeValue t) {}
        void ReloadDialog(void) {}
        void ActivateDlg(BOOL onOff) {}
    
    private:
        void add_ui_parameter(DialogTemplate& dialog_template, ShaderInfo::ParamInfo* param_info)
        {
            switch (param_info->m_param_type)
            {
            case ShaderInfo::ParamType::Float:
            {
                dialog_template.AddStatic((LPCSTR)"Float:", WS_VISIBLE, NULL, 7, m_y_pos, 48, 8, param_info->m_ctrl_res_id);
                dialog_template.AddComponent((LPCSTR)"CustEdit", (LPCSTR)"Parameter Edit", WS_VISIBLE, NULL, 85, m_y_pos, 35, 10, param_info->m_ctrl_res_id+1);
                dialog_template.AddComponent((LPCSTR)"SpinnerControl", (LPCSTR)"Parameter Spinner", WS_VISIBLE, NULL, 121, m_y_pos, 7, 10, param_info->m_ctrl_res_id+2);
            }
            break;
            case ShaderInfo::ParamType::Int:
                break;
            case ShaderInfo::ParamType::Color:
                dialog_template.AddStatic((LPCSTR)"Color:", WS_VISIBLE, NULL, 7, m_y_pos, 48, 8, param_info->m_ctrl_res_id);
                dialog_template.AddComponent((LPCSTR)"ColorSwatch", (LPCSTR)"Color Swatch", WS_VISIBLE, NULL, 85, m_y_pos, 30, 10, param_info->m_ctrl_res_id + 1);
                break;
            case ShaderInfo::ParamType::ColorAlpha:
                break;
            case ShaderInfo::ParamType::TextureColor:
                break;
            case ShaderInfo::ParamType::Point:
                break;
            case ShaderInfo::ParamType::String:
                break;
            default:
                DbgAssert(false);
                break;
            }

            m_y_pos += 12;
        }

        void create_dialog()
        {
            DialogTemplate dialogTemplate((LPCSTR)"OSL Texture", DS_SETFONT | WS_CHILD | WS_VISIBLE, 0, 0, 217, 80, (LPCSTR)"MS Sans Serif", 8);
            
            for (auto param_info : m_shader_info->m_params)
            {
                add_ui_parameter(dialogTemplate, param_info);
            }
            m_pmap = CreateMParamMap2(m_texture->m_pblock, m_imp, g_module, m_hmedit, nullptr, nullptr, (DLGTEMPLATE*)dialogTemplate, L"Header Title", 0);
        }

        int                 m_y_pos;
        GenericOSLTexture*  m_texture;
        HWND  	            m_hmedit;
        IMtlParams*         m_imp;
        IParamMap2*         m_pmap;
        ShaderInfo*         m_shader_info;
        Class_ID            m_class_id;
    };
}

//
// GenericOSLTexture class implementation.
//

Class_ID GenericOSLTexture::get_class_id()
{
    return m_classid;
}

GenericOSLTexture::GenericOSLTexture(Class_ID class_id, ClassDesc2* class_desc)
    : m_pblock(nullptr)
    , m_classid(class_id)
    , m_class_desc(class_desc)
{
    m_class_desc->MakeAutoParamBlocks(this);
    Reset();
}

void GenericOSLTexture::DeleteThis()
{
    delete this;
}

void GenericOSLTexture::GetClassName(TSTR& s)
{
    s = L"GenericOSLTexture";
}

SClass_ID GenericOSLTexture::SuperClassID()
{
    return TEXMAP_CLASS_ID;
}

Class_ID GenericOSLTexture::ClassID()
{
    return get_class_id();
}

int GenericOSLTexture::NumSubs()
{
    return 1;   // pblock
}

Animatable* GenericOSLTexture::SubAnim(int i)
{
    switch (i)
    {
      case 0:
        return m_pblock;
      default:
        return nullptr;
    }
}

TSTR GenericOSLTexture::SubAnimName(int i)
{
    switch (i)
    {
      case 0:
        return L"Parameters";
      default:
        return nullptr;
    }
}

int GenericOSLTexture::SubNumToRefNum(int subNum)
{
    return subNum;
}

int GenericOSLTexture::NumParamBlocks()
{
    return 1;
}

IParamBlock2* GenericOSLTexture::GetParamBlock(int i)
{
    return i == 0 ? m_pblock : nullptr;
}

IParamBlock2* GenericOSLTexture::GetParamBlockByID(BlockID id)
{
    return id == m_pblock->ID() ? m_pblock : nullptr;
}

int GenericOSLTexture::NumRefs()
{
    return 1;   // pblock
}

RefTargetHandle GenericOSLTexture::GetReference(int i)
{
    switch (i)
    {
    case 0: return m_pblock;
    default: return nullptr;
    }
}

void GenericOSLTexture::SetReference(int i, RefTargetHandle rtarg)
{
    switch (i)
    {
      case 0:
        m_pblock = (IParamBlock2 *)rtarg;
        break;
    }
}

RefResult GenericOSLTexture::NotifyRefChanged(
    const Interval&   /*changeInt*/,
    RefTargetHandle   hTarget,
    PartID&           partID,
    RefMessage        message,
    BOOL              /*propagate*/)
{
    switch (message)
    {
      case REFMSG_TARGET_DELETED:
        if (hTarget == m_pblock)
            m_pblock = nullptr;
        break;

      case REFMSG_CHANGE:
        if (hTarget == m_pblock)
            m_class_desc->GetParamBlockDesc(0)->InvalidateUI(m_pblock->LastNotifyParamID());
        break;
    }

    return REF_SUCCEED;
}

RefTargetHandle GenericOSLTexture::Clone(RemapDir& remap)
{
    GenericOSLTexture* mnew = new GenericOSLTexture(m_classid, m_class_desc);
    *static_cast<MtlBase*>(mnew) = *static_cast<MtlBase*>(this);
    BaseClone(this, mnew, remap);

    mnew->ReplaceReference(0, remap.CloneRef(m_pblock));

    return (RefTargetHandle)mnew;
}

int GenericOSLTexture::NumSubTexmaps()
{
    return 0;
}

Texmap* GenericOSLTexture::GetSubTexmap(int i)
{
    Texmap* texmap = nullptr;
    const auto param_id = 0;
    Interval iv;
    if (i == 0)
    {
        m_pblock->GetValue(param_id, 0, texmap, iv);
    }
    return nullptr;
}

void GenericOSLTexture::SetSubTexmap(int i, Texmap* texmap)
{
    if (i == 0)
    {
        const auto param_id = 0;
        m_pblock->SetValue(param_id, 0, texmap);
        m_class_desc->GetParamBlockDesc(0)->InvalidateUI(param_id);
    }
}

int GenericOSLTexture::MapSlotType(int i)
{
    return MAPSLOT_TEXTURE;
}

TSTR GenericOSLTexture::GetSubTexmapSlotName(int i)
{
    return L"osl texture input";
}

void GenericOSLTexture::Update(TimeValue t, Interval& valid)
{
    if (!m_params_validity.InInterval(t))
    {
        NotifyDependents(FOREVER, PART_ALL, REFMSG_CHANGE);
    }
    m_params_validity.SetInfinite();

    //if (m_turbidity_map)
    //    m_turbidity_map->Update(t, m_params_validity);

    valid = m_params_validity;
}

void GenericOSLTexture::Reset()
{
    m_params_validity.SetEmpty();
}

Interval GenericOSLTexture::Validity(TimeValue t)
{
    Interval valid = FOREVER;
    Update(t, valid);
    return valid;
}

ParamDlg* GenericOSLTexture::CreateParamDlg(HWND hwMtlEdit, IMtlParams* imp)
{
    ParamDlg* master_dlg = new OSLTextureParamDlg(hwMtlEdit, imp, this, static_cast<GenericOSLTextureClassDesc*>(m_class_desc)->m_shader_info);
    return master_dlg;
}

IOResult GenericOSLTexture::Save(ISave* isave)
{
    bool success = true;

    isave->BeginChunk(ChunkFileFormatVersion);
    success &= write(isave, &FileFormatVersion, sizeof(FileFormatVersion));
    isave->EndChunk();

    isave->BeginChunk(ChunkMtlBase);
    success &= MtlBase::Save(isave) == IO_OK;
    isave->EndChunk();

    return success ? IO_OK : IO_ERROR;
}

IOResult GenericOSLTexture::Load(ILoad* iload)
{
    IOResult result = IO_OK;

    while (true)
    {
        result = iload->OpenChunk();
        if (result == IO_END)
            return IO_OK;
        if (result != IO_OK)
            break;

        switch (iload->CurChunkID())
        {
        case ChunkFileFormatVersion:
        {
            USHORT version;
            result = read<USHORT>(iload, &version);
        }
        break;

        case ChunkMtlBase:
            result = MtlBase::Load(iload);
            break;
        }

        if (result != IO_OK)
            break;

        result = iload->CloseChunk();
        if (result != IO_OK)
            break;
    }

    return result;
}

AColor GenericOSLTexture::EvalColor(ShadeContext& sc)
{
    const Color base_color(0.13f, 0.58f, 1.0f);
    return base_color;
}

Point3 GenericOSLTexture::EvalNormalPerturb(ShadeContext& /*sc*/)
{
    return Point3(0.0f, 0.0f, 0.0f);
}

//
// GenericOSLTextureBrowserEntryInfo class implementation.
//

const MCHAR* GenericOSLTextureBrowserEntryInfo::GetEntryName() const
{
    return L"tututu";
}

const MCHAR* GenericOSLTextureBrowserEntryInfo::GetEntryCategory() const
{
    return L"Maps\\appleseed";
}

Bitmap* GenericOSLTextureBrowserEntryInfo::GetEntryThumbnail() const
{
    return nullptr;
}

//
// GenericOSLTextureClassDesc class implementation.
//

GenericOSLTextureClassDesc::GenericOSLTextureClassDesc(ShaderInfo* shader_info, std::map<int, const ShaderInfo::ParamInfo*>* label_map)
    : m_class_name(shader_info->m_shader_name)
    , m_internal_name(shader_info->m_internal_name)
    , m_class_id(shader_info->m_class_id)
    , m_shader_info(shader_info)
    , m_label_map(label_map)
{
    IMtlRender_Compatibility_MtlBase::Init(*this);

    //create_parameter_block_desc();
}

int GenericOSLTextureClassDesc::IsPublic()
{
    return TRUE;
}

void* GenericOSLTextureClassDesc::Create(BOOL loading)
{
    return new GenericOSLTexture(m_shader_info->m_class_id, this);
}

const MCHAR* GenericOSLTextureClassDesc::ClassName()
{
    return m_class_name;
}

SClass_ID GenericOSLTextureClassDesc::SuperClassID()
{
    return TEXMAP_CLASS_ID;
}

Class_ID GenericOSLTextureClassDesc::ClassID()
{
    return m_class_id;
}

const MCHAR* GenericOSLTextureClassDesc::Category()
{
    return L"";
}

const MCHAR* GenericOSLTextureClassDesc::InternalName()
{
    // Parsable name used by MAXScript.
    return m_internal_name;
}

FPInterface* GenericOSLTextureClassDesc::GetInterface(Interface_ID id)
{
    if (id == IMATERIAL_BROWSER_ENTRY_INFO_INTERFACE)
        return &m_browser_entry_info;

    return ClassDesc2::GetInterface(id);
}

HINSTANCE GenericOSLTextureClassDesc::HInstance()
{
    return g_module;
}

const MCHAR* GenericOSLTextureClassDesc::GetRsrcString(INT_PTR id)
{
    const auto it = m_label_map->find(static_cast<int>(id));

    if (it != m_label_map->end())
        return it->second->m_label_str;
    else
        return ClassDesc2::GetRsrcString(id);
}

bool GenericOSLTextureClassDesc::IsCompatibleWithRenderer(ClassDesc& renderer_class_desc)
{
    // Before 3ds Max 2017, Class_ID::operator==() returned an int.
    return renderer_class_desc.ClassID() == AppleseedRenderer::get_class_id() ? true : false;
}
