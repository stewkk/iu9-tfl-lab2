#!/usr/bin/env sh

rm -f /tmp/fifo && mkfifo /tmp/fifo && (build/bin/learner 1 1 < /tmp/fifo) 1>&2 | RANDOM_SEED=322 HEIGHT=1 WIDTH=1 python mat/advanced12iq/main.py > /tmp/fifo 1>&2

build/bin/learner 1 1 < fifo | RANDOM_SEED=322 HEIGHT=1 WIDTH=1 python mat/advanced12iq/main.py > fifo
