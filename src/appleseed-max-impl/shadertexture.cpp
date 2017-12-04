

// Interface header.
#include "shadertexture.h"

// appleseed-max headers.
#include "main.h"
#include "utilities.h"
#include "version.h"

namespace asf = foundation;
namespace asr = renderer;

namespace
{
    const wchar_t* AppleseedShaderTextureFriendlyClassName = L"appleseed Sky";
}

//
// AppleseedShaderTexture class implementation.
//

namespace
{
    enum { ParamBlockIdEnvMap };
    enum { ParamBlockRefEnvMap };

    enum ParamId
    {
        ParamIdSunTheta = 0,
        ParamIdSunPhi = 1,
        ParamIdTurbidity = 2,
        ParamIdTurbidityMap = 3,
        ParamIdTurbidityMapOn = 4,
        ParamIdTurbMultiplier = 5,
        ParamIdLuminMultiplier = 6,
        ParamIdLuminGamma = 7,
        ParamIdSatMultiplier = 8,
        ParamIdHorizonShift = 10,
        ParamIdGroundAlbedo = 11,
        ParamIdSunNode = 12,
        ParamIdSunNodeOn = 13,
        ParamIdSunSizeMultiplier = 14
    };

    enum TexmapId
    {
        TexmapIdTurbidity = 0,
        TexmapCount                 // keep last
    };

    const MSTR g_texmap_slot_names[TexmapCount] =
    {
        L"Turbidity",
    };

    const ParamId g_texmap_id_to_param_id[TexmapCount] =
    {
        ParamIdTurbidityMap
    };

