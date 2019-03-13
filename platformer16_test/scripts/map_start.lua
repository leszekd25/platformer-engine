WorldSetGravity(game_state, 2600)

MapLoad(game_state, tex_manager, "map_test")

CreateEntityFromType("ACTOR_1", Vector.new(48, 72))
CreateEntityFromType("SLIME_1", Vector.new(264, 768))