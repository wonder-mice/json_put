#include <stdio.h>
#include <json_put.h>

int main(int argc, char *argv[])
{
	(void)argc; (void)argv;
	struct json_put jp;
	struct json_put_buffer jp_buf;
	json_put_heap_buffer_alloc(&jp_buf);
	json_put_init(&jp, &jp_buf, json_put_to_heap_buffer);
	json_put_object_begin(&jp);
		json_put_name(&jp, "numbers");
		json_put_array_begin(&jp);
			json_put_string(&jp, "1");
			json_put_string(&jp, "2");
			json_put_string(&jp, "3");
		json_put_array_end(&jp);
		json_put_name(&jp, "key");
		json_put_string(&jp, "42");
	json_put_object_end(&jp);
	json_put_to_heap_buffer(&jp_buf, "\0", 1);
	printf("JSON: %s\n", jp_buf.b);
	json_put_heap_buffer_free(&jp_buf);
	return 0;
}
