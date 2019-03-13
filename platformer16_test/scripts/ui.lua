TextureLoad(tex_manager, "textures\\ui_atlas.png", "ui")
SpriteCreate(spr_manager, tex_manager, "ui", Vector.new(0, 0), Vector.new(142, 56), 1, "ui_button1")

control_base = UICreateControl()
UIControlSetWorkingArea(control_base, Rect.new(10, 10, 142, 56))
UIControlSetSprite(control_base, spr_manager, "ui_button1")
UIControlSetMouseTrigger(control_base, true, false, MOUSE_LEFT)
UIControlOnTriggerCallback(control_base, function()
	print("BUTTON PRESSED")
end)

--UIAddBaseControl(ui_manager, control_base, "base1", 0)
--UIActivateBaseControl(ui_manager, "base1")