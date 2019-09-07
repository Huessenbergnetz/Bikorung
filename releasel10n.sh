#!/bin/bash

lrelease-qt5 -idbased translations/bikorung.ts

for LANG in en_GB en_US de
do
lrelease-qt5 -idbased translations/bikorung_$LANG.ts
done