    ParamBlockDesc2 g_block_desc(
        // --- Required arguments ---
        ParamBlockIdEnvMap,                         // parameter block's ID
        L"appleseedEnvironmentMapParams",           // internal parameter block's name
        0,                                          // ID of the localized name string
        &g_appleseed_envmap_classdesc,              // class descriptor
        P_AUTO_CONSTRUCT + P_AUTO_UI,               // block flags

                                                    // --- P_AUTO_CONSTRUCT arguments ---
        ParamBlockRefEnvMap,                        // parameter block's reference number

                                                    // --- P_AUTO_UI arguments for Parameters rollup ---
        IDD_ENVMAP_PANEL,                           // ID of the dialog template
        IDS_ENVMAP_PARAMS,                          // ID of the dialog's title string
        0,                                          // IParamMap2 creation/deletion flag mask
        0,                                          // rollup creation flag
        nullptr,

        // --- Parameters specifications for Parameters rollup ---
        ParamIdSunTheta, L"sun_theta", TYPE_FLOAT, P_ANIMATABLE, IDS_THETA,
        p_default, 45.0f,
        p_range, 0.0f, 180.0f,
        p_ui, TYPE_SPINNER, EDITTYPE_FLOAT, IDC_EDIT_THETA, IDC_SPIN_THETA, 0.01f,
        p_end,

        ParamIdSunPhi, L"sun_phi", TYPE_FLOAT, P_ANIMATABLE, IDS_PHI,
        p_default, 0.0f,
        p_range, 0.0f, 360.0f,
        p_ui, TYPE_SPINNER, EDITTYPE_FLOAT, IDC_EDIT_PHI, IDC_SPIN_PHI, 0.01f,
        p_end,

        ParamIdSunSizeMultiplier, L"sun_size_multiplier", TYPE_FLOAT, P_ANIMATABLE, IDS_SIZE_MULTIPLIER,
        p_default, 1.0f,
        p_range, 0.0f, 1000.0f,
        p_ui, TYPE_SPINNER, EDITTYPE_FLOAT, IDC_EDIT_SIZE_MULTIPLIER, IDC_SPIN_SIZE_MULTIPLIER, 0.1f,
        p_end,

        ParamIdSunNode, L"sun_node", TYPE_INODE, 0, IDS_SUN_NODE,
        p_ui, TYPE_PICKNODEBUTTON, IDC_PICK_SUN_NODE,
        p_prompt, IDS_PICK_SUN_PROMPT,
        p_validator, &g_sun_node_validator,
        p_accessor, &g_sun_node_accessor,
        p_end,

        ParamIdSunNodeOn, L"sun_node_on", TYPE_BOOL, 0, IDS_SUN_NODE_ON,
        p_default, TRUE,
        p_ui, TYPE_SINGLECHEKBOX, IDC_SUN_NODE_ON,
        p_accessor, &g_sun_node_accessor,
        p_end,

        ParamIdTurbidity, L"turbidity", TYPE_FLOAT, P_ANIMATABLE, IDS_TURBIDITY,
        p_default, 1.0f,
        p_range, 0.0f, 1.0f,
        p_ui, TYPE_SPINNER, EDITTYPE_FLOAT, IDC_EDIT_TURBIDITY, IDC_SPIN_TURBIDITY, 0.01f,
        p_end,

        ParamIdTurbidityMap, L"turbidity_map", TYPE_TEXMAP, 0, IDS_TURB_MAP,
        p_subtexno, TexmapIdTurbidity,
        p_ui, TYPE_TEXMAPBUTTON, IDC_PICK_TURB_TEXTURE,
        p_end,

        ParamIdTurbidityMapOn, L"turbidity_map_on", TYPE_BOOL, 0, IDS_TURB_MAP_ON,
        p_default, TRUE,
        p_ui, TYPE_SINGLECHEKBOX, IDC_TURB_TEX_ON,
        p_end,

        ParamIdTurbMultiplier, L"turbidity_multiplier", TYPE_FLOAT, P_ANIMATABLE, IDS_TURB_MULTIPLIER,
        p_default, 2.0f,
        p_range, 0.0f, 8.0f,
        p_ui, TYPE_SPINNER, EDITTYPE_FLOAT, IDC_EDIT_TURB_MULTIPLIER, IDC_SPIN_TURB_MULTIPLIER, 0.01f,
        p_end,

        ParamIdLuminMultiplier, L"luminance_multiplier", TYPE_FLOAT, P_ANIMATABLE, IDS_LUMINANCE_MULTIPLIER,
        p_default, 1.0f,
        p_range, 0.0f, 10.0f,
        p_ui, TYPE_SPINNER, EDITTYPE_FLOAT, IDC_EDIT_LUMIN_MULTIPLIER, IDC_SPIN_LUMIN_MULTIPLIER, 0.01f,
        p_end,

        ParamIdLuminGamma, L"luminance_gamma", TYPE_FLOAT, P_ANIMATABLE, IDS_LUMINANCE_GAMMA,
        p_default, 1.0f,
        p_range, 0.0f, 3.0f,
        p_ui, TYPE_SPINNER, EDITTYPE_FLOAT, IDC_EDIT_LUMIN_GAMMA, IDC_SPIN_LUMIN_GAMMA, 0.01f,
        p_end,

        ParamIdSatMultiplier, L"saturation_multiplier", TYPE_FLOAT, P_ANIMATABLE, IDS_SAT_MULITPLIER,
        p_default, 1.0f,
        p_range, 0.0f, 10.0f,
        p_ui, TYPE_SPINNER, EDITTYPE_FLOAT, IDC_EDIT_SATUR_MULTIPLIER, IDC_SPIN_SATUR_MULTIPLIER, 0.01f,
        p_end,

        ParamIdHorizonShift, L"horizon_shift", TYPE_FLOAT, P_ANIMATABLE, IDS_HORIZON_SHIFT,
        p_default, 0.0f,
        p_range, -10.0f, 10.0f,
        p_ui, TYPE_SPINNER, EDITTYPE_FLOAT, IDC_EDIT_HORIZON_SHIFT, IDC_SPIN_HORIZON_SHIFT, 0.01f,
        p_end,

        ParamIdGroundAlbedo, L"ground_albedo", TYPE_FLOAT, P_ANIMATABLE, IDS_GROUND_ALBEDO,
        p_default, 0.3f,
        p_range, 0.0f, 1.0f,
        p_ui, TYPE_SPINNER, EDITTYPE_FLOAT, IDC_EDIT_GROUND_ALBEDO, IDC_SPIN_GROUND_ALBEDO, 0.01f,
        p_end,

        p_end
    );
}

Class_ID AppleseedShaderTexture::get_class_id()
{
    return Class_ID(0x52848b4a, 0x5e6cb361);
}

AppleseedShaderTexture::AppleseedShaderTexture()
    : m_pblock(nullptr)
{
    Reset();
}

void AppleseedShaderTexture::DeleteThis()
{
    delete this;
}

void AppleseedShaderTexture::GetClassName(TSTR& s)
{
    s = L"appleseedSky";
}

SClass_ID AppleseedShaderTexture::SuperClassID()
{
    return TEXMAP_CLASS_ID;
}

Class_ID AppleseedShaderTexture::ClassID()
{
    return get_class_id();
}

int AppleseedShaderTexture::NumSubs()
{
    return 1;   // pblock
}

Animatable* AppleseedShaderTexture::SubAnim(int i)
{
    switch (i)
    {
    case 0: return m_pblock;
    default: return nullptr;
    }
}

TSTR AppleseedShaderTexture::SubAnimName(int i)
{
    switch (i)
    {
    case 0: return L"Parameters";
    default: return nullptr;
    }
}

int AppleseedShaderTexture::SubNumToRefNum(int subNum)
{
    return subNum;
}

int AppleseedShaderTexture::NumParamBlocks()
{
    return 1;
}

IParamBlock2* AppleseedShaderTexture::GetParamBlock(int i)
{
    return i == 0 ? m_pblock : nullptr;
}

IParamBlock2* AppleseedShaderTexture::GetParamBlockByID(BlockID id)
{
    return id == m_pblock->ID() ? m_pblock : nullptr;
}

int AppleseedShaderTexture::NumRefs()
{
    return 1;   // pblock
}

RefTargetHandle AppleseedShaderTexture::GetReference(int i)
{
    switch (i)
    {
    case ParamBlockIdEnvMap: return m_pblock;
    default: return nullptr;
    }
}

void AppleseedShaderTexture::SetReference(int i, RefTargetHandle rtarg)
{
    switch (i)
    {
    case ParamBlockIdEnvMap: m_pblock = (IParamBlock2 *)rtarg; break;
    }
}

RefResult AppleseedShaderTexture::NotifyRefChanged(
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
            g_block_desc.InvalidateUI(m_pblock->LastNotifyParamID());
        break;
    }

    return REF_SUCCEED;
}

RefTargetHandle AppleseedShaderTexture::Clone(RemapDir& remap)
{
    AppleseedShaderTexture* mnew = new AppleseedShaderTexture();
    *static_cast<MtlBase*>(mnew) = *static_cast<MtlBase*>(this);
    BaseClone(this, mnew, remap);

    mnew->ReplaceReference(0, remap.CloneRef(m_pblock));

    return (RefTargetHandle)mnew;
}

int AppleseedShaderTexture::NumSubTexmaps()
{
    return TexmapCount;
}

Texmap* AppleseedShaderTexture::GetSubTexmap(int i)
{
    Texmap* texmap = nullptr;
    Interval iv;
    if (i == 0)
    {
        m_pblock->GetValue(ParamIdTurbidityMap, 0, texmap, iv);
    }
    return texmap;
}

void AppleseedShaderTexture::SetSubTexmap(int i, Texmap* texmap)
{
    if (i == 0)
    {
        const auto texmap_id = static_cast<TexmapId>(i);
        const auto param_id = g_texmap_id_to_param_id[texmap_id];
        m_pblock->SetValue(param_id, 0, texmap);
        g_block_desc.InvalidateUI(param_id);
    }
}

int AppleseedShaderTexture::MapSlotType(int i)
{
    return MAPSLOT_TEXTURE;
}

TSTR AppleseedShaderTexture::GetSubTexmapSlotName(int i)
{
    const auto texmap_id = static_cast<TexmapId>(i);
    return g_texmap_slot_names[texmap_id];
}

void AppleseedShaderTexture::Update(TimeValue t, Interval& valid)
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

void AppleseedShaderTexture::Reset()
{
    m_params_validity.SetEmpty();
}

Interval AppleseedShaderTexture::Validity(TimeValue t)
{
    Interval valid = FOREVER;
    Update(t, valid);
    return valid;
}

ParamDlg* AppleseedShaderTexture::CreateParamDlg(HWND hwMtlEdit, IMtlParams* imp)
{
    IAutoMParamDlg* master_dlg = g_appleseed_envmap_classdesc.CreateParamDlgs(hwMtlEdit, imp, this);
    g_block_desc.SetUserDlgProc(new EnvMapParamMapDlgProc());
    return master_dlg;
}

IOResult AppleseedShaderTexture::Save(ISave* isave)
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

IOResult AppleseedShaderTexture::Load(ILoad* iload)
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

AColor AppleseedShaderTexture::EvalColor(ShadeContext& sc)
{
    const Color base_color(0.13f, 0.58f, 1.0f);
    return base_color;
}

Point3 AppleseedShaderTexture::EvalNormalPerturb(ShadeContext& /*sc*/)
{
    return Point3(0.0f, 0.0f, 0.0f);
}
