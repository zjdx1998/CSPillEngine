import PyCSPillEngine

engine = PyCSPillEngine.Engine("Super Mario", 1280, 720)


class CharacterControllerComponent(PyCSPillEngine.Component):
    def __init__(self, name):
        super(CharacterControllerComponent, self).__init__(name)

    def Update(self, object, dt):
        # PyCSPillEngine.Component.Update(self, object, dt)
        print("wtf")
        if engine.IsKeyPressed("Right"):
            print("Right Key Prssed")

    pass


resource_manager = PyCSPillEngine.ResourceManager.GetInstance()
resource_manager.LoadResources(".")
engine.SwitchScene(resource_manager.LoadScene("default.scene"))

character = PyCSPillEngine.GameObject()
# character.AddComponent(PyCSPillEngine.CameraComponent())
controller = CharacterControllerComponent("ControllerComponent")
controller.Update(character, 0.0)
character.AddComponent(controller)

engine.AddObject("Character", character)

engine.Run(60)
