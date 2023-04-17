from PyCSPillEngine import Core, Utils

engine = Core.Engine("Super Mario", 1280, 720)


class CharacterControllerComponent(Core.Component):
    def Update(self, object, dt):
        if engine.IsKeyPressed("Right"):
            object.GetComponent("TransformComponent").velocity().x += 1
        if engine.IsKeyPressed("Left"):
            object.GetComponent("TransformComponent").velocity().x -= 1

    pass


resource_manager = Core.ResourceManager.GetInstance()
resource_manager.LoadResources(".")
engine.SwitchScene(resource_manager.LoadScene("default.scene"))

Utils.PlayMusic("background_winter.wav")

character = Core.GameObject()
camera = Core.GameObject()

character.AddComponent(CharacterControllerComponent("ControllerComponent"))
camera_component = Core.CameraComponent("CameraComponent")
camera_component.SetViewport(1280, 720)
camera_component.Bind(character)
camera.AddComponent(camera_component)

engine.AddObject("Character", character)
engine.AddObject("Camera", camera)
engine.Run(60)

resource_manager.ReleaseAll()
