#include <include/type.h>

const corto_string ETC_PATH = "/usr/local/etc/corto/1.3/type";

int DumbRandom(int min, int max){
   return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

void QuickWeatherUpdate(type_Weather weather, int temp, int humidity, int uv) {
    corto_time now;
    corto_timeGet(&now);
    if (type_WeatherUpdate(
        weather,
        DumbRandom(temp-20, temp+20),
        DumbRandom(humidity-10, humidity+10),
        DumbRandom(uv-3, 12),
        &now)) {
        corto_error("Weather update failed. Error: %s", corto_lasterr());
    }
}

int initStore(void) {
    corto_time now;
    corto_timeGet(&now);

    corto_object data = corto_voidCreateChild(root_o, "data");
    corto_object weather = corto_voidCreateChild(data, "weather");
    type_State kentucky = type_StateCreateChild(weather, "kentucky", "south", true);
    type_State texas =  type_StateCreateChild(weather, "texas", "southwest", false);
    type_State california =  type_StateCreateChild(weather, "california", "west", false);
    type_City houston = type_CityCreateChild(texas, "houston", 1837, 8538000);
    type_City lexington = type_CityCreateChild(kentucky, "lexington", 1782, 318449);
    type_City nicholasville = type_CityCreateChild(kentucky, "nicholasville", 1798, 30006);
    type_City sanDiego = type_CityCreateChild(california, "San Diego", 1769, 1407000);

    type_Weather weatherH = type_WeatherCreateChild(houston, "weather", 82, 45.5, 8, &now);
    type_Weather weatherL = type_WeatherCreateChild(lexington, "weather", 95, 78.8, 6, &now);
    type_Weather weatherN = type_WeatherCreateChild(nicholasville, "weather", 45, 47, 4, &now);
    type_Weather weatherS = type_WeatherCreateChild(sanDiego, "weather", 50, 48, 6, &now);

    sleep(2);

    int cnt = 0;
    while (cnt < 2) {
        cnt++;
        QuickWeatherUpdate(weatherL, 82, 45, 8);
        QuickWeatherUpdate(weatherH, 95, 78, 6);
        QuickWeatherUpdate(weatherN, 45, 47, 4);
        QuickWeatherUpdate(weatherS, 50, 48, 6);
        usleep(1000);
    }

    corto_release(data);
    corto_release(weather);
    corto_release(kentucky);
    corto_release(texas);
    corto_release(california);
    corto_release(houston);
    corto_release(lexington);
    corto_release(nicholasville);
    corto_release(sanDiego);

    return 0;
}

int select(void) {
    corto_iter it;

    corto_select("//")
        .from("/data/weather")
        .type(corto_fullpath(NULL, (corto_type)type_Weather_o))
        .iter(&it);

    int cnt = 0;
    while(corto_iter_hasNext(&it) == 1) {
        cnt++;
        corto_result *r = (corto_result*)corto_iter_next(&it);
        corto_trace("Type Query Parent[%s] ID[%s]", r->parent, r->id);
    }

    if (cnt != 4) {
        corto_error("Received [%d] of [4]", cnt);
    } else {
        corto_info("Received all [%d] nodes", cnt);
    }

    return 0;
}

int typeMain(int argc, char *argv[]) {
    corto_string file = "filestore.json";
    corto_string path = corto_asprintf("%s/%s", ETC_PATH, file);
    if (corto_load(path, 0, NULL) != 0)
    {
        corto_error("Failed to load [%s] - Error [%s]", path, corto_lasterr());
        corto_dealloc(path);
        return -1;
    }
    corto_dealloc(path);

    initStore();

    select();

    return 0;
}
