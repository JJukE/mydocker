#!/bin/bash
set -e
SCRIPT_DIR=$(dirname ${BASH_SOURCE})
if [ ! -z "$OMNI_KIT_LD_PRELOAD" ]; then
    export LD_PRELOAD="$OMNI_KIT_LD_PRELOAD:$LD_PRELOAD"
fi
${EXEC:-exec} "$SCRIPT_DIR/kit/kit" "$SCRIPT_DIR/apps/omni.isaac.sim.compatibility_check.kit"  "$@"
