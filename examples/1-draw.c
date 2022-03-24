#include "matoya.h"

struct context {
	MTY_App *app;
	uint32_t index;
	void *image;
	uint32_t image_w;
	uint32_t image_h;
	bool quit;
};

static bool poll_image(struct context *ctx) 
{
	if (ctx->image)
		return true;

	void *png = NULL;
	size_t png_size = 0;
	uint16_t code = 0;

	MTY_HttpAsyncPoll(ctx->index, &png, &png_size, &code);

	if (code != 200)
		return false;

	// On success, decompress it into BGRA
	ctx->image = MTY_DecompressImage(png, png_size, &ctx->image_w, &ctx->image_h);

	MTY_Free(png);

	return true;
}

static void event_func(const MTY_Event *evt, void *opaque)
{
	struct context *ctx = opaque;

	MTY_PrintEvent(evt);

	if (evt->type == MTY_EVENT_CLOSE)
		ctx->quit = true;
}

static bool app_func(void *opaque)
{
	struct context *ctx = opaque;

	if (!poll_image(ctx))
		return !ctx->quit;

	// Set up a render description for the PNG
	MTY_RenderDesc desc = {
		.format = MTY_COLOR_FORMAT_RGBA,
		.effect = MTY_EFFECT_SCANLINES,
		.imageWidth = ctx->image_w,
		.imageHeight = ctx->image_h,
		.cropWidth = ctx->image_w,
		.cropHeight = ctx->image_h,
		.aspectRatio = (float) ctx->image_w / (float) ctx->image_h,
	};

	// Draw the quad
	MTY_WindowDrawQuad(ctx->app, 0, ctx->image, &desc);

	MTY_WindowPresent(ctx->app, 0, 1);

	return !ctx->quit;
}

int main(int argc, char **argv)
{
	struct context ctx = {0};
	ctx.app = MTY_AppCreate(app_func, event_func, &ctx);
	if (!ctx.app)
		return 1;

	MTY_WindowDesc desc = {
		.title = "My Window",
		.api = MTY_GFX_GL,
		.width = 800,
		.height = 600,
	};

	MTY_WindowCreate(ctx.app, &desc);
	MTY_WindowMakeCurrent(ctx.app, 0, true);

	MTY_HttpAsyncCreate(1);

	// Fetch a PNG from the internet
	MTY_HttpAsyncRequest(&ctx.index, 
		"i.imgur.com", 0, true, "GET", "/58cuUkY.png", 
		NULL, NULL, 0, 5000, NULL);

	MTY_AppRun(ctx.app);
	MTY_AppDestroy(&ctx.app);

	MTY_Free(ctx.image);

	return 0;
}
