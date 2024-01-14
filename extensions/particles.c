#include <assert.h>

#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"

#define emitter_udata "particles.emitter"
#define PARTICLE_COUNT 2000

typedef struct {
	int life;
	float x;
	float y;
	float xvel;
	float yvel;
	float scale;
} particle_t;

typedef struct {
	int texture;
	float width;
	float height;
	int life;
	float damping;
	float delta_scale;
	int next;
	particle_t particles[PARTICLE_COUNT];
} emitter_t;

typedef struct {
	float alpha;
	float p1x,p1y,p2x,p2y,p3x,p3y,p4x,p4y;
} quad_t;

static inline void draw_particle(emitter_t *emitter, particle_t *particle, quad_t *quad) {
	if (particle->life) {
		const float dx = emitter->width/2 * particle->scale;
		const float dy = emitter->height/2 * particle->scale;
		quad->alpha = (float)particle->life / emitter->life;
		quad->p1x = particle->x - dx;
		quad->p1y = particle->y - dy;
		quad->p2x = particle->x + dx;
		quad->p2y = particle->y - dy;
		quad->p3x = particle->x + dx;
		quad->p3y = particle->y + dy;
		quad->p4x = particle->x - dx;
		quad->p4y = particle->y + dy;
	}
}

static int emitter__draw(lua_State *L) {
	emitter_t *emitter = luaL_checkudata(L, 1, "particles.emitter");
	quad_t quad;
	for (int i = emitter->next; i != PARTICLE_COUNT; i++) {
		draw_particle(emitter, &emitter->particles[i], &quad);
	}
	for (int i = 0; i != emitter->next; i++) {
		draw_particle(emitter, &emitter->particles[i], &quad);
	}
	return 0;
}

static int emitter__update(lua_State *L) {
	emitter_t *emitter = luaL_checkudata(L, 1, emitter_udata);

	for (particle_t *particle = emitter->particles;
		particle != emitter->particles + PARTICLE_COUNT;
		particle++) {
		if (particle->life) {
			particle->life--;
			particle->x += particle->xvel;
			particle->y += particle->yvel;
			particle->xvel *= emitter->damping;
			particle->yvel *= emitter->damping;
			particle->scale += emitter->delta_scale;
		}
	}

	return 0;
}

static int emitter__add_particle(lua_State *L) {
	emitter_t *emitter = luaL_checkudata(L, 1, emitter_udata);
	float x = (float)luaL_checknumber(L, 2);
	float y = (float)luaL_checknumber(L, 3);
	float xvel = (float)luaL_checknumber(L, 4);
	float yvel = (float)luaL_checknumber(L, 5);

	// grab a particle slot from the emitter
	particle_t *particle = &emitter->particles[emitter->next];
	emitter->next = (emitter->next + 1) % PARTICLE_COUNT;

	// initialize it
	particle->life = emitter->life;
	particle->x = x;
	particle->y = y;
	particle->xvel = xvel;
	particle->yvel = yvel;
	particle->scale = 1;

	return 0;
}

static int particles__make_emitter(lua_State *L) {
	float width = (float)luaL_checknumber(L, 1);
	float height = (float)luaL_checknumber(L, 2);
	int texture = luaL_checkint(L, 3);
	int life = luaL_checkint(L, 4);
	float damping = luaL_checknumber(L, 5);
	float delta_scale = luaL_checknumber(L, 6);

	emitter_t *emitter = lua_newuserdata(L, sizeof(emitter_t));
	luaL_getmetatable(L, emitter_udata);
	lua_setmetatable(L, -2);

	emitter->width = width;
	emitter->height = height;
	emitter->texture = texture;
	emitter->life = life;
	emitter->damping = damping;
	emitter->delta_scale = delta_scale;

	emitter->next = 0;
	for (int i = 0; i < PARTICLE_COUNT; i++) {
		emitter->particles[i].life = 0;
	}

	return 1;
}

static const luaL_Reg emitter_lib[] = {
	{"draw", emitter__draw},
	{"update", emitter__update},
	{"add_particle", emitter__add_particle},
	{NULL, NULL}
};

static const luaL_Reg particles_lib[] = {
	{"make_emitter", particles__make_emitter},
	{NULL, NULL}
};

int luaopen_particles(lua_State *L) {
	luaL_newmetatable(L, "particles.emitter");
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	luaL_register(L, NULL, emitter_lib);

	lua_newtable(L);
	luaL_register(L, NULL, particles_lib);
	return 1;
}

