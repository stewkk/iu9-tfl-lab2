set -e
HEIGHT=4
WIDTH=4
for((i = 1; ; ++i)); do
    echo "Run test: "  $i
    RANDOM_SEED=$(date +%s%N)
    echo "With seed: "  $RANDOM_SEED
    rm -f fifo logs mat.log && mkfifo fifo && time build/bin/learner "$HEIGHT" "$WIDTH" < fifo | HEIGHT="$HEIGHT" WIDTH="$WIDTH" RANDOM_SEED="$RANDOM_SEED" python mat/advanced12iq/main.py 2>mat.errlogs > fifo || echo "Done"
    grep "Success" logs || break
    echo "Passed test: "  $i
done

echo "FAILED on the following test:"
echo "$RANDOM_SEED"
