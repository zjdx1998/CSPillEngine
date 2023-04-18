import random
import sys
sys.path.append('../build')
from PyCSPillEngine import Core, Utils, UI, Physics

# engine through the game
engine = Core.Engine("Super Mario", 1280, 720)
# global area
score = 0
level = 1
gravity = 0.5


class CharacterControllerComponent(Core.Component):
    """Controller component for character"""
    def __init__(self, name):
        super().__init__(name)
        self.speed = 2
        self.jump_speed = -2

    def Update(self, obj, dt):
        
        if obj.GetComponent("TransformComponent").position().y >=510:
                obj.GetComponent("TransformComponent").velocity().y = 0
                
        if engine.IsKeyPressed("Right"):
            print("Right")
            obj.GetComponent("TransformComponent").velocity().x = self.speed
        elif engine.IsKeyPressed("Left"):
            obj.GetComponent("TransformComponent").velocity().x = -self.speed
        elif engine.IsKeyPressed("Up"):
            obj.GetComponent("TransformComponent").velocity().y = self.jump_speed
        else:
            obj.GetComponent("TransformComponent").velocity().x = 0

        if(obj.GetComponent("TransformComponent").position().y <=500 and obj.GetComponent("TransformComponent").velocity().y < 10):
            obj.GetComponent("TransformComponent").velocity().y += gravity
        
        
        


    pass


resource_manager = Core.ResourceManager.GetInstance()
resource_manager.LoadResources(".")
engine.SwitchScene(resource_manager.LoadScene("default.scene"))

Utils.PlayMusic("background_winter.wav")

message_ui = UI.UIText.Create("comic.ttf", "",
                            Utils.Vec2D(800, 600), Utils.Vec2D(500, 300), 200)

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
character_bounding_box = Utils.RectF()
# TODO: size
character_bounding_box.x = character.GetComponent("TransformComponent").position().x
character_bounding_box.y = character.GetComponent("TransformComponent").position().y
character_bounding_box.w = 10
character_bounding_box.h = 10
character_collision_component = Physics.CollisionComponent(character_bounding_box)
character.AddComponent(character_collision_component)

# Pipe game object
pipe = Core.GameObject()
def pipe_collision_callback(obj):
    """Pipe collision callback"""
    obj.GetComponent("TransformComponent").velocity().x = 0
    obj.GetComponent("TransformComponent").velocity().y = 0

pip_transform = pipe.GetComponent("TransformComponent")
pip_transform.position().x = 400
pip_transform.position().y = 510

pipe_bounding_box = Utils.RectF()
# TODO: size
pipe_bounding_box.w = 50
pipe_bounding_box.h = 50
pipe_collision_component = Physics.CollisionComponent(pipe_bounding_box)
pipe_collision_component.callback = pipe_collision_callback

pipe_collision_component.Register(character)
pipe.AddComponent(pipe_collision_component)

# Enemy game object
enemy = Core.GameObject()
def enemy_collision_callback(obj):
    """Enemy collision callback"""
    Utils.PlayMusic("mario_dead.wav")
    message_ui.SetContent("Failed")

enemy_transform = enemy.GetComponent("TransformComponent")
enemy_transform.position().x = 500
enemy_transform.position().y = 510

enemy_bounding_box = Utils.RectF()
# TODO: size
enemy_bounding_box.w = 10
enemy_bounding_box.h = 10
enemy_collision_component = Physics.CollisionComponent(enemy_bounding_box)
enemy_collision_component.callback = enemy_collision_callback
enemy_collision_component.Register(character)
enemy.AddComponent(enemy_collision_component)

# Coin game object
coin = Core.GameObject()
def coin_collision_callback(obj):
    """Coin collision callback"""
    Utils.PlayMusic("coin_sound.wav")
    global score
    score += random.randint(1, 5)
    score_ui.SetContent("Score: " + str(score))

coin_transform = coin.GetComponent("TransformComponent")
coin_transform.position().x = 200
coin_transform.position().y = 510

coin_bounding_box = Utils.RectF()
coin_bounding_box.x = coin_transform.position().x
coin_bounding_box.y = coin_transform.position().y
# TODO: size
coin_bounding_box.w = 10
coin_bounding_box.h = 10
coin_collision_component = Physics.CollisionComponent(coin_bounding_box)
coin_collision_component.callback = coin_collision_callback
coin_collision_component.Register(character)
coin.AddComponent(coin_collision_component)

# Flag game object
flag = Core.GameObject()
def flag_collision_callback(obj):
    """Flag collision callback"""
    Utils.PlayMusic("world_finished.wav")
    message_ui.SetContent("Success!")

flag_transform = flag.GetComponent("TransformComponent")
flag_transform.position().x = 900
flag_transform.position().y = 510

flag_bounding_box = Utils.RectF()
# TODO: size
flag_bounding_box.w = 10
flag_bounding_box.h = 10
flag_collision_component = Physics.CollisionComponent(flag_bounding_box)
flag_collision_component.callback = flag_collision_callback
flag_collision_component.Register(character)
flag.AddComponent(flag_collision_component)

camera_component = Core.CameraComponent()
camera_component.SetViewport(1280, 720)
camera_component.Bind(character)
camera.AddComponent(camera_component)

score_ui = UI.UIText.Create("comic.ttf", "Score: " + str(score),
                            Utils.Vec2D(1150, 50), Utils.Vec2D(200, 100), 100)
level_ui = UI.UIText.Create("comic.ttf", "Level: 1-" + str(level),
                            Utils.Vec2D(650, 50), Utils.Vec2D(200, 100), 100)

engine.AddObject("Character", character)
engine.AddObject("Camera", camera)
engine.AddObject("ScoreUI", score_ui)
engine.AddObject("LevelUI", level_ui)
engine.AddObject("MessageUI", message_ui)
engine.AddObject("Pipe", pipe)
engine.AddObject("Enemy", enemy)
engine.AddObject("Coin", coin)
engine.AddObject("Flag", flag)

engine.Run(60)

resource_manager.ReleaseAll()
