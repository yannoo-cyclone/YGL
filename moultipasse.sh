#!/bin/sh

./compil.sh > compil.err 2>&1
./push.sh

cat compil.err
