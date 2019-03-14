#!/bin/sh

./pagetable -p ./reports/table_20.txt -t ./trace.sample.tr 20 > ./reports/report_20.txt;
./pagetable -p ./reports/table_4_8_8.txt -t ./trace.sample.tr 4 8 8 > ./reports/report_4_8_8.txt;
./pagetable -p ./reports/table_4_4_12.txt -t ./trace.sample.tr 4 4 12 > ./reports/report_4_4_12.txt;