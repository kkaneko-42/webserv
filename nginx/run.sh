#!/bin/bash

echo "[simple]"
cd simple && bash run.sh && cd .. || exit
echo "[tekito]"
cd tekito && bash run.sh && cd .. || exit
