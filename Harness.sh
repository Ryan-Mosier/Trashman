#start program
./TrashMan "$@"

#loop while exit code is not 42
while [ $? -ne 42 ]; do
  ./IO

  if [ $? -eq 1 ]; then
    ./TrashMan "$@"
    fi

#exit
done
