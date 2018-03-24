#include <string.h>
#include "template.hpp"

void t_new(char *str) {
  strcpy(str, "<!DOCTYPE html>\n");
}

void t_header(char *str) {
  strcat(str, "<html>\n");
  strcat(str, "<head>\n");
  strcat(str, "</head>\n");
  strcat(str, "<body>\n");
}

void t_footer(char *str) {
  strcat(str, "</body>\n");
  strcat(str, "</html>\n");
}

void t_h1(char *str, char *text) {
  strcat(str, "<h1>");
  strcat(str, text);
  strcat(str, "</h1>\n");
}

void t_form_start(char *str, char *action) {
  strcat(str, "<form action=\"");
  strcat(str, action);
  strcat(str, "\" method=\"POST\">\n");
}

void t_form_end(char *str) {
  strcat(str, "</form>\n");
};

void t_submit(char *str, char *text) {
  strcat(str, "<input type=\"submit\" value=\"");
  strcat(str, text);
  strcat(str, "\">\n");
}

void t_link(char *str, char *url, char *text) {
  strcat(str, "<a href=\"");
  strcat(str, url);
  strcat(str, "\">");
  strcat(str, text);
  strcat(str, "</a>\n");
}
