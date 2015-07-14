# json_put
Simple json writer for C (and C++)

Example (using fixed preallocated array):

```c
char json[128];
size_t json_len;
struct json_put jp;
struct json_put_buffer jp_buf;
json_put_static_buffer_init(&buf, json, sizeof(json));
json_put_init(&jp, &jp_buf, json_put_to_static_buffer);
/* write some json */
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
/* print generated string */
json_put_buffer_data(&jp_buf, &json_len);
fwrite(json, 1, json_len, stdout);
fprintf(stdout, "\n");
```

Example (using std::string from C++):

```c++
std::string json;
json_put jp;
json_put_init(&jp, &json, json_put_to_std_string);
// write some json
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
// print generated string
printf("%s\n", json.c_str());
```

Both examples will print:

```json
{"numbers":["1","2","3"],"key":"42"}
```
