progen -o zcm.pro.org
cat zcm.pro.org progen.template > zcm.pro
tmake -o Makefile zcm.pro
make


