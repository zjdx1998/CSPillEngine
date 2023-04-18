import random
import sys

sys.path.append('../build')
from PyCSPillEngine import Core, Utils, UI, Physics

# engine through the game
engine = Core.Engine("Super Mario", 1280, 750)
# global area
score = 0
level = 1
gravity = 0.5
ground_y = 555


class CharacterControllerComponent(Core.Component):
    """Controller component for character"""

    def __init__(self, name):
        super().__init__(name)
        self.speed = 2
        self.jump_speed = -2

    def Update(self, obj, dt):
        transform = obj.GetComponent("TransformComponent")
        if engine.IsKeyPressed("1"):
            print(transform.position())
        if transform.position().y >= ground_y:
            transform.velocity().y = 0

        if engine.IsKeyPressed("Right"):
            obj.GetComponent("AnimationComponent").SetCurrentAnimation("walkright")
            transform.velocity().x = self.speed
        if engine.IsKeyPressed("Left"):
            obj.GetComponent("AnimationComponent").SetCurrentAnimation("walkleft")
            if transform.position().x <= 0:
                transform.position().x = 0
                return
            transform.velocity().x = -self.speed
        if engine.IsKeyPressed("Space"):
            transform.velocity().y = self.jump_speed

        if not engine.IsKeyPressed("Left") and not engine.IsKeyPressed("Right"):
            transform.velocity().x = 0

        if transform.position().y <= ground_y - 10 and transform.velocity().y < 10:
            transform.velocity().y += gravity
        if transform.position().y > ground_y:
            transform.position().y = ground_y

    pass


resource_manager = Core.ResourceManager.GetInstance()
resource_manager.LoadResources(".")
engine.SwitchScene(resource_manager.LoadScene("default.scene"))

Utils.PlayMusic("background_winter.wav")

message_ui = UI.UIText.Create("comic.ttf", "",
                              Utils.Vec2D(800, 600), Utils.Vec2D(500, 300), 200)

character = Core.GameObject(Utils.Vec2D(100, ground_y), Utils.Vec2D(5, 5))
camera = Core.GameObject()

character.AddComponent(CharacterControllerComponent("ControllerComponent"))
character_animation_component = Core.AnimationComponent()
character_animation_component.AddAnimations("walkright",
                                            ["mario-edited.png-cropped-1000", "mario-edited.png-cropped-1001",
                                             "mario-edited.png-cropped-1002", "mario-edited.png-cropped-1003"])
character_animation_component.AddAnimations("walkleft",
                                            ["mario-edited.png-cropped-0", "mario-edited.png-cropped-1",
                                             "mario-edited.png-cropped-2", "mario-edited.png-cropped-3"])
character_animation_component.SetCurrentAnimation("walkright")
character.AddComponent(character_animation_component)
character_bounding_box = Utils.RectF()
# TODO: size
character_bounding_box.x = character.GetComponent("TransformComponent").position().x
character_bounding_box.y = character.GetComponent("TransformComponent").position().y
character_bounding_box.w = 50
character_bounding_box.h = 50
character_collision_component = Physics.CollisionComponent(character_bounding_box)
character.AddComponent(character_collision_component)

ground = Core.GameObject(Utils.Vec2D(0, ground_y), Utils.Vec2D(1, 1))


def ground_collision_callback(self, obj):
    """Pipe collision callback"""
    v_y = obj.GetComponent("TransformComponent").velocity().y
    pos_y = obj.GetComponent("TransformComponent").position().y
    height = obj.GetComponent("CollisionComponent").bouding_box.h
    print("collide " + str(v_y) + " " + str(pos_y) + " " + str(height))
    obj.GetComponent("TransformComponent").position().y = ground_y - height
    obj.GetComponent("TransformComponent").velocity().y = 0


ground_bounding_box = Utils.RectF()
ground_bounding_box.w = 68 * 50
ground_bounding_box.h = 100
ground_collision_component = Physics.CollisionComponent(ground_bounding_box)
ground_collision_component.callback = ground_collision_callback
ground_collision_component.Register(character)
ground.AddComponent(ground_collision_component)

# Pipe game object
pipe = Core.GameObject(Utils.Vec2D(400, 510))

pipe = Core.GameObject(Utils.Vec2D(29 * 50, ground_y - 100), Utils.Vec2D(5, 5))


def pipe_collision_callback(self, obj):
    """Pipe collision callback"""
    v_x = obj.GetComponent("TransformComponent").velocity().x
    pos_x = obj.GetComponent("TransformComponent").position().x
    print(str(v_x))
    print(str(pos_x))
    if v_x > 0:
        obj.GetComponent("TransformComponent").position().x -= 10 + obj.GetComponent("CollisionComponent").bouding_box.w
    elif v_x < 0:
        obj.GetComponent("TransformComponent").position().x += 10
    obj.GetComponent("TransformComponent").velocity().x = 0


pipe_bounding_box = Utils.RectF()
pipe_bounding_box.x = pipe.GetComponent("TransformComponent").position().x
pipe_bounding_box.y = pipe.GetComponent("TransformComponent").position().y
pipe_bounding_box.w = 100
pipe_bounding_box.h = 100
pipe_collision_component = Physics.CollisionComponent(pipe_bounding_box)
pipe_collision_component.callback = pipe_collision_callback

pipe_collision_component.Register(character)
pipe.AddComponent(pipe_collision_component)


def enemy_collision_callback(self, obj):
    """Enemy collision callback"""
    tc = obj.GetComponent("TransformComponent")
    if tc.position().y < 550:
        print("you kill a monster")

    else:
        Utils.StopMusic(-1)
        Utils.PlayMusic("mario_dead.wav")
        message_ui.SetContent("Failed")


class EnemyControllerComponent(Core.Component):
    def __init__(self, name):
        super().__init__(name)
        self.velocity = 0.3
        self.direction = 1
        self.move = 0
        self.threshold = 100
        self.walk = "walkright"

    def Update(self, obj, dt):
        t = obj.GetComponent("TransformComponent")
        t.velocity().x = self.velocity
        self.move += abs(dt * self.velocity)
        if self.move > self.threshold:
            self.move = 0
            self.velocity = self.velocity * -1
            if self.walk == "walkright":
                self.walk = "walkleft"
            else:
                self.walk = "walkright"
            obj.GetComponent("AnimationComponent").SetCurrentAnimation(self.walk)


# Enemy game object
enemy = Core.GameObject(Utils.Vec2D(500, 585), Utils.Vec2D(1, 1))
# TODO: size
enemy_bounding_box = Utils.RectF(0, 0, 50, 50)
enemy_collision_component = Physics.CollisionComponent(enemy_bounding_box)
enemy_collision_component.callback = enemy_collision_callback
enemy_collision_component.Register(character)
enemy.AddComponent(enemy_collision_component)

enemy_animation_component = Core.AnimationComponent()
enemy_animation_component.AddAnimations("walkright",
                                        ["enemy_1.png-cropped-0", "enemy_2.png-cropped-0",
                                         "enemy_3.png-cropped-0", "enemy_4.png-cropped-0"])
enemy_animation_component.AddAnimations("walkleft",
                                        ["enemy_left_1.png-cropped-0", "enemy_left_2.png-cropped-0",
                                         "enemy_left_3.png-cropped-0", "enemy_left_4.png-cropped-0"])
enemy.AddComponent(enemy_animation_component)
enemy.AddComponent(EnemyControllerComponent("ControllerComponent"))

# Coin game object
coin = Core.GameObject(Utils.Vec2D(200, 510))


def coin_collision_callback(self, obj):
    """Coin collision callback"""
    Utils.PlayMusic("coin_sound.wav")
    global score
    score += random.randint(1, 5)
    score_ui.SetContent("Score: " + str(score))


# TODO: size
coin_bounding_box = Utils.RectF()
coin_bounding_box.x = coin.GetComponent("TransformComponent").position().x
coin_bounding_box.y = coin.GetComponent("TransformComponent").position().y
coin_bounding_box.w = 10
coin_bounding_box.h = 10

coin_collision_component = Physics.CollisionComponent(coin_bounding_box)
coin_collision_component.callback = coin_collision_callback
coin_collision_component.Register(character)
coin.AddComponent(coin_collision_component)

# Flag game object
flag = Core.GameObject(Utils.Vec2D(100, 510))


def flag_collision_callback(self, obj):
    """Flag collision callback"""
    Utils.PlayMusic("world_finished.wav")
    message_ui.SetContent("Success!")


flag_bounding_box = Utils.RectF()
# TODO: size
flag_bounding_box.w = 10
flag_bounding_box.h = 10
flag_collision_component = Physics.CollisionComponent(flag_bounding_box)
flag_collision_component.callback = flag_collision_callback
flag_collision_component.Register(character)
flag.AddComponent(flag_collision_component)

camera_component = Core.CameraComponent()
camera_component.SetViewport(1280, 750)
camera_component.Bind(character, Utils.Vec2D(400, 200))
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
# engine.AddObject("Coin", coin)
# engine.AddObject("Flag", flag)

engine.Run(60)

resource_manager.ReleaseAll()
