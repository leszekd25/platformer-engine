#include "LuaGameState.h"

#include "Message.h"

// general ---------------------------------------------------------------------------------------

Tilepos WorldToTile (GameState* gs, Vector v)
{
	Tilepos t;
	gs->map->WorldToTile(v, t);
	return t;
}

Vector TileToWorld(GameState* gs, Tilepos t)
{
	Vector v;
	gs->map->TileToWorld(t, v);
	return v;
}

// note: for now, distance is in manhattan metric
// note: slow for now, todo: lookup structure...
sol::table QueryEntities(GameState* gs, Vector pos, float distance, ControlSide side)
{
	std::vector<GameObject>& entities = gs->entities;
	std::vector<TransformComponent>& transforms = gs->transforms;
	std::vector<CollidableComponent>& collidables = gs->collidables;
	std::vector<ControlComponent>& controls = gs->controls;
	Tilemap* map = gs->map;

	int last_active_entity = gs->last_active_entity;
	Rect r;

	sol::table tb = gs->lua->create_table();
	int tb_index = 0;

	for (int i = 0; i <= last_active_entity; i++)
	{
		if ((entities[i].components[C_TRANSFORM]) && (entities[i].components[C_COLLIDABLE]))
		{
			r = gs->collidables[i].bbox + gs->transforms[i].position;
			if (r.ManhattanDistance(pos) <= distance)
			{
				if (entities[i].components[C_CONTROL])
				{
					if (controls[i].side == side)
					{
						// add to table
						tb_index++;
						tb[tb_index] = i;
					}
				}
				else if(side == SIDE_NEUTRAL)
				{

					tb_index++;
					tb[tb_index] = i;
				}
			}
		}
	}

	return tb;
}

// entities ------------------------------------------------------------------------------------------

int EntityCreate(GameState* gs)
{
	int entity_id = gs->GetFirstFreeEntity();
	gs->ActivateEntity(entity_id);

	return entity_id;
}

void EntityDestroy(GameState* gs, int entity_id)
{
	gs->FreeEntity(entity_id);
}

void EntityComponentEnable(GameState* gs, int entity_id, int component_id)
{
	gs->SetEntityComponent(entity_id, component_id, true);
}

void EntityComponentDisable(GameState* gs, int entity_id, int component_id)
{
	gs->SetEntityComponent(entity_id, component_id, false);
}
// time

unsigned int TimeGetTicks(GameState* gs)
{
	return gs->timer.GetTicks();
}

float TimeGetSeconds(GameState* gs)
{
	return gs->timer.GetTime();
}

float TimeGetDeltatime(GameState* gs)
{
	return gs->timer.GetDeltaTime();
}

// transform component ------------------------------------------------------------------------

void TransformSet(GameState* gs, int entity_id, Vector pos, float rot, Vector scale)
{
	TransformComponent& transform = gs->transforms[entity_id];
	transform.position = pos;
	transform.angle = rot;
	transform.scale = scale;
}

void TransformSetPosition(GameState* gs, int entity_id, Vector pos)
{
	TransformComponent& transform = gs->transforms[entity_id];
	transform.position = pos;
}

void TransformAddPosition(GameState* gs, int entity_id, Vector pos)
{
	TransformComponent& transform = gs->transforms[entity_id];
	transform.position += pos;
}

Vector TransformGetPosition(GameState* gs, int entity_id)
{
	TransformComponent& transform = gs->transforms[entity_id];
	return transform.position;
}

void TransformSetRotation(GameState* gs, int entity_id, float rot)
{
	TransformComponent& transform = gs->transforms[entity_id];
	transform.angle = rot;
}

void TransformAddRotation(GameState* gs, int entity_id, float rot)
{
	TransformComponent& transform = gs->transforms[entity_id];
	transform.angle += rot;
}

float TransformGetRotation(GameState* gs, int entity_id)
{
	TransformComponent& transform = gs->transforms[entity_id];
	return transform.angle;
}

void TransformSetScale(GameState* gs, int entity_id, Vector scale)
{
	TransformComponent& transform = gs->transforms[entity_id];
	transform.scale = scale;
}

void TransformAddScale(GameState* gs, int entity_id, Vector scale)
{
	TransformComponent& transform = gs->transforms[entity_id];
	transform.scale += scale;
}

Vector TransformGetScale(GameState* gs, int entity_id)
{
	TransformComponent& transform = gs->transforms[entity_id];
	return transform.scale;
}

Tilepos TransformGetTilepos(GameState* gs, int entity_id)
{
	TransformComponent& transform = gs->transforms[entity_id];
	Tilepos t;
	gs->map->WorldToTile(transform.position, t);
	return t;
}

// movable component ---------------------------------------------------------------------------------------

void MovableSet(GameState* gs, int entity_id, Vector speed, Vector accel)
{
	MovableComponent& movable = gs->movables[entity_id];
	movable.speed = speed;
	movable.acceleration = accel;
}

void MovableSetSpeed(GameState* gs, int entity_id, Vector speed)
{
	MovableComponent& movable = gs->movables[entity_id];
	movable.speed = speed;
}

void MovableAddSpeed(GameState* gs, int entity_id, Vector speed)
{
	MovableComponent& movable = gs->movables[entity_id];
	movable.speed += speed;
}

Vector MovableGetSpeed(GameState* gs, int entity_id)
{
	MovableComponent& movable = gs->movables[entity_id];
	return movable.speed;
}

void MovableSetAcceleration(GameState* gs, int entity_id, Vector accel)
{
	MovableComponent& movable = gs->movables[entity_id];
	movable.acceleration = accel;
}

void MovableAddAcceleration(GameState* gs, int entity_id, Vector accel)
{
	MovableComponent& movable = gs->movables[entity_id];
	movable.acceleration += accel;
}

Vector MovableGetAcceleration(GameState* gs, int entity_id)
{
	MovableComponent& movable = gs->movables[entity_id];
	return movable.acceleration;
}

// drawables ---------------------------------------------------------------------------------------------

void DrawableSet(GameState* gs, SpriteManager* sm, int entity_id, const std::string& s_name, Color col)
{
	DrawableComponent& drawable = gs->drawables[entity_id];
	drawable.sprite = sm->GetSprite(s_name);
	drawable.color = col;
}

void DrawableSetSprite(GameState* gs, SpriteManager* sm, int entity_id, const std::string& s_name)
{
	DrawableComponent& drawable = gs->drawables[entity_id];
	drawable.sprite = sm->GetSprite(s_name);
}

std::string DrawableGetSprite(GameState* gs, SpriteManager* sm, int entity_id)
{
	DrawableComponent& drawable = gs->drawables[entity_id];
	return drawable.sprite->name;
}

void DrawableSetColor(GameState* gs, int entity_id, Color col)
{
	DrawableComponent& drawable = gs->drawables[entity_id];
	drawable.color = col;
}

Color DrawableGetColor(GameState* gs, int entity_id)
{
	DrawableComponent& drawable = gs->drawables[entity_id];
	return drawable.color;
}

// animation ----------------------------------------------------------------------------------------------------

void AnimationSet(GameState* gs, int entity_id, int curimage, float frametime, bool repeat)
{
	AnimationComponent& animation = gs->animations[entity_id];
	animation.current_image = curimage;
	animation.time_per_frame = frametime;
	animation.repeat = repeat;
}

void AnimationSetCurrentImage(GameState* gs, int entity_id, int curimage)
{
	AnimationComponent& animation = gs->animations[entity_id];
	animation.current_image = curimage;
}

int AnimationGetCurrentImage(GameState* gs, int entity_id)
{
	AnimationComponent& animation = gs->animations[entity_id];
	return animation.current_image;
}

void AnimationSetFrameTime(GameState* gs, int entity_id, float frametime)
{
	AnimationComponent& animation = gs->animations[entity_id];
	animation.time_per_frame = frametime;
}

float AnimationGetFrameTime(GameState* gs, int entity_id)
{
	AnimationComponent& animation = gs->animations[entity_id];
	return animation.time_per_frame;
}

void AnimationSetRepeat(GameState* gs, int entity_id, bool repeat)
{
	AnimationComponent& animation = gs->animations[entity_id];
	animation.repeat = repeat;
}

bool AnimationGetRepeat(GameState* gs, int entity_id)
{
	AnimationComponent& animation = gs->animations[entity_id];
	return animation.repeat;
}

void AnimationSetOnAnimationEndCallback(GameState* gs, int entity_id, sol::function f)
{
	AnimationComponent& animation = gs->animations[entity_id];
	animation.on_anim_end = new sol::function(f);
}

// collidable component ------------------------------------------------------------------------------------

void CollidableSet(GameState* gs, int entity_id, unsigned int col_mask, Rect bbox)
{
	CollidableComponent& collidable = gs->collidables[entity_id];
	collidable.collision_mask = col_mask;
	collidable.bbox = bbox;
}

void CollidableSetCollisionMask(GameState* gs, int entity_id, unsigned int col_mask)
{
	CollidableComponent& collidable = gs->collidables[entity_id];
	collidable.collision_mask = col_mask;
}

unsigned int CollidableGetCollisionMask(GameState* gs, int entity_id)
{
	CollidableComponent& collidable = gs->collidables[entity_id];
	return collidable.collision_mask;
}

void CollidableSetBoundingBox(GameState* gs, int entity_id, Rect bbox)
{
	CollidableComponent& collidable = gs->collidables[entity_id];
	collidable.bbox = bbox;
}

Rect CollidableGetBoundingBox(GameState* gs, int entity_id)
{
	CollidableComponent& collidable = gs->collidables[entity_id];
	return collidable.bbox;
}

// damageable component --------------------------------------------------------------------------------

void DamageableSet(GameState* gs, int entity_id, unsigned int damagedby_mask, float hp, float max_hp)
{
	DamageableComponent& damageable = gs->damageables[entity_id];
	damageable.damagedby_mask = damagedby_mask;
	damageable.health = hp;
	damageable.max_health = max_hp;
}

void DamageableSetMask(GameState* gs, int entity_id, unsigned int damagedby_mask)
{
	DamageableComponent& damageable = gs->damageables[entity_id];
	damageable.damagedby_mask = damagedby_mask;
}

unsigned int DamageableGetMask(GameState* gs, int entity_id)
{
	DamageableComponent& damageable = gs->damageables[entity_id];
	return damageable.damagedby_mask;
}

void DamageableSetHealth(GameState* gs, int entity_id, float hp)
{
	DamageableComponent& damageable = gs->damageables[entity_id];
	damageable.health = hp;
}

float DamageableGetHealth(GameState* gs, int entity_id)
{
	DamageableComponent& damageable = gs->damageables[entity_id];
	return damageable.health;
}

void DamageableSetMaxHealth(GameState* gs, int entity_id, float max_hp)
{
	DamageableComponent& damageable = gs->damageables[entity_id];
	damageable.max_health = max_hp;
}

float DamageableGetMaxHealth(GameState* gs, int entity_id)
{
	DamageableComponent& damageable = gs->damageables[entity_id];
	return damageable.max_health;
}

// damaging component ----------------------------------------------------------------------

void DamagingSet(GameState* gs, int entity_id, unsigned int damaging_mask, float dmg)
{
	DamagingComponent& damaging = gs->damagings[entity_id];
	damaging.damages_mask = damaging_mask;
	damaging.damage = dmg;
}

void DamagingSetMask(GameState* gs, int entity_id, unsigned int damaging_mask)
{
	DamagingComponent& damaging = gs->damagings[entity_id];
	damaging.damages_mask = damaging_mask;
}

unsigned int DamagingGetMask(GameState* gs, int entity_id)
{
	DamagingComponent& damaging = gs->damagings[entity_id];
	return damaging.damages_mask;
}

void DamagingSetDamage(GameState* gs, int entity_id, float dmg)
{
	DamagingComponent& damaging = gs->damagings[entity_id];
	damaging.damage = dmg;
}

float DamagingGetDamage(GameState* gs, int entity_id)
{
	DamagingComponent& damaging = gs->damagings[entity_id];
	return damaging.damage;
}

// physics component --------------------------------------------------------------------------

void PhysicsSet(GameState* gs, int entity_id, int world_state, float mass)
{
	PhysicsComponent& physics = gs->physicss[entity_id];
	physics.world_state = (WorldState)world_state;
	physics.mass = mass;
}

void PhysicsSetWorldState(GameState* gs, int entity_id, int world_state)
{
	PhysicsComponent& physics = gs->physicss[entity_id];
	physics.world_state = (WorldState)world_state;
}

int PhysicsGetWorldState(GameState* gs, int entity_id)
{
	PhysicsComponent& physics = gs->physicss[entity_id];
	return physics.world_state;
}

void PhysicsSetMass(GameState* gs, int entity_id, float mass)
{
	PhysicsComponent& physics = gs->physicss[entity_id];
	physics.mass = mass;
}

float PhysicsGetMass(GameState* gs, int entity_id)
{
	PhysicsComponent& physics = gs->physicss[entity_id];
	return physics.mass;
}

// input component ----------------------------------------------------------------------------

void InputSetEventCallback(GameState* gs, int entity_id, int event_code, sol::function func)
{
	InputComponent& input = gs->inputs[entity_id];
	if (input.functions[event_code] != 0)
		delete input.functions[event_code];
	input.functions[event_code] = new sol::function(func);
}

bool InputIsKeyPressed(GameState* gs, int key_scancode)
{
	return gs->keys_pressed[key_scancode];
}

// control component --------------------------------------------------------------------------

int ControlGetSide(GameState* gs, int entity_id)
{
	return gs->controls[entity_id].side;
}

void ControlSetSide(GameState* gs, int entity_id, ControlSide side)
{
	gs->controls[entity_id].side = side;
}

void ControlSetStepCallback(GameState* gs, int entity_id, sol::function f)
{
	gs->controls[entity_id].step_function = new sol::function(f);
}

void ControlInitializeTable(GameState* gs, int entity_id)
{
	gs->controls[entity_id].lua_values = gs->lua->create_table();
}

sol::table& ControlGetTable(GameState* gs, int entity_id)
{
	return gs->controls[entity_id].lua_values;
}

// state machine component --------------------------------------------------------------------

void StateMachineAddState(GameState* gs, int entity_id, int state, sol::function f)
{
	gs->statemachines[entity_id].AddState(state, f);
}

int StateMachineGetState(GameState* gs, int entity_id)
{
	return gs->statemachines[entity_id].current_state;
}

void StateMachineSetState(GameState* gs, int entity_id, int state)
{
	gs->statemachines[entity_id].SetState(state);
}

void StateMachineInitTransitions(GameState* gs, int entity_id)
{
	gs->statemachines[entity_id].InitTransitions();
}

void StateMachineAddTransition(GameState* gs, int entity_id, int from, int to, sol::function f)
{
	gs->statemachines[entity_id].AddTransition(from, to, new sol::function(f));
}
void StateMachineTransitionTo(GameState* gs, int entity_id, int to)
{
	StateTransitionMessage* msg = new StateTransitionMessage();
	msg->source = UniqueEntityIdentifier(entity_id, gs->entities[entity_id].id);
	msg->from = gs->statemachines[entity_id].current_state;
	msg->to = to;
	gs->messages.push(msg);
}

