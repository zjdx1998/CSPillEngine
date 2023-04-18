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
falling = False


class CharacterControllerComponent(Core.Component):
    """Controller component for character"""

    def __init__(self, name):
        super().__init__(name)
        self.speed = 2
        self.jump_speed = -2
        self.jumping = 0
        self.max_jumping = 300

    def Update(self, obj, dt):
        transform = obj.GetComponent("TransformComponent")
        if engine.IsKeyPressed("1"):
            print(transform.position())
        if falling:
            transform.velocity().x = 0
            transform.velocity().y = 0.5
            if transform.position().y > 1200:
                exit(0)
            return
        if transform.position().y >= ground_y:
            transform.velocity().y = 0
            transform.position().y = ground_y
            self.jumping = 0

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
            self.jumping += self.jump_speed * dt
            if self.jumping < -self.max_jumping:
                transform.velocity().y = 0
        elif self.jumping < 0:
            self.jumping = min(self.jumping - self.jump_speed * dt, 0)

        if not engine.IsKeyPressed("Left") and not engine.IsKeyPressed("Right"):
            transform.velocity().x = 0

        if transform.position().y <= ground_y - 10 and transform.velocity().y < 10:
            transform.velocity().y += gravity

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


def cliff_collision_callback(self, obj):
    """Pipe collision callback"""
    global falling
    if not falling:
        Utils.StopMusic(-1)
        Utils.PlayMusic("mario_dead.wav")
        message_ui.SetContent("Failed")
        falling = True


def create_cliff(position, bounding_width, bounding_height):
    cliff = Core.GameObject(position, Utils.Vec2D(1, 1))
    cliff_bounding_box = Utils.RectF()
    cliff_bounding_box.w = bounding_width
    cliff_bounding_box.h = bounding_height
    cliff_collision_component = Physics.CollisionComponent(cliff_bounding_box)
    cliff_collision_component.callback = cliff_collision_callback
    cliff_collision_component.Register(character)
    cliff.AddComponent(cliff_collision_component)
    return cliff


cliff = create_cliff(Utils.Vec2D(68 * 50, ground_y + 40), 100, 100)
cliff2 = create_cliff(Utils.Vec2D(83 * 50, ground_y + 40), 100, 100)
cliff3 = create_cliff(Utils.Vec2D(151 * 50, ground_y + 40), 100, 100)


# Pipe game object
def pipe_collision_callback(self, obj):
    """Pipe collision callback"""
    self_pos = self.GetComponent("TransformComponent").position()
    self_box = self.GetComponent("CollisionComponent").bouding_box
    obj_pos = obj.GetComponent("TransformComponent").position()
    obj_vel = obj.GetComponent("TransformComponent").velocity()
    if obj_vel.x > 0 and obj_pos.x < self_pos.x and obj_pos.y > self_pos.y:
        obj.GetComponent("TransformComponent").velocity().x = 0
    elif obj_vel.x < 0 and obj_pos.x > self_pos.x and obj_pos.y > self_pos.y:
        obj.GetComponent("TransformComponent").velocity().x = 0
    elif self_pos.x < obj_pos.x < self_pos.x + self_box.w and obj_pos.y < self_pos.y:
        obj.GetComponent("TransformComponent").velocity().y = 0
        obj.GetComponent("TransformComponent").position().y = self_pos.y - obj.GetComponent(
            "CollisionComponent").bouding_box.h


def create_pipe(position, bounding_width, bounding_height):
    """Create a pipe object"""
    pipe = Core.GameObject(position, Utils.Vec2D(1, 1))
    pipe_bounding_box = Utils.RectF()
    pipe_bounding_box.x = pipe.GetComponent("TransformComponent").position().x
    pipe_bounding_box.y = pipe.GetComponent("TransformComponent").position().y
    pipe_bounding_box.w = bounding_width
    pipe_bounding_box.h = bounding_height
    pipe_collision_component = Physics.CollisionComponent(pipe_bounding_box)
    pipe_collision_component.callback = pipe_collision_callback
    pipe_collision_component.Register(character)
    pipe.AddComponent(pipe_collision_component)
    return pipe


