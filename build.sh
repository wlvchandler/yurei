#!/bin/bash
PROJECTROOT=$(git rev-parse --show-toplevel)

cmake -B ${PROJECTROOT}/build -DCMAKE_BUILD_TYPE=Release
cmake --build ${PROJECTROOT}/build --config Release
mkdir -p ${PROJECTROOT}/bin

#ln -srf build/bin/oni bin/oni
#ln -srf build/bin/yokai bin/yokai
ln -sf ${PROJECTROOT}/build/bin/yurei ${PROJECTROOT}/bin/yurei
