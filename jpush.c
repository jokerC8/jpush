#include <string.h>
#include <stdio.h>
#include "jpush.h"

#include <curl/curl.h>
#include <json-c/json.h>

struct json_object *jpushInit()
{
	struct json_object *jpush;

	jpush = json_object_new_object();
	return jpush;
}

int jpush_setPlatform(struct json_object *jpush, int platform)
{
	struct json_object *jpush_platform;

	if (!jpush) return -1;

	if (J_PUSH_PLATFORM_ALL == platform) {
		json_object_object_add(jpush, "platform", json_object_new_string("all"));
		return 0;
	}
	jpush_platform = json_object_new_array();
	if (J_PUSH_PLATFORM_ANDROID & platform)
		json_object_array_add(jpush_platform, json_object_new_string("android"));
	if (J_PUSH_PLATFORM_IOS & platform)
		json_object_array_add(jpush_platform, json_object_new_string("ios"));
	if (platform & J_PUSH_PLATFORM_WINPHONE)
		json_object_array_add(jpush_platform, json_object_new_string("winphone"));

	json_object_object_add(jpush, "platform", jpush_platform);
	return 0;
}

int jpush_setAudience(struct json_object *jpush, const char *type, char **value, int count)
{
	int i;
	struct json_object *jpush_audience, *temp;
	if (!jpush || !type || !value) return -1;

	if (!strcmp(type, "all")) {
		json_object_object_add(jpush, "audience", json_object_new_string("all"));
		return 0;
	}
	jpush_audience = json_object_object_get(jpush, "audience");
	if (!jpush_audience) {
		jpush_audience = json_object_new_object();
		json_object_object_add(jpush, "audience", jpush_audience);
	}
	temp = json_object_new_array();
	for (i = 0; i < count; i++) {
		json_object_array_add(temp, json_object_new_string(value[i]));
	}
	json_object_object_add(jpush_audience, type, temp);

	return 0;
}

int jpush_setAndroidAPN(struct json_object *jpush, const char *type, void *value)
{
	struct json_object *jpush_notification, *android;

	if (!jpush || !type) return -1;

	/* for all platform */
	if (!strcmp(type, "all")) {
		jpush_notification = json_object_object_get(jpush, "notification");
		if (!jpush_notification) {
			jpush_notification = json_object_new_object();
			json_object_object_add(jpush, "notification", jpush_notification);
		}
		json_object_object_add(jpush_notification, "alert", json_object_new_string((char *)value));
		return 0;
	}

	jpush_notification = json_object_object_get(jpush, "notification");
	if (!jpush_notification) {
		jpush_notification = json_object_new_object();
		json_object_object_add(jpush, "notification", jpush_notification);
	}

	android = json_object_object_get(jpush_notification, "android");
	if (!android) {
		android = json_object_new_object();
		json_object_object_add(jpush_notification, "android", android);
	}

	if (!strcmp(type, "alert") || !strcmp(type, "title") || !strcmp(type, "channel_id") || !strcmp(type, "category") ||
			!strcmp(type, "big_text") || !strcmp(type, "big_pic_path") || !strcmp(type, "large_icon"))
		json_object_object_add(android, type, json_object_new_string((char *)value));
	else if (!strcmp(type, "builder_id") || !strcmp(type, "priority") || !strcmp(type, "style") || !strcmp(type, "alert_type"))
		json_object_object_add(android, type, json_object_new_int(*((int *)value)));
	else if (!strcmp(type, "inbox") || !strcmp(type, "extras") || !strcmp(type, "intent"))
		json_object_object_add(android, type, (struct json_object *)value);
	return 0;
}

int jpush_setIOSAPN(struct json_object *jpush, const char *type, void *value)
{
	struct json_object *jpush_notification, *ios;

	if (!jpush || !type) return -1;

	/* for all platform */
	if (!strcmp(type, "all")) {
		jpush_notification = json_object_object_get(jpush, "notification");
		if (!jpush_notification) {
			jpush_notification = json_object_new_object();
			json_object_object_add(jpush, "notification", jpush_notification);
		}
		json_object_object_add(jpush_notification, "alert", json_object_new_string((char *)value));
		return 0;
	}

	jpush_notification = json_object_object_get(jpush, "notification");
	if (!jpush_notification) {
		jpush_notification = json_object_new_object();
		json_object_object_add(jpush, "notification", jpush_notification);
	}

	ios = json_object_object_get(jpush_notification, "ios");
	if (!ios) {
		ios = json_object_new_object();
		json_object_object_add(jpush_notification, "ios", ios);
	}

	if (!strcmp(type, "alert") || !strcmp(type, "sound") || !strcmp(type, "category") || !strcmp(type, "thread-id"))
		json_object_object_add(ios, type, json_object_new_string((char *)value));
	else if (!strcmp(type, "badge") || !strcmp(type, "content-available") || !strcmp(type, "mutable-content"))
		json_object_object_add(ios, type, json_object_new_int(*((int *)value)));
	else if (!strcmp(type, "extras"))
		json_object_object_add(ios, type, (struct json_object *)value);
	return 0;

}

