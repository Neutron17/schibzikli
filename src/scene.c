#include "scene.h"
#include "base/arr.h"
#include "base/error.h"
#include "base/log.h"
#include "graph.h"
#include <SDL2/SDL_render.h>
#include <stdint.h>
#include <stdio.h>
#include <arpa/inet.h>

// TODO: endian-independence (ntohl)
Scene sceneParse(const char *fname, const char *resourcePath) { 
	FILE *file = fopen(fname, "rb");
	if(!file) {
		// TODO: error handling
		LOG(L_ERR, "Couldn't open file");
		return (Scene) {0};
	}
	Scene ret = { 0 };
	UNWRAP_TO_COMPLEX(array_init(SDL_Texture *, 10), ret.textures, Array_t);
	UNWRAP_TO_COMPLEX(array_init(struct EntitySereal, 10), ret.entities, Array_t);
	int c;
	while((c=fgetc(file)) != EOF && c != PT_START);
	while((c=fgetc(file)) != EOF && c != PT_END) {
		if(c == PT_INSTR_B) {
			switch(fgetc(file)) {
				case PI_TEXTURE: txt:{
					const long startOffs = ftell(file);
					while(fgetc(file) != '\0');
					const long sz = ftell(file)-startOffs;
					char *buff = malloc(sz*sizeof(char));
					fseek(file, startOffs, SEEK_SET);
					fgets(buff, sz, file);

					SDL_Texture *texture = textureLoad(buff);
					free(buff);
					array_push(&ret.textures, texture);

					fgetc(file);
					if((c=fgetc(file)) != PT_INSTR_E) {
						ungetc(c, file);
						goto txt;
					}
				} break;

				case PI_ENTITY: ent:{
					struct EntitySereal entity;
					fread(&entity, sizeof(struct EntitySereal), 1, file);
					array_push(&ret.entities, &entity);
					if((c=fgetc(file)) != PT_INSTR_E) {
						ungetc(c, file);
						goto ent;
					}

				} break;

				case PI_TILE: {
					uint32_t d[2];
					fread(d, 4, 2, file);
					ret.tile_w = d[0];
					ret.tile_h = d[1];

					ret.tiles = malloc(d[0]*d[1]*sizeof(uint32_t));
					fread(ret.tiles, 4, d[0]*d[1], file);
				} break;

				default: {
					// TODO: error handling
					LOG(L_ERR, "Unexpected instruction");
					return (Scene) {0};
				} break;
			}
		}
	}
	fclose(file);
	return ret;
}
void sceneUnload(Scene *sc);
