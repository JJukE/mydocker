#!/bin/bash
set -e
SCRIPT_DIR=$(dirname ${BASH_SOURCE})
if [ ! -z "$OMNI_KIT_LD_PRELOAD" ]; then
    export LD_PRELOAD="$OMNI_KIT_LD_PRELOAD:$LD_PRELOAD"
fi
${EXEC:-exec} "$SCRIPT_DIR/kit/kit"  --enable omni.kit.test --enable omni.kit.loop-default --/app/enableStdoutOutput=0 --/exts/omni.kit.test/testExts/0='omni.isaac.sim.compatibility_check' --ext-folder "$SCRIPT_DIR/exts"  --ext-folder "$SCRIPT_DIR/extscache"  --ext-folder "$SCRIPT_DIR/apps"  --/exts/omni.kit.test/testExtOutputPath="$SCRIPT_DIR/../../../_testoutput"  --portable-root "$SCRIPT_DIR/"  --/telemetry/mode=test --/crashreporter/data/testName="ext-test-omni.isaac.sim.compatibility_check" "$@"
