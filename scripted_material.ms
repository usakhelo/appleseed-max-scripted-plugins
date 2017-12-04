plugin material myGlass
name:"Supa Glass"
classID:#(695425,446581)
extends:Standard replaceUI:true version:1
(
  parameters main rollout:params
  (
    trans type:#float default:27 ui:trans
    refrac type:#float default:1.5 ui:refrac
    col type:#color default:blue ui:col
    tex type:#texturemap ui:tex
  )
  rollout params "Glass Parameters"
  (
    spinner trans "Transparency: " fieldwidth:45 offset:[-90,0]
    spinner refrac "Index of Refraction: " fieldwidth:45 offset:[-90,0]
    colorpicker col "Base color: " align:#center
    mapButton tex "main texture"
  )
  on create do
  (
    -- setup initial material
  )
)