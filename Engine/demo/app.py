import PyCSPillEngine

engine = PyCSPillEngine.Engine("Super Mario", 1280, 720)
resource_manager = PyCSPillEngine.ResourceManager.GetInstance()
resource_manager.LoadResources(".")
engine.Run(60)
