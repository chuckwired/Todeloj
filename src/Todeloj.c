#include <pebble.h>

static Window *window;
static TextLayer *time_layer, *hour_layer, *minute_layer, *date_layer, *day_layer;

static void update_time() {
	time_t temp = time(NULL);
	struct tm *tick_time = localtime(&temp);
	
	//Update time buffers
	static char timeBuffer[] = "00:00";
	if(clock_is_24h_style() == true){
		strftime(timeBuffer, sizeof("00:00"), "%H:%M", tick_time);
	} else {
		 strftime(timeBuffer, sizeof("00:00"), "%I:%M", tick_time);
	} 
	
	//Set the values
	if (timeBuffer[0] == '0') {timeBuffer[0] = ' '; }
	text_layer_set_text(time_layer, timeBuffer);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  //Setuip + styling
  time_layer = text_layer_create(GRect(0, 55, 144, 50));
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
  text_layer_set_font(time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
  //Add
  layer_add_child(window_layer, text_layer_get_layer(time_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(time_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	update_time();
}

static void init(void) {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  window_stack_push(window, true);
  update_time();
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);
}
