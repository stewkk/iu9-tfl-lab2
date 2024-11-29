set -e
HEIGHT=8
WIDTH=8
for((i = 1; ; ++i)); do
    echo "Run test: "  $i
    RANDOM_SEED=$(date +%s%N)
    echo "With seed: "  $RANDOM_SEED
    time build/bin/learner "$HEIGHT" "$WIDTH" "$RANDOM_SEED" 2>/dev/null || break
    echo "Passed test: "  $i
done

echo "FAILED on the following test:"
echo "$RANDOM_SEED"
