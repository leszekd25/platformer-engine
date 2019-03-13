/*

Collision resolve quasi-system will fix object positions so that they're no longer intersecting walls
It also forwards damage data to DamageResolveQuasiSystem in case colliding objects can damage each other

Collisions are taken from a queue previously filled with collision events
*/


#pragma once

#include <vector>
#include <queue>
#include "GameState.h"
#include <iostream>

class CollisionResolveQuasiSystem
{
public:
	std::queue<CollisionMessage*> messages;
	CollisionResolveQuasiSystem() 
	{ 
		messages = std::queue<CollisionMessage*>(); 
	};
	~CollisionResolveQuasiSystem() 
	{  
		while (!messages.empty())
		{
			delete messages.front();
			messages.pop();
		}
	};
	void Resolve(GameState& gs)
	{
		std::vector<GameObject>& entities = gs.entities;
		std::vector<TransformComponent>& transforms = gs.transforms;
		std::vector<MovableComponent>& movables = gs.movables;
		std::vector<DamageableComponent>& damageables = gs.damageables;
		std::vector<DamagingComponent>& damagings = gs.damagings;
		std::vector<CollidableComponent>& collidables = gs.collidables;
		std::vector<PhysicsComponent>& physicss = gs.physicss;
		Tilemap* map = gs.map;

		CollisionMessage* message;
		UniqueEntityIdentifier source;
		while (!messages.empty())
		{
			message = messages.front();
			messages.pop();
			// extract info and delete the message
			source = message->source;
			// must check if the invoking entity from the event still exists in case it was removed before
			// this system could resolve the collision
			if (!gs.EntityIsValid(source))
			{
				delete message;
				continue;
			}

			switch (message->collision_type)
			{
			case COLLISION_OBJECT:
			{
				UniqueEntityIdentifier target = message->target;
				// similar check to target entity
				if (!gs.EntityIsValid(target))
					break;
				// check for damage
				// if source is damaging and target is damageable, and the masks are valid, send message to damage the target
				if ((entities[source.entity_id].components[C_DAMAGING]) && (entities[target.entity_id].components[C_DAMAGEABLE]))
				{
					if (damagings[source.entity_id].damages_mask & damageables[target.entity_id].damagedby_mask)
					{
						DamageMessage* dmg_msg = new DamageMessage();
						dmg_msg->source = source;
						dmg_msg->target = target;
						dmg_msg->damage = damagings[source.entity_id].damage;
						gs.messages.push(dmg_msg);
					}
				}
				break;
			}
			case COLLISION_WALL:         // heavy optimization and refactoring required!
			{
				int collision_mask = message->collision_mask;
				std::vector<Tilepos>* walls = message->collided_walls;
				//std::cout << gs.timer.GetTime() << " COLLISION MASK " << collision_mask << " WALLS COLLIDED: ";
				//for (unsigned int wi2 = 0; wi2 < walls->size(); wi2++)
				//	std::cout << (*walls)[wi2] << " | ";
				//std::cout << std::endl;
				if (collision_mask == 0)
					break;

				// hold tightly, magic below
				// positive_dx/dy is the MINIMUM difference between invoking entity's position and top-left corner of 
				// ANY wall rectangle that is determined to have collided with the entity on this frame
				// negative_dx/dy denotes whether there are any other walls in the direction inverted relative
				// to entity's movement
				// pot_x/y is the position determined to be the entity's next coordinates after collision is resolved
				// diff is the difference between invoking entity's position and top-left corner of
				// currently reviewed wall rectangle
				// top_left/top_right/bottom_left/bottom_right are the coordinates of entity's bounding box vertices
				// wall_pos_world is world top-left position of wall, determined from wall cell coordinates
				// wall_offset depends on collision mask
				// movement_vector is the amount the entity has moved this frame so far (different from speed!)
				// wall_pos_map is tile position of currently reviewed wall
				// first_frame_intersection_x/y denote whether the collision in a given axis could only gave happened in 
				// the current frame

				float positive_dx = 100000, positive_dy = 100000, negative_dx = -100000, negative_dy = -100000, pot_x = 0, pot_y = 0;
				Vector diff = Vector(0, 0), top_left = Vector(0, 0), top_right = Vector(0, 0), bottom_left = Vector(0, 0),
					bottom_right = Vector(0, 0), wall_pos_world = Vector(0, 0), wall_offset = Vector(0, 0);
				Vector movement_vector = transforms[source.entity_id].position - transforms[source.entity_id].previous_position;
				Tilepos wall_pos_map = Tilepos(0, 0);
				bool first_frame_intersection_x, first_frame_intersection_y;

				top_left = transforms[source.entity_id].position;
				top_right = top_left; top_right.x += collidables[source.entity_id].bbox.w2 * 2;
				bottom_left = top_left; bottom_left.y += collidables[source.entity_id].bbox.h2 * 2;
				bottom_right = top_left;
				bottom_right.x += collidables[source.entity_id].bbox.w2 * 2;
				bottom_right.y += collidables[source.entity_id].bbox.h2 * 2;

				if (collision_mask & COL_LEFT)
				{
					if (collision_mask & COL_UP)
					{
						for (unsigned int wi = 0; wi < walls->size(); wi++)
						{
							wall_pos_map = (*walls)[wi];
							map->TileToWorld(wall_pos_map, wall_pos_world);
							diff = wall_pos_world - top_left;
							if ((-diff.x < positive_dx) && (-diff.x > 0))
								positive_dx = -diff.x;
							if ((-diff.y < positive_dy) && (-diff.y > 0))
								positive_dy = -diff.y;
							if ((-diff.x > negative_dx) && (-diff.x < 0))
								negative_dx = -diff.x;
							if ((-diff.y > negative_dy) && (-diff.y < 0))
								negative_dy = -diff.y;
						}

						if (negative_dx == -100000)
							negative_dx = 0;
						if (negative_dy == -100000)
							negative_dy = 0;

						pot_x = top_left.x + CELL_SIZE - positive_dx;
						pot_y = top_left.y + CELL_SIZE - positive_dy;

						if ((negative_dx < 0) && (negative_dy < 0))
						{
							transforms[source.entity_id].position.x = pot_x;
							transforms[source.entity_id].position.y = pot_y;

							movables[source.entity_id].speed.x = 0;
							movables[source.entity_id].speed.y *= -1;
						}
						else if (negative_dx == 0)
						{
							if (negative_dy == 0)
							{
								first_frame_intersection_x = (top_left.x - movement_vector.x > wall_pos_world.x + 31);
								first_frame_intersection_y = (top_left.y - movement_vector.y > wall_pos_world.y + 31);
								if (!(first_frame_intersection_x ^ first_frame_intersection_y))
								{
									if (positive_dx*movement_vector.y < positive_dy*movement_vector.x)       // to avoid division by 0
									{
										transforms[source.entity_id].position.x = pot_x;
										movables[source.entity_id].speed.x = 0;
									}
									else
									{
										transforms[source.entity_id].position.y = pot_y;
										movables[source.entity_id].speed.y *= -1;
									}
								}
								else if (first_frame_intersection_x)
								{
									transforms[source.entity_id].position.x = pot_x;
									movables[source.entity_id].speed.x = 0;
								}
								else
								{
									transforms[source.entity_id].position.y = pot_y;
									movables[source.entity_id].speed.y *= -1;
								}
							}
							else
							{
								transforms[source.entity_id].position.x = pot_x;
								movables[source.entity_id].speed.x = 0;
							}
						}
						else
						{
							transforms[source.entity_id].position.y = pot_y;
							movables[source.entity_id].speed.y *= -1;
						}
					}
					else if (collision_mask & COL_DOWN)
					{
						wall_offset = Vector(0, CELL_SIZE);
						for (unsigned int wi = 0; wi < walls->size(); wi++)
						{
							wall_pos_map = (*walls)[wi];
							map->TileToWorld(wall_pos_map, wall_pos_world);
							diff = wall_pos_world - bottom_left + wall_offset;
							if ((-diff.x < positive_dx) && (-diff.x > 0))
								positive_dx = -diff.x;
							if ((diff.y < positive_dy) && (diff.y > 0))
								positive_dy = diff.y;
							if ((-diff.x > negative_dx) && (-diff.x < 0))
								negative_dx = -diff.x;
							if ((diff.y > negative_dy) && (diff.y < 0))
								negative_dy = diff.y;
						}

						if (negative_dx == -100000)
							negative_dx = 0;
						if (negative_dy == -100000)
							negative_dy = 0;

						pot_x = top_left.x + CELL_SIZE - positive_dx;
						pot_y = top_left.y - CELL_SIZE + positive_dy;

						if ((negative_dx < 0) && (negative_dy < 0))
						{
							transforms[source.entity_id].position.x = pot_x;
							transforms[source.entity_id].position.y = pot_y;

							movables[source.entity_id].speed.x = 0;
							movables[source.entity_id].speed.y *= -1;
						}
						else if (negative_dx == 0)
						{
							if (negative_dy == 0)
							{     // only one wall collides
								first_frame_intersection_x = (top_left.x - movement_vector.x > wall_pos_world.x + 31);
								first_frame_intersection_y = (bottom_left.y - movement_vector.y < wall_pos_world.y + 1);
								if (!(first_frame_intersection_x ^ first_frame_intersection_y))
								{
									if (positive_dx*movement_vector.y < positive_dy*movement_vector.x)       // to avoid division by 0
									{
										transforms[source.entity_id].position.x = pot_x;
										movables[source.entity_id].speed.x = 0;
									}
									else
									{
										transforms[source.entity_id].position.y = pot_y;
										movables[source.entity_id].speed.y = 0;
										if (physicss[source.entity_id].world_state == IN_AIR)
											physicss[source.entity_id].world_state = ON_GROUND;
									}
								}
								else if (first_frame_intersection_x)
								{
									transforms[source.entity_id].position.x = pot_x;
									movables[source.entity_id].speed.x = 0;
								}
								else
								{
									transforms[source.entity_id].position.y = pot_y;
									movables[source.entity_id].speed.y = 0;
									if (physicss[source.entity_id].world_state == IN_AIR)
										physicss[source.entity_id].world_state = ON_GROUND;
								}
							}
							else
							{
								transforms[source.entity_id].position.x = pot_x;
								movables[source.entity_id].speed.x = 0;
							}
						}
						else
						{
							transforms[source.entity_id].position.y = pot_y;
							movables[source.entity_id].speed.y = 0;
							if (physicss[source.entity_id].world_state == IN_AIR)
								physicss[source.entity_id].world_state = ON_GROUND;
						}
					}
					else
					{
						wall_pos_map = (*walls)[0];
						map->TileToWorld(wall_pos_map, wall_pos_world);
						diff = wall_pos_world - top_left;

						transforms[source.entity_id].position.x += CELL_SIZE + diff.x;
					}
				}
				else if (collision_mask & COL_RIGHT)
				{
					if (collision_mask & COL_UP)
					{
						wall_offset = Vector(CELL_SIZE, 0);
						for (unsigned int wi = 0; wi < walls->size(); wi++)
						{
							wall_pos_map = (*walls)[wi];
							map->TileToWorld(wall_pos_map, wall_pos_world);
							diff = wall_pos_world - top_right + wall_offset;
							if ((diff.x < positive_dx) && (diff.x > 0))
								positive_dx = diff.x;
							if ((-diff.y < positive_dy) && (-diff.y > 0))
								positive_dy = -diff.y;
							if ((diff.x > negative_dx) && (diff.x < 0))
								negative_dx = diff.x;
							if ((-diff.y > negative_dy) && (-diff.y < 0))
								negative_dy = -diff.y;
						}

						if (negative_dx == -100000)
							negative_dx = 0;
						if (negative_dy == -100000)
							negative_dy = 0;

						pot_x = top_left.x - CELL_SIZE + positive_dx;
						pot_y = top_left.y + CELL_SIZE - positive_dy;

						if ((negative_dx < 0) && (negative_dy < 0))
						{
							transforms[source.entity_id].position.x = pot_x;
							transforms[source.entity_id].position.y = pot_y;

							movables[source.entity_id].speed.x = 0;
							movables[source.entity_id].speed.y *= -1;
						}
						else if (negative_dx == 0)
						{
							if (negative_dy == 0)
							{
								first_frame_intersection_x = (top_right.x - movement_vector.x < wall_pos_world.x + 1);
								first_frame_intersection_y = (top_left.y - movement_vector.y > wall_pos_world.y +31);
								if (!(first_frame_intersection_x ^ first_frame_intersection_y))
								{
									if (positive_dx*movement_vector.y < positive_dy*movement_vector.x)       // to avoid division by 0
									{
										transforms[source.entity_id].position.x = pot_x;
										movables[source.entity_id].speed.x = 0;
									}
									else
									{
										transforms[source.entity_id].position.y = pot_y;
										movables[source.entity_id].speed.y *= -1;
									}
								}
								else if (first_frame_intersection_x)
								{
									transforms[source.entity_id].position.x = pot_x;
									movables[source.entity_id].speed.x = 0;
								}
								else
								{
									transforms[source.entity_id].position.y = pot_y;
									movables[source.entity_id].speed.y *= -1;
								}
							}
							else
							{
								transforms[source.entity_id].position.x = pot_x;
								movables[source.entity_id].speed.x = 0;
							}
						}
						else
						{
							transforms[source.entity_id].position.y = pot_y;
							movables[source.entity_id].speed.y *= -1;
						}
					}
					else if (collision_mask & COL_DOWN)
					{
						wall_offset = Vector(CELL_SIZE, CELL_SIZE);
						for (unsigned int wi = 0; wi < walls->size(); wi++)
						{
							wall_pos_map = (*walls)[wi];
							map->TileToWorld(wall_pos_map, wall_pos_world);
							diff = wall_pos_world - bottom_right + wall_offset;
							if ((diff.x < positive_dx) && (diff.x > 0))
								positive_dx = diff.x;
							if ((diff.y < positive_dy) && (diff.y > 0))
								positive_dy = diff.y;
							if ((diff.x > negative_dx) && (diff.x < 0))
								negative_dx = diff.x;
							if ((diff.y > negative_dy) && (diff.y < 0))
								negative_dy = diff.y;
						}

						if (negative_dx == -100000)
							negative_dx = 0;
						if (negative_dy == -100000)
							negative_dy = 0;

						pot_x = top_left.x - CELL_SIZE + positive_dx;
						pot_y = top_left.y - CELL_SIZE + positive_dy;

						if ((negative_dx < 0) && (negative_dy < 0))
						{
							transforms[source.entity_id].position.x = pot_x;
							transforms[source.entity_id].position.y = pot_y;

							movables[source.entity_id].speed.x = 0;
							movables[source.entity_id].speed.y *= -1;
						}
						else if (negative_dx == 0)
						{
							if (negative_dy == 0)
							{
								first_frame_intersection_x = (top_right.x - movement_vector.x < wall_pos_world.x + 1);
								first_frame_intersection_y = (bottom_left.y - movement_vector.y < wall_pos_world.y + 1);
								if (!(first_frame_intersection_x ^ first_frame_intersection_y))
								{
									if (positive_dx*movement_vector.y < positive_dy*movement_vector.x)       // to avoid division by 0
									{
										transforms[source.entity_id].position.x = pot_x;
										movables[source.entity_id].speed.x = 0;
									}
									else
									{
										transforms[source.entity_id].position.y = pot_y;
										movables[source.entity_id].speed.y = 0;
										if (physicss[source.entity_id].world_state == IN_AIR)
											physicss[source.entity_id].world_state = ON_GROUND;
									}
								}
								else if (first_frame_intersection_x)
								{
									transforms[source.entity_id].position.x = pot_x;
									movables[source.entity_id].speed.x = 0;
								}
								else
								{
									transforms[source.entity_id].position.y = pot_y;
									movables[source.entity_id].speed.y = 0;
									if (physicss[source.entity_id].world_state == IN_AIR)
										physicss[source.entity_id].world_state = ON_GROUND;
								}
							}
							else
							{
								transforms[source.entity_id].position.x = pot_x;
								movables[source.entity_id].speed.x = 0;
							}
						}
						else
						{
							transforms[source.entity_id].position.y = pot_y;
							movables[source.entity_id].speed.y = 0;
							if (physicss[source.entity_id].world_state == IN_AIR)
								physicss[source.entity_id].world_state = ON_GROUND;
						}
					}
					else
					{
						wall_pos_map = (*walls)[0];
						map->TileToWorld(wall_pos_map, wall_pos_world);
						diff = wall_pos_world - top_right;

						transforms[source.entity_id].position.x += diff.x;
					}
				}
				else if (collision_mask & COL_UP)
				{
					wall_pos_map = (*walls)[0];
					map->TileToWorld(wall_pos_map, wall_pos_world);
					diff = wall_pos_world - top_left;

					transforms[source.entity_id].position.y += CELL_SIZE + diff.y;
					movables[source.entity_id].speed.y *= -1;
				}
				else
				{
					wall_pos_map = (*walls)[0];
					map->TileToWorld(wall_pos_map, wall_pos_world);
					diff = wall_pos_world - bottom_left;

					transforms[source.entity_id].position.y += diff.y;
					movables[source.entity_id].speed.y = 0;
					if (physicss[source.entity_id].world_state == IN_AIR)
						physicss[source.entity_id].world_state = ON_GROUND;
				}

				//std::cout << "ENTITY POSITION: " << transforms[source.entity_id].position << std::endl;
				break;
			}
			default:
				break;
			}

			delete message;
		}
	}
};