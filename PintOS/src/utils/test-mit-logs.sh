#!/usr/bin/env zsh

LOG_FILE="$(date +%F-%s).log"

exec > >(tee -ia /tmp/$LOG_FILE)

./pintos -v -- -mlfqs run mlfqs-recent-1
