set -e
HEIGHT=5
WIDTH=5
for((i = 1; ; ++i)); do
    echo "Run test: "  $i
    RANDOM_SEED=$(date +%s%N)
    echo "With seed: "  $RANDOM_SEED
    time build/bin/learner "$HEIGHT" "$WIDTH" "$RANDOM_SEED" 2>>/tmp/tfl-lab2-logs || break
    echo "Passed test: "  $i
done

echo "FAILED on the following test:"
echo "$RANDOM_SEED"
