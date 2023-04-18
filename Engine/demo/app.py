from PyCSPillEngine import Core, Utils, UI

# engine through the game
engine = Core.Engine("Super Mario", 1280, 720)
# global area
score = 0
level = 1


class CharacterControllerComponent(Core.Component):

    def __init__(self, name):
        super().__init__(name)
        self.speed = 2

    def Update(self, obj, dt):
        if engine.IsKeyPressed("Right"):
            print("Right")
            obj.GetComponent("TransformComponent").velocity().x = self.speed
        elif engine.IsKeyPressed("Left"):
            obj.GetComponent("TransformComponent").velocity().x = -self.speed
        else:
            obj.GetComponent("TransformComponent").velocity().x = 0

    pass


resource_manager = Core.ResourceManager.GetInstance()
resource_manager.LoadResources(".")
engine.SwitchScene(resource_manager.LoadScene("default.scene"))

Utils.PlayMusic("background_winter.wav")

character = Core.GameObject()
character.GetComponent("TransformComponent").position().x = 100
character.GetComponent("TransformComponent").position().y = 510
character.GetComponent("TransformComponent").scale = Utils.Vec2D(5, 5)
camera = Core.GameObject()

character.AddComponent(CharacterControllerComponent("ControllerComponent"))
character_animation_component = Core.AnimationComponent()
character_animation_component.AddAnimations("default",
                                            ["mario-edited.png-cropped-1000", "mario-edited.png-cropped-1001",
                                             "mario-edited.png-cropped-1002", "mario-edited.png-cropped-1003"])
character_animation_component.SetCurrentAnimation("default")
character.AddComponent(character_animation_component)
camera_component = Core.CameraComponent()
camera_component.SetViewport(1280, 720)
camera_component.Bind(character)
camera.AddComponent(camera_component)

score_ui = UI.UIText.Create("comic.ttf", "Score: " + str(score), Utils.Vec2D(1150, 50), Utils.Vec2D(200, 100), 100)
level_ui = UI.UIText.Create("comic.ttf", "Level: 1-" + str(level), Utils.Vec2D(650, 50), Utils.Vec2D(200, 100), 100)

engine.AddObject("Character", character)
engine.AddObject("Camera", camera)
engine.AddObject("ScoreUI", score_ui)
engine.AddObject("LevelUI", level_ui)

engine.Run(60)

resource_manager.ReleaseAll()
