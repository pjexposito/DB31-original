// Gráficos de los iconos adaptados de http://icons.primail.ch/index.php?page=iconset_weather
// Código del tiempo adaptado de https://github.com/Niknam/futura-weather-sdk2.0.
// Lo anteriormente mencionado ya no se usa en esta version
// Código del reloj basado en https://github.com/orviwan/91-Dub-v2.0
// Con código de https://ninedof.wordpress.com/2014/05/24/pebble-sdk-2-0-tutorial-9-app-configuration/


// UUID para versión en español: "11d1527e-985b-4844-bc10-34ede0ee9caf"
// UUID para versión en inglés:  "a8336799-b197-4e3d-8afd-3290317c65b2"



#include "pebble.h"
  
#define KEY_IDIOMA 0
#define KEY_VIBE 1
#define KEY_DATEFORMAT 2
#define KEY_SEGUNDOS 3
#define KEY_HOURLYVIBE 4  
  
static Window *window;
static Layer *window_layer;
static uint8_t batteryPercent;


int IDIOMA;  
// IDIOMA = 1, texto en español
// IDIOMA = 0, text in english  
  
bool DATEFORMAT;
// DATEFORMAT = 1, Formato europeo (DD/MM/AAAA)
// DATEFORMAT = 0, Formato americano (MM/DD/AAAA)
  

// Vibra al perder la conexión BT  
int BluetoothVibe;

// Vibrar en el cambio de hora
static int HourlyVibe;

// Si es 1 se muestra el segundero. Si es 0, desaparece.
static int SEGUNDOS;

static bool appStarted = false;

static GBitmap *background_image;
static BitmapLayer *background_layer;

static GBitmap *separator_image;
static BitmapLayer *separator_layer;

static GBitmap *meter_bar_image;
static BitmapLayer *meter_bar_layer;

static GBitmap *bluetooth_image;
static BitmapLayer *bluetooth_layer;

static GBitmap *tilde_image;
static BitmapLayer *tilde_layer;

static GBitmap *sepames_image;
static BitmapLayer *sepames_layer;

static GBitmap *battery_image;
static BitmapLayer *battery_image_layer;
static BitmapLayer *battery_layer;

static GBitmap *time_format_image;
static BitmapLayer *time_format_layer;

static GBitmap *day_name_image;
static BitmapLayer *day_name_layer;

const int DAY_NAME_IMAGE_RESOURCE_IDS[] = {
  RESOURCE_ID_IMAGE_DAY_NAME_SUN,
  RESOURCE_ID_IMAGE_DAY_NAME_MON,
  RESOURCE_ID_IMAGE_DAY_NAME_TUE,
  RESOURCE_ID_IMAGE_DAY_NAME_WED,
  RESOURCE_ID_IMAGE_DAY_NAME_THU,
  RESOURCE_ID_IMAGE_DAY_NAME_FRI,
  RESOURCE_ID_IMAGE_DAY_NAME_SAT
};

const int DAY_NAME_EN_IMAGE_RESOURCE_IDS[] = {
  RESOURCE_ID_IMAGE_DAY_NAME_SUN_EN,
  RESOURCE_ID_IMAGE_DAY_NAME_MON_EN,
  RESOURCE_ID_IMAGE_DAY_NAME_TUE_EN,
  RESOURCE_ID_IMAGE_DAY_NAME_WED_EN,
  RESOURCE_ID_IMAGE_DAY_NAME_THU_EN,
  RESOURCE_ID_IMAGE_DAY_NAME_FRI_EN,
  RESOURCE_ID_IMAGE_DAY_NAME_SAT_EN
};

#define TOTAL_DATE_DIGITS 2	
static GBitmap *date_digits_images[TOTAL_DATE_DIGITS];
static BitmapLayer *date_digits_layers[TOTAL_DATE_DIGITS];

static GBitmap *month_digits_images[TOTAL_DATE_DIGITS];
static BitmapLayer *month_digits_layers[TOTAL_DATE_DIGITS];

const int DATENUM_IMAGE_RESOURCE_IDS[] = {
  RESOURCE_ID_IMAGE_DATENUM_0,
  RESOURCE_ID_IMAGE_DATENUM_1,
  RESOURCE_ID_IMAGE_DATENUM_2,
  RESOURCE_ID_IMAGE_DATENUM_3,
  RESOURCE_ID_IMAGE_DATENUM_4,
  RESOURCE_ID_IMAGE_DATENUM_5,
  RESOURCE_ID_IMAGE_DATENUM_6,
  RESOURCE_ID_IMAGE_DATENUM_7,
  RESOURCE_ID_IMAGE_DATENUM_8,
  RESOURCE_ID_IMAGE_DATENUM_9
};

#define TOTAL_TIME_DIGITS 4
static GBitmap *time_digits_images[TOTAL_TIME_DIGITS];
static BitmapLayer *time_digits_layers[TOTAL_TIME_DIGITS];

const int BIG_DIGIT_IMAGE_RESOURCE_IDS[] = {
  RESOURCE_ID_IMAGE_NUM_0,
  RESOURCE_ID_IMAGE_NUM_1,
  RESOURCE_ID_IMAGE_NUM_2,
  RESOURCE_ID_IMAGE_NUM_3,
  RESOURCE_ID_IMAGE_NUM_4,
  RESOURCE_ID_IMAGE_NUM_5,
  RESOURCE_ID_IMAGE_NUM_6,
  RESOURCE_ID_IMAGE_NUM_7,
  RESOURCE_ID_IMAGE_NUM_8,
  RESOURCE_ID_IMAGE_NUM_9
};


#define TOTAL_YEAR_DIGITS 2
static GBitmap *year_digits_images[TOTAL_YEAR_DIGITS];
static BitmapLayer *year_digits_layers[TOTAL_YEAR_DIGITS];

const int YEAR_DIGIT_IMAGE_RESOURCE_IDS[] = {
  RESOURCE_ID_ANO_NUM_0,
  RESOURCE_ID_ANO_NUM_1,
  RESOURCE_ID_ANO_NUM_2,
  RESOURCE_ID_ANO_NUM_3,
  RESOURCE_ID_ANO_NUM_4,
  RESOURCE_ID_ANO_NUM_5,
  RESOURCE_ID_ANO_NUM_6,
  RESOURCE_ID_ANO_NUM_7,
  RESOURCE_ID_ANO_NUM_8,
  RESOURCE_ID_ANO_NUM_9
};

#define TOTAL_SEG_DIGITS 2
static GBitmap *seg_digits_images[TOTAL_SEG_DIGITS];
static BitmapLayer *seg_digits_layers[TOTAL_SEG_DIGITS];

const int SEG_DIGIT_IMAGE_RESOURCE_IDS[] = {
  RESOURCE_ID_SEG_NUM_0,
  RESOURCE_ID_SEG_NUM_1,
  RESOURCE_ID_SEG_NUM_2,
  RESOURCE_ID_SEG_NUM_3,
  RESOURCE_ID_SEG_NUM_4,
  RESOURCE_ID_SEG_NUM_5,
  RESOURCE_ID_SEG_NUM_6,
  RESOURCE_ID_SEG_NUM_7,
  RESOURCE_ID_SEG_NUM_8,
  RESOURCE_ID_SEG_NUM_9
};

#define TOTAL_BATTERY_PERCENT_DIGITS 3
static GBitmap *battery_percent_image[TOTAL_BATTERY_PERCENT_DIGITS];
static BitmapLayer *battery_percent_layers[TOTAL_BATTERY_PERCENT_DIGITS];

const int TINY_IMAGE_RESOURCE_IDS[] = {
  RESOURCE_ID_IMAGE_TINY_0,
  RESOURCE_ID_IMAGE_TINY_1,
  RESOURCE_ID_IMAGE_TINY_2,
  RESOURCE_ID_IMAGE_TINY_3,
  RESOURCE_ID_IMAGE_TINY_4,
  RESOURCE_ID_IMAGE_TINY_5,
  RESOURCE_ID_IMAGE_TINY_6,
  RESOURCE_ID_IMAGE_TINY_7,
  RESOURCE_ID_IMAGE_TINY_8,
  RESOURCE_ID_IMAGE_TINY_9,
  RESOURCE_ID_IMAGE_TINY_PERCENT,
  RESOURCE_ID_IMAGE_TINY_G  
};


static void carga_preferencias(void)
  { 
    // Carga las preferencias
    IDIOMA = persist_exists(KEY_IDIOMA) ? persist_read_int(KEY_IDIOMA) : 0;
    DATEFORMAT = persist_exists(KEY_DATEFORMAT) ? persist_read_bool(KEY_DATEFORMAT) : 0;
    BluetoothVibe = persist_exists(KEY_VIBE) ? persist_read_int(KEY_VIBE) : 1;
    SEGUNDOS = persist_exists(KEY_SEGUNDOS) ? persist_read_int(KEY_SEGUNDOS) : 1;
    HourlyVibe = persist_exists(KEY_HOURLYVIBE) ? persist_read_int(KEY_HOURLYVIBE) : 0;
  }

static void handle_tick(struct tm *tick_time, TimeUnits units_changed);


static void in_recv_handler(DictionaryIterator *iterator, void *context)
  {
  //Recibe los datos de configuración
  Tuple *key_idioma_tuple = dict_find(iterator, KEY_IDIOMA);
  Tuple *key_vibe_tuple = dict_find(iterator, KEY_VIBE);
  Tuple *key_dateformat_tuple = dict_find(iterator, KEY_DATEFORMAT);
  Tuple *key_segundos_tuple = dict_find(iterator, KEY_SEGUNDOS);
  Tuple *key_hourlyvibe_tuple = dict_find(iterator, KEY_HOURLYVIBE);

  if(strcmp(key_idioma_tuple->value->cstring, "spanish") == 0)
    persist_write_int(KEY_IDIOMA, 1);
  else if(strcmp(key_idioma_tuple->value->cstring, "english") == 0)
    persist_write_int(KEY_IDIOMA, 0);
 
  if(strcmp(key_dateformat_tuple->value->cstring, "DDMM") == 0)
      persist_write_bool(KEY_DATEFORMAT, 1);
  else if(strcmp(key_dateformat_tuple->value->cstring, "MMDD") == 0)
    persist_write_bool(KEY_DATEFORMAT, 0);  
    
  if(strcmp(key_vibe_tuple->value->cstring, "on") == 0)
      persist_write_int(KEY_VIBE, 1);
  else if(strcmp(key_vibe_tuple->value->cstring, "off") == 0)
      persist_write_int(KEY_VIBE, 0); 
    
  if(strcmp(key_segundos_tuple->value->cstring, "on") == 0)
      persist_write_int(KEY_SEGUNDOS, 1);
  else if(strcmp(key_segundos_tuple->value->cstring, "off") == 0)
      persist_write_int(KEY_SEGUNDOS, 0);     
    
  if(strcmp(key_hourlyvibe_tuple->value->cstring, "on") == 0)
     persist_write_int(KEY_HOURLYVIBE, 1);
  else if(strcmp(key_hourlyvibe_tuple->value->cstring, "off") == 0)
     persist_write_int(KEY_HOURLYVIBE, 0);
  
  // Vuelve a dibujar el reloj tras cerrar las preferencias
  carga_preferencias();
  
  if(SEGUNDOS)
  {
    tick_timer_service_subscribe(SECOND_UNIT, handle_tick);
    layer_set_hidden(bitmap_layer_get_layer(seg_digits_layers[0]), false);
    layer_set_hidden(bitmap_layer_get_layer(seg_digits_layers[1]), false);    
  }
  else
  {
    layer_set_hidden(bitmap_layer_get_layer(seg_digits_layers[0]), true);
    layer_set_hidden(bitmap_layer_get_layer(seg_digits_layers[1]), true);    
    tick_timer_service_subscribe(MINUTE_UNIT, handle_tick);
  }
  time_t now = time(NULL);
  struct tm *tick_time = localtime(&now);  
  handle_tick(tick_time, YEAR_UNIT + MONTH_UNIT + DAY_UNIT + HOUR_UNIT + MINUTE_UNIT + SECOND_UNIT);

}


