#include <stdbool.h>
#include <SDL2/SDL.h>

#define PICTURE_SIZE_WIDTH  640
#define PICTURE_SIZE_HEIGHT 480
#define PICTURE_AT(x, y)    (y * PICTURE_SIZE_WIDTH + x)

/*
 * Declarations
 */

typedef struct SDL_Context {
    SDL_Window   *window;
    SDL_Renderer *renderer;
    SDL_Texture  *texture;
    uint32_t      pixels[PICTURE_SIZE_HEIGHT * PICTURE_SIZE_WIDTH];
    void          (*update)(struct SDL_Context *ctx);
    void          (*draw  )(struct SDL_Context *ctx);
    bool          (*events)(struct SDL_Context *ctx);
} SDL_Context;

typedef void (*SDL_ContextUpdateFunc)(SDL_Context *ctx);
typedef void (*SDL_ContextDrawFunc  )(SDL_Context *ctx);
typedef bool (*SDL_ContextEventsFunc)(SDL_Context *ctx);

SDL_Context * SDL_CreateContext(SDL_ContextUpdateFunc update,
                                SDL_ContextDrawFunc   draw,
                                SDL_ContextEventsFunc events);
void          SDL_MainLoop(SDL_Context *ctx);
void          SDL_DeleteContext(SDL_Context *ctx);

/*
 * Internal functions
 */

SDL_Context * SDL_CreateContext(SDL_ContextUpdateFunc update,
                                SDL_ContextDrawFunc   draw,
                                SDL_ContextEventsFunc events)
{
    SDL_Context *ctx = NULL;

    ctx = malloc(sizeof(SDL_Context));
    if (ctx == NULL) {
        return NULL;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        goto err_cleanup;
    }

    ctx->window = SDL_CreateWindow("SDL_Context",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            PICTURE_SIZE_WIDTH,
            PICTURE_SIZE_HEIGHT,
            SDL_WINDOW_SHOWN);
    if (ctx->window == NULL) {
        goto err_cleanup;
    }

    ctx->renderer = SDL_CreateRenderer(ctx->window,
            -1, SDL_RENDERER_ACCELERATED);
    if (ctx->renderer == NULL) {
        goto err_cleanup;
    }

    ctx->texture = SDL_CreateTexture(ctx->renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STATIC,
            PICTURE_SIZE_WIDTH,
            PICTURE_SIZE_HEIGHT);
    if (ctx->texture == NULL) {
        goto err_cleanup;
    }

    memset(ctx->pixels, 0xFF, PICTURE_SIZE_HEIGHT * PICTURE_SIZE_WIDTH * 4);

    if (update == NULL || draw == NULL || events == NULL) {
        goto err_cleanup;
    }
    ctx->update = update;
    ctx->draw   = draw;
    ctx->events = events;

    return ctx;

err_cleanup:
    SDL_DeleteContext(ctx);
    return NULL;
}

void SDL_MainLoop(SDL_Context *ctx)
{
    for (bool quit = false; quit != true;) {
        ctx->update(ctx);
        quit = ctx->events(ctx);
        ctx->draw(ctx);
    }
}

void SDL_DeleteContext(SDL_Context *ctx)
{
    if (ctx != NULL) {
        SDL_DestroyTexture(ctx->texture);
        SDL_DestroyRenderer(ctx->renderer);
        SDL_DestroyWindow(ctx->window);
    }
    free(ctx);
}

/*
 * Context manipulation functions provided by the user
 */

static void UpdatePicture(SDL_Context *ctx)
{
    SDL_UpdateTexture(ctx->texture, NULL, ctx->pixels, PICTURE_SIZE_WIDTH * 4);
}

static void DrawPicture(SDL_Context *ctx)
{
    SDL_RenderClear(ctx->renderer);
    SDL_RenderCopy(ctx->renderer, ctx->texture, NULL, NULL);
    SDL_RenderPresent(ctx->renderer);
}

static bool HandleEvents(SDL_Context *ctx)
{
    SDL_Event e;
    static bool clicked = false;

    while (SDL_PollEvent(&e)) {
        switch (e.type) {
        case SDL_MOUSEBUTTONUP:
            clicked = (e.button.button == SDL_BUTTON_LEFT) ? false : clicked;
            break;
        case SDL_MOUSEBUTTONDOWN:
            clicked = (e.button.button == SDL_BUTTON_LEFT) ? true : clicked;
            break;
        case SDL_MOUSEMOTION:
            if (clicked) {
                ctx->pixels[PICTURE_AT(e.motion.x, e.motion.y)] = 0x00;
            }
            break;
        case SDL_QUIT:
            return true;
        }
    }

    return false;
}

/*
 * Main game procedure
 */

int main(int argc, char *argv[])
{
    SDL_Context *ctx = NULL;

    ctx = SDL_CreateContext(UpdatePicture, DrawPicture, HandleEvents);
    if (ctx == NULL) {
        fprintf(stderr, "Could not create the SDL context\n");
        return -1;
    }

    SDL_MainLoop(ctx);

    SDL_DeleteContext(ctx);
    SDL_Quit();

    return 0;
}
