mkdir -p icons
mkdir -p png

SVG_FILES="./svg/*.svg"
PNG_PATH="./png/${1}x${1}"
PNG_FILES="${PNG_PATH}/*.png"
HEADER_PATH="./icons/${1}x${1}"
HEADER="./icons/icons_${1}x${1}.h"

if [ -e "$PNG_PATH" ];then rm -rf "$PNG_PATH" ; fi
mkdir $PNG_PATH
if [ -e "$HEADER_PATH" ];then rm -rf "$HEADER_PATH" ; fi
mkdir $HEADER_PATH
if [ -e "$HEADER" ];then rm "$HEADER" ; fi

for f in $SVG_FILES
do
  echo "Converting .svg to .png for $f..."

  # SVG_SIZE=$(identify -format '%w' $f)
  # DENSITY=$(bc -l <<< "96 * $1 / $SVG_SIZE")
  # mogrify -format png -path $PNG_PATH -colorspace sRGB -density $DENSITY $f

  out="$PNG_PATH/$(basename $f .svg).png"
  inkscape -w 64 -h 64 $f -o $out --export-background="#ffffff"
done

for f in $PNG_FILES
do
  echo "Generating header for $f..."
  out="${HEADER_PATH}/$(basename $f .png | tr -s -c [:alnum:] _)${1}x${1}.h"
  python3 png_to_header.py -i $f -o $out
done

echo "Generating include statements..."
echo "#ifndef __ICONS_${1}x${1}_H__" > $HEADER
echo "#define __ICONS_${1}x${1}_H__" >> $HEADER
for f in ${HEADER_PATH}/*.h
do
    echo "#include \"${1}x${1}/$(basename $f)\"" >> $HEADER
done
echo "#endif" >> $HEADER

echo "Done."
