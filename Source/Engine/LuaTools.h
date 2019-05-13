#pragma once
#include <iostream>
#include "Source/Engine/SmartPointer.h"
#include "Source/Engine/GameObject.h"

#ifdef __cplusplus
#include "External/Lua/src/lua.hpp"
#else
#include "External/Lua/src/lua.h"
#include "External/Lua/src/lualib.h"
#include "External/Lua/src/lauxlib.h"
#endif

float getNumberField(lua_State * L, const char * field) {

	lua_getfield(L, 1, field);
	int top = lua_gettop(L);
	LUA_NUMBER result = lua_tonumber(L, -1);
	printf("	%s: %lf\n", field, result);
	return result;

}
static SmartPointer<GameObject> * CreateSmartPointerFromFile(const char * fileName) {

	//vars
	GameObject * gameObject = new GameObject();
	int top = 0;

	//get lua instance
	lua_State * pLuaState = luaL_newstate();
	assert(pLuaState);

	luaL_openlibs(pLuaState); //load lua libraries 

	//load file from data folder
	luaL_dofile(pLuaState, fileName);
	printf("Reading from file: %s\nGameObject {\n\n", fileName);

	top = lua_gettop(pLuaState);

	lua_getglobal(pLuaState, "Tag");
	top = lua_gettop(pLuaState);
	if (lua_isnil(pLuaState, 1)) {
		printf("Tag is nil!\n");
	}
	else {
		gameObject->tag = lua_tostring(pLuaState, 1);
		printf("	Tag: %s\n\n", gameObject->tag.c_str());
	}
	//pop string
	lua_pop(pLuaState, top);
	top = lua_gettop(pLuaState);

	//pop string
	lua_pop(pLuaState, top);
	top = lua_gettop(pLuaState);

	//pop string
	lua_pop(pLuaState, top);
	top = lua_gettop(pLuaState);

	lua_getglobal(pLuaState, "PhysicsComponent");
	top = lua_gettop(pLuaState);
	if (lua_isnil(pLuaState, -1)) {
		printf("PhysicsComponent is nil!\n");
	}
	else if (lua_istable(pLuaState, -1)) {

		printf("	PhysicsComponent: {\n\n");

		gameObject->physics->mass = getNumberField(pLuaState, "mass");
		lua_pop(pLuaState, -2);

		gameObject->physics->drag = getNumberField(pLuaState, "drag");
		lua_pop(pLuaState, -2);

		gameObject->physics->Xforce = getNumberField(pLuaState, "Xforce");
		lua_pop(pLuaState, -2);

		gameObject->physics->Yforce = getNumberField(pLuaState, "Yforce");
		lua_pop(pLuaState, -2);

		gameObject->physics->appliedForce = getNumberField(pLuaState, "appliedForce");
		lua_pop(pLuaState, -2);


		lua_pushstring(pLuaState, "position");
		lua_gettable(pLuaState, -2);
		if (lua_istable(pLuaState, -1)) {
			size_t arrayLength = lua_rawlen(pLuaState, -1);
			int top = lua_gettop(pLuaState);
			float pos[3];
			size_t index = 0;

			for (lua_pushnil(pLuaState); lua_next(pLuaState, -2); lua_pop(pLuaState, 1))
			{
				if (index >= arrayLength) { break; }
				/* perform operation */
				pos[index] = lua_tonumber(pLuaState, -1);
				index++;

			}
			gameObject->physics->transform.position.x = pos[0];
			gameObject->physics->transform.position.y = pos[1];
			gameObject->physics->transform.position.z = pos[2];
			printf("	position: { %f, %f, %f }\n", pos[0], pos[1], pos[2]);

		}
		lua_pop(pLuaState, -2);

		lua_pushstring(pLuaState, "velocity");
		lua_gettable(pLuaState, -2);
		if (lua_istable(pLuaState, -1)) {
			size_t arrayLength = lua_rawlen(pLuaState, -1);
			int top = lua_gettop(pLuaState);
			float pos[3];
			size_t index = 0;

			for (lua_pushnil(pLuaState); lua_next(pLuaState, -2); lua_pop(pLuaState, 1))
			{
				if (index >= arrayLength) { break; }
				/* perform operation */
				pos[index] = lua_tonumber(pLuaState, -1);
				index++;

			}
			gameObject->physics->velocity.x = pos[0];
			gameObject->physics->velocity.y = pos[1];
			gameObject->physics->velocity.z = pos[2];
			printf("	velocity: { %f, %f, %f }\n", pos[0], pos[1], pos[2]);

		}
		printf("	\n	}\n");
		lua_pop(pLuaState, -2);

		printf("\n}\n");

		//pop table
		lua_pop(pLuaState, -1);
	}

	lua_close(pLuaState);

	return new SmartPointer<GameObject>(gameObject);
}
