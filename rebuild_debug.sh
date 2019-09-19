#!/bin/bash

# Rebuild all DEBUG versions of the TraceMiner2 utility.
# To compile all, use compile_release.sh instead.

# Where is CodeBlocks to be found?
CB=`which codeblocks`

# What is the project file called?
CBP=TraceMiner2.Linux.cbp

# What will the logfile be called?
LOG=TraceMiner2.debug.log

# Rebuild all release targets
echo Rebuilding 32bit Debug .... > ${LOG}
echo =========================== >> ${LOG}
echo >> ${LOG}
${CB} --rebuild --target="32bit Debug" ${CBP} >>${LOG}

echo >> ${LOG}
echo Rebuilding 64bit Debug .... >> ${LOG}
echo =========================== >> ${LOG}
echo >> ${LOG}
${CB} --rebuild --target="64bit Debug" ${CBP} >>${LOG}

echo >> ${LOG}
echo Debug Rebuild completed. See ${LOG} for details.