// timer component ----------------------------------------------------------------------------

int TimerAddTimer(GameState* gs, int entity_id, sol::function f)
{
	gs->timers[entity_id].timers[gs->timers[entity_id].max_timers].callback = f;
	gs->timers[entity_id].max_timers++;
	return gs->timers[entity_id].max_timers - 1;
}

void TimerSetTime(GameState* gs, int entity_id, int timer_id, float time)
{
	gs->timers[entity_id].timers[timer_id].time = gs->timer.GetTime() + time;
	gs->timers[entity_id].timers[timer_id].active = true;
}

// world configuration ------------------------------------------------------------------------

void WorldSetGravity(GameState* gs, float gv)
{
	gs->world_properties.gravity_strength = gv;
}

float WorldGetGravity(GameState* gs)
{
	return gs->world_properties.gravity_strength;
}

void RegisterLuaGameState(sol::state& lua)
{
	lua["C_TRANSFORM"] = C_TRANSFORM;
	lua["C_MOVABLE"] = C_MOVABLE;
	lua["C_DRAWABLE"] = C_DRAWABLE;
	lua["C_ANIMATION"] = C_ANIMATION;
	lua["C_COLLIDABLE"] = C_COLLIDABLE;
	lua["C_DAMAGEABLE"] = C_DAMAGEABLE;
	lua["C_DAMAGING"] = C_DAMAGING;
	lua["C_PHYSICS"] = C_PHYSICS;
	lua["C_INPUT"] = C_INPUT;
	lua["C_CONTROL"] = C_CONTROL;
	lua["C_STATEMACHINE"] = C_STATEMACHINE;
	lua["C_TIMER"] = C_TIMER;
	lua["C_MAX"] = C_MAX;

	lua["ON_GROUND"] = ON_GROUND;
	lua["IN_AIR"] = IN_AIR;

	lua["INPUT_KEY_PRESS"] = INPUT_KEY_PRESS;
	lua["INPUT_KEY_RELEASE"] = INPUT_KEY_RELEASE;
	lua["INPUT_MAX"] = INPUT_MAX;

	lua["SIDE_PLAYER"] = SIDE_PLAYER;
	lua["SIDE_NEUTRAL"] = SIDE_NEUTRAL;
	lua["SIDE_ENEMY"] = SIDE_ENEMY;

	lua["STATE_DEAD"] = STATE_DEAD;
	lua["STATE_IDLE"] = STATE_IDLE;

	lua["WorldToTile"] = WorldToTile;
	lua["TileToWorld"] = TileToWorld;
	lua["QueryEntities"] = QueryEntities;

	lua["EntityCreate"] = EntityCreate;
	lua["EntityDestroy"] = EntityDestroy;
	lua["EntityComponentEnable"] = EntityComponentEnable;
	lua["EntityComponentDisable"] = EntityComponentDisable;

	lua["TimeGetTicks"] = TimeGetTicks;
	lua["TimeGetSeconds"] = TimeGetSeconds;
	lua["TimeGetDeltatime"] = TimeGetDeltatime;

	lua["TransformSet"] = TransformSet;
	lua["TransformSetPosition"] = TransformSetPosition;
	lua["TransformAddPosition"] = TransformAddPosition;
	lua["TransformGetPosition"] = TransformGetPosition;
	lua["TransformSetRotation"] = TransformSetRotation;
	lua["TransformAddRotation"] = TransformAddRotation;
	lua["TransformGetRotation"] = TransformGetRotation;
	lua["TransformSetScale"] = TransformSetScale;
	lua["TransformAddScale"] = TransformAddScale;
	lua["TransformGetScale"] = TransformGetScale;
	lua["TransformGetTile"] = TransformGetTilepos;

	lua["MovableSet"] = MovableSet;
	lua["MovableSetSpeed"] = MovableSetSpeed;
	lua["MovableAddSpeed"] = MovableAddSpeed;
	lua["MovableGetSpeed"] = MovableGetSpeed;
	lua["MovableSetAcceleration"] = MovableSetAcceleration;
	lua["MovableAddAcceleration"] = MovableAddAcceleration;
	lua["MovableGetAcceleration"] = MovableGetAcceleration;

	lua["DrawableSet"] = DrawableSet;
	lua["DrawableSetSprite"] = DrawableSetSprite;
	lua["DrawableGetSprite"] = DrawableGetSprite;
	lua["DrawableSetColor"] = DrawableSetColor;
	lua["DrawableGetColor"] = DrawableGetColor;

	lua["AnimationSet"] = AnimationSet;
	lua["AnimationSetCurrentImage"] = AnimationSetCurrentImage;
	lua["AnimationGetCurrentImage"] = AnimationGetCurrentImage;
	lua["AnimationSetFrameTime"] = AnimationSetFrameTime;
	lua["AnimationGetFrameTime"] = AnimationGetFrameTime;
	lua["AnimationSetRepeat"] = AnimationSetRepeat;
	lua["AnimationGetRepeat"] = AnimationGetRepeat;
	lua["AnimationSetOnAnimationEndCallback"] = AnimationSetOnAnimationEndCallback;

	lua["CollidableSet"] = CollidableSet;
	lua["CollidableSetCollisionMask"] = CollidableSetCollisionMask;
	lua["CollidableGetCollisionMask"] = CollidableGetCollisionMask;
	lua["CollidableSetBoundingBox"] = CollidableSetBoundingBox;
	lua["CollidableGetBoundingBox"] = CollidableGetBoundingBox;

	lua["DamageableSet"] = DamageableSet;
	lua["DamageableSetMask"] = DamageableSetMask;
	lua["DamageableGetMask"] = DamageableGetMask;
	lua["DamageableSetHealth"] = DamageableSetHealth;
	lua["DamageableGetHealth"] = DamageableGetHealth;
	lua["DamageableSetMaxHealth"] = DamageableSetMaxHealth;
	lua["DamageableGetMaxHealth"] = DamageableGetMaxHealth;

	lua["DamagingSet"] = DamagingSet;
	lua["DamagingSetMask"] = DamagingSetMask;
	lua["DamagingGetMask"] = DamagingGetMask;
	lua["DamagingSetDamage"] = DamagingSetDamage;
	lua["DamagingGetDamage"] = DamagingGetDamage;

	lua["PhysicsSet"] = PhysicsSet;
	lua["PhysicsSetWorldState"] = PhysicsSetWorldState;
	lua["PhysicsGetWorldState"] = PhysicsGetWorldState;
	lua["PhysicsSetMass"] = PhysicsSetMass;
	lua["PhysicsGetMass"] = PhysicsGetMass;

	lua["InputSetEventCallback"] = InputSetEventCallback;
	lua["InputIsKeyPressed"] = InputIsKeyPressed;

	lua["ControlGetSide"] = ControlGetSide;
	lua["ControlSetSide"] = ControlSetSide;
	lua["ControlSetStepCallback"] = ControlSetStepCallback;
	lua["ControlInitializeTable"] = ControlInitializeTable;
	lua["ControlGetTable"] = ControlGetTable;

	lua["StateMachineAddState"] = StateMachineAddState;
	lua["StateMachineGetState"] = StateMachineGetState;
	lua["StateMachineSetState"] = StateMachineSetState;
	lua["StateMachineInitTransitions"] = StateMachineInitTransitions;
	lua["StateMachineAddTransition"] = StateMachineAddTransition;
	lua["StateMachineTransitionTo"] = StateMachineTransitionTo;

	lua["TimerAddTimer"] = TimerAddTimer;
	lua["TimerSetTime"] = TimerSetTime;

	lua["WorldSetGravity"] = WorldSetGravity;
	lua["WorldGetGravity"] = WorldGetGravity;
}