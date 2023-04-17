import PyCSPillEngine

engine = PyCSPillEngine.Engine("Super Mario", 1280, 720)


class CharacterControllerComponent(PyCSPillEngine.Component):
    def Update(self, object, dt):
        if engine.IsKeyPressed("Right"):
            print("Right Key Pressed")

    pass


resource_manager = PyCSPillEngine.ResourceManager.GetInstance()
resource_manager.LoadResources(".")
engine.SwitchScene(resource_manager.LoadScene("default.scene"))

character = PyCSPillEngine.GameObject()
# camera = PyCSPillEngine.GameObject()

character.AddComponent(CharacterControllerComponent("ControllerComponent"))
# camera.AddComponent(PyCSPillEngine.CameraComponent("CameraComponent"))
# camera.GetComponent("CameraComponent").bind(character)

engine.AddObject("Character", character)
# engine.AddObject("Camera", camera)

engine.Run(60)

resource_manager.ReleaseAll()
