#!/bin/env zsh

# Script to convert RAW data to EDM
# Specify input files with inputFile key using syntax -inputFile="file:file1.dat,file:file2.dat,..."

cmsRun EventStreamFileConverter.py \
inputFile="file:USC.00000366.0001.A.storageManager.00.0000.dat" \
outputFile="edm.root"
