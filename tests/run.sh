for dir in `ls -d */`; do
  (cd ${dir} && ./run.sh)
done
