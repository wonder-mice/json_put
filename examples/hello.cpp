#include<string>
#include <json_put.h>

int main(int argc, char *argv[])
{
	(void)argc; (void)argv;
	std::string json;
	json_put jp;
	json_put_init(&jp, &json, json_put_to_std_string);
	json_put_object_begin(&jp);
		json_put_name(&jp, "numbers");
		json_put_array_begin(&jp);
			json_put_value(&jp, "1");
			json_put_value(&jp, "2");
			json_put_value(&jp, "3");
		json_put_array_end(&jp);
		json_put_name(&jp, "key");
		json_put_value(&jp, "42");
	json_put_object_end(&jp);
	printf("JSON: %s\n", json.c_str());
	return 0;
}
