#!/usr/bin/env bash
#
# runtests.sh
#
# A command line tool that enables subnet network calculations using **CIDR notation**.
#
# Version: v0.1.0
# --------------------------------------------------------------------------------------

readonly RED=$(tput setaf 1 || echo "")
readonly GREEN=$(tput setaf 2 || echo "")
readonly YELLOW=$(tput setaf 3 || echo "")
readonly RESET=$(tput sgr0 || echo "")

function debug {
    [ "$DEBUG" = "true" ] && echo "${YELLOW}[DEBUG] $*${RESET}"
}

function exec_help {
    echo "Usage: $(basename $0) [OPTIONS]"
    echo
    echo "Run unit tests."
    echo
    echo "OPTIONS:"
    echo "  -h, --help      Print this help message"
    echo "  -v, --version   Print current version"
}

function exec_version {
    grep '^# Version: ' "$0" | cut -d ':' -f 2 | tr -d ' '
}

function exec_tests {
    local TEST_BINS=$(find ./build/tests -name 'test_*' 2> /dev/null)
    local START_TIME=$(date +%s.%N)

    while read -r TEST_BIN; do
        local REPORT=""
        local RAW_REPORT=""
        local FAILED="false"
        local TESTS_RUN="0"
        local TOTAL_TESTS_RUN="0"
        local STATUS_MSG="${GREEN}[PASSED]${RESET}"

        # run test
        RAW_REPORT=$("$TEST_BIN" 2> /dev/null)
        [ $? -ne 0 ] && FAILED="true" && STATUS_MSG="${RED}[FAILED]${RESET}"
        TESTS_RUN=$(echo "$RAW_REPORT" | head -n 1)
        TOTAL_TESTS_RUN=$((TOTAL_TESTS_RUN + TESTS_RUN))
        REPORT=$(echo "$RAW_REPORT" | tail -n 1)

        # show report
        printf "%-64s%s\n" "Running tests in $TEST_BIN" "$STATUS_MSG"
        if [ "$FAILED" = "true" ]; then
            echo "$REPORT"
            echo ""
        fi
    done <<< "$TEST_BINS"

    local END_TIME=$(date +%s.%N)
    local TOTAL_TIME=$(echo "($END_TIME - $START_TIME)" | bc)
    echo "SUMMARY" 
    echo "Total tests run: $TOTAL_TESTS_RUN"
    echo "Total time: ${TOTAL_TIME}s"
}

# Main execution flow
while [ -n "$1" ]; do
    case "$1" in
        -h | --help    ) exec_help    && exit 0 ;;
        -v | --version ) exec_version && exit 0 ;;

        # Undefined option
        *) echo -e "Invalid parameter $1\n"
           exec_help
           exit 1
           ;;
    esac
    shift
done
exec_tests
