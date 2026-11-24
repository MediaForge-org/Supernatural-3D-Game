"""Read-only: documents the Python API for skeletons and IK retargeting of this engine version (0013 retarget preparation). Changes nothing."""

import json
import os

import unreal

OUT = os.path.join(unreal.Paths.project_saved_dir(), "SpnCharacter", "ikrig_api.json")
api = {}
for name in ("IKRigController", "IKRetargeterController", "IKRetargetBatchOperation", "IKRigDefinition", "IKRetargeter",
             "Skeleton", "SkeletalMesh", "SkeletonModifier", "SkeletalMeshEditorSubsystem", "AnimationLibrary", "SkeletalMeshSocket"):
    cls = getattr(unreal, name, None)
    api[name] = sorted(m for m in dir(cls) if not m.startswith("_")) if cls is not None else None
docs = {}
for owner, member in (("IKRetargetBatchOperation", "duplicate_and_retarget"), ("IKRetargetBatchOperation", "run_batch_retarget"),
                      ("IKRetargeterController", "set_ik_rig"), ("IKRetargeterController", "auto_map_chains"),
                      ("IKRetargeterController", "add_default_ops"), ("IKRetargeterController", "set_preview_mesh"),
                      ("IKRigController", "apply_auto_generated_retarget_definition"), ("IKRigController", "apply_auto_fbik")):
    cls = getattr(unreal, owner, None)
    fn = getattr(cls, member, None) if cls is not None else None
    docs["%s.%s" % (owner, member)] = fn.__doc__ if fn is not None else None
for type_name in ("IKRetargetBatchOperationContext", "RetargetSourceOrTarget", "AutoMapChainType", "IKRigDefinitionFactory", "IKRetargetFactory"):
    t = getattr(unreal, type_name, None)
    docs[type_name] = (t.__doc__ or "") + " | members: " + ", ".join(m for m in dir(t) if not m.startswith("_"))[:1500] if t is not None else None
api["_docs"] = docs
os.makedirs(os.path.dirname(OUT), exist_ok=True)
with open(OUT, "w") as f:
    json.dump(api, f, indent=1)
unreal.log("SPN api: wrote %s" % OUT)
