#!/bin/sh
./my_otp -i messageA.txt -o mA.enc -k keyfile16K.key

./my_otp -i messageB.txt -o mB.enc -k keyfile16K.key

./my_otp -i mA.enc -o mA.txt -k keyfile16K.key

./my_otp -i mB.enc -o mB.txt -k keyfile16K.key

if diff mA.txt messageA.txt; then : ; else echo FAILED;  exit 1 ; fi

if diff mB.txt messageB.txt; then : ; else echo FAILED ; exit 1 ; fi

rm -f mA* mB*
echo PASSED
exit 0
