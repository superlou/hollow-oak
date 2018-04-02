#!/bin/bash
# Build header
$(echo "#ifndef PAGE_TEMPLATES_HPP_" > page_templates.hpp)
$(echo "#define PAGE_TEMPLATES_HPP_" >> page_templates.hpp)
$(echo "" >> page_templates.hpp)

for filename in page_templates/*; do
  array_name=${filename//[\/. -]/\_}""
  array_len=${filename//[\/. -]/\_}
  $(echo "extern unsigned char" $array_name"[];" >> page_templates.hpp)
  $(echo "extern unsigned int" $array_len"_len;" >> page_templates.hpp)
done

$(echo "" >> page_templates.hpp)
$(echo "#endif" >> page_templates.hpp)

# Build source
$(echo "#include \"page_templates.hpp\"" > page_templates.cpp)
$(echo "" >> page_templates.cpp)

for filename in page_templates/*; do
  $(xxd -i $filename | sed 's/\([0-9a-f]\)$/\0, 0x00/' >> page_templates.cpp)
  $(echo "" >> page_templates.cpp)
done
