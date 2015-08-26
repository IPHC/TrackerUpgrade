#!/bin/env zsh

list=($(pgrep histProd))
for i in $list
do
kill -9 $i
done
