
#include "shaderscriptedplugins.h"

#include "maxscript\maxscript.h"

wchar_t* test_script = L"plugin material myGlass\n"
L"name:\"appleseed OSL Material\"\n"
L"classID: #(695425, 446581)\n"
L"version: 1\n"
L"(\n"
L"parameters main rollout: params\n"
L"(\n"
L"trans type: #float default:27 ui: trans\n"
L"refrac type: #float default:1.5 ui: refrac\n"
L"col type: #color default:blue ui: col\n"
L")\n"
L"rollout params \"Glass Parameters\"\n"
L"(\n"
L"spinner trans \"Transparency: \" fieldwidth:45 offset: [-90, 0]\n"
L"spinner refrac \"Index of Refraction: \" fieldwidth: 45 offset: [-90, 0]\n"
L"colorpicker col \"Base color: \" align: #center\n"
L")\n"
L"on create do\n"
L"(\n"
L")\n"
L")\n";


void registerShaderPlugins()
{
    BOOL result = ExecuteMAXScriptScript(test_script, FALSE);
}
