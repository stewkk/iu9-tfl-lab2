#!/usr/bin/env sh

rm -f /tmp/fifo && mkfifo /tmp/fifo && (build/bin/learner 1 1 < /tmp/fifo) 1>&2 | RANDOM_SEED=322 HEIGHT=1 WIDTH=1 python mat/advanced12iq/main.py > /tmp/fifo 1>&2

build/bin/learner 1 1 < fifo | RANDOM_SEED=6255 HEIGHT=1 WIDTH=1 python mat/advanced12iq/main.py > fifo

rm -f fifo logs mat.log && mkfifo fifo && build/bin/learner 1 1 < fifo | RANDOM_SEED=6255 HEIGHT=1 WIDTH=1 python mat/advanced12iq/main.py > fifo

hyperfine --export-markdown "benchmarks/$(git rev-parse --short HEAD).md" --parameter-scan seed 123 133 'build/bin/learner 5 5 {seed}'

Benchmark 1: rm -f fifo logs mat.log && mkfifo fifo && time build/bin/learner 3 3 < fifo | HEIGHT=3 WIDTH=3 RANDOM_SEED=123 python mat/advanced12iq/main.py 2>mat.errlogs > fifo || echo "Done"
