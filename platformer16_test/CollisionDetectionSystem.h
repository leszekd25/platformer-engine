/*

Collision detection system consists of two parts
Part 1: collisions of objects with walls
System will detect if any MOVING object has collided with any wall in this frame - if so, relevant data
is forwarded to CollisionResolveQuasiSystem
Part 2: collisions of objects with other objects
System will detect if any object has collided with any other object, if so, see above

NOTE: System accuracy fails with speeds exceeding CELL_SIZE per frame (for example, speed >= 1864)
NOTE: Objects and walls are treated as axis-aligned rectangles
*/

#pragma once

#include <vector>
#include "GameState.h"

class CollisionDetectionSystem
{
public:
	CollisionDetectionSystem() {};
	~CollisionDetectionSystem() {};
	void Step(GameState& gs)
	{
		std::vector<GameObject>& entities = gs.entities;
		std::vector<TransformComponent>& transforms = gs.transforms;
		std::vector<MovableComponent>& movables = gs.movables;
		std::vector<CollidableComponent>& collidables = gs.collidables;
		std::vector<PhysicsComponent>& physicss = gs.physicss;
		Time& time = gs.timer;
		float delta_time = time.GetDeltaTime();
		Tilemap* map = gs.map;

		int last_active_entity = gs.last_active_entity;

		for (int i = 0; i <= last_active_entity; i++)
		{
			if (!entities[i].active)
				continue;
			if ((entities[i].components[C_TRANSFORM]) && (entities[i].components[C_COLLIDABLE]))
			{
				// collisions with walls -------------------------------------------------------------------------------------
				// can only collide with walls if entity can move (reasonable)
				if (entities[i].components[C_MOVABLE])
				{
					int check_mask = 0;   // 1 - right, 2 - up, 4 - left, 8 - down
					// entity moved this much so far
					// need to check this, so no unnecessary checks are made when entity cant move
					Vector movement_vector = transforms[i].position - transforms[i].previous_position;
					std::vector<Tilepos>* walls_collided = new std::vector<Tilepos>();

					// walls right to the entity
					if (movement_vector.x > 0)
					{
						// get first and last tiles to check in a given direction
						// the tiles checked will always form a row or a column
						float x_limit = transforms[i].position.x + collidables[i].bbox.x + collidables[i].bbox.w2;
						float y_start = transforms[i].position.y + collidables[i].bbox.y - collidables[i].bbox.h2 + 1;
						float y_end = y_start + 2 * collidables[i].bbox.h2 - 2;
						Tilepos tpos1, tpos2;
						map->WorldToTile(Vector(x_limit, y_start), tpos1);
						map->WorldToTile(Vector(x_limit, y_end), tpos2);
						// add collided tiles to the list of tiles to check
						for (int m = tpos1.y; m <= tpos2.y; m++)
						{
							if (map->collision_masks[tpos1.x + m * map->width] & collidables[i].collision_mask)
							{
								check_mask |= COL_RIGHT;
								walls_collided->push_back(Tilepos(tpos1.x, m));
							}
						}
					}
					// walls up to the entity
					if (movement_vector.y < 0)
					{
						float y_limit = transforms[i].position.y + collidables[i].bbox.y - collidables[i].bbox.h2;
						float x_start = transforms[i].position.x + collidables[i].bbox.x - collidables[i].bbox.w2 + 1;
						float x_end = x_start + 2 * collidables[i].bbox.w2 - 2;
						Tilepos tpos1, tpos2;
						map->WorldToTile(Vector(x_start, y_limit), tpos1);
						map->WorldToTile(Vector(x_end, y_limit), tpos2);
						for (int m = tpos1.x; m <= tpos2.x; m++)
						{
							if (map->collision_masks[m + tpos1.y * map->width] & collidables[i].collision_mask)
							{
								check_mask |= COL_UP;
								walls_collided->push_back(Tilepos(m, tpos1.y));
							}
						}
					}
					// walls left to the entity
					if (movement_vector.x < 0)
					{
						float x_limit = transforms[i].position.x + collidables[i].bbox.x - collidables[i].bbox.w2;
						float y_start = transforms[i].position.y + collidables[i].bbox.y - collidables[i].bbox.h2 + 1;
						float y_end = y_start + 2 * collidables[i].bbox.h2 - 2;
						Tilepos tpos1, tpos2;
						map->WorldToTile(Vector(x_limit, y_start), tpos1);
						map->WorldToTile(Vector(x_limit, y_end), tpos2);
						for (int m = tpos1.y; m <= tpos2.y; m++)
						{
							if (map->collision_masks[tpos1.x + m * map->width] & collidables[i].collision_mask)
							{
								check_mask |= COL_LEFT;
								walls_collided->push_back(Tilepos(tpos1.x, m));
							}
						}
					}
					// walls down to the entity
					if (movement_vector.y > 0)
					{
						float y_limit = transforms[i].position.y + collidables[i].bbox.y + collidables[i].bbox.h2;
						float x_start = transforms[i].position.x + collidables[i].bbox.x - collidables[i].bbox.w2 + 1;
						float x_end = x_start + 2 * collidables[i].bbox.w2 - 2;
						Tilepos tpos1, tpos2;
						map->WorldToTile(Vector(x_start, y_limit), tpos1);
						map->WorldToTile(Vector(x_end, y_limit), tpos2);
						for (int m = tpos1.x; m <= tpos2.x; m++)
						{
							if (map->collision_masks[m + tpos1.y * map->width] & collidables[i].collision_mask)
							{
								check_mask |= COL_DOWN;
								walls_collided->push_back(Tilepos(m, tpos1.y));
							}
						}
					}

					// if any walls collided, construct and send message that collision occured
					if (check_mask)
					{
						CollisionMessage* msg = new CollisionMessage();
						msg->collision_type = COLLISION_WALL;
						msg->source = UniqueEntityIdentifier(i, entities[i].id);
						msg->collision_mask = check_mask;
						msg->collided_walls = walls_collided;
						gs.messages.push(msg);
					}
					else
					{
						delete walls_collided;
						walls_collided = 0;
					}

					// separate check if entity is still in the ground
					// only applicable if physics enabled
					if (entities[i].components[C_PHYSICS])
					{
						if (physicss[i].world_state == ON_GROUND)
						{
							bool still_on_ground = false;
							float y_limit = transforms[i].position.y + collidables[i].bbox.y + collidables[i].bbox.h2 + 1;
							float x_start = transforms[i].position.x + collidables[i].bbox.x - collidables[i].bbox.w2 + 1;
							float x_end = x_start + 2 * collidables[i].bbox.w2 - 2;
							Tilepos tpos1, tpos2;
							map->WorldToTile(Vector(x_start, y_limit), tpos1);
							map->WorldToTile(Vector(x_end, y_limit), tpos2);
							for (int m = tpos1.x; m <= tpos2.x; m++)
							{
								if (map->collision_masks[m + tpos1.y * map->width] & collidables[i].collision_mask)
								{
									still_on_ground = true;
									break;
								}
							}
							if (!still_on_ground)
							{
								physicss[i].world_state = IN_AIR;
							}
						}
					}
				}

				// collisions with objects -------------------------------------------------------------------------
				// slow way, nested loop for checking collisions with other objects
				// todo: make a lookup structure for this...
				for (int j = i+1; j <= last_active_entity; j++)
				{
					if (!entities[j].active)
						continue;
					if ((entities[j].components[C_TRANSFORM]) && (entities[j].components[C_COLLIDABLE]))
					{
						if (collidables[i].collision_mask & collidables[j].collision_mask)
						{
							Rect r1 = collidables[i].bbox + transforms[i].position;
							Rect r2 = collidables[j].bbox + transforms[j].position;
							if (r1.Intersects(r2))
							{
								// need to send message for both objects
								CollisionMessage* msg = new CollisionMessage();
								msg->collision_type = COLLISION_OBJECT;
								msg->source = UniqueEntityIdentifier(i, entities[i].id);
								msg->target = UniqueEntityIdentifier(j, entities[j].id);
								gs.messages.push(msg);
								msg = new CollisionMessage();
								msg->collision_type = COLLISION_OBJECT;
								msg->source = UniqueEntityIdentifier(j, entities[j].id);
								msg->target = UniqueEntityIdentifier(i, entities[i].id);
								gs.messages.push(msg);
							}
						}
					}
				}
			}
		}
	}
};