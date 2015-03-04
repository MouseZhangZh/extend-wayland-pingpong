#include <stdio.h>
#include <stdlib.h>

#include "pingpong-client-protocol.h"

struct context
{
    struct wl_registry *registry;
    struct wl_display *display;
    struct ping_pong *ping_pong;
    int stop;
};

static void
handle_pong(void *data, struct ping_pong *ping_pong, const char *message)
{
    struct context *ctx = data;
    printf("Received pong, message='%s'\n", message);
    ctx->stop = 1;
}

static const struct ping_pong_listener ping_pong_listener = {
    handle_pong
};

static void
registry_handle_global_remove(void *data, struct wl_registry *registry,
                              uint32_t name)
{
    /* Nobody cares about globals being removed */
}

static void
registry_handle_global(void *data, struct wl_registry *registry, uint32_t name,
                       const char *interface, uint32_t version)
{
    struct context *ctx = data;
    if (strcmp(interface, "ping_pong") == 0)
    {
        ctx->ping_pong = wl_registry_bind(registry, name, &ping_pong_interface, 1);
        if (!ctx->ping_pong)
        {
            fprintf(stderr, "Failed to registry bind ping_pong");
            return;
        }
        ping_pong_add_listener(ctx->ping_pong, &ping_pong_listener, ctx);
    }
}

static const struct wl_registry_listener registry_client_listener = {
    registry_handle_global,
    registry_handle_global_remove
};

int main(int argc, char *argv[])
{
    struct context ctx = {0};
    
    if (argc <= 1)
    {
        fprintf(stderr, "Not enough arguments. Usage: PROG MESSAGE\n");
        return EXIT_FAILURE;
    }

    ctx.display = wl_display_connect(NULL);
    if (!ctx.display)
    {
        fprintf(stderr, "Failed to get display\n");
        return EXIT_FAILURE;
    }

    ctx.registry = wl_display_get_registry(ctx.display);
    if (!ctx.registry)
    {
        fprintf(stderr, "Failed to get registry\n");
        return EXIT_FAILURE;
    }

    if (wl_registry_add_listener(ctx.registry, &registry_client_listener, &ctx))
    {
        fprintf(stderr, "Failed to add registry listener\n");
        return EXIT_FAILURE;
    }

    wl_display_roundtrip(ctx.display);

    if (ctx.ping_pong == NULL)
    {
        fprintf(stderr, "ping pong hasn't been bound\n");
        return EXIT_FAILURE;
    }
    
    printf("Sending ping with message %s\n", argv[1]);
    ping_pong_ping(ctx.ping_pong, argv[1]);

    while(!ctx.stop)
        wl_display_dispatch(ctx.display);

}
