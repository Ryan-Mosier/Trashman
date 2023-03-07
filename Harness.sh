#start program
./TrashMan "$@"

#loop while exit code is not 42
while [ $? -ne 42 ]; do
    ./TrashMan "$@"

#exit
done
