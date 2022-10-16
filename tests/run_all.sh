for dir in `ls -d */`; do
    if [ "$dir" = "conf/" ] || [ "$dir" = "server_name/" ]; then
        true
    else
        cp ./run.sh ${dir}
    fi

    echo "============= ${dir} =============="
    (cd ${dir} && bash run.sh)
done
