#!/usr/bin/env bash

BINDIR=bin
TESTS=$(find ${BINDIR} -name 'test_*')

echo -e "Running all Tests\n"

for TEST in ${TESTS}
do
    echo "Testing ${TEST} ..."
    ./${TEST}
    echo ""
done
