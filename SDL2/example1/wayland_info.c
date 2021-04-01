#include "SDL.h"
#include "SDL_syswm.h"
#include <wayland-client.h>

int main(int argc, char *argv[]) {
  SDL_Window* window;
  SDL_SysWMinfo info;

  SDL_Init(0);

  window = SDL_CreateWindow("", 0, 0, 0, 0, SDL_WINDOW_HIDDEN);

   if (SDL_GetWindowWMInfo(window,&info)) {
    /* success */
    if (info.subsystem == SDL_SYSWM_WAYLAND) {
      printf("Is Wayland\n");
      
      struct wl_display* display = info.info.wl.display;
      struct wl_surface* surface = info.info.wl.surface;
      struct wl_shell_surface* shell_surface = info.info.wl.shell_surface;
      // and do whatever you want with the Wayland stuff
    } else {
      printf("Not a Wayland system\n");
    }
   } else {
     printf("Failed to get WM info\n");
   }
}
