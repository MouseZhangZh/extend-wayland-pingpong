#include <stdlib.h>

#include <weston/compositor.h>

#include "pingpong-server-protocol.h"

struct context
{
    struct wl_list list_clients;
};

struct client_context
{
    struct wl_list link;
    struct wl_resource *ping_pong_interface_resource;
    struct context *context;
};

static void
ping_pong_ping(struct wl_client *client, struct wl_resource *resource, const char *message)
{
    weston_log("Received ping %s\n", message);
    ping_pong_send_pong(resource, message);
}

static const struct ping_pong_interface ping_pong_implementation = {
    ping_pong_ping
};

static void
unbind_ping_pong(struct wl_resource *resource)
{
    struct client_context *client_ctx = wl_resource_get_user_data(resource);
    wl_list_remove(&client_ctx->link);
    free(client_ctx);
}

static void
bind_ping_pong(struct wl_client *client, void *data, uint32_t version, uint32_t id)
{
    struct context *ctx = data;
    struct client_context *client_ctx;
    client_ctx = calloc(1, sizeof *client_ctx);
    if (!client_ctx)
    {
        weston_log("No memory to allocate client context\n");
        return;
    }

    client_ctx->ping_pong_interface_resource =
        wl_resource_create(client, &ping_pong_interface, 1, id);
    wl_resource_set_implementation(client_ctx->ping_pong_interface_resource,
                                   &ping_pong_implementation, client_ctx,
                                   unbind_ping_pong);
    client_ctx->context = ctx;
    wl_list_init(&client_ctx->link);
    wl_list_insert(&ctx->list_clients, &client_ctx->link);
}

WL_EXPORT int
module_init(struct weston_compositor *compositor, int *argc, char *argv[])
{
    struct context *ctx;
    weston_log("Pingpong module initialized!");
    
    ctx = calloc(1, sizeof *ctx);
    if (!ctx)
    {
        weston_log("No memory ot initialize context\n");
        return -1;
    }

    wl_list_init(&ctx->list_clients);

    if (wl_global_create(compositor->wl_display, &ping_pong_interface, 1,
                         ctx, bind_ping_pong) == NULL)
    {
        return -1;
    }

}
