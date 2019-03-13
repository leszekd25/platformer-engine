component_types = {C_TRANSFORM, C_MOVABLE, C_DRAWABLE, C_ANIMATION, C_COLLIDABLE, C_DAMAGEABLE, C_DAMAGING, C_PHYSICS, C_INPUT, C_CONTROL, C_STATEMACHINE, C_TIMER}

function transformcomponent_modify(gs, e, comp)
	if comp["position"] ~= nil then
		TransformSetPosition(gs, e, comp.position)
	end
	if comp["rotation"] ~= nil then
		TransformSetRotation(gs, e, comp.rotation)
	end
	if comp["scale"] ~= nil then
		TransformSetScale(gs, e, comp.scale)
	end
end

function movablecomponent_modify(gs, e, comp)
	if comp["speed"] ~= nil then
		MovableSetSpeed(gs, e, comp.speed)
	end
	if comp["acceleration"] ~= nil then
		MovableSetAcceleration(gs, e, comp.acceleration)
	end
end

function drawablecomponent_modify(gs, e, comp)
	if comp["sprite"] ~= nil then
		DrawableSetSprite(gs, spr_manager, e, comp.sprite)
	end
	if comp["color"] ~= nil then
		DrawableSetColor(gs, e, comp.color)
	end
end

function animationcomponent_modify(gs, e, comp)
	if comp["current_image"] ~= nil then
		AnimationSetCurrentImage(gs, e, comp.current_image)
	end
	if comp["frame_time"] ~= nil then
		AnimationSetFrameTime(gs, e, comp.frame_time)
	end
	if comp["repeating"] ~= nil then
		AnimationSetRepeat(gs, e, comp.repeating)
	end
	if comp["animation_end"] ~= nil then
		AnimationSetOnAnimationEndCallback(gs, e, comp.animation_end)
	end
end

function collidablecomponent_modify(gs, e, comp)
	if comp["collision_mask"] ~= nil then
		CollidableSetCollisionMask(gs, e, comp.collision_mask)
	end
	if comp["bounding_box"] ~= nil then
		CollidableSetBoundingBox(gs, e, comp.bounding_box)
	end
end

function damageablecomponent_modify(gs, e, comp)
	if comp["damagedby_mask"] ~= nil then
		DamageableSetMask(gs, e, comp.damagedby_mask)
	end
	if comp["health"] ~= nil then
		DamageableSetHealth(gs, e, comp.health)
	end
	if comp["max_health"] ~= nil then
		DamageableSetMaxHealth(gs, e, comp.max_health)
	end
end

function damagingcomponent_modify(gs, e, comp)
	if comp["damages_mask"] ~= nil then
		DamagingSetMask(gs, e, comp.damages_mask)
	end
	if comp["damage"] ~= nil then
		DamagingSetDamage(gs, e, comp.damage)
	end
end

function physicscomponent_modify(gs, e, comp)
	if comp["world_state"] ~= nil then
		PhysicsSetWorldState(gs, e, comp.world_state)
	end
	if comp["mass"] ~= nil then
		PhysicsSetMass(gs, e, comp.mass)
	end
end

function inputcomponent_modify(gs, e, comp)
	if comp["key_down"] ~= nil then
		InputSetEventCallback(gs, e, INPUT_KEY_DOWN, comp.key_down)
	end
	if comp["key_release"] ~= nil then
		InputSetEventCallback(gs, e, INPUT_KEY_RELEASE, comp.key_release)
	end
end

function controlcomponent_modify(gs, e, comp)
	if comp["side"] ~= nil then
		ControlSetSide(gs, e, comp.side)
	end
	if comp["step"] ~= nil then
		ControlSetStepCallback(gs, e, comp.step)
	end
	if comp["members"] ~= nil then
		ControlInitializeTable(gs, e)
		local tb = ControlGetTable(gs, e)
		for k, v in pairs(comp.members) do
			tb[k] = v
		end
	end
end

-- states = {
-- { STATE_IDLE = function(gs, e) {},
--   STATE_DEAD = nil,
--   STATE_RUSH = nil},
-- transitions = {
-- {STATE_IDLE, STATE_DEAD, function(gs, e) [body] end},
-- }
-- }
function statemachinecomponent_modify(gs, e, comp)
	if comp["states"] ~= nil then
		for k, v in pairs(comp.states) do
			print(k)
			StateMachineAddState(gs, e, k, v)
		end
	end
	StateMachineInitTransitions(gs, e)
	if comp["transitions"] ~= nil then
		for i, v in ipairs(comp.transitions) do
			StateMachineAddTransition(gs, e, v[1], v[2], v[3])
		end
	end
	if comp["init_state"] ~= nil then
		StateMachineSetState(gs, e, comp.init_state)
	end
end

function timercomponent_modify(gs, e, comp)
	for i, v in ipairs(comp) do
		TimerAddTimer(gs, e, v)
	end
end

component_modifiers = 
{
	transformcomponent_modify, 
	movablecomponent_modify, 
	drawablecomponent_modify, 
	animationcomponent_modify, 
	collidablecomponent_modify, 
	damageablecomponent_modify, 
	damagingcomponent_modify, 
	physicscomponent_modify, 
	inputcomponent_modify, 
	controlcomponent_modify, 
	statemachinecomponent_modify,
	timercomponent_modify
}