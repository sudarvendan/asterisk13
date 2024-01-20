/*
 * Asterisk -- An open source telephony toolkit.
 *
 * Copyright (C) 2012 - 2013, Digium, Inc.
 *
 * David M. Lee, II <dlee@digium.com>
 *
 * See http://www.asterisk.org for more information about
 * the Asterisk project. Please do not directly contact
 * any of the maintainers of this project for assistance;
 * the project provides a web site, mailing lists and IRC
 * channels for your use.
 *
 * This program is free software, distributed under the terms of
 * the GNU General Public License Version 2. See the LICENSE file
 * at the top of the source tree.
 */

/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!!!!                               DO NOT EDIT                        !!!!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * This file is generated by a mustache template. Please see the original
 * template in rest-api-templates/res_ari_resource.c.mustache
 */

/*! \file
 *
 * \brief Endpoint resources
 *
 * \author David M. Lee, II <dlee@digium.com>
 */

/*** MODULEINFO
	<depend type="module">res_ari</depend>
	<depend type="module">res_ari_model</depend>
	<depend type="module">res_stasis</depend>
	<support_level>core</support_level>
 ***/

#include "asterisk.h"

ASTERISK_FILE_VERSION(__FILE__, "$Revision$")

#include "asterisk/app.h"
#include "asterisk/module.h"
#include "asterisk/stasis_app.h"
#include "ari/resource_endpoints.h"
#if defined(AST_DEVMODE)
#include "ari/ari_model_validators.h"
#endif

#define MAX_VALS 128

/*!
 * \brief Parameter parsing callback for /endpoints.
 * \param get_params GET parameters in the HTTP request.
 * \param path_vars Path variables extracted from the request.
 * \param headers HTTP headers.
 * \param[out] response Response to the HTTP request.
 */
static void ast_ari_endpoints_list_cb(
	struct ast_tcptls_session_instance *ser,
	struct ast_variable *get_params, struct ast_variable *path_vars,
	struct ast_variable *headers, struct ast_ari_response *response)
{
	struct ast_ari_endpoints_list_args args = {};
	RAII_VAR(struct ast_json *, body, NULL, ast_json_unref);
#if defined(AST_DEVMODE)
	int is_valid;
	int code;
#endif /* AST_DEVMODE */

	ast_ari_endpoints_list(headers, &args, response);
#if defined(AST_DEVMODE)
	code = response->response_code;

	switch (code) {
	case 0: /* Implementation is still a stub, or the code wasn't set */
		is_valid = response->message == NULL;
		break;
	case 500: /* Internal Server Error */
	case 501: /* Not Implemented */
		is_valid = 1;
		break;
	default:
		if (200 <= code && code <= 299) {
			is_valid = ast_ari_validate_list(response->message,
				ast_ari_validate_endpoint_fn());
		} else {
			ast_log(LOG_ERROR, "Invalid error response %d for /endpoints\n", code);
			is_valid = 0;
		}
	}

	if (!is_valid) {
		ast_log(LOG_ERROR, "Response validation failed for /endpoints\n");
		ast_ari_response_error(response, 500,
			"Internal Server Error", "Response validation failed");
	}
#endif /* AST_DEVMODE */

fin: __attribute__((unused))
	return;
}
int ast_ari_endpoints_send_message_parse_body(
	struct ast_json *body,
	struct ast_ari_endpoints_send_message_args *args)
{
	struct ast_json *field;
	/* Parse query parameters out of it */
	field = ast_json_object_get(body, "to");
	if (field) {
		args->to = ast_json_string_get(field);
	}
	field = ast_json_object_get(body, "from");
	if (field) {
		args->from = ast_json_string_get(field);
	}
	field = ast_json_object_get(body, "body");
	if (field) {
		args->body = ast_json_string_get(field);
	}
	return 0;
}

/*!
 * \brief Parameter parsing callback for /endpoints/sendMessage.
 * \param get_params GET parameters in the HTTP request.
 * \param path_vars Path variables extracted from the request.
 * \param headers HTTP headers.
 * \param[out] response Response to the HTTP request.
 */
static void ast_ari_endpoints_send_message_cb(
	struct ast_tcptls_session_instance *ser,
	struct ast_variable *get_params, struct ast_variable *path_vars,
	struct ast_variable *headers, struct ast_ari_response *response)
{
	struct ast_ari_endpoints_send_message_args args = {};
	struct ast_variable *i;
	RAII_VAR(struct ast_json *, body, NULL, ast_json_unref);
#if defined(AST_DEVMODE)
	int is_valid;
	int code;
#endif /* AST_DEVMODE */

	for (i = get_params; i; i = i->next) {
		if (strcmp(i->name, "to") == 0) {
			args.to = (i->value);
		} else
		if (strcmp(i->name, "from") == 0) {
			args.from = (i->value);
		} else
		if (strcmp(i->name, "body") == 0) {
			args.body = (i->value);
		} else
		{}
	}
	/* Look for a JSON request entity */
	body = ast_http_get_json(ser, headers);
	if (!body) {
		switch (errno) {
		case EFBIG:
			ast_ari_response_error(response, 413, "Request Entity Too Large", "Request body too large");
			goto fin;
		case ENOMEM:
			ast_ari_response_error(response, 500, "Internal Server Error", "Error processing request");
			goto fin;
		case EIO:
			ast_ari_response_error(response, 400, "Bad Request", "Error parsing request body");
			goto fin;
		}
	}
	args.variables = body;
	ast_ari_endpoints_send_message(headers, &args, response);
#if defined(AST_DEVMODE)
	code = response->response_code;

	switch (code) {
	case 0: /* Implementation is still a stub, or the code wasn't set */
		is_valid = response->message == NULL;
		break;
	case 500: /* Internal Server Error */
	case 501: /* Not Implemented */
	case 400: /* Invalid parameters for sending a message. */
	case 404: /* Endpoint not found */
		is_valid = 1;
		break;
	default:
		if (200 <= code && code <= 299) {
			is_valid = ast_ari_validate_void(
				response->message);
		} else {
			ast_log(LOG_ERROR, "Invalid error response %d for /endpoints/sendMessage\n", code);
			is_valid = 0;
		}
	}

	if (!is_valid) {
		ast_log(LOG_ERROR, "Response validation failed for /endpoints/sendMessage\n");
		ast_ari_response_error(response, 500,
			"Internal Server Error", "Response validation failed");
	}
#endif /* AST_DEVMODE */

fin: __attribute__((unused))
	return;
}
/*!
 * \brief Parameter parsing callback for /endpoints/{tech}.
 * \param get_params GET parameters in the HTTP request.
 * \param path_vars Path variables extracted from the request.
 * \param headers HTTP headers.
 * \param[out] response Response to the HTTP request.
 */
static void ast_ari_endpoints_list_by_tech_cb(
	struct ast_tcptls_session_instance *ser,
	struct ast_variable *get_params, struct ast_variable *path_vars,
	struct ast_variable *headers, struct ast_ari_response *response)
{
	struct ast_ari_endpoints_list_by_tech_args args = {};
	struct ast_variable *i;
	RAII_VAR(struct ast_json *, body, NULL, ast_json_unref);
#if defined(AST_DEVMODE)
	int is_valid;
	int code;
#endif /* AST_DEVMODE */

	for (i = path_vars; i; i = i->next) {
		if (strcmp(i->name, "tech") == 0) {
			args.tech = (i->value);
		} else
		{}
	}
	ast_ari_endpoints_list_by_tech(headers, &args, response);
#if defined(AST_DEVMODE)
	code = response->response_code;

	switch (code) {
	case 0: /* Implementation is still a stub, or the code wasn't set */
		is_valid = response->message == NULL;
		break;
	case 500: /* Internal Server Error */
	case 501: /* Not Implemented */
	case 404: /* Endpoints not found */
		is_valid = 1;
		break;
	default:
		if (200 <= code && code <= 299) {
			is_valid = ast_ari_validate_list(response->message,
				ast_ari_validate_endpoint_fn());
		} else {
			ast_log(LOG_ERROR, "Invalid error response %d for /endpoints/{tech}\n", code);
			is_valid = 0;
		}
	}

	if (!is_valid) {
		ast_log(LOG_ERROR, "Response validation failed for /endpoints/{tech}\n");
		ast_ari_response_error(response, 500,
			"Internal Server Error", "Response validation failed");
	}
#endif /* AST_DEVMODE */

fin: __attribute__((unused))
	return;
}
/*!
 * \brief Parameter parsing callback for /endpoints/{tech}/{resource}.
 * \param get_params GET parameters in the HTTP request.
 * \param path_vars Path variables extracted from the request.
 * \param headers HTTP headers.
 * \param[out] response Response to the HTTP request.
 */
static void ast_ari_endpoints_get_cb(
	struct ast_tcptls_session_instance *ser,
	struct ast_variable *get_params, struct ast_variable *path_vars,
	struct ast_variable *headers, struct ast_ari_response *response)
{
	struct ast_ari_endpoints_get_args args = {};
	struct ast_variable *i;
	RAII_VAR(struct ast_json *, body, NULL, ast_json_unref);
#if defined(AST_DEVMODE)
	int is_valid;
	int code;
#endif /* AST_DEVMODE */

	for (i = path_vars; i; i = i->next) {
		if (strcmp(i->name, "tech") == 0) {
			args.tech = (i->value);
		} else
		if (strcmp(i->name, "resource") == 0) {
			args.resource = (i->value);
		} else
		{}
	}
	ast_ari_endpoints_get(headers, &args, response);
#if defined(AST_DEVMODE)
	code = response->response_code;

	switch (code) {
	case 0: /* Implementation is still a stub, or the code wasn't set */
		is_valid = response->message == NULL;
		break;
	case 500: /* Internal Server Error */
	case 501: /* Not Implemented */
	case 400: /* Invalid parameters for sending a message. */
	case 404: /* Endpoints not found */
		is_valid = 1;
		break;
	default:
		if (200 <= code && code <= 299) {
			is_valid = ast_ari_validate_endpoint(
				response->message);
		} else {
			ast_log(LOG_ERROR, "Invalid error response %d for /endpoints/{tech}/{resource}\n", code);
			is_valid = 0;
		}
	}

	if (!is_valid) {
		ast_log(LOG_ERROR, "Response validation failed for /endpoints/{tech}/{resource}\n");
		ast_ari_response_error(response, 500,
			"Internal Server Error", "Response validation failed");
	}
#endif /* AST_DEVMODE */

fin: __attribute__((unused))
	return;
}
int ast_ari_endpoints_send_message_to_endpoint_parse_body(
	struct ast_json *body,
	struct ast_ari_endpoints_send_message_to_endpoint_args *args)
{
	struct ast_json *field;
	/* Parse query parameters out of it */
	field = ast_json_object_get(body, "from");
	if (field) {
		args->from = ast_json_string_get(field);
	}
	field = ast_json_object_get(body, "body");
	if (field) {
		args->body = ast_json_string_get(field);
	}
	return 0;
}

/*!
 * \brief Parameter parsing callback for /endpoints/{tech}/{resource}/sendMessage.
 * \param get_params GET parameters in the HTTP request.
 * \param path_vars Path variables extracted from the request.
 * \param headers HTTP headers.
 * \param[out] response Response to the HTTP request.
 */
static void ast_ari_endpoints_send_message_to_endpoint_cb(
	struct ast_tcptls_session_instance *ser,
	struct ast_variable *get_params, struct ast_variable *path_vars,
	struct ast_variable *headers, struct ast_ari_response *response)
{
	struct ast_ari_endpoints_send_message_to_endpoint_args args = {};
	struct ast_variable *i;
	RAII_VAR(struct ast_json *, body, NULL, ast_json_unref);
#if defined(AST_DEVMODE)
	int is_valid;
	int code;
#endif /* AST_DEVMODE */

	for (i = get_params; i; i = i->next) {
		if (strcmp(i->name, "from") == 0) {
			args.from = (i->value);
		} else
		if (strcmp(i->name, "body") == 0) {
			args.body = (i->value);
		} else
		{}
	}
	for (i = path_vars; i; i = i->next) {
		if (strcmp(i->name, "tech") == 0) {
			args.tech = (i->value);
		} else
		if (strcmp(i->name, "resource") == 0) {
			args.resource = (i->value);
		} else
		{}
	}
	/* Look for a JSON request entity */
	body = ast_http_get_json(ser, headers);
	if (!body) {
		switch (errno) {
		case EFBIG:
			ast_ari_response_error(response, 413, "Request Entity Too Large", "Request body too large");
			goto fin;
		case ENOMEM:
			ast_ari_response_error(response, 500, "Internal Server Error", "Error processing request");
			goto fin;
		case EIO:
			ast_ari_response_error(response, 400, "Bad Request", "Error parsing request body");
			goto fin;
		}
	}
	args.variables = body;
	ast_ari_endpoints_send_message_to_endpoint(headers, &args, response);
#if defined(AST_DEVMODE)
	code = response->response_code;

	switch (code) {
	case 0: /* Implementation is still a stub, or the code wasn't set */
		is_valid = response->message == NULL;
		break;
	case 500: /* Internal Server Error */
	case 501: /* Not Implemented */
	case 400: /* Invalid parameters for sending a message. */
	case 404: /* Endpoint not found */
		is_valid = 1;
		break;
	default:
		if (200 <= code && code <= 299) {
			is_valid = ast_ari_validate_void(
				response->message);
		} else {
			ast_log(LOG_ERROR, "Invalid error response %d for /endpoints/{tech}/{resource}/sendMessage\n", code);
			is_valid = 0;
		}
	}

	if (!is_valid) {
		ast_log(LOG_ERROR, "Response validation failed for /endpoints/{tech}/{resource}/sendMessage\n");
		ast_ari_response_error(response, 500,
			"Internal Server Error", "Response validation failed");
	}
#endif /* AST_DEVMODE */

fin: __attribute__((unused))
	return;
}

/*! \brief REST handler for /api-docs/endpoints.{format} */
static struct stasis_rest_handlers endpoints_sendMessage = {
	.path_segment = "sendMessage",
	.callbacks = {
		[AST_HTTP_PUT] = ast_ari_endpoints_send_message_cb,
	},
	.num_children = 0,
	.children = {  }
};
/*! \brief REST handler for /api-docs/endpoints.{format} */
static struct stasis_rest_handlers endpoints_tech_resource_sendMessage = {
	.path_segment = "sendMessage",
	.callbacks = {
		[AST_HTTP_PUT] = ast_ari_endpoints_send_message_to_endpoint_cb,
	},
	.num_children = 0,
	.children = {  }
};
/*! \brief REST handler for /api-docs/endpoints.{format} */
static struct stasis_rest_handlers endpoints_tech_resource = {
	.path_segment = "resource",
	.is_wildcard = 1,
	.callbacks = {
		[AST_HTTP_GET] = ast_ari_endpoints_get_cb,
	},
	.num_children = 1,
	.children = { &endpoints_tech_resource_sendMessage, }
};
/*! \brief REST handler for /api-docs/endpoints.{format} */
static struct stasis_rest_handlers endpoints_tech = {
	.path_segment = "tech",
	.is_wildcard = 1,
	.callbacks = {
		[AST_HTTP_GET] = ast_ari_endpoints_list_by_tech_cb,
	},
	.num_children = 1,
	.children = { &endpoints_tech_resource, }
};
/*! \brief REST handler for /api-docs/endpoints.{format} */
static struct stasis_rest_handlers endpoints = {
	.path_segment = "endpoints",
	.callbacks = {
		[AST_HTTP_GET] = ast_ari_endpoints_list_cb,
	},
	.num_children = 2,
	.children = { &endpoints_sendMessage,&endpoints_tech, }
};

static int load_module(void)
{
	int res = 0;
	stasis_app_ref();
	res |= ast_ari_add_handler(&endpoints);
	return res;
}

static int unload_module(void)
{
	ast_ari_remove_handler(&endpoints);
	stasis_app_unref();
	return 0;
}

AST_MODULE_INFO(ASTERISK_GPL_KEY, AST_MODFLAG_DEFAULT, "RESTful API module - Endpoint resources",
	.support_level = AST_MODULE_SUPPORT_CORE,
	.load = load_module,
	.unload = unload_module,
	.nonoptreq = "res_ari,res_stasis",
	);
