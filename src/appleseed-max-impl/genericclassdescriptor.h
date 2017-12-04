
#pragma once

// appleseed-max headers.
#include "iappleseedmtl.h"

// appleseed.foundation headers.
#include "foundation/platform/windows.h"    // include before 3ds Max headers
#include "foundation/utility/autoreleaseptr.h"

// 3ds Max headers.
#include <color.h>
#include <IMaterialBrowserEntryInfo.h>
#include <IMtlRender_Compatibility.h>
#include <imtl.h>
#include <interval.h>
#include <iparamb2.h>
#include <maxtypes.h>
#include <ref.h>
#include <strbasic.h>
#undef base_type

//
// AppleseedSSSMtl material browser info.
//

class AppleseedShaderBrowserEntryInfo
    : public IMaterialBrowserEntryInfo
{
public:
    virtual const MCHAR* GetEntryName() const override;
    virtual const MCHAR* GetEntryCategory() const override;
    virtual Bitmap* GetEntryThumbnail() const override;
};

//
// AppleseedSSSMtl class descriptor.
//

class AppleseedShaderClassDesc
    : public ClassDesc2
    , public IMtlRender_Compatibility_MtlBase
{
public:
    AppleseedShaderClassDesc();
    virtual int IsPublic() override;
    virtual void* Create(BOOL loading) override;
    virtual const MCHAR* ClassName() override;
    virtual SClass_ID SuperClassID() override;
    virtual Class_ID ClassID() override;
    virtual const MCHAR* Category() override;
    virtual const MCHAR* InternalName() override;
    virtual FPInterface* GetInterface(Interface_ID id) override;
    virtual HINSTANCE HInstance() override;

    // IMtlRender_Compatibility_MtlBase methods.
    virtual bool IsCompatibleWithRenderer(ClassDesc& renderer_class_desc) override;

private:
    AppleseedSSSMtlBrowserEntryInfo m_browser_entry_info;
};
