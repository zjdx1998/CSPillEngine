import PyCSPillEngine
import gc

gc.freeze()

engine = PyCSPillEngine.Engine("Super Mario", 1280, 720)


# gc.set_debug(gc.DEBUG_LEAK)


class CharacterControllerComponent(PyCSPillEngine.Component):
    def Update(self, object, dt):
        print("wtf")
        if engine.IsKeyPressed("Right"):
            print("Right Key Prssed")

    pass


resource_manager = PyCSPillEngine.ResourceManager.GetInstance()
resource_manager.LoadResources(".")
engine.SwitchScene(resource_manager.LoadScene("default.scene"))

character = PyCSPillEngine.GameObject()
# character.AddComponent(PyCSPillEngine.CameraComponent())

character.AddComponent(CharacterControllerComponent("Controller"))

engine.AddObject("Character", character)

engine.Run(60)

resource_manager.ReleaseAll()
