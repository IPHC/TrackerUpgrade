#!/bin/env zsh

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/kskovpen/plugins/atlasstyle-00-03-04

inf1=../output.root
fn1="DATA"

./plot ${inf1} ${fn1}
