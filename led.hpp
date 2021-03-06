#ifndef LED_HPP_
#define LED_HPP_

void led_init(void);
void led_process(void);
void led_do_power(void);
void led_do_count(int count);
void led_do_morse(void);
void led_do_blink(int period_ms);
void led_do_off(void);
void led_do_on(void);

#endif
