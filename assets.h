#ifndef ASSETS_H_INCLUDED
#define ASSETS_H_INCLUDED

#include "library.h"

#include "input.h"
#include "sound.h"

typedef enum { BTN_NORMAL, BTN_HOVER, BTN_PRESSED } BtnState;
typedef enum { CSET_DS, CSET_GREEK } CardSetType;

extern const char *ds_card_paths[MAX_CARD_TYPES];
extern const char *greek_card_paths[MAX_CARD_TYPES];

typedef struct {
  SDL_Rect rect;
  SDL_Texture *normal;
  SDL_Texture *hover;
  SDL_Texture *pressed;
  BtnState state;
  tSound *melody;
} tButton;

typedef struct {
  SDL_Texture *cardFront[MAX_CARD_TYPES];
  SDL_Texture *cardBack;
} tCardSet;

typedef struct {
  tCardSet dsSet;
  tCardSet greekSet;

  SDL_Texture *buttonNormal;
  SDL_Texture *buttonHover;
  SDL_Texture *buttonPressed;

  SDL_Texture *background;
  SDL_Texture *backgroundGame;

  SDL_Texture *settings;
  SDL_Texture *plus;
  SDL_Texture *minus;
  SDL_Texture *scoreBox;

  SDL_Texture *logo;

  SDL_Texture *back;

  TTF_Font *font;
} tAssets;

int cardsetLoad(tCardSet *, SDL_Renderer *, CardSetType);
void cardsetUnload(tCardSet *);

int assetsLoad(tAssets *, SDL_Renderer *);
void assetsUnload(tAssets *);

void btnUpdate(tButton *, tInput *);

#endif // ASSETS_H_INCLUDED