void change_battery_icon(bool charging) {
  gbitmap_destroy(battery_image);
  if(charging) {
    battery_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY_CHARGE);
  }
  else {
    battery_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY);
  }  
  bitmap_layer_set_bitmap(battery_image_layer, battery_image);
  layer_mark_dirty(bitmap_layer_get_layer(battery_image_layer));
}


static void set_container_image(GBitmap **bmp_image, BitmapLayer *bmp_layer, const int resource_id, GPoint origin) {
  GBitmap *old_image = *bmp_image;
  *bmp_image = gbitmap_create_with_resource(resource_id);
  GRect frame = (GRect) {
    .origin = origin,
    .size = (*bmp_image)->bounds.size
  };
  bitmap_layer_set_bitmap(bmp_layer, *bmp_image);
  layer_set_frame(bitmap_layer_get_layer(bmp_layer), frame);
  if (old_image != NULL) {
	  gbitmap_destroy(old_image);
	old_image = NULL;
  }
}


static void update_battery(BatteryChargeState charge_state) {
  batteryPercent = charge_state.charge_percent;
  if(batteryPercent==100) 
  {
	  change_battery_icon(false);
	  layer_set_hidden(bitmap_layer_get_layer(battery_layer), false);
    for (int i = 0; i < TOTAL_BATTERY_PERCENT_DIGITS; ++i) {
      layer_set_hidden(bitmap_layer_get_layer(battery_percent_layers[i]), true);
    }  
    return;
  }
  layer_set_hidden(bitmap_layer_get_layer(battery_layer), charge_state.is_charging);
  change_battery_icon(charge_state.is_charging);  
  for (int i = 0; i < TOTAL_BATTERY_PERCENT_DIGITS; ++i) {
    layer_set_hidden(bitmap_layer_get_layer(battery_percent_layers[i]), false);
  }  
  set_container_image(&battery_percent_image[0], battery_percent_layers[0], TINY_IMAGE_RESOURCE_IDS[charge_state.charge_percent/10], GPoint(13, 41));
  set_container_image(&battery_percent_image[1], battery_percent_layers[1], TINY_IMAGE_RESOURCE_IDS[charge_state.charge_percent%10], GPoint(20, 41));
  set_container_image(&battery_percent_image[2], battery_percent_layers[2], TINY_IMAGE_RESOURCE_IDS[10], GPoint(27, 41));
}

static void toggle_bluetooth_icon(bool connected) {
  if(appStarted && !connected && BluetoothVibe) {
    static uint32_t const segments[] = { 200, 100, 100, 100, 500 };
    VibePattern pat = {
      .durations = segments,
      .num_segments = ARRAY_LENGTH(segments),
      };
    vibes_enqueue_custom_pattern(pat);
    }
  layer_set_hidden(bitmap_layer_get_layer(bluetooth_layer), !connected);
}

void bluetooth_connection_callback(bool connected) {
  toggle_bluetooth_icon(connected);
}

void battery_layer_update_callback(Layer *me, GContext* ctx) {        
  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, GRect(2, 2, ((batteryPercent/100.0)*11.0), 3), 0, GCornerNone);
}


unsigned short get_display_hour(unsigned short hour) {
  if (clock_is_24h_style()) {
    return hour;
  }
  unsigned short display_hour = hour % 12;
  // Converts "0" to "12"
  return display_hour ? display_hour : 12;
}

static void update_days(struct tm *tick_time) {
  if (IDIOMA==1)
    set_container_image(&day_name_image, day_name_layer, DAY_NAME_IMAGE_RESOURCE_IDS[tick_time->tm_wday], GPoint(41, 42));
  else
    set_container_image(&day_name_image, day_name_layer, DAY_NAME_EN_IMAGE_RESOURCE_IDS[tick_time->tm_wday], GPoint(41, 42));
 
  if (DATEFORMAT==1)
    {
    set_container_image(&date_digits_images[0], date_digits_layers[0], DATENUM_IMAGE_RESOURCE_IDS[tick_time->tm_mday/10], GPoint(72, 70));
    set_container_image(&date_digits_images[1], date_digits_layers[1], DATENUM_IMAGE_RESOURCE_IDS[tick_time->tm_mday%10], GPoint(85, 70));
    }
  else
    {
    set_container_image(&date_digits_images[0], date_digits_layers[0], DATENUM_IMAGE_RESOURCE_IDS[tick_time->tm_mday/10], GPoint(110, 70));
    set_container_image(&date_digits_images[1], date_digits_layers[1], DATENUM_IMAGE_RESOURCE_IDS[tick_time->tm_mday%10], GPoint(123, 70));    
    }  
}

