#!/bin/bash

./check_properties/bin/evaluate.elf logs/evaluate_summary.txt 2>&1 | tee logs/evaluate_stats.txt
