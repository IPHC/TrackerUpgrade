#!/bin/env zsh

# Script to read EDM data
# Specify input files with inputFile key using syntax -inputFile="file:file1.root,file:file2.root,..."

cmsRun run_cfg.py \
inputFile="file:edm.root" \
outputFile="output.root"
