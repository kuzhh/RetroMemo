#include "sound.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MIXER_CANAL_LIBRE -1 // Reproduce en el primer canal libre disponible
#define MIXER_CANAL_PRINCIPAL 0 //canal para la musica principal.
#define BUFFER_AUDIO 1024
#define MONO 1

struct sSound{
    Mix_Chunk *chunk;
    uint8_t isTone;
};

tFormatsSnd sound_start()
{
    tFormatsSnd formatsSnd = SOUND_MP3;
    int32_t flags = MIX_INIT_MP3;// Soporte para MP3
    int32_t init = Mix_Init(flags); // Inicializa SDL_mixer

    if ((init & flags) != flags) {
        fprintf(stderr,"Error: %s\n", Mix_GetError());
        Mix_Quit();
        return SOUND_ERR;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MONO, BUFFER_AUDIO) < 0) { // Abre el dispositivo de audio en modo Mono
        fprintf(stderr, "Error: No se pudo abrir el dispositivo de audio: %s\n", Mix_GetError());
        Mix_Quit();
        return SOUND_ERR;
    }

    Mix_Volume(MIXER_CANAL_PRINCIPAL,5); // reduccion del volumen.
    Mix_Volume(MIXER_CANAL_LIBRE,50); // reduccion del volumen.

    return formatsSnd;
}

tSound* sound_load(const char *path)
{
    tSound *sound = malloc(sizeof(tSound));
    if (!sound) {
        return NULL;
    }

    sound->isTone = 0;
    sound->chunk = Mix_LoadWAV(path);
    if (!sound->chunk) {
        fprintf(stderr, "Fallo la carga del sonido \"%s:\" %s\n", path, Mix_GetError());
        free(sound);
        return NULL;
    }

    return sound;
}

void sound_play(const tSound *sound, int32_t cantVeces, int channel)
{
    if (!sound->chunk) {
        return;
    }

    int32_t loops;
    if (cantVeces == -1) {
        loops = -1; //reproduce de forma infinito hasta finalizar programa
    } else {
        loops = (cantVeces > 0) ? (cantVeces - 1) : 0; // 0 reproduce 1 vez, 1 reproduce 2 veces
    }

    if (Mix_PlayChannel(channel == 0 ? MIXER_CANAL_PRINCIPAL : MIXER_CANAL_LIBRE, sound->chunk, loops) == -1) {
        fprintf(stderr, "No se pudo reproducir el sonido: %s\n", Mix_GetError());
    }
}

void sound_destroy(tSound *sound)
{
    if (!sound) {
        return;
    }

    if (sound->isTone) {
        if (sound->chunk) {
            if (sound->chunk->abuf) {
                free(sound->chunk->abuf);
            }
            free(sound->chunk);
        }
    } else {
        if (sound->chunk) {
            Mix_FreeChunk(sound->chunk);
        }
    }

    free(sound);
}

void sound_finish(void)
{
    Mix_HaltChannel(-1);
    Mix_CloseAudio();
    Mix_Quit();
}
