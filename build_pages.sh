#!/bin/bash
$(echo "#ifndef PAGE_TEMPLATES_HPP_" > page_templates.hpp)
$(echo "#define PAGE_TEMPLATES_HPP_" >> page_templates.hpp)
$(echo "" >> page_templates.hpp)

for filename in page_templates/*; do
  $(xxd -i $filename | sed 's/\([0-9a-f]\)$/\0, 0x00/' >> page_templates.hpp)
  $(echo "" >> page_templates.hpp)
done

$(echo "#endif" >> page_templates.hpp)
