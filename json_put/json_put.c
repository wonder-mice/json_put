#include <memory.h>
#include <stdlib.h>
#include <json_put.h>

enum _FLAGS
{
	_FLAG_PUT_COMMA = 1 << 0,
	_FLAG_NAME_PUTS_COMMA = 1 << 1,
};

static const char _escapees[0xff] =
{
	['\0'] = '0',   /* Zero */
	['\b'] = 'b',  /* Backspace (ascii code 08) */
	['\f'] = 'f',  /* Form feed (ascii code 0C) */
	['\n'] = 'n',  /* New line */
	['\r'] = 'r',  /* Carriage return */
	['\t'] = 't',  /* Tab */
	['\"'] = '\"', /* Double quote */
	['\\'] = '\\', /* Backslash caracter */
};

static const char  *_escapee(const char *const b, const char *const e)
{
	for(const char *p = b; e != p; ++p)
	{
		if (0 != _escapees[(unsigned char)*p])
		{
			return p;
		}
	}
	return e;
}

static int _put(struct json_put *const jp,
				const char *const s, const size_t len)
{
	if (0 == jp->_err)
	{
		jp->_err = jp->_put(jp->_buffer, s, len);
	}
	return jp->_err;
}

static int _put_escaped(struct json_put *const jp,
						const char *const s, const size_t len)
{
	const char *b = s;
	const char *const e = b + len;
	const char *p;
	char esc[2];
	while(0 == jp->_err)
	{
		p = _escapee(b, e);
		if (b < p)
		{
			_put(jp, b, p - b);
		}
		if (p == e)
		{
			break;
		}
		esc[0] = '\\';
		esc[1] = _escapees[(unsigned char)*p++];
		_put(jp, esc, 2);
		b = p;
	}
	return jp->_err;
}

static int _put_quoted(struct json_put *const jp,
					 const char *const value, const size_t len)
{
	_put(jp, "\"", 1);
	_put_escaped(jp, value, len);
	return _put(jp, "\"", 1);
}

void json_put_init(struct json_put *const jp,
				  void *const buffer, const json_put_to_cb put)
{
	jp->_err = 0;
	jp->_flags = 0;
	jp->_buffer = buffer;
	jp->_put = put;
}

int json_put_object_begin(struct json_put *const jp)
{
	if (_FLAG_PUT_COMMA & jp->_flags && !(_FLAG_NAME_PUTS_COMMA & jp->_flags))
	{
		_put(jp, ",", 1);
	}
	jp->_flags = 0;
	return _put(jp, "{", 1);
}

int json_put_object_end(struct json_put *const jp)
{
	jp->_flags = _FLAG_PUT_COMMA;
	return _put(jp, "}", 1);
}

int json_put_array_begin(struct json_put *const jp)
{
	if (_FLAG_PUT_COMMA & jp->_flags && !(_FLAG_PUT_COMMA & jp->_flags))
	{
		_put(jp, ",", 1);
	}
	jp->_flags = 0;
	return _put(jp, "[", 1);
}

int json_put_array_end(struct json_put *const jp)
{
	jp->_flags = _FLAG_PUT_COMMA;
	return _put(jp, "]", 1);
}

int json_put_name(struct json_put *const jp, const char *const name)
{
	return json_put_name_len(jp, name, strlen(name));
}

int json_put_name_len(struct json_put *const jp, const char *const name,
					  const size_t len)
{
	if (_FLAG_PUT_COMMA & jp->_flags)
	{
		_put(jp, ",", 1);
	}
	jp->_flags |= _FLAG_PUT_COMMA | _FLAG_NAME_PUTS_COMMA;
	_put_quoted(jp, name, len);
	return _put(jp, ":", 1);
}

int json_put_value(struct json_put *const jp, const char *const value)
{
	return	json_put_value_len(jp, value, strlen(value));
}

int json_put_value_len(struct json_put *const jp, const char *const value,
					   const size_t len)
{
	if (_FLAG_PUT_COMMA & jp->_flags && !(_FLAG_NAME_PUTS_COMMA & jp->_flags))
	{
		_put(jp, ",", 1);
	}
	jp->_flags |= _FLAG_PUT_COMMA;
	return _put_quoted(jp, value, len);
}

void json_put_static_buffer_init(struct json_put_buffer *const buf,
								 char *const p, const size_t len)
{
	buf->e = (buf->b = buf->p = p) + len;
}

int json_put_to_static_buffer(void *buffer, const char *s, size_t len)
{
	struct json_put_buffer *const buf = (struct json_put_buffer *)buffer;
	const size_t avail = buf->e - buf->p;
	if (avail < len)
	{
		return -1;
	}
	memcpy(buf->p, s, len);
	buf->p += len;
	return 0;
}

int json_put_heap_buffer_alloc(struct json_put_buffer *const buf)
{
	const size_t buf_sz = 256;
	buf->b = buf->p = buf->e = (char *)malloc(buf_sz);
	if (0 == buf->b)
	{
		return -1;
	}
	buf->e += buf_sz;
	return 0;
}

void json_put_heap_buffer_free(struct json_put_buffer *const buf)
{
	free(buf->b);
}

int json_put_to_heap_buffer(void *buffer, const char *s, size_t len)
{
	struct json_put_buffer *const buf = (struct json_put_buffer *)buffer;
	const size_t avail = buf->e - buf->p;
	if (avail < len)
	{
		const size_t new_sz = 2 * (buf->e - buf->b) + len;
		char *const tmp = (char *)realloc(buf->b, new_sz);
		if (0 == tmp)
		{
			return -1;
		}
		buf->p = tmp + (buf->p - buf->b);
		buf->e = (buf->b = tmp) + new_sz;
	}
	memcpy(buf->p, s, len);
	buf->p += len;
	return 0;
}
