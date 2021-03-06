/* Copyright (c) 2013 Fu Haiping <haipingf@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/custom.h>
#include <caml/fail.h>
#include <caml/mlvalues.h>
#include <caml/memory.h>

#include "http_parser_stubs.h"

#include "http-parser/http_parser.h"

#define caml_http_parser_struct_val(v) \
  (*(caml_http_parser_t **)Data_custom_val(v))

/** Parser Type */
static const enum http_parser_type HTTP_PARSER_TYPE_TABLE[] = {
  HTTP_REQUEST,
  HTTP_RESPONSE,
  HTTP_BOTH
};

/* Request Methods */
static const enum http_method HTTP_METHOD_TABLE[] = {
  HTTP_DELETE,
  HTTP_GET,
  HTTP_HEAD,
  HTTP_POST,
  HTTP_PUT,
  /* pathological */
  HTTP_CONNECT,
  HTTP_OPTIONS,
  HTTP_TRACE,
  /* webdav */
  HTTP_COPY,
  HTTP_LOCK,
  HTTP_MKCOL,
  HTTP_MOVE,
  HTTP_PROPFIND,
  HTTP_PROPPATCH,
  HTTP_SEARCH,
  HTTP_UNLOCK,
  /* subversion */
  HTTP_REPORT,
  HTTP_MKACTIVITY,
  HTTP_CHECKOUT,
  HTTP_MERGE,
  /* upnp */
  HTTP_MSEARCH,
  HTTP_NOTIFY,
  HTTP_SUBSCRIBE,
  HTTP_UNSUBSCRIBE,
  /* RFC-5789 */
  HTTP_PATCH,
  HTTP_PURGE
};

/** Errno-related constants */
static const enum http_errno HTTP_ERRNO_TABLE[] = {
  HPE_OK,
  HPE_CB_message_begin,
  HPE_CB_status_complete,
  HPE_CB_url,
  HPE_CB_header_field,
  HPE_CB_header_value,
  HPE_CB_headers_complete,
  HPE_CB_body,
  HPE_CB_message_complete,
  HPE_INVALID_EOF_STATE,
  HPE_HEADER_OVERFLOW,
  HPE_CLOSED_CONNECTION,
  HPE_INVALID_VERSION,
  HPE_INVALID_STATUS,
  HPE_INVALID_METHOD,
  HPE_INVALID_URL,
  HPE_INVALID_HOST,
  HPE_INVALID_PORT,
  HPE_INVALID_PATH,
  HPE_INVALID_QUERY_STRING,
  HPE_INVALID_FRAGMENT,
  HPE_LF_EXPECTED,
  HPE_INVALID_HEADER_TOKEN,
  HPE_INVALID_CONTENT_LENGTH,
  HPE_INVALID_CHUNK_SIZE,
  HPE_INVALID_CONSTANT,
  HPE_INVALID_INTERNAL_STATE,
  HPE_STRICT,
  HPE_PAUSED,
  HPE_UNKNOWN,
};

static struct custom_operations caml_http_parser_struct_ops = {
  "org.ocaml.http_parser",
  custom_finalize_default,
  custom_compare_default,
  custom_hash_default,
  custom_serialize_default,
  custom_deserialize_default
};

static enum http_parser_type
caml_http_parser_type_ml2c(value v)
{
  return HTTP_PARSER_TYPE_TABLE[Long_val(v)];
}

static enum http_method
caml_http_method_ml2c(value v)
{
  return HTTP_METHOD_TABLE[Long_val(v)];
}

static enum http_errno
caml_http_errno_ml2c(value v)
{
  return HTTP_ERRNO_TABLE[Long_val(v)];
}

static value
caml_copy_http_parser(http_parser *parser,
                      http_parser_settings *native_settings,
                      caml_http_parser_settings_t *caml_settings)
{
  CAMLparam0();
  CAMLlocal1(caml_parser);

  caml_http_parser_t *native_parser = (caml_http_parser_t *)
      malloc(sizeof(caml_http_parser_t));
  native_parser->parser = parser;
  native_parser->native_settings = native_settings;
  native_parser->parser->data = caml_settings;
  caml_parser = caml_alloc_custom(&caml_http_parser_struct_ops,
          sizeof(caml_http_parser_t *), 0, 1);
  caml_http_parser_struct_val(caml_parser) = native_parser;

  CAMLreturn(caml_parser);
}

static int
on_url_cb(http_parser *parser, const char *at, size_t length)
{
  CAMLlocal4(caml_parser, r, data, len);

  caml_http_parser_settings_t *settings =
    (caml_http_parser_settings_t *)parser->data;
  caml_parser = caml_copy_http_parser(parser, NULL, settings);
  data = caml_alloc_string(length);
  memcpy(String_val(data), at, length);
  len = Val_int(length);

  r = callback3(settings->on_url, caml_parser, data, len);

  return Int_val(r);
}

static int
on_header_field_cb(http_parser *parser, const char *at, size_t length)
{
  CAMLlocal4(caml_parser, r, data, len);

  caml_http_parser_settings_t *settings =
    (caml_http_parser_settings_t *)parser->data;
  caml_parser = caml_copy_http_parser(parser, NULL, settings);
  data = caml_alloc_string(length);
  memcpy(String_val(data), at, length);
  len = Val_int(length);

  r = callback3(settings->on_header_field, caml_parser, data, len);

  return Int_val(r);
}

static int
on_header_value_cb(http_parser *parser, const char *at, size_t length)
{
  CAMLlocal4(caml_parser, r, data, len);

  caml_http_parser_settings_t *settings =
    (caml_http_parser_settings_t *)parser->data;
  caml_parser = caml_copy_http_parser(parser, NULL, settings);
  data = caml_alloc_string(length);
  memcpy(String_val(data), at, length);
  len = Val_int(length);

  r = callback3(settings->on_header_value, caml_parser, data, len);

  return Int_val(r);
}

static int
on_body_cb(http_parser *parser, const char *at, size_t length)
{
  CAMLlocal4(caml_parser, r, data, len);

  caml_http_parser_settings_t *settings =
    (caml_http_parser_settings_t *)parser->data;
  caml_parser = caml_copy_http_parser(parser, NULL, settings);
  data = caml_alloc_string(length);
  memcpy(String_val(data), at, length);
  len = Val_int(length);

  r = callback3(settings->on_body, caml_parser, data, len);

  return Int_val(r);
}

static int on_message_begin_cb(http_parser* parser)
{
  CAMLlocal2(caml_parser, r);

  caml_http_parser_settings_t *settings =
    (caml_http_parser_settings_t *)parser->data;
  caml_parser = caml_copy_http_parser(parser, NULL, settings);

  r = callback(settings->on_message_begin, caml_parser);

  return Int_val(r);
}

static int on_status_complete_cb(http_parser* parser)
{
  CAMLlocal2(caml_parser, r);

  caml_http_parser_settings_t *settings =
    (caml_http_parser_settings_t *)parser->data;
  caml_parser = caml_copy_http_parser(parser, NULL, settings);

  r = callback(settings->on_status_complete, caml_parser);

  return Int_val(r);
}

static int on_headers_complete_cb(http_parser* parser)
{
  CAMLlocal2(caml_parser, r);

  caml_http_parser_settings_t *settings =
    (caml_http_parser_settings_t *)parser->data;
  caml_parser = caml_copy_http_parser(parser, NULL, settings);

  r = callback(settings->on_headers_complete, caml_parser);

  return Int_val(r);
}

static int on_message_complete_cb(http_parser* parser)
{
  CAMLlocal2(caml_parser, r);

  caml_http_parser_settings_t *settings =
    (caml_http_parser_settings_t *)parser->data;
  caml_parser = caml_copy_http_parser(parser, NULL, settings);

  r = callback(settings->on_message_complete, caml_parser);

  return Int_val(r);
}

static int
caml_parse_http_parser_settings(value settings,
                                caml_http_parser_settings_t *native_settings)
{

  // TODO: error code should be returned if settings doesn't meet with
  // the specifications. value @settings is an OCaml object containing
  // the necessary callbacks needed by the http_parser instance.
  // The value @settings is defined like this:
  //
  // type http_parser_settings = {
  //   on_message_begin:    http_cb;
  //   on_url:              http_data_cb;
  //   on_status_complete:  http_cb;
  //   on_header_field:     http_data_cb;
  //   on_header_value:     http_data_cb;
  //   on_headers_complete: http_cb;
  //   on_body:             http_data_cb;
  //   on_message_complete: http_cb
  // }

  native_settings->on_message_begin    = Field(settings, 0);
  native_settings->on_url              = Field(settings, 1);
  native_settings->on_status_complete  = Field(settings, 2);
  native_settings->on_header_field     = Field(settings, 3);
  native_settings->on_header_value     = Field(settings, 4);
  native_settings->on_headers_complete = Field(settings, 5);
  native_settings->on_body             = Field(settings, 6);
  native_settings->on_message_complete = Field(settings, 7);

  return 0;
}

CAMLprim value
caml_http_parser_version(value unit)
{
  CAMLparam1(unit);
  CAMLlocal1(caml_version);

  caml_version = caml_alloc(3, 0);
  unsigned long version = http_parser_version();
  unsigned major = (version >> 16) & 255;
  unsigned minor = (version >> 8) & 255;
  unsigned patch = version & 255;
  Store_field(caml_version, 0, Val_int(major));
  Store_field(caml_version, 1, Val_int(minor));
  Store_field(caml_version, 2, Val_int(patch));

  CAMLreturn(caml_version);
}

CAMLprim value
caml_http_parser_init(value settings, value type)
{
  CAMLparam1(type);
  CAMLlocal1(caml_parser);

  http_parser *native_parser =
    (http_parser *)malloc(sizeof(http_parser));

  http_parser_settings *native_settings =
    (http_parser_settings *)malloc(sizeof(http_parser_settings));
  native_settings->on_message_begin = on_message_begin_cb;
  native_settings->on_url = on_url_cb;
  native_settings->on_status_complete = on_status_complete_cb;
  native_settings->on_header_field = on_header_field_cb;
  native_settings->on_header_value = on_header_value_cb;
  native_settings->on_headers_complete = on_headers_complete_cb;
  native_settings->on_body = on_body_cb;
  native_settings->on_message_complete = on_message_complete_cb;

  caml_http_parser_settings_t *caml_settings =
    (caml_http_parser_settings_t *)malloc(sizeof(caml_http_parser_settings_t));
  int rc = caml_parse_http_parser_settings(settings,
                                           caml_settings);
  if (rc == -1) {
    // TODO: error handling here.
  }
  enum http_parser_type parser_type = caml_http_parser_type_ml2c(type);
  http_parser_init(native_parser, parser_type);
  caml_parser = caml_copy_http_parser(native_parser,
                                      native_settings,
                                      caml_settings);

  CAMLreturn(caml_parser);
}

CAMLprim value
caml_http_parser_execute(value parser, value data)
{
  CAMLparam2(parser, data);
  CAMLlocal1(nparsed);

  caml_http_parser_t *native_parser =
    caml_http_parser_struct_val(parser);
  const char *local_data = String_val(data);
  int parsed = http_parser_execute(native_parser->parser,
                                   native_parser->native_settings,
                                   local_data,
                                   strlen(local_data));
  nparsed = Val_int(parsed);

  CAMLreturn(nparsed);
}

CAMLprim value
caml_http_should_keep_alive(value parser)
{
  CAMLparam1(parser);
  CAMLlocal1(r);

  caml_http_parser_t *native_parser =
    caml_http_parser_struct_val(parser);
  int rc = http_should_keep_alive(native_parser->parser);
  r = Val_int(rc);

  CAMLreturn(r);
}

CAMLprim value
caml_http_method_str(value method)
{
  CAMLparam1(method);
  CAMLlocal1(r);

  enum http_method m = caml_http_method_ml2c(method);
  const char *method_str = http_method_str(m);
  r = caml_copy_string(method_str);

  CAMLreturn(r);
}

CAMLprim value
caml_http_errno_code(value parser)
{
  CAMLparam1(parser);
  CAMLlocal1(r);

  caml_http_parser_t *native_parser =
    caml_http_parser_struct_val(parser);

  int http_errno  = native_parser->parser->http_errno;
  r = Val_int(http_errno);

  CAMLreturn(r);
}

CAMLprim value
caml_http_errno_name(value errno)
{
  CAMLparam1(errno);
  CAMLlocal1(r);

  enum http_errno e = caml_http_errno_ml2c(errno);
  const char *name = http_errno_name(e);
  r = caml_copy_string(name);

  CAMLreturn(r);
}

CAMLprim value
caml_http_errno_description(value errno)
{
  CAMLparam1(errno);
  CAMLlocal1(r);

  enum http_errno e = caml_http_errno_ml2c(errno);
  const char *descr = http_errno_description(e);
  r = caml_copy_string(descr);

  CAMLreturn(r);
}

CAMLprim value
caml_http_parser_pause(value parser, value paused)
{
  CAMLparam2(parser, paused);

  caml_http_parser_t *native_parser =
    caml_http_parser_struct_val(parser);
  http_parser_pause(native_parser->parser, Int_val(paused));

  CAMLreturn(Val_unit);
}

CAMLprim value
caml_http_body_is_final(value parser)
{
  CAMLparam1(parser);
  CAMLlocal1(r);

  caml_http_parser_t *native_parser =
    caml_http_parser_struct_val(parser);
  int rc = http_body_is_final(native_parser->parser);
  r = Val_int(rc);

  CAMLreturn(r);
}

CAMLprim value
caml_http_status_code(value parser)
{
  CAMLparam1(parser);
  CAMLlocal1(r);

  caml_http_parser_t *native_parser =
    caml_http_parser_struct_val(parser);

  int status_code  = native_parser->parser->status_code;
  r = Val_int(status_code);

  CAMLreturn(r);
}

CAMLprim value
caml_http_method_code(value parser)
{
  CAMLparam1(parser);
  CAMLlocal1(r);

  caml_http_parser_t *native_parser =
    caml_http_parser_struct_val(parser);

  int method  = native_parser->parser->method;
  r = Val_int(method);

  CAMLreturn(r);
}

/**
 * 1 = Upgrade header was present and the parser has exited because of that.
 * 0 = No upgrade header present.
 * Should be checked when http_parser_execute() returns in addition to
 * error checking.
 */
CAMLprim value
caml_http_is_upgrade(value parser)
{
  CAMLparam1(parser);
  CAMLlocal1(r);

  caml_http_parser_t *native_parser =
    caml_http_parser_struct_val(parser);

  int upgrade  = native_parser->parser->upgrade;
  r = Val_int(upgrade);

  CAMLreturn(r);
}

CAMLprim value
caml_http_parser_parse_url(value url, value is_connect)
{
  CAMLparam2(url, is_connect);
  CAMLlocal2(caml_url, url_part);

  int field_index;
  struct http_parser_url native_url;
  const char *url_str = String_val(url);
  memset(&native_url, 0, sizeof(struct http_parser_url));
  caml_url = caml_alloc(7, 0);
  int rc = http_parser_parse_url(url_str,
                                 strlen(url_str),
                                 Int_val(is_connect),
                                 &native_url);

  if (rc == 0) {
    for (field_index = 0; field_index < UF_MAX; field_index++) {
      if (native_url.field_set & (1 << field_index)) {
        uint16_t off = native_url.field_data[field_index].off;
        uint16_t len = native_url.field_data[field_index].len;
        url_part = caml_alloc_string(len);
        memcpy(String_val(url_part), url_str + off, len);
        Store_field(caml_url, field_index, url_part);
      }
    }
  } else {
    for (field_index = 0; field_index < UF_MAX; field_index++) {
      Store_field(caml_url, field_index, caml_copy_string(""));
    }
  }

  CAMLreturn(caml_url);
}

