#pragma once

class Scene;

class SceneManager
{
public:
	SceneManager(shared_ptr<Graphics> graphics);
	~SceneManager();

public:
	void Init();
	void Update();
	void LoadScene(wstring sceneName);

	
public:
	inline shared_ptr<Scene> GetActiveScene() { return _activeScene; }

private:
	shared_ptr<Scene> LoadTestScene();

private:
	shared_ptr<Graphics> _graphics;

private:
	shared_ptr<Scene> _activeScene;
};

