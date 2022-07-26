#!/usr/bin/env bash

#   Run all unit tests
#
#   Exit(s) status code(s)
#   0 - success
#   1 - fail

# ======================================
#   Debug
# ======================================
if [ "${DEBUG}" = true ]; then

    # enable debug mode
    set -x
    export
    whoami

else

  # unset if flag is not set
  unset DEBUG

fi

# ======================================
#   Setup
# ======================================

# set default options
set -o errexit
set -o pipefail
set -o nounset

# check binaries
__FIND=$(which find)
__BASENAME=$(which basename)

# variables
readonly __bindir='bin'
readonly __tests=$(${__FIND} ${__bindir} -name 'test_*')

# colors
readonly __nocolor='\033[0m'
readonly __black='\033[0;30m'
readonly __red='\033[0;31m'
readonly __green='\033[0;32m'
readonly __yellow='\033[0;33m'

# ======================================
#   Functions
# ======================================
function run_test {

    local __testbin="${1:-}"

    # run test
    # don't print stderr since we are testing invalid inputs
    echo "Testing ${__testbin}"
    ${__testbin} 2> /dev/null || return $?

}

# ======================================
#   Script
# ======================================
echo -e "${__yellow}Running all Tests${__nocolor}\n"

for __test in ${__tests}
do

    __error_code='0'

    run_test ${__test} || __error_code=$?
    if [ ${__error_code} -ne 0 ]; then

        # test failed
        echo -e "\n${__red}TEST ${__test} FAILED${__nocolor}"
        exit 1

    fi

    # break line between test results
    echo ""

done

# success message
echo -e "${__green}ALL TESTS PASSED${__nocolor}"
