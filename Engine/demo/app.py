import PyCSPillEngine

engine = PyCSPillEngine.Engine("Super Mario", 1280, 720)
resource_manager = PyCSPillEngine.ResourceManager.GetInstance()
resource_manager.LoadResources(".")
engine.SwitchScene(resource_manager.LoadScene("default.scene"))
engine.Run(60)
