#include <pebble.h>

static Window *window;
static TextLayer *time_layer, *hour_layer, *minute_layer, *date_layer, *day_layer;
static InverterLayer *inverter_layer;

static void update_time() {
	time_t temp = time(NULL);
	struct tm *tick_time = localtime(&temp);
	
	//Update time buffers
	static char timeBuffer[] = "00:00";
	if(clock_is_24h_style() == true){
		strftime(timeBuffer, sizeof(timeBuffer), "%H:%M", tick_time);
	} else {
		 strftime(timeBuffer, sizeof(timeBuffer), "%I:%M", tick_time);
	} 
	//Handle leading 0's
	if (timeBuffer[0] == '0') {timeBuffer[0] = ' '; }
	text_layer_set_text(time_layer, timeBuffer);
	
	//Dates
	static char dateBuffer[] = "DECEMBER 01";
	strftime(dateBuffer, sizeof(dateBuffer), "%B %e", tick_time);
	text_layer_set_text(date_layer, dateBuffer);
	
	//Day
	static char dayBuffer[] = "SuN";
	strftime(dayBuffer, sizeof(dayBuffer), "%a", tick_time);
	text_layer_set_text(day_layer, dayBuffer);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  //Time layer
  time_layer = text_layer_create(GRect(10, 112, 144, 50));
  text_layer_set_text_alignment(time_layer, GTextAlignmentLeft);
  text_layer_set_font(time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
  layer_add_child(window_layer, text_layer_get_layer(time_layer));
  
  //Date layer
  date_layer = text_layer_create(GRect(10, 5, 144, 50));
  text_layer_set_text_alignment(date_layer, GTextAlignmentLeft);
  text_layer_set_font(date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  layer_add_child(window_layer, text_layer_get_layer(date_layer));
  
  //Day layer
  day_layer = text_layer_create(GRect(110, 112, 30, 25));
  text_layer_set_text_alignment(day_layer, GTextAlignmentRight);
  text_layer_set_font(day_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  layer_add_child(window_layer, text_layer_get_layer(day_layer));
  
  //Inverter layer
  inverter_layer = inverter_layer_create(GRect(0, 0, 144, 80));
  layer_add_child(window_layer, inverter_layer_get_layer(inverter_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(time_layer);
  text_layer_destroy(date_layer);
  text_layer_destroy(day_layer);
  inverter_layer_destroy(inverter_layer);
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