static void update_months(struct tm *tick_time) {
  if (DATEFORMAT==1)
  {
    set_container_image(&month_digits_images[0], month_digits_layers[0], DATENUM_IMAGE_RESOURCE_IDS[(tick_time->tm_mon+1)/10], GPoint(110, 70));
    set_container_image(&month_digits_images[1], month_digits_layers[1], DATENUM_IMAGE_RESOURCE_IDS[(tick_time->tm_mon+1)%10], GPoint(123, 70));  }
  else
  {
    set_container_image(&month_digits_images[0], month_digits_layers[0], DATENUM_IMAGE_RESOURCE_IDS[(tick_time->tm_mon+1)/10], GPoint(72, 70));
    set_container_image(&month_digits_images[1], month_digits_layers[1], DATENUM_IMAGE_RESOURCE_IDS[(tick_time->tm_mon+1)%10], GPoint(85, 70));    
  }  
    
}

static void update_years(struct tm *tick_time) {
  set_container_image(&year_digits_images[0], year_digits_layers[0],YEAR_DIGIT_IMAGE_RESOURCE_IDS[(tick_time->tm_year/10)%10], GPoint(108, 42));
  set_container_image(&year_digits_images[1], year_digits_layers[1], YEAR_DIGIT_IMAGE_RESOURCE_IDS[(tick_time->tm_year)%10], GPoint(120, 42));
}

static void update_hours(struct tm *tick_time) {

  if(appStarted && HourlyVibe) {
    vibes_short_pulse();
  }
  
  unsigned short display_hour = get_display_hour(tick_time->tm_hour);

  set_container_image(&time_digits_images[0], time_digits_layers[0], BIG_DIGIT_IMAGE_RESOURCE_IDS[display_hour/10], GPoint(7, 97));
  set_container_image(&time_digits_images[1], time_digits_layers[1], BIG_DIGIT_IMAGE_RESOURCE_IDS[display_hour%10], GPoint(30, 97));

  if (!clock_is_24h_style()) {
    if (tick_time->tm_hour >= 12) {
      set_container_image(&time_format_image, time_format_layer, RESOURCE_ID_IMAGE_PM_MODE, GPoint(11, 83));
      layer_set_hidden(bitmap_layer_get_layer(time_format_layer), false);
    } 
    else {
      layer_set_hidden(bitmap_layer_get_layer(time_format_layer), true);
    }
    
    if (display_hour/10 == 0) {
      layer_set_hidden(bitmap_layer_get_layer(time_digits_layers[0]), true);
    }
    else {
      layer_set_hidden(bitmap_layer_get_layer(time_digits_layers[0]), false);
    }

  }
}

static void update_minutes(struct tm *tick_time) {
  set_container_image(&time_digits_images[2], time_digits_layers[2], BIG_DIGIT_IMAGE_RESOURCE_IDS[tick_time->tm_min/10], GPoint(60, 97));
  set_container_image(&time_digits_images[3], time_digits_layers[3], BIG_DIGIT_IMAGE_RESOURCE_IDS[tick_time->tm_min%10], GPoint(83, 97));
}

static void update_seconds(struct tm *tick_time) {
  if (SEGUNDOS==1)
  {
    set_container_image(&seg_digits_images[0], seg_digits_layers[0], SEG_DIGIT_IMAGE_RESOURCE_IDS[tick_time->tm_sec/10], GPoint(107, 110));
    set_container_image(&seg_digits_images[1], seg_digits_layers[1], SEG_DIGIT_IMAGE_RESOURCE_IDS[tick_time->tm_sec%10], GPoint(122, 110));
  }

}

