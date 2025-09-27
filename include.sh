#!/usr/bin/env bash

## GETS THE CURRENT MODULE ROOT DIRECTORY
MOD_FASTFORWARD_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )/" && pwd )"

source $MOD_FASTFORWARD_ROOT"/conf/conf.sh.dist"

if [ -f $MOD_FASTFORWARD_ROOT"/conf/conf.sh" ]; then
    source $MOD_FASTFORWARD_ROOT"/conf/conf.sh"
fi
