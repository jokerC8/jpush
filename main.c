#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jpush.h"

int pushMessage(char **tag, int count, char *msg)
{
	struct json_object *jpush;

	jpush = jpushInit();
	jpush_setPlatform(jpush, J_PUSH_PLATFORM_IOS | J_PUSH_PLATFORM_ANDROID);
	jpush_setAudience(jpush, "tag", tag, count);
	jpush_setAndroidAPN(jpush, "alert", msg);
	jpush_setIOSAPN(jpush, "alert", msg);
	jpush_setOptions(jpush, J_PUSH_INT_NULL, 10, J_PUSH_INT_NULL, 1, J_PUSH_STRING_NULL, J_PUSH_INT_NULL);
	jpushEval(jpush);
	jpushClean(jpush);
	return 0;
}

int main(void)
{
	int i;
	char *tag[] = {"Hi", "Jpush"};
	for (i = 0; i < 10000; i++) {
		pushMessage(tag, 1, "Hi Jpush!");
		sleep(1);
	}
}
