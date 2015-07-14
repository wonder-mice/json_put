#include <stdio.h>
#include <json_put.h>

static int g_errors = 0;

#define JSON_PUT_TEST_BEGIN() \
	char json[256]; \
	size_t json_len; \
	struct json_put jp; \
	struct json_put_buffer buf; \
	json_put_static_buffer_init(&buf, json, sizeof(json)); \
	json_put_init(&jp, &buf, json_put_to_static_buffer);

#define JSON_PUT_TEST_END(expected) \
	json_put_buffer_data(&buf, &json_len); \
	json[json_len] = 0; \
	if (0 != strcmp((expected), json)) { \
		fprintf(stderr, "Error at %s, line %i:\n", __FUNCTION__, __LINE__); \
		fprintf(stderr, "    expected: \"%s\"\n", (expected)); \
		fprintf(stderr, "      actual: \"%s\"\n", json); \
		++g_errors; \
	}


static void test_empty_object()
{
	JSON_PUT_TEST_BEGIN()
	json_put_object_begin(&jp);
	json_put_object_end(&jp);
	JSON_PUT_TEST_END("{}")
}

static void test_mono_object()
{
	JSON_PUT_TEST_BEGIN()
	json_put_object_begin(&jp);
	json_put_name(&jp, "key");
	json_put_string(&jp, "value");
	json_put_object_end(&jp);
	JSON_PUT_TEST_END("{\"key\":\"value\"}")
}

static void test_multi_object()
{
	JSON_PUT_TEST_BEGIN()
	json_put_object_begin(&jp);
	json_put_name(&jp, "key1");
	json_put_string(&jp, "value1");
	json_put_name(&jp, "key2");
	json_put_string(&jp, "value2");
	json_put_name(&jp, "key3");
	json_put_string(&jp, "value3");
	json_put_object_end(&jp);
	JSON_PUT_TEST_END("{\"key1\":\"value1\",\"key2\":\"value2\",\"key3\":\"value3\"}")
}

static void test_empty_array()
{
	JSON_PUT_TEST_BEGIN()
	json_put_array_begin(&jp);
	json_put_array_end(&jp);
	JSON_PUT_TEST_END("[]")
}

static void test_mono_array()
{
	JSON_PUT_TEST_BEGIN()
	json_put_array_begin(&jp);
	json_put_string(&jp, "value");
	json_put_array_end(&jp);
	JSON_PUT_TEST_END("[\"value\"]")
}

static void test_multi_array()
{
	JSON_PUT_TEST_BEGIN()
	json_put_array_begin(&jp);
	json_put_string(&jp, "value1");
	json_put_string(&jp, "value2");
	json_put_string(&jp, "value3");
	json_put_array_end(&jp);
	JSON_PUT_TEST_END("[\"value1\",\"value2\",\"value3\"]")
}

static void test_array_inside_object()
{
	JSON_PUT_TEST_BEGIN()
	json_put_object_begin(&jp);
	json_put_name(&jp, "key1");
		json_put_array_begin(&jp);
			json_put_string(&jp, "a");
			json_put_string(&jp, "b");
			json_put_string(&jp, "c");
		json_put_array_end(&jp);
	json_put_name(&jp, "key2");
		json_put_array_begin(&jp);
			json_put_string(&jp, "d");
			json_put_string(&jp, "e");
			json_put_string(&jp, "f");
		json_put_array_end(&jp);
	json_put_name(&jp, "key3");
		json_put_array_begin(&jp);
			json_put_string(&jp, "g");
			json_put_string(&jp, "h");
			json_put_string(&jp, "i");
		json_put_array_end(&jp);
	json_put_object_end(&jp);
	JSON_PUT_TEST_END("{\"key1\":[\"a\",\"b\",\"c\"],"
					  "\"key2\":[\"d\",\"e\",\"f\"],"
					  "\"key3\":[\"g\",\"h\",\"i\"]}")
}

