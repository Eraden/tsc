#!/usr/bin/env zsh

for f in $(ls | grep gv); do
  echo "dot -o$( echo $f | sed 's/gv/png/' ) -Tpng $f"
  dot -o$( echo $f | sed 's/gv/png/' ) -v -Tpng $f
done
