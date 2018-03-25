#include "game_state.hpp"
#include "web.hpp"
#include "power.hpp"

void power_status_msg(char *msg) {
  if (token_is_set(&power_on)) {
    strcpy(msg, "on");
  } else {
    strcpy(msg, "off - FAULT [<a href='/power'>restore</a>]");
  }
}

bool stabilizer_active = false;
bool chi_active = false;
bool psi_active = false;
bool omega_active = false;

void power_route(void) {
  char power_status[64];
  power_status_msg(power_status);

  char stabilizer_row[80];
  char chi_row[80];
  char psi_row[80];
  char omega_row[80];

  if (token_is_set(&power_on)) {
    stabilizer_active = true;
    chi_active = true;
    psi_active = true;
    omega_active = true;
  }

  if (stabilizer_active) {
    strcpy(stabilizer_row, "<td></td><td><input type='submit' name='stb-' value='<'></td>");
  } else {
    strcpy(stabilizer_row, "<td><input type='submit' name='stb+' value='>'></td><td></td>");
  }

  if (chi_active) {
    if (stabilizer_active) {
      strcpy(chi_row, "<td></td><td><input type='submit' name='chi-' value='<'></td>");
    } else {
      strcpy(chi_row, "<td></td><td><input type='submit' disabled value='<'></td>");
    }
  } else {
    if (stabilizer_active) {
      strcpy(chi_row, "<td><input type='submit' disabled value='>'></td><td></td>");
    } else {
      strcpy(chi_row, "<td><input type='submit' name='chi+' value='>'></td><td></td>");
    }
  }

  if (psi_active) {
    if (stabilizer_active) {
      strcpy(psi_row, "<td></td><td><input type='submit' name='psi-' value='<'></td>");
    } else {
      strcpy(psi_row, "<td></td><td><input type='submit' disabled value='<'></td>");
    }
  } else {
    if (stabilizer_active) {
      strcpy(psi_row, "<td><input type='submit' disabled value='>'></td><td></td>");
    } else {
      strcpy(psi_row, "<td><input type='submit' name='psi+' value='>'></td><td></td>");
    }
  }

  if (omega_active) {
    if (stabilizer_active) {
      strcpy(omega_row, "<td></td><td><input type='submit' name='omega-' value='<'></td>");
    } else {
      strcpy(omega_row, "<td></td><td><input type='submit' disabled value='<'></td>");
    }
  } else {
    if (stabilizer_active) {
      strcpy(omega_row, "<td><input type='submit' disabled value='>'></td><td></td>");
    } else {
      strcpy(omega_row, "<td><input type='submit' name='omega+' value='>'></td><td></td>");
    }
  }

  web_send_html(&server, PAGE_TEMPLATE(power), power_status, stabilizer_row,
                chi_row, psi_row, omega_row);
}

void reset_power_controls(void) {
  stabilizer_active = false;
  chi_active = false;
  psi_active = false;
  omega_active = false;
}

void power_form(void) {
  if (web_form_arg_present(&server, "stb+")) {
    stabilizer_active = true;
  } else if (web_form_arg_present(&server, "stb-")) {
    stabilizer_active = false;
  } else if (web_form_arg_present(&server, "chi+")) {
    chi_active = true;
    stabilizer_active = true;
  } else if (web_form_arg_present(&server, "chi-")) {
    chi_active = false;
    stabilizer_active = false;
  } else if (web_form_arg_present(&server, "psi+")) {
    psi_active = true;
    stabilizer_active = true;
  } else if (web_form_arg_present(&server, "psi-")) {
    psi_active = false;
    stabilizer_active = false;
  } else if (web_form_arg_present(&server, "omega+")) {
    omega_active = true;
    stabilizer_active = true;
  } else if (web_form_arg_present(&server, "omega-")) {
    omega_active = false;
    stabilizer_active = false;
  }

  // Reset unstable configurations
  if (stabilizer_active &&
      ((!chi_active && !psi_active) || (!psi_active && !omega_active)))
  {
    reset_power_controls();
  } else if (!stabilizer_active &&
             ((chi_active && psi_active) || (psi_active && omega_active)))
  {
    reset_power_controls();
  }

  // Set tokens based on progress
  if (stabilizer_active && chi_active && psi_active && omega_active) {
    if (token_is_clear(&power_on)) {
      token_set(&power_on);
    }
  } else {
    if (token_is_set(&power_on)) {
      token_clear(&power_on);
    }
  }

  web_redirect(&server, "power");
}