static void handle_tick(struct tm *tick_time, TimeUnits units_changed) {
  if (units_changed & YEAR_UNIT) update_years(tick_time);
  if (units_changed & MONTH_UNIT) update_months(tick_time);
  if (units_changed & DAY_UNIT) update_days(tick_time);
  if (units_changed & HOUR_UNIT) update_hours(tick_time);
  if (units_changed & MINUTE_UNIT) update_minutes(tick_time);
  if (units_changed & SECOND_UNIT) update_seconds(tick_time);
}


static void init(void) {
  
  memset(&time_digits_layers, 0, sizeof(time_digits_layers));
  memset(&time_digits_images, 0, sizeof(time_digits_images));
  memset(&year_digits_layers, 0, sizeof(year_digits_layers));
  memset(&year_digits_images, 0, sizeof(year_digits_images));
  memset(&month_digits_layers, 0, sizeof(month_digits_layers));
  memset(&month_digits_images, 0, sizeof(month_digits_images));
  memset(&date_digits_layers, 0, sizeof(date_digits_layers));
  memset(&date_digits_images, 0, sizeof(date_digits_images));
  memset(&battery_percent_layers, 0, sizeof(battery_percent_layers));
  memset(&battery_percent_image, 0, sizeof(battery_percent_image));

  carga_preferencias();
  
  window = window_create();
  if (window == NULL) {
      //APP_LOG(APP_LOG_LEVEL_DEBUG, "OOM: couldn't allocate window");
      return;
  }
  window_stack_push(window, true /* Animated */);
  window_layer = window_get_root_layer(window);
  
	
  background_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
  background_layer = bitmap_layer_create(layer_get_frame(window_layer));
  bitmap_layer_set_bitmap(background_layer, background_image);
  layer_add_child(window_layer, bitmap_layer_get_layer(background_layer));
  
  separator_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SEPARATOR);
  GRect frame = (GRect) {
    .origin = { .x = 53, .y = 105 },
    .size = separator_image->bounds.size
  };
  separator_layer = bitmap_layer_create(frame);
  bitmap_layer_set_bitmap(separator_layer, separator_image);
  layer_add_child(window_layer, bitmap_layer_get_layer(separator_layer));   


    meter_bar_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_METER_BAR);
    GRect frame2 = (GRect) {
      .origin = { .x = 13, .y = 70 },
      .size = meter_bar_image->bounds.size
    };
    meter_bar_layer = bitmap_layer_create(frame2);
    bitmap_layer_set_bitmap(meter_bar_layer, meter_bar_image);
    if (BluetoothVibe == 1)
      layer_add_child(window_layer, bitmap_layer_get_layer(meter_bar_layer)); 

  bluetooth_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BLUETOOTH);

    
  GRect frame3 = (GRect) {
    .origin = { .x = 29, .y = 70 },
    .size = bluetooth_image->bounds.size
  };
  bluetooth_layer = bitmap_layer_create(frame3);
  bitmap_layer_set_bitmap(bluetooth_layer, bluetooth_image);
  layer_add_child(window_layer, bitmap_layer_get_layer(bluetooth_layer));

  battery_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY);
  GRect frame4 = (GRect) {
    .origin = { .x = 13, .y = 51 },
    .size = battery_image->bounds.size
  };
  battery_layer = bitmap_layer_create(frame4);
  battery_image_layer = bitmap_layer_create(frame4);
  bitmap_layer_set_bitmap(battery_image_layer, battery_image);
  layer_set_update_proc(bitmap_layer_get_layer(battery_layer), battery_layer_update_callback);
  
  layer_add_child(window_layer, bitmap_layer_get_layer(battery_image_layer));
  layer_add_child(window_layer, bitmap_layer_get_layer(battery_layer));
  
  GRect frame5 = (GRect) {
    .origin = { .x = 13, .y = 83 },
    .size = {.w = 19, .h = 8}
  };
  time_format_layer = bitmap_layer_create(frame5);
  if (clock_is_24h_style()) {
    time_format_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_24_HOUR_MODE);
    bitmap_layer_set_bitmap(time_format_layer, time_format_image); 
  }
  layer_add_child(window_layer, bitmap_layer_get_layer(time_format_layer));
  tilde_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TILDE);
  GRect frame6 = (GRect) {
    .origin = { .x = 99, .y = 42 },
    .size = tilde_image->bounds.size
  };
  tilde_layer = bitmap_layer_create(frame6);
  bitmap_layer_set_bitmap(tilde_layer, tilde_image);
  layer_add_child(window_layer, bitmap_layer_get_layer(tilde_layer));
  
  sepames_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SEPAMES);
  GRect frame7 = (GRect) {
    .origin = { .x = 101, .y = 79 },
    .size = sepames_image->bounds.size
  };
  sepames_layer = bitmap_layer_create(frame7);
  bitmap_layer_set_bitmap(sepames_layer, sepames_image);
  layer_add_child(window_layer, bitmap_layer_get_layer(sepames_layer));
  
  
  // Crea capa de fecha y hora
  GRect dummy_frame = { {0, 0}, {0, 0} };
  day_name_layer = bitmap_layer_create(dummy_frame);
  layer_add_child(window_layer, bitmap_layer_get_layer(day_name_layer));
   
  for (int i = 0; i < TOTAL_TIME_DIGITS; ++i) {
    time_digits_layers[i] = bitmap_layer_create(dummy_frame);
    layer_add_child(window_layer, bitmap_layer_get_layer(time_digits_layers[i]));
  }
  
  for (int i = 0; i < TOTAL_SEG_DIGITS; ++i) {
    seg_digits_layers[i] = bitmap_layer_create(dummy_frame);
    if (SEGUNDOS==1)
      layer_add_child(window_layer, bitmap_layer_get_layer(seg_digits_layers[i]));
  }  
  
  for (int i = 0; i < TOTAL_DATE_DIGITS; ++i) {
    date_digits_layers[i] = bitmap_layer_create(dummy_frame);
    layer_add_child(window_layer, bitmap_layer_get_layer(date_digits_layers[i]));
  }
  
  for (int i = 0; i < TOTAL_DATE_DIGITS; ++i) {
    month_digits_layers[i] = bitmap_layer_create(dummy_frame);
    layer_add_child(window_layer, bitmap_layer_get_layer(month_digits_layers[i]));
  }
  
  for (int i = 0; i < TOTAL_YEAR_DIGITS; ++i) {
    year_digits_layers[i] = bitmap_layer_create(dummy_frame);
    layer_add_child(window_layer, bitmap_layer_get_layer(year_digits_layers[i]));
  }
  
  for (int i = 0; i < TOTAL_BATTERY_PERCENT_DIGITS; ++i) {
    battery_percent_layers[i] = bitmap_layer_create(dummy_frame);
    layer_add_child(window_layer, bitmap_layer_get_layer(battery_percent_layers[i]));
  }
    
  toggle_bluetooth_icon(bluetooth_connection_service_peek());
  update_battery(battery_state_service_peek());

  appStarted = true;
  
  // Avoids a blank screen on watch start.
  time_t now = time(NULL);
  struct tm *tick_time = localtime(&now);  
  handle_tick(tick_time, YEAR_UNIT + MONTH_UNIT + DAY_UNIT + HOUR_UNIT + MINUTE_UNIT + SECOND_UNIT);

  tick_timer_service_subscribe(SECOND_UNIT, handle_tick);
  
  
  if(SEGUNDOS)
    tick_timer_service_subscribe(SECOND_UNIT, handle_tick);
  else
    tick_timer_service_subscribe(MINUTE_UNIT, handle_tick);

  bluetooth_connection_service_subscribe(bluetooth_connection_callback);
  battery_state_service_subscribe(&update_battery);
  app_message_register_inbox_received((AppMessageInboxReceived) in_recv_handler);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

}


