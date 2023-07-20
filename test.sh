#!/bin/bash
./server $1 > tmp.out
diff -w tmp.out $2
if [ $? -eq 0 ]
then
    echo "Test passed"
    rm tmp.out
else 
    echo "Test failed: check tmp.out for output"
fi
