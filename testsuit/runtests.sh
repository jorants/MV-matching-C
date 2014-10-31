#!/bin/bash

for i in graphs/*;
do
  echo $i >> abc.out;
  ./process $i >> ab.out;
done