int jpush_setWinphoneAPN(struct json_object *jpush, const char *type, void *value)
{
	struct json_object *jpush_notification, *winphone;

	if (!jpush || !type) return -1;

	/* for all platform */
	if (!strcmp(type, "all")) {
		jpush_notification = json_object_object_get(jpush, "notification");
		if (!jpush_notification) {
			jpush_notification = json_object_new_object();
			json_object_object_add(jpush, "notification", jpush_notification);
		}
		json_object_object_add(jpush_notification, "alert", json_object_new_string((char *)value));
		return 0;
	}

	jpush_notification = json_object_object_get(jpush, "notification");
	if (!jpush_notification) {
		jpush_notification = json_object_new_object();
		json_object_object_add(jpush, "notification", jpush_notification);
	}

	winphone = json_object_object_get(jpush_notification, "winphone");
	if (!winphone) {
		winphone = json_object_new_object();
		json_object_object_add(jpush_notification, "winphone", winphone);
	}

	if (!strcmp(type, "alert") || !strcmp(type, "sound") || !strcmp(type, "_open_page"))
		json_object_object_add(winphone, type, json_object_new_string((char *)value));
	else if (!strcmp(type, "extras"))
		json_object_object_add(winphone, type, (struct json_object *)value);
	return 0;
}

int jpush_setMessage(struct json_object *jpush, char *msg_content, char *title, char *content_type, struct json_object *extras)
{
	struct json_object *message;

	if (!jpush) return -1;

	message = json_object_new_object();
	if (msg_content)
		json_object_object_add(message, "msg_content", json_object_new_string(msg_content));
	if (title)
		json_object_object_add(message, "title", json_object_new_string(title));
	if (content_type)
		json_object_object_add(message, "content_type", json_object_new_string(content_type));
	if (extras)
		json_object_object_add(message, "extras", extras);

	json_object_object_add(jpush, "message", message);
	return 0;
}

int jpush_setSmsMessage(struct json_object *jpush, int delay_time, int signid, long temp_id, int active_filter, struct json_object *temp_para)
{
	struct json_object *smsmessage;

	if (!jpush) return -1;

	smsmessage = json_object_new_object();
	if (J_PUSH_INT_NULL != delay_time)
		json_object_object_add(smsmessage, "delay_time", json_object_new_int(delay_time));
	if (J_PUSH_INT_NULL != signid)
		json_object_object_add(smsmessage, "signid", json_object_new_int(signid));
	if (J_PUSH_INT_NULL != temp_id)
		json_object_object_add(smsmessage, "temp_id", json_object_new_int(temp_id));
	if (J_PUSH_INT_NULL != active_filter)
		json_object_object_add(smsmessage, "active_filter", json_object_new_int(active_filter));
	if (J_PUSH_STRING_NULL != temp_para)
		json_object_object_add(smsmessage, "temp_para", (struct json_object *)temp_para);

	json_object_object_add(jpush, "sms_message", smsmessage);
	return 0;
}

int jpush_setOptions(struct json_object *jpush, int sendno, int time_to_live, long override_msg_id, int apns_production, char *apns_collapse_id, int big_push_duration)
{
	struct json_object *options;

	if (!jpush) return -1;

	options = json_object_new_object();
	if (J_PUSH_INT_NULL != sendno)
		json_object_object_add(options, "sendno", json_object_new_int(sendno));
	if (J_PUSH_INT_NULL != time_to_live)
		json_object_object_add(options, "time_to_live", json_object_new_int(time_to_live));
	if (J_PUSH_INT_NULL != override_msg_id)
		json_object_object_add(options, "override_msg_id", json_object_new_int(override_msg_id));
	if (J_PUSH_INT_NULL != apns_production)
		json_object_object_add(options, "apns_production", json_object_new_boolean(1));
	if (J_PUSH_STRING_NULL != apns_collapse_id)
		json_object_object_add(options, "apns_collapse_id", json_object_new_string(apns_collapse_id));
	if (J_PUSH_INT_NULL != big_push_duration)
		json_object_object_add(options, "big_push_duration", json_object_new_int(big_push_duration));

	json_object_object_add(jpush, "options", options);
	return 0;
}

size_t jpushWriteData(void *ptr, size_t size, size_t nmemb, void *stream)
{
	fwrite(ptr, size, nmemb, stream);
    return size * nmemb;
}

void jpushEval(struct json_object *jpush)
{
    const char *postData = json_object_to_json_string(jpush); 

    CURL *curl;
    // CURLcode res;
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, J_PUSH_API);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
    curl_easy_setopt(curl, CURLOPT_USERPWD, J_PUSH_APP_KEY":"J_PUSH_SECRET);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, jpushWriteData);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, stdout);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
}

int jpush_setcid(struct json_object *jpush, const char *value)
{
	if (!jpush || !value) return -1;

	json_object_object_add(jpush, "cid", json_object_new_string(value));

	return 0;
}

void jpushClean(json_object *jpush)
{
	json_object_put(jpush);
}