static void test_array_object_inside_array()
{
	JSON_PUT_TEST_BEGIN()
	json_put_array_begin(&jp);
		json_put_object_begin(&jp);
			json_put_name(&jp, "k1");
			json_put_string(&jp, "a");
			json_put_name(&jp, "k2");
			json_put_string(&jp, "b");
			json_put_name(&jp, "k3");
			json_put_string(&jp, "c");
		json_put_object_end(&jp);
		json_put_object_begin(&jp);
			json_put_name(&jp, "k4");
			json_put_string(&jp, "d");
			json_put_name(&jp, "k5");
			json_put_string(&jp, "e");
			json_put_name(&jp, "k6");
			json_put_string(&jp, "f");
		json_put_object_end(&jp);
		json_put_object_begin(&jp);
			json_put_name(&jp, "k7");
			json_put_string(&jp, "g");
			json_put_name(&jp, "k8");
			json_put_string(&jp, "h");
			json_put_name(&jp, "k9");
			json_put_string(&jp, "i");
		json_put_object_end(&jp);
	json_put_array_end(&jp);
	JSON_PUT_TEST_END("[{\"k1\":\"a\",\"k2\":\"b\",\"k3\":\"c\"},"
					  "{\"k4\":\"d\",\"k5\":\"e\",\"k6\":\"f\"},"
					  "{\"k7\":\"g\",\"k8\":\"h\",\"k9\":\"i\"}]")
}

static void test_mix()
{
	JSON_PUT_TEST_BEGIN()
	json_put_object_begin(&jp);
		json_put_name(&jp, "menu");
		json_put_object_begin(&jp);
			json_put_name(&jp, "id");
			json_put_string(&jp, "file");
			json_put_name(&jp, "value");
			json_put_string(&jp, "File");
			json_put_name(&jp, "popup");
			json_put_object_begin(&jp);
				json_put_name(&jp, "menuitem");
				json_put_array_begin(&jp);
					json_put_object_begin(&jp);
						json_put_name(&jp, "value");
						json_put_string(&jp, "New");
						json_put_name(&jp, "onclick");
						json_put_string(&jp, "CreateNewDoc()");
					json_put_object_end(&jp);
					json_put_object_begin(&jp);
						json_put_name(&jp, "value");
						json_put_string(&jp, "Open");
						json_put_name(&jp, "onclick");
						json_put_string(&jp, "OpenDoc()");
					json_put_object_end(&jp);
					json_put_object_begin(&jp);
						json_put_name(&jp, "value");
						json_put_string(&jp, "Close");
						json_put_name(&jp, "onclick");
						json_put_string(&jp, "CloseDoc()");
					json_put_object_end(&jp);
				json_put_array_end(&jp);
			json_put_object_end(&jp);
		json_put_object_end(&jp);
	json_put_object_end(&jp);
	JSON_PUT_TEST_END(
			"{\"menu\":{\"id\":\"file\",\"value\":\"File\",\"popup\":"
			"{\"menuitem\":[{\"value\":\"New\",\"onclick\":\"CreateNewDoc()\"},"
			"{\"value\":\"Open\",\"onclick\":\"OpenDoc()\"},{\"value\":\"Close"
			"\",\"onclick\":\"CloseDoc()\"}]}}}")
}

static void test_escapes()
{
	JSON_PUT_TEST_BEGIN()
	json_put_array_begin(&jp);
	json_put_string_len(&jp, "\0\b\f\n\r\t\"\\", 8);
	json_put_array_end(&jp);
	JSON_PUT_TEST_END("[\"\\0\\b\\f\\n\\r\\t\\\"\\\\\"]")
}

static void test_numbers()
{
	JSON_PUT_TEST_BEGIN()
	json_put_array_begin(&jp);
	json_put_uint(&jp, 42);
	json_put_uint(&jp, 0);
	json_put_int(&jp, 123);
	json_put_int(&jp, 0);
	json_put_int(&jp, -7);
	json_put_array_end(&jp);
	JSON_PUT_TEST_END("[42,0,123,0,-7]")
}

int main(int argc, char *argv[])
{
	(void)argc; (void)argv;
	test_empty_object();
	test_mono_object();
	test_multi_object();
	test_empty_array();
	test_mono_array();
	test_multi_array();
	test_array_inside_object();
	test_array_object_inside_array();
	test_mix();
	test_escapes();
	test_numbers();
	return g_errors;
}
