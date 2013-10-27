#!/bin/sh

(
cd ../../../..
third_party/FireBreath/prepmake.sh src/js_api/projects src/js_api/build
)

name=WebrtcEngine
(
cd ../../build
make $name 2>/tmp/$name.log >/tmp/err.log
)

tail /tmp/$name.log
