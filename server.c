#include <weston/compositor.h>

WL_EXPORT int
module_init(struct weston_compositor *compositor, int *argc, char *argv[])
{
    weston_log("Pingpong module initialized!");
}
