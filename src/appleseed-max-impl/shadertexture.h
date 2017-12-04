#pragma once


// appleseed.foundation headers.
#include "foundation/platform/windows.h"    // include before 3ds Max headers
#include "foundation/utility/autoreleaseptr.h"

// 3ds Max headers.
#include <imtl.h>
#include <iparamb2.h>
#include <iparamm2.h>
#include <istdplug.h>
#include <maxtypes.h>
#include <stdmat.h>
#undef base_type


class AppleseedShaderTexture
    : public Texmap
{
public:
    static Class_ID get_class_id();

    // Constructor.
    AppleseedShaderTexture();

    // Animatable methods.
    virtual void DeleteThis() override;
    virtual void GetClassName(TSTR& s) override;
    virtual SClass_ID SuperClassID() override;
    virtual Class_ID  ClassID() override;
    virtual int NumSubs() override;
    virtual Animatable* SubAnim(int i) override;
    virtual TSTR SubAnimName(int i) override;
    virtual int SubNumToRefNum(int subNum) override;
    virtual int NumParamBlocks() override;
    virtual IParamBlock2* GetParamBlock(int i) override;
    virtual IParamBlock2* GetParamBlockByID(BlockID id) override;

    // ReferenceMaker methods.
    virtual int NumRefs() override;
    virtual RefTargetHandle GetReference(int i) override;
    virtual RefResult NotifyRefChanged(
        const Interval&     changeInt,
        RefTargetHandle     hTarget,
        PartID&             partID,
        RefMessage          message,
        BOOL                propagate) override;

    // ReferenceTarget methods.
    virtual RefTargetHandle Clone(RemapDir& remap) override;

    // ISubMap methods.
    virtual int NumSubTexmaps() override;
    virtual Texmap* GetSubTexmap(int i) override;
    virtual void SetSubTexmap(int i, Texmap* texmap) override;
    virtual int MapSlotType(int i) override;
    virtual MSTR GetSubTexmapSlotName(int i) override;

    // MtlBase methods.
    virtual void Update(TimeValue t, Interval& valid) override;
    virtual void Reset() override;
    virtual Interval Validity(TimeValue t) override;
    virtual ParamDlg* CreateParamDlg(HWND hwMtlEdit, IMtlParams* imp) override;
    virtual IOResult Save(ISave* isave) override;
    virtual IOResult Load(ILoad* iload) override;

    // Texmap methods.
    virtual RGBA EvalColor(ShadeContext& sc) override;
    virtual Point3 EvalNormalPerturb(ShadeContext& sc) override;

protected:
    virtual void SetReference(int i, RefTargetHandle rtarg) override;

private:
    IParamBlock2*   m_pblock;          // ref 0
};

