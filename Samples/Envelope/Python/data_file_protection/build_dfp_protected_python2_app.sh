#!/bin/bash -x
set -e

#Paths to required programs:
PYINSTALLER="/usr/local/bin/pyinstaller2"

LDK_VENDORCODE="../../../../VendorCodes/DEMOMA.hvc"
LDK_DFCRYPT="../../../../VendorTools/Envelope/dfcrypt"
LDK_LINUXENV="../../../../VendorTools/Envelope/linuxenv"

#Location of your application:
APP_DIR="../sample_app"
START_SCRIPT="main.py"

#Output directory:
OUT_DIR="./out_python2"

#Preparation:
rm -rf $OUT_DIR
mkdir  $OUT_DIR

#Obtain script's filename without extension:
APP_NAME=$(basename -- "$START_SCRIPT")
APP_NAME="${APP_NAME%.*}"
echo "APP_NAME: $APP_NAME" # APP_NAME="main"

#Run pyinstaller:
set +x #Disables command echoing.
printf "\n"
printf "#####################################################################\n"
printf "# Running pyinstaller                                               #\n"
printf "#####################################################################\n"
set -x #Enables command echoing.

#Copy the input app for pyinstaller's processing:
cp  -r $APP_DIR "$OUT_DIR/pyinst_work_dir"

cd "$OUT_DIR/pyinst_work_dir"
#We need '-d noarchive' so we can replace pyinstaller's collected modules with encrypted ones.
$PYINSTALLER -d noarchive --log-level ERROR $START_SCRIPT
cd "../.."

set +x #Disables command echoing.
printf "\n"
printf "#####################################################################\n"
printf "# Encrypt desired modules:                                          #\n"
printf "#####################################################################\n"
set -x #Enables command echoing.

chmod u+x $LDK_DFCRYPT
# Encryption command: dfcrypt --vcf:DEMOMA.hvc --fid:0 --overwrite <file1> <file2> ... <out_dir>
mkdir "$OUT_DIR/pyinst_work_dir/dist/$APP_NAME/temp"
mv "$OUT_DIR/pyinst_work_dir/dist/$APP_NAME/moduleA.pyc" "$OUT_DIR/pyinst_work_dir/dist/$APP_NAME/temp/moduleA.pyc"
mv "$OUT_DIR/pyinst_work_dir/dist/$APP_NAME/moduleB.pyc" "$OUT_DIR/pyinst_work_dir/dist/$APP_NAME/temp/moduleB.pyc"

$LDK_DFCRYPT --vcf:$LDK_VENDORCODE --fid:0 "--key:a secret" --overwrite "$OUT_DIR/pyinst_work_dir/dist/$APP_NAME/temp/moduleA.pyc" "$OUT_DIR/pyinst_work_dir/dist/$APP_NAME/temp/moduleB.pyc" "$OUT_DIR/pyinst_work_dir/dist/$APP_NAME"

rm -rf "$OUT_DIR/pyinst_work_dir/dist/$APP_NAME/temp"

#Envelope-protect the python interpreter, so it can access the encrypted bytecode modules:
set +x #Disables command echoing.
printf "\n"
printf "#####################################################################\n"
printf "# Envelope python interpreter                                       #\n"
printf "#####################################################################\n"
set -x #Enables command echoing.
chmod u+x $LDK_LINUXENV
$LDK_LINUXENV --vcf:$LDK_VENDORCODE --fid:0 --dfp "$OUT_DIR/pyinst_work_dir/dist/$APP_NAME/libpython2.7.so.1.0" "$OUT_DIR/pyinst_work_dir/dist/$APP_NAME/libpython2.7.so.1.0"

# Cleanup temorary files:
set +x #Disables command echoing.
printf "\n"
printf "#####################################################################\n"
printf "# Cleanup temporary files                                           #\n"
printf "#####################################################################\n"
set -x #Enables command echoing.
mv "$OUT_DIR/pyinst_work_dir/dist/$APP_NAME" "$OUT_DIR/$APP_NAME"
rm -rf "$OUT_DIR/pyinst_work_dir"

set +x #Disables command echoing.
printf "\n"
printf "#####################################################################\n"
printf "# SUCCESS                                                           #\n"
printf "#                                                                   #\n"
printf "# To distribute your application copy the folder:                   #\n"
printf "#   '$OUT_DIR/$APP_NAME'\n"
printf "# To run your application execute:                                  #\n"
printf "#   '$OUT_DIR/$APP_NAME/$APP_NAME'\n"
printf "#####################################################################\n"
