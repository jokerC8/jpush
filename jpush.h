#ifndef _JPUSH_H
#define _JPUSH_H

#include <json-c/json.h>

#define J_PUSH_APP_KEY "a71dc97a060356d5539d784a" // AppKey
#define J_PUSH_SECRET "208489dbf47b172c597fd7f5" // Secret 
#define J_PUSH_API "https://api.jpush.cn/v3/push" // push interface

#define J_PUSH_JSON_NULL NULL
#define J_PUSH_STRING_NULL NULL
#define J_PUSH_INT_NULL -1000
typedef enum {
	J_PUSH_PLATFORM_ANDROID = 1 << 0,
	J_PUSH_PLATFORM_IOS = 1 << 1,
	J_PUSH_PLATFORM_WINPHONE = 1 << 2,
	J_PUSH_PLATFORM_ALL = 1 << 3,
} E_Platform;

struct json_object *jpushInit();
void jpushClean(struct json_object *jpush);
int jpush_setPlatform(struct json_object *jpush, int platform);
int jpush_setAudience(struct json_object *jpush, const char *type, char **value, int count);
int jpush_setAndroidAPN(struct json_object *json, const char *type, void *value);
int jpush_setIOSAPN(struct json_object *json, const char *type, void *value);
int jpush_setWinphoneAPN(struct json_object *json, const char *type, void *value);
int jpush_setMessage(struct json_object *jpush, char *msg_content, char *title, char *content_type, struct json_object *extras);
int jpush_setSmsMessage(struct json_object *jpush, int delay_time, int signid, long temp_id, int active_filter, struct json_object *temp_para);
int jpush_setOptions(struct json_object *jpush, int sendno, int time_to_live, long override_msg_id, int apns_production, char *apns_collapse_id, int big_push_duration);
void jpushEval(struct json_object *jpush);
#endif
