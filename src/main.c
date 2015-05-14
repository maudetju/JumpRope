#include <pebble.h>
  
static Window *s_window;
static TextLayer *s_time_layer;
static TextLayer *s_time_counter_layer;
static TextLayer *s_jumps_layer;
static TextLayer *s_jumps_counter_layer;

static bool s_launch = false;

static int s_uptime = 0;
static char s_uptime_buffer[32];

static int s_jumps = 0;
static char s_jumps_buffer[6];

static void tap_handler(AccelAxisType axis, int32_t direction) {
  s_jumps++;
  snprintf(s_jumps_buffer, sizeof(s_jumps_buffer), "%d", s_jumps);
  text_layer_set_text(s_jumps_counter_layer, s_jumps_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  // Increment s_uptime
  s_uptime++;
  
  // Get time since launch
  int seconds = s_uptime % 60;
  int minutes = (s_uptime % 3600) / 60;
  int hours = s_uptime / 3600;
  
  // Format uptime to 00:00:00
  char format[10];
  strcpy(format,  "");
  
  if (hours < 10) {
    strcat(format, "0%d:");
  } else {
    strcat(format, "%d:");
  }
  if (minutes < 10) {
    strcat(format, "0%d:");
  } else {
    strcat(format, "%d:");
  }
  if (seconds < 10) {
    strcat(format, "0%d");
  } else {
    strcat(format, "%d");
  }
  
  // Update the TextLayer
  snprintf(s_uptime_buffer, sizeof(s_uptime_buffer), format, hours, minutes, seconds);
  text_layer_set_text(s_time_counter_layer, s_uptime_buffer);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {

}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (s_launch) {
    // Unregister with TickTimerService
    tick_timer_service_unsubscribe();
    // Unregister with AccelTapService
    accel_tap_service_unsubscribe();
  } else {
    // Register with TickTimerService
    tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
    // Register with AccelTapService
    accel_tap_service_subscribe(tap_handler);
  }
  s_launch = !s_launch;
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  // Reset all counters
  s_uptime = 0;
  text_layer_set_text(s_time_counter_layer, "00:00:00");
  s_jumps = 0;
  text_layer_set_text(s_jumps_counter_layer, "0");
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  //text_layer_time
  s_time_layer = text_layer_create((GRect) { .origin = { 0, 20 }, .size = { bounds.size.w, 15 } });
  text_layer_set_text(s_time_layer, "TIME");
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  
  //text_layer_time_counter
  s_time_counter_layer = text_layer_create((GRect) { .origin = { 0, 35 }, .size = { bounds.size.w, 15 } });
  text_layer_set_text(s_time_counter_layer, "00:00:00");
  text_layer_set_text_alignment(s_time_counter_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_time_counter_layer));
  
  //text_layer_jumps
  s_jumps_layer = text_layer_create((GRect) { .origin = { 0, 60 }, .size = { bounds.size.w, 15 } });
  text_layer_set_text(s_jumps_layer, "JUMPS");
  text_layer_set_text_alignment(s_jumps_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_jumps_layer));
  
  //text_layer_jumps_counter
  s_jumps_counter_layer = text_layer_create((GRect) { .origin = { 0, 75 }, .size = { bounds.size.w, 30 } });
  text_layer_set_text(s_jumps_counter_layer, "0");
  text_layer_set_text_alignment(s_jumps_counter_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_jumps_counter_layer));
  
}

static void window_unload(Window *window) {
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_time_counter_layer);
  text_layer_destroy(s_jumps_layer);
  text_layer_destroy(s_jumps_counter_layer);
}

static void init(void) {
  s_window = window_create();
  window_set_click_config_provider(s_window, click_config_provider);
  window_set_window_handlers(s_window, (WindowHandlers) {
	.load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(s_window, animated);
}

static void deinit(void) {
  window_destroy(s_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}