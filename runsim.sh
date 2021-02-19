#!/bin/bash
function need_make(){
    local CUR_DIR="$(pwd)"
    cd $1
    make -q
    local RESULT=$?
    (cd $CUR_DIR)
    echo $RESULT
}

if [[ -f /.dockerenv ]]; then 
    NEED_MAKE=$(need_make /code/riscv-sim )
    EXEC_FILE="/code/riscv-sim/bin/riscv-console-sim"
    if [ $NEED_MAKE -ne "0" ]; then
        CUR_DIR="$(pwd)"
        MAKE_CMD="cd /code/riscv-sim && make clean && make && cd $CUR_DIR"
        echo $MAKE_CMD
        ( cd /code/riscv-sim && make clean && make && cd $CUR_DIR )
        if [ $? -ne 0 ]; then
            exit
        fi
    fi
    SIM_RUN_CMD="runuser -u docker $EXEC_FILE -- $@"
    echo $SIM_RUN_CMD
    $SIM_RUN_CMD
else
    NEED_MAKE=$(need_make ./riscv-sim )
    EXEC_FILE="./riscv-sim/bin/riscv-console-sim"
    if [ $NEED_MAKE -ne "0" ]; then
        CUR_DIR="$(pwd)"
        MAKE_CMD="cd ./riscv-sim && make clean && make && cd $CUR_DIR"
        echo $MAKE_CMD
        (cd ./riscv-sim && make clean && make && cd $CUR_DIR)
        if [ $? -ne 0 ]; then
            exit
        fi
    fi
    echo "$EXEC_FILE" "$@"
    $EXEC_FILE "$@"
fi
