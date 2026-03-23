#!/bin/bash
set -e
SCRIPT_DIR=$(dirname ${BASH_SOURCE})
if [ ! -z "$OMNI_KIT_LD_PRELOAD" ]; then
    export LD_PRELOAD="$OMNI_KIT_LD_PRELOAD:$LD_PRELOAD"
fi
${EXEC:-exec} "$SCRIPT_DIR//kit" "$SCRIPT_DIR/apps/omni.app.hydra.kit"  "$@"