static void deinit(void) {

  tick_timer_service_unsubscribe();
  bluetooth_connection_service_unsubscribe();
  battery_state_service_unsubscribe();

  layer_remove_from_parent(bitmap_layer_get_layer(background_layer));
  bitmap_layer_destroy(background_layer);
  gbitmap_destroy(background_image);
  background_image = NULL;
  
  layer_remove_from_parent(bitmap_layer_get_layer(separator_layer));
  bitmap_layer_destroy(separator_layer);
  gbitmap_destroy(separator_image);
  separator_image = NULL;
  
  layer_remove_from_parent(bitmap_layer_get_layer(meter_bar_layer));
  bitmap_layer_destroy(meter_bar_layer);
  gbitmap_destroy(meter_bar_image);
  background_image = NULL;
	
  layer_remove_from_parent(bitmap_layer_get_layer(bluetooth_layer));
  bitmap_layer_destroy(bluetooth_layer);
  gbitmap_destroy(bluetooth_image);
  bluetooth_image = NULL;
	
  layer_remove_from_parent(bitmap_layer_get_layer(battery_layer));
  bitmap_layer_destroy(battery_layer);
  gbitmap_destroy(battery_image);
  battery_image = NULL;
  
  layer_remove_from_parent(bitmap_layer_get_layer(tilde_layer));
  bitmap_layer_destroy(tilde_layer);
  gbitmap_destroy(tilde_image);
  tilde_image = NULL;
  
  layer_remove_from_parent(bitmap_layer_get_layer(sepames_layer));
  bitmap_layer_destroy(sepames_layer);
  gbitmap_destroy(sepames_image);
  sepames_image = NULL;
	
  layer_remove_from_parent(bitmap_layer_get_layer(battery_image_layer));
  bitmap_layer_destroy(battery_image_layer);

  layer_remove_from_parent(bitmap_layer_get_layer(time_format_layer));
  bitmap_layer_destroy(time_format_layer);
  gbitmap_destroy(time_format_image);
  time_format_image = NULL;
	
  layer_remove_from_parent(bitmap_layer_get_layer(day_name_layer));
  bitmap_layer_destroy(day_name_layer);
  gbitmap_destroy(day_name_image);
  day_name_image = NULL;

  for (int i = 0; i < TOTAL_DATE_DIGITS; i++) {
    layer_remove_from_parent(bitmap_layer_get_layer(date_digits_layers[i]));
    gbitmap_destroy(date_digits_images[i]);
    date_digits_images[i] = NULL;
    bitmap_layer_destroy(date_digits_layers[i]);
	  date_digits_layers[i] = NULL;
  }
  
  for (int i = 0; i < TOTAL_DATE_DIGITS; i++) {
    layer_remove_from_parent(bitmap_layer_get_layer(month_digits_layers[i]));
    gbitmap_destroy(month_digits_images[i]);
    month_digits_images[i] = NULL;
    bitmap_layer_destroy(month_digits_layers[i]);
	  month_digits_layers[i] = NULL;
  }

  for (int i = 0; i < TOTAL_TIME_DIGITS; i++) {
    layer_remove_from_parent(bitmap_layer_get_layer(time_digits_layers[i]));
    gbitmap_destroy(time_digits_images[i]);
    time_digits_images[i] = NULL;
    bitmap_layer_destroy(time_digits_layers[i]);
	time_digits_layers[i] = NULL;
  }
  
  for (int i = 0; i < TOTAL_SEG_DIGITS; i++) {
    layer_remove_from_parent(bitmap_layer_get_layer(seg_digits_layers[i]));
    gbitmap_destroy(seg_digits_images[i]);
    seg_digits_images[i] = NULL;
    bitmap_layer_destroy(seg_digits_layers[i]);
	  seg_digits_layers[i] = NULL;
  }
  
  for (int i = 0; i < TOTAL_YEAR_DIGITS; i++) {
    layer_remove_from_parent(bitmap_layer_get_layer(year_digits_layers[i]));
    gbitmap_destroy(year_digits_images[i]);
    year_digits_images[i] = NULL;
    bitmap_layer_destroy(year_digits_layers[i]);
	year_digits_layers[i] = NULL;
  }

  for (int i = 0; i < TOTAL_BATTERY_PERCENT_DIGITS; i++) {
    layer_remove_from_parent(bitmap_layer_get_layer(battery_percent_layers[i]));
    gbitmap_destroy(battery_percent_image[i]);
    battery_percent_image[i] = NULL;
    bitmap_layer_destroy(battery_percent_layers[i]); 
	battery_percent_layers[i] = NULL;
  } 
	
  layer_remove_from_parent(window_layer);
  layer_destroy(window_layer);
	
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}