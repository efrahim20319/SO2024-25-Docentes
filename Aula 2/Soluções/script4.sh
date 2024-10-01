#!/usr/bin/bash​

# 4 pipes​
ls | wc
ls -l | wc
ls | grep ".bash" > bashfiles
cat bashfiles