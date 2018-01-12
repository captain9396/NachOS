#!/bin/bash

gmake all
cd userprog
gmake depend
gmake
./nachos -x ../test/"$1"
