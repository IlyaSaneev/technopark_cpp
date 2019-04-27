#include <iostream>

#include "SkCanvas.h"
#include "SkGraphics.h"
#include "SkImageEncoder.h"
#include "SkRRect.h"
#include "SkString.h"
#include "SkTemplates.h"
#include "SkTypeface.h"

#include "SkCanvas.h"
#include "SkData.h"
#include "SkImage.h"
#include "SkPath.h"
#include "SkStream.h"
#include "SkSurface.h"

#include "SkFont.h"
#include <SDL2/SDL.h>
#include <SkBitmap.h>
#include <SkCanvas.h>
#include <SkTypeface.h>
#include "SkDiscretePathEffect.h"
#include "SkGradientShader.h"
#include "SkShader.h"
#include "SkBlurImageFilter.h"
#include "SkTileMode.h"


// RGBA
struct RGBA {
  // Red
  Uint32 rmask = 0x00ff0000;
  // Green
  Uint32 gmask = 0x0000ff00;
  // Blue
  Uint32 bmask = 0x000000ff;
  // Alpha
  Uint32 amask = 0xff000000;
};

void draw_canvas(SkCanvas *canvas) {
  canvas->drawColor(SK_ColorWHITE);

  SkPaint paint;
  paint.setStyle(SkPaint::kFill_Style);
  paint.setAntiAlias(true);
  paint.setStrokeWidth(4);
  paint.setColor(0xff4285F4);

  SkRect rect = SkRect::MakeXYWH(10, 10, 100, 160);
  canvas->drawRect(rect, paint);

  SkRRect oval;
  oval.setOval(rect);
  oval.offset(40, 80);
  paint.setColor(0xffDB4437);
  canvas->drawRRect(oval, paint);

  paint.setColor(0xff0F9D58);
  canvas->drawCircle(180, 50, 25, paint);

  rect.offset(80, 50);
  paint.setColor(0xffF4B400);
  paint.setStyle(SkPaint::kStroke_Style);
  canvas->drawRoundRect(rect, 10, 10, paint);
}

SkPath star() {
  const SkScalar R = 60.0f, C = 128.0f;
  SkPath path;
  path.moveTo(C + R, C);
  for (int i = 1; i < 15; ++i) {
    SkScalar a = 0.44879895f * i;
    SkScalar r = R + R * (i % 2);
    path.lineTo(C + r * cos(a), C + r * sin(a));
  }
  return path;
}



void draw_canvas2(SkCanvas* canvas) {
  SkPaint paint;
  paint.setPathEffect(SkDiscretePathEffect::Make(10.0f, 4.0f));
  SkPoint points[2] = {
    SkPoint::Make(0.0f, 0.0f),
    SkPoint::Make(256.0f, 256.0f)
  };
  SkColor colors[2] = {SkColorSetRGB(66,133,244), SkColorSetRGB(15,157,88)};
  paint.setShader(SkGradientShader::MakeLinear(
    points, colors, NULL, 2,
    SkTileMode::kClamp, 0, NULL));
  paint.setAntiAlias(true);
  canvas->clear(SK_ColorWHITE);
  SkPath path(star());
  canvas->drawPath(path, paint);
}

SkBitmap draw(int w, int h) {
  SkBitmap bitmap;
  bitmap.setInfo(
      SkImageInfo::Make(w, h, kBGRA_8888_SkColorType, kOpaque_SkAlphaType));
  bitmap.allocPixels();
  SkCanvas canvas(bitmap);
  draw_canvas(&canvas);
  return bitmap;
}

SDL_Rect create_rect(SDL_Surface *surface) {
  SDL_Rect src = {0, 0, surface->w, surface->h};
  return src;
}

int main(int argc, char *const argv[]) {
  SDL_Window *window;
  SDL_Surface *surface;
  SDL_Renderer *renderer;
  SDL_Texture *texture;
  SkBitmap bitmap;
  RGBA rgba;
  SDL_Rect rect;
  int width = 800;
  int height = 480;

  SDL_Init(SDL_INIT_VIDEO);

  window = SDL_CreateWindow("Hello Skia", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, width, height,
                            SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
  if (window == NULL) {
    return -1;
  }

  bitmap = draw(width, height);
  surface =
      SDL_CreateRGBSurfaceFrom(bitmap.getPixels(), width, height, 32, width * 4,
                               rgba.rmask, rgba.gmask, rgba.bmask, rgba.amask);
  rect = create_rect(surface);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_RenderClear(renderer);
  texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_RenderCopy(renderer, texture, NULL, &rect);
  SDL_RenderPresent(renderer);
  SDL_Event windowEvent;

  while (true) {
    if (SDL_PollEvent(&windowEvent)) {
      if (SDL_QUIT == windowEvent.type) {
        break;
      }
    }
  }

  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