pipe_height = 50
pipe_width = 100
# pipe = Core.GameObject(Utils.Vec2D(29*50, ground_y - 50), Utils.Vec2D(1, 1))
pipe = create_pipe(Utils.Vec2D(28.5 * 50, ground_y - pipe_height), pipe_width, pipe_height * 2)
pipe2 = create_pipe(Utils.Vec2D(38.5 * 50, ground_y - pipe_height * 2), pipe_width, pipe_height * 3)
pipe3 = create_pipe(Utils.Vec2D(46.5 * 50, ground_y - pipe_height * 3), pipe_width, pipe_height * 4)
pipe4 = create_pipe(Utils.Vec2D(57.5 * 50, ground_y - pipe_height * 3), pipe_width, pipe_height * 4)
pipe5 = create_pipe(Utils.Vec2D(160.5 * 50, ground_y - pipe_height), pipe_width, pipe_height * 2)
pipe6 = create_pipe(Utils.Vec2D(176.5 * 50, ground_y - pipe_height), pipe_width, pipe_height * 2)

block1 = create_pipe(Utils.Vec2D(16 * 50, ground_y - pipe_height * 3), pipe_width / 2, pipe_height / 2)
block2 = create_pipe(Utils.Vec2D(950, ground_y - pipe_height * 3), 300, pipe_height / 2)
block3 = create_pipe(Utils.Vec2D(1050, ground_y - pipe_height * 6), 65, pipe_height / 2)

block4 = create_pipe(Utils.Vec2D(3650, ground_y - pipe_height * 3), 150, pipe_height / 2)
block5 = create_pipe(Utils.Vec2D(3800, ground_y - pipe_height * 7), 475, pipe_height / 2)


def enemy_collision_callback(self, obj):
    """Enemy collision callback"""
    global score
    global falling
    tc = obj.GetComponent("TransformComponent")
    if tc.position().y < ground_y:
        print("You kill a monster!")
        Utils.PlayMusic("1up_collected.wav")
        self.live = False
        score += 1
        score_ui.SetContent("Score: " + str(score))
    else:
        if not falling:
            Utils.StopMusic(-1)
            Utils.PlayMusic("mario_dead.wav")
            message_ui.SetContent("Failed")
            falling = True


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


enemies = []
for i in range(0, 10):
    # Enemy game object
    import random

    enemy = Core.GameObject(Utils.Vec2D(random.randint(400, 10000), 585), Utils.Vec2D(1, 1))

    enemy_bounding_box = Utils.RectF(0, 0, 66, 66)
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
    enemies.append(enemy)

# Coin game object
coins = []
for i in range(0, 20):
    coin = Core.GameObject(Utils.Vec2D(random.randint(300, 10000), random.randint(ground_y - 300, ground_y)))

    coin_animation_component = Core.AnimationComponent()
    coin_animation_component.AddAnimations("coin",
                                           ["coin_1.png-cropped-0", "coin_2.png-cropped-0",
                                            "coin_3.png-cropped-0", "coin_4.png-cropped-0"])
    coin_animation_component.SetCurrentAnimation("coin")
    coin.AddComponent(coin_animation_component)


    def coin_collision_callback(self, obj):
        """Coin collision callback"""
        Utils.PlayMusic("coin_sound.wav")
        global score
        score += random.randint(1, 3)
        score_ui.SetContent("Score: " + str(score))
        self.live = False


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
    coins.append(coin)

# Flag game object
flag = Core.GameObject(Utils.Vec2D(9770, ground_y))


def flag_collision_callback(self, obj):
    """Flag collision callback"""
    Utils.StopMusic(-1)
    Utils.PlayMusic("world_finished.wav")
    message_ui.SetContent("Success!")
    self.live = False


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
engine.AddObject("Pipe2", pipe2)
engine.AddObject("Pipe3", pipe3)
engine.AddObject("Pipe4", pipe4)
engine.AddObject("Pipe5", pipe5)
engine.AddObject("Pipe6", pipe6)
engine.AddObject("Flag", flag)
engine.AddObject("Cliff", cliff)
engine.AddObject("Cliff2", cliff2)
engine.AddObject("Cliff3", cliff3)
for i in range(0, len(enemies)):
    engine.AddObject("Enemy" + str(i), enemies[i])
for i in range(0, len(coins)):
    engine.AddObject("Coin" + str(i), coins[i])
engine.AddObject("Block1", block1)
engine.AddObject("Block2", block2)
engine.AddObject("Block3", block3)
engine.AddObject("Block4", block4)
engine.AddObject("Block5", block5)


engine.Run(60)

resource_manager.ReleaseAll()
