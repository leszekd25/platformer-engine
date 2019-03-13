/*

Lua bindings for functions that manipulate entities and components
*/
    

#pragma once

#include <SDL.h>
#include "sol.hpp"
#include "Vector.h"
#include "Rect.h"
#include "GameState.h"
#include "SpriteManager.h"

// general -----------------------------------------------------------------------------------

Tilepos WorldToTile(GameState* gs, Vector v);
Vector TileToWorld(GameState* gs, Tilepos t);
sol::table QueryEntities(GameState* gs, Vector position, float distance, ControlSide side);

// entity ------------------------------------------------------------------------------------

int EntityCreate(GameState* gs);
void EntityDestroy(GameState* gs, int entity_id);
void EntityComponentEnable(GameState* gs, int entity_id, int component_id);
void EntityComponentDisable(GameState* gs, int entity_id, int component_id);

// time

unsigned int TimeGetTicks(GameState* gs);
float TimeGetSeconds(GameState* gs);
float TimeGetDeltatime(GameState* gs);

// transform component ------------------------------------------------------------------------

void TransformSet(GameState* gs, int entity_id, Vector pos, float rot, Vector scale);
void TransformSetPosition(GameState* gs, int entity_id, Vector pos);
void TransformAddPosition(GameState* gs, int entity_id, Vector pos);
Vector TransformGetPosition(GameState* gs, int entity_id);
void TransformSetRotation(GameState* gs, int entity_id, float rot);
void TransformAddRotation(GameState* gs, int entity_id, float rot);
float TransformGetRotation(GameState* gs, int entity_id);
void TransformSetScale(GameState* gs, int entity_id, Vector scale);
void TransformAddScale(GameState* gs, int entity_id, Vector scale);
Vector TransformGetScale(GameState* gs, int entity_id);
Tilepos TransformGetTilepos(GameState* gs, int entity_id);

// movable component ---------------------------------------------------------------------------------------

void MovableSet(GameState* gs, int entity_id, Vector speed, Vector accel);
void MovableSetSpeed(GameState* gs, int entity_id, Vector speed);
void MovableAddSpeed(GameState* gs, int entity_id, Vector speed);
Vector MovableGetSpeed(GameState* gs, int entity_id);
void MovableSetAcceleration(GameState* gs, int entity_id, Vector accel);
void MovableAddAcceleration(GameState* gs, int entity_id, Vector accel);
Vector MovableGetAcceleration(GameState* gs, int entity_id);

// drawables ---------------------------------------------------------------------------------------------

void DrawableSet(GameState* gs, SpriteManager* sm, int entity_id, const std::string& s_name, Color col);
void DrawableSetSprite(GameState* gs, SpriteManager* sm, int entity_id, const std::string& s_name);
std::string DrawableGetSprite(GameState* gs, SpriteManager* sm, int entity_id);
void DrawableSetColor(GameState* gs, int entity_id, Color col);
Color DrawableGetColor(GameState* gs, int entity_id);

// animation ----------------------------------------------------------------------------------------------------

void AnimationSet(GameState* gs, int entity_id, int curimage, float frametime, bool repeat);
void AnimationSetCurrentImage(GameState* gs, int entity_id, int curimage);
int AnimationGetCurrentImage(GameState* gs, int entity_id);
void AnimationSetFrameTime(GameState* gs, int entity_id, float frametime);
float AnimationGetFrameTime(GameState* gs, int entity_id);
void AnimationSetRepeat(GameState* gs, int entity_id, bool repeat);
bool AnimationGetRepeat(GameState* gs, int entity_id);
void AnimationSetOnAnimationEndCallback(GameState* gs, int entity_id, sol::function f);

// collision

void CollidableSet(GameState* gs, int entity_id, unsigned int col_mask, Rect bbox);
void CollidableSetCollisionMask(GameState* gs, int entity_id, unsigned int col_mask);
unsigned int CollidableGetCollisionMask(GameState* gs, int entity_id);
void CollidableSetBoundingBox(GameState* gs, int entity_id, Rect bbox);
Rect CollidableGetBoundingBox(GameState* gs, int entity_id);

// damageable

void DamageableSet(GameState* gs, int entity_id, unsigned int damagedby_mask, float hp, float max_hp);
void DamageableSetMask(GameState* gs, int entity_id, unsigned int damagedby_mask);
unsigned int DamageableGetMask(GameState* gs, int entity_id);
void DamageableSetHealth(GameState* gs, int entity_id, float hp);
float DamageableGetHealth(GameState* gs, int entity_id);
void DamageableSetMaxHealth(GameState* gs, int entity_id, float max_hp);
float DamageableGetMaxHealth(GameState* gs, int entity_id);

// damaging

void DamagingSet(GameState* gs, int entity_id, unsigned int damaging_mask, float dmg);
void DamagingSetMask(GameState* gs, int entity_id, unsigned int damaging_mask);
unsigned int DamagingGetMask(GameState* gs, int entity_id);
void DamagingSetDamage(GameState* gs, int entity_id, float dmg);
float DamagingGetDamage(GameState* gs, int entity_id);

// physics

void PhysicsSet(GameState* gs, int entity_id, int world_state, float mass);
void PhysicsSetWorldState(GameState* gs, int entity_id, int world_state);
int PhysicsGetWorldState(GameState* gs, int entity_id);
void PhysicsSetMass(GameState* gs, int entity_id, float mass);
float PhysicsGetMass(GameState* gs, int entity_id);

// world configuration

void WorldSetGravity(GameState* gs, float gv);
float WorldGetGravity(GameState* gs);

// input

void InputSetEventCallback(GameState* gs, int entity_id, int event_code, sol::function func);
bool InputIsKeyPressed(GameState* gs, int scancode);

// control

int ControlGetSide(GameState* gs, int entity_id);
void ControlSetSide(GameState* gs, int entity_id, ControlSide side);
void ControlSetStepCallback(GameState* gs, int entity_id, sol::function f);
void ControlInitializeTable(GameState* gs, int entity_id);
sol::table& ControlGetTable(GameState* gs, int entity_id);

// state machine

void StateMachineAddState(GameState* gs, int entity_id, int state, sol::function f);
int StateMachineGetState(GameState* gs, int entity_id);
void StateMachineSetState(GameState* gs, int entity_id, int state);
void StateMachineInitTransitions(GameState* gs, int entity_id);
void StateMachineAddTransition(GameState* gs, int entity_id, int from, int to, sol::function f);
void StateMachineTransitionTo(GameState* gs, int entity_id, int to);

// timer

int TimerAddTimer(GameState* gs, int entity_id, sol::function f);
void TimerSetTime(GameState* gs, int entity_id, int timer_id, float next_time);

void RegisterLuaGameState(sol::state& lua);