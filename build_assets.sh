#!/bin/bash
# Build assets header
$(echo "#ifndef ASSETS_HPP_" > assets.hpp)
$(echo "#define ASSETS_HPP_" >> assets.hpp)
$(echo "" >> assets.hpp)
$(echo "#include \"Arduino.h\"" >> assets.hpp)
$(echo "" >> assets.hpp)

for filename in page_templates/*; do
  array_name=${filename//[\/. -]/\_}""
  array_len=${filename//[\/. -]/\_}
  $(echo "extern unsigned char" $array_name"[];" >> assets.hpp)
  $(echo "extern unsigned int" $array_len"_len;" >> assets.hpp)
done

for filename in images/*; do
  array_name=${filename//[\/. -]/\_}""
  array_len=${filename//[\/. -]/\_}
  $(echo "extern const PROGMEM unsigned char" $array_name"[];" >> assets.hpp)
  $(echo "extern unsigned int" $array_len"_len;" >> assets.hpp)
done

$(echo "" >> assets.hpp)
$(echo "#endif" >> assets.hpp)

# Build assets source
$(echo "#include \"assets.hpp\"" > assets.cpp)
$(echo "" >> assets.cpp)

for filename in page_templates/*; do
  $(xxd -i $filename | sed 's/\([0-9a-f]\)$/\0, 0x00/' >> assets.cpp)
  $(echo "" >> assets.cpp)
done

for filename in images/*; do
  $(xxd -i $filename | sed 's/\([0-9a-f]\)$/\0, 0x00/' | sed -e 's/^unsigned char /const PROGMEM unsigned char /' >> assets.cpp)
  $(echo "" >> assets.cpp)
done
