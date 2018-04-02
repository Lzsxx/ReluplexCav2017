#!/bin/bash

./check_properties/bin/checkmynet.elf logs/checkmynet_summary.txt 2>&1 | tee logs/checkmynet_stats.txt
