#pragma once

#ifndef _JSON_PUT_H_
#define _JSON_PUT_H_

/* To detect incompatible changes you can define JSON_PUT_VERSION_REQUIRED to
 * the current value of JSON_PUT_VERSION before including this file (or via
 * compiler command line):
 *
 *   #define JSON_PUT_VERSION_REQUIRED 1
 *   #include <json_put.h>
 *
 * In that case compilation will fail when included file has incompatible
 * version.
 */
#define JSON_PUT_VERSION 1
#if defined(JSON_PUT_VERSION_REQUIRED)
	#if JSON_PUT_VERSION_REQUIRED != JSON_PUT_VERSION
		#error different json_put version required
	#endif
#endif

#include <stddef.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Output callback invoked by json_put functions. Must return 0 on success, any
 * other value on error. Error value is preserved and can be obtained with
 * json_put_error() function. Parameters:
 * - buffer points to the buffer implementation (e.g. FILE, std::string, etc.)
 * - s is a char sequence to add to the output
 * - len is a length of s
 *
 * Example:
 *   int output_to_file(void *buffer, const char *s, size_t len)
 *   {
 *       return len == fwrite((FILE *)buffer, 1, len, s)? 0: -1;
 *   }
 */
typedef int (*json_put_to_cb)(void *buffer, const char *s, size_t len);

/* JSON writer. Must be initialized using json_put_init() function. Example:
 *   json_put jp;
 *   json_put_init(&jp, stdout, output_to_file);
 */
struct json_put
{
	int _err;
	int _flags;
	void *_buffer;
	json_put_to_cb _put;
};

/* Initializes the write. Writer will use buffer with put output function.
 */
void json_put_init(struct json_put *const jp,
				   void *const buffer, const json_put_to_cb put);

/* Currently does nothing. Could be used to flush writers inertnal buffers (but
 * no such thing exists right now). Internal buffer could be used to reduce the
 * number of output callback invocations. However, not clear whether such
 * optimization is necessary.
 */
static inline
int json_put_fin(struct json_put *const jp) { return jp->_err; }

/* Returns error from the last output callback (or 0 if there are was no error).
 * Once output callbacks returns an error, all json_put functions will do
 * nothing, but return that error. That simplifies error handling. Example:
 *   json_put_object_begin(&jp);
 *   json_put_name(&jp, "key");
 *   json_put_value(&jp, "value");
 *   json_put_object_end(&jp);
 *   if (0 != json_put_fin(&jp))
 *   {
 *       fprintf(stderr, "Error %i\n", json_put_error(&jp));
 *   }
 */
static inline
int json_put_error(const struct json_put *const jp) { return jp->_err; }

int json_put_object_begin(struct json_put *const jp);
int json_put_object_end(struct json_put *const jp);
int json_put_array_begin(struct json_put *const jp);
int json_put_array_end(struct json_put *const jp);
int json_put_name(struct json_put *const jp, const char *const name);
int json_put_name_len(struct json_put *const jp, const char *const name,
					  const size_t len);
int json_put_value(struct json_put *const jp, const char *const value);
int json_put_value_len(struct json_put *const jp, const char *const value,
					   const size_t len);

/* Simple memory buffer. */
struct json_put_buffer
{
	char *b;
	char *e;
	char *p;
};

/* Returns pointer on data contained in that memory buffer and its size.
 */
static inline
char *json_put_buffer_data(const struct json_put_buffer *const buf,
						   size_t *const len)
{
	*len = buf->p - buf->b;
	return buf->b;
}

/* Initialize memory buffer with preallocated array. Example:
 *   char json[512];
 *   json_put jp;
 *   json_put_buffer jp_buf;
 *   json_put_static_buffer_init(&jp_buf, json, sizeof(json));
 *   json_put_init(&jp, &jp_buf, json_put_to_static_buffer);
 *   // ...
 *   size_t json_len;
 *   json_put_buffer_data(&jp_buf, &json_len);
 *   fwrite(json, 1, json_len, stdout);
 */
void json_put_static_buffer_init(struct json_put_buffer *const buf,
								 char *const p, const size_t len);
int json_put_to_static_buffer(void *buffer, const char *s, size_t len);

/* Initialize memory buffer with dynamically allocated array. Array will grow
 * when necessary. Example:
 *   json_put jp;
 *   json_put_buffer jp_buf;
 *   json_put_heap_buffer_alloc(&jp_buf);
 *   json_put_init(&jp, &jp_buf, json_put_to_heap_buffer);
 *   // ...
 *   size_t json_len;
 *   char *const json = json_put_buffer_data(&jp_buf, &json_len);
 *   fwrite(json, 1, json_len, stdout);
 *   json_put_heap_buffer_free(&jp_buf);
 */
int json_put_heap_buffer_alloc(struct json_put_buffer *const buf);
void json_put_heap_buffer_free(struct json_put_buffer *const buf);
int json_put_to_heap_buffer(void *buffer, const char *s, size_t len);

#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus
#include <string>
/* Example:
 *   std::string buf;
 *   json_put jp;
 *   json_put_init(&jp, &buf, json_put_to_std_string);
 *   // ...
 *   printf("JSON: %s\n", buf.c_str());
 */
static inline
int json_put_to_std_string(void *buffer, const char *s, size_t len)
{
	((std::string *)buffer)->append(s, len);
	return 0;
}
#endif

#endif
