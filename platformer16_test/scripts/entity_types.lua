STATE_SHOOT = 2

entity_types = {}

function CreateTypeFromTemplate(t)
	__ret = {}  
	tmpl = entity_templates[t]
	for i, ct in ipairs(tmpl) do
		__ret[ct] = {}
	end
	return __ret
end

function CreateEntityFromType(t, pos)
	e_type = entity_types[t]
	__e = EntityCreate(game_state)
	
	for i, ct in ipairs(component_types) do
		if e_type[ct] ~= nil then
			EntityComponentEnable(game_state, __e, ct)
			component_modifiers[i](game_state, __e, e_type[ct])
		end
	end
	
	TransformSetPosition(game_state, __e, pos)
	
	if e_type[C_CONTROL] ~= nil then
		if e_type[C_CONTROL]["create"] ~= nil then
			e_type[C_CONTROL]["create"](game_state, __e)
		end
	end
	
	return __e
end

entity_types["ACTOR_1"] = CreateTypeFromTemplate("ACTOR")

entity_types["ACTOR_1"][C_DRAWABLE] = 
{
sprite = "test1"
}

entity_types["ACTOR_1"][C_COLLIDABLE] = 
{
collision_mask = 1,
bounding_box = Rect.new(0, 0, 24, 24)
}

entity_types["ACTOR_1"][C_DAMAGEABLE] = 
{
damagedby_mask = 1,
health = 40,
max_health = 40
}

entity_types["ACTOR_1"][C_PHYSICS] = 
{
world_state = IN_AIR,
mass = 1
}

entity_types["ACTOR_1"][C_CONTROL] = 
{
side = SIDE_PLAYER,
step = function(gs, e)
	local cur_speed = MovableGetSpeed(gs, e)
	if InputIsKeyPressed(gs, KEY_A) then
		cur_speed.x = -200
	else
		if cur_speed.x < 0 then cur_speed.x = 0 end
	end
	if InputIsKeyPressed(gs, KEY_D) then
		cur_speed.x = 200
	else
		if cur_speed.x > 0 then cur_speed.x = 0 end
	end
	if InputIsKeyPressed(gs, KEY_W) and (PhysicsGetWorldState(gs, e) == ON_GROUND) then
		cur_speed.y = -600
	end
	MovableSetSpeed(gs, e, cur_speed)
end
}




entity_types["SLIME_1"] = CreateTypeFromTemplate("SLIME")

entity_types["SLIME_1"][C_DRAWABLE] = 
{
sprite = "slime1_idle"
}

entity_types["SLIME_1"][C_COLLIDABLE] = 
{
collision_mask = 1,
bounding_box = Rect.new(0, 0, 24, 14)
}

entity_types["SLIME_1"][C_DAMAGEABLE] = 
{
damagedby_mask = 2,
health = 10,
max_health = 10
}

entity_types["SLIME_1"][C_DAMAGING] = 
{
damages_mask = 1,
damage = 1
}

entity_types["SLIME_1"][C_CONTROL] = 
{
side = SIDE_ENEMY,
members = 
{
	x_min = -1,
	x_max = -1,
	speed = 40,
	direction = 0
},
create = function(gs, e)
	local tb = ControlGetTable(gs, e)
	local pos = TransformGetPosition(gs, e)
	local tpos1 = WorldToTile(gs, pos)
	local tpos2 = WorldToTile(gs, pos)
	local emask = CollidableGetCollisionMask(gs, e)
	local has_ceiling = false
	local mask1
	local mask2
	-- find wall directly above slime
	while true do
		tpos1.y = tpos1.y - 1
		mask1 = MapGetCollisionMask(gs, tpos1.x, tpos1.y)
		mask2 = MapGetCollisionMask(gs, tpos1.x+1, tpos1.y)
		if(((mask1 | mask2) & emask) ~= 0) then
			has_ceiling = true
			break
		end
		if(tpos1.y == 0) then
			break
		end
	end
	if not has_ceiling then return end
	-- find all walls horizontally that are connected to it and don't have any wall directly below it
	local map_bounds = MapGetSize(gs)
	while true do
		if((MapGetCollisionMask(gs, tpos1.x, tpos1.y) & emask) ~= 0) then
			tpos1.x = tpos1.x-1
			if(tpos1.x < 0) then break end
		else break end
	end
	while true do
		if((MapGetCollisionMask(gs, tpos2.x+1, tpos1.y) & emask) ~= 0) then
			tpos2.x = tpos2.x+1
			if(tpos2.x >= map_bounds.x) then break end
		else break end
	end
	-- get min_x and max_x of the walls
	tpos1.x = tpos1.x+1
	local wbounds1 = TileToWorld(gs, tpos1)
	local wbounds2 = TileToWorld(gs, tpos2)
	tb.x_min = wbounds1.x
	tb.x_max = wbounds2.x
	-- set starting speed randomly
	local rand = math.random(0, 1)
	if(rand == 0) then
		tb.direction = -1
	else
		tb.direction = 1
	end
	MovableSetSpeed(gs, e, Vector.new(tb.speed*tb.direction, 0))
end
}

entity_types["SLIME_1"][C_STATEMACHINE] =
{
states =
{
	[STATE_IDLE] = function(gs, e)
		local ticks = TimeGetTicks(gs)
		local pos = TransformGetPosition(gs, e)
		local speed = MovableGetSpeed(gs, e)
		local tb = ControlGetTable(gs, e)
		if(pos.x < tb.x_min) then
			tb.direction = 1
			MovableSetSpeed(gs, e, Vector.new(-speed.x, 0))
		end
		if(pos.x > tb.x_max) then
			tb.direction = -1
			MovableSetSpeed(gs, e, Vector.new(-speed.x, 0))
		end
		
		if (ticks % 10) == 0 then
			entities = QueryEntities(gs, pos, 60, SIDE_PLAYER)
			if(entities[1] ~= nil) then
				StateMachineTransitionTo(gs, e, STATE_SHOOT)
			end
		end
	end,
	
	[STATE_SHOOT] = function(gs, e)
	end
},
transitions = 
{
	{STATE_IDLE, STATE_SHOOT, function(gs, e)
		TimerSetTime(gs, e, 0, 1)
		MovableSetSpeed(gs, e, Vector.new(0, 0))
	end},
	{STATE_SHOOT, STATE_IDLE, function(gs, e)
		local tb = ControlGetTable(gs, e)
		MovableSetSpeed(gs, e, Vector.new(tb.speed*tb.direction, 0))
	end}
},
init_state = STATE_IDLE
}

entity_types["SLIME_1"][C_TIMER] = 
{
	function(gs, e)
		StateMachineTransitionTo(gs, e, STATE_IDLE)
	end
}