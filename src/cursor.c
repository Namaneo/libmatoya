#include "matoya.h"

#include <stdbool.h>
#include <stdint.h>
#include <math.h>

struct MTY_Cursor {
	MTY_App *app;
	bool enabled;
	void *image;
	uint32_t width;
	uint32_t height;
	float hot_x;
	float hot_y;
	float x;
	float y;
	float scale;
};

MTY_Cursor *MTY_CursorCreate(MTY_App *app)
{
	MTY_Cursor *ctx = MTY_Alloc(1, sizeof(MTY_Cursor));

	ctx->app = app;
	ctx->enabled = true;

	return ctx;
}

void MTY_CursorDestroy(MTY_Cursor **cursor)
{
	if (!cursor || !*cursor)
		return;

	MTY_Cursor *ctx = *cursor;

	if (ctx->image)
		MTY_Free(ctx->image);

	MTY_Free(ctx);
	*cursor = NULL;
}

void MTY_CursorEnable(MTY_Cursor *ctx, bool enable)
{
	ctx->enabled = enable;
}

void MTY_CursorSetHotspot(MTY_Cursor *ctx, uint16_t hotX, uint16_t hotY)
{
	ctx->hot_x = (float) hotX;
	ctx->hot_y = (float) hotY;
}

void MTY_CursorSetImage(MTY_Cursor *ctx, const void *data, size_t size)
{
	if (ctx->image)
		MTY_Free(ctx->image);

	ctx->image = MTY_DecompressImage(data, size, &ctx->width, &ctx->height);
}

void MTY_CursorMove(MTY_Cursor *ctx, int32_t x, int32_t y, float scale)
{
	ctx->scale = scale;
	ctx->x = (float) x;
	ctx->y = (float) y;
}

void MTY_CursorMoveFromZoom(MTY_Cursor *ctx, MTY_Zoom *zoom)
{
	float scale = MTY_ZoomGetScale(zoom);
	int32_t cursor_x = MTY_ZoomGetCursorX(zoom);
	int32_t cursor_y = MTY_ZoomGetCursorY(zoom);

	MTY_CursorMove(ctx, cursor_x, cursor_y, scale);
}

void MTY_CursorDraw(MTY_Cursor *ctx, MTY_Window window)
{
	if (!ctx->image || !ctx->enabled)
		return;

	MTY_RenderDesc desc = {0};

	desc.format = MTY_COLOR_FORMAT_RGBA;
	desc.filter = MTY_FILTER_LINEAR;
	desc.cropWidth = ctx->width;
	desc.cropHeight = ctx->height;
	desc.imageWidth = ctx->width;
	desc.imageHeight = ctx->width;
	desc.aspectRatio = (float) ctx->width / (float) ctx->height;
	desc.blend = true;

	desc.position = MTY_POSITION_FIXED;
	desc.scale = ctx->scale;
	desc.imageX = lrint(ctx->x - (ctx->hot_x * desc.scale));
	desc.imageY = lrint(ctx->y - (ctx->hot_y * desc.scale));

	MTY_WindowDrawQuad(ctx->app, window, ctx->image, &desc);
}
