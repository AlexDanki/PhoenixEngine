#pragma once
#include "GameObject.h"
#include "Scene.h"
//#include "App.h"
#include "SceneSerializer.h"
#include "AssetManager.h"
#include <GLFW/glfw3.h>

class App;

class EditorLayer
{
public:
	void Inity(GLFWwindow* window);
	void Draw(float dt);

	void SetSelectedObject(GameObject* object);
	GameObject* GetSelectedObject() { return m_selectedObject; };
	void SetScene(Scene* scene) { m_scene = scene; };
	void SetApp(App* app) { m_app = app; }
	void SetSceneScerializer(SceneSerializer* sceneScerializer) { m_seceneSerializer = sceneScerializer; }

private:

	float m_dammySpacing = 2.0f;
	float m_saveMessageTime = 0;
	bool m_uniformTranformScale = true;
	bool m_uniformBoxColliderSize = true;
	bool m_sceneSaved = false;
	char m_nameBuffer[256] = " ";
	char m_tagBuffer[256] = " ";
	char m_textBuffer[256] = " ";

	App* m_app = nullptr;
	GLFWwindow* m_window = nullptr;
	Scene* m_scene = nullptr;
	GameObject* m_selectedObject = nullptr;
	SceneSerializer* m_seceneSerializer = nullptr;
	AssetManager m_assetManager;

	void DrawInspector();
	void DrawHierarchy();
	void DrawCreateObjects(float dt);
	void DrawLights();
};