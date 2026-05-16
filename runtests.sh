#!/bin/bash

make -j

for test in $(ls ./bin/*.test); do
    valgrind $test > $test.log
done

gcovr *
