from PyCSPillEngine import Core, Utils

# engine through the game
engine = Core.Engine("Super Mario", 1280, 720)
# global area
score = 0
level = 1


class CharacterControllerComponent(Core.Component):

    def __init__(self, name):
        super().__init__(name)
        self.speed = 0.1

    def Update(self, object, dt):
        if engine.IsKeyPressed("Right"):
            object.GetComponent("TransformComponent").velocity().x += self.speed * dt
        elif engine.IsKeyPressed("Left"):
            object.GetComponent("TransformComponent").velocity().x -= self.speed * dt
        else:
            object.GetComponent("TransformComponent").velocity().x = 0

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

score_ui = Core.UIText.Create("comic.ttf", "Score: " + str(score), Utils.Vec2D(1150, 50), Utils.Vec2D(200, 100), 100)
level_ui = Core.UIText.Create("comic.ttf", "Level: 1-" + str(level), Utils.Vec2D(650, 50), Utils.Vec2D(200, 100), 100)

engine.AddObject("Character", character)
engine.AddObject("Camera", camera)
engine.AddObject("ScoreUI", score_ui)
engine.AddObject("LevelUI", level_ui)

engine.Run(60)

resource_manager.ReleaseAll()
