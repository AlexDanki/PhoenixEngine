#include "EditorLayer.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "MeshComponent.h"
#include "Transform.h"
#include "RigidbodyComponent.h"
#include "CameraComponent.h"
#include "RotatorScript.h"
#include "BoxCollider.h"
#include "ScriptComponent.h"
#include "Material.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "App.h"

namespace ui = ImGui;

void EditorLayer::Inity(GLFWwindow* window)
{
	m_window = window;
	if(m_window)
	{
		std::cout << "EditorLayer Iniciado\n";
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void EditorLayer::Draw(float dt)
{
	if(m_selectedObject)
	{
		DrawInspector();
	}

	DrawHierarchy();
	DrawLights();
	DrawCreateObjects(dt);
}

void EditorLayer::DrawInspector()
{
	ImGui::Begin("Inspector");
	MeshComponent* meshComponent = m_selectedObject->GetComponent<MeshComponent>();
	if (meshComponent)
	{
		Material* material = meshComponent->GetMaterial();
		Transform& transform = m_selectedObject->GetTransform();

		ImGui::Text(("Object Name: " + m_selectedObject->GetName()).c_str());
		ImGui::Dummy(ImVec2(0.0f, m_dammySpacing));
		ImGui::Separator();//----------------------------------------
		ImGui::Dummy(ImVec2(0.0f, m_dammySpacing));
		ui::Checkbox("Is Dinamic", &m_selectedObject->dinamic);

		if (ui::CollapsingHeader("Transform"))
		{
			ImGui::Text("Transform");
			ImGui::DragFloat3("Position", &transform.position.x, 0.1f);
			ImGui::DragFloat3("Rotation", &transform.rotation.x, 0.1f);

			ui::Checkbox("Uniform scale", &m_uniformTranformScale);

			if (m_uniformTranformScale)
			{
				float scale = transform.scale.x;

				if(ui::DragFloat("All(x, y, z)", &scale, 0.1))
				{
					transform.scale.x = scale;
					transform.scale.y = scale;
					transform.scale.z = scale;
				}

			}
			if(!m_uniformTranformScale)
			{
				ui::DragFloat3("Scale", &transform.scale.x, 0.1);
			}
		}
		if (ui::CollapsingHeader("Mesh Renderer"))
		{
			if (material)
			{

				ImGui::Text("Material Color");
				ImGui::ColorEdit3(" ", glm::value_ptr(material->GetMaterialColor()));

			}
		}
		

	} // END MESH COMPONENT

	if (auto collider = m_selectedObject->GetComponent<BoxCollider>())
	{
		if (ui::CollapsingHeader("BoxCollider"))
		{
			ui::Text("Box Collider");

			bool trigger = collider->isTrigger;
			if (ui::Checkbox("Is trigger", &trigger))
			{
				collider->isTrigger = trigger;
			}

			ui::DragFloat3("Center", &collider->center.x, 0.1);

			ui::Checkbox("Uniform Scale", &m_uniformBoxColliderSize);

			if (m_uniformBoxColliderSize)
			{
				float size = 0.0;
				if(ui::DragFloat("all(x,y,z)", &size, 0.1))
				{
					collider->size.x += size;
					collider->size.y += size;
					collider->size.z += size;
					collider->UpdateBounds();
				}
			}

			if (!m_uniformBoxColliderSize)
			{
				ui::DragFloat3("Size", &collider->size.x, 0.1);
				collider->UpdateBounds();
			}
		}
	}
	if (auto rigidbody = m_selectedObject->GetComponent<RigidbodyComponent>())
	{
		if (ui::CollapsingHeader("RigidBody"))
		{
			ui::Text("Rigidbody");
			ImGui::Dummy(ImVec2(0.0f, m_dammySpacing));
			ui::DragFloat("Mass", &rigidbody->mass, 0.1);
			ui::Checkbox("HasGravity", &rigidbody->useGravity);
			ui::DragFloat("GravitScale", &rigidbody->gravityScale, 0.1);
		}

	}
	if(auto camera = m_selectedObject->GetComponent<CameraComponent>())
	{
		if(ui::CollapsingHeader("Camera"))
		{
			ui::DragFloat("Fov", &camera->FOV, 0.1);
			ui::DragFloat("NearPlane", &camera->nearPlane, 0.1);
			ui::DragFloat("FarPlane", &camera->farPlane, 0.1);
			ImGui::Checkbox("Primary",&camera->primary);
		}
	}

	if (auto script = m_selectedObject->GetComponent<ScriptComponent>())
	{
		if (ImGui::CollapsingHeader("Script"))
		{
			ImGui::Text("Script Component");
		}
	}
	ImGui::Dummy(ImVec2(0.0f, m_dammySpacing));
	if(ui::Button("AddComponent"))
	{
		ui::OpenPopup("AddComponentPopPup");
	}
	if (ui::BeginPopup("AddComponentPopPup"))
	{
		ui::Text("COMPONENTES:");
		// Script não precisa ser verificado pq sempre podem ser adicionados
		ui::Separator();
		if (ui::MenuItem("BoxCollider"))
		{
			m_selectedObject->AddComponent(std::make_unique<BoxCollider>());
		}
		if(!m_selectedObject->GetComponent<RigidbodyComponent>())
		{
			ui::Separator();
			if(ui::MenuItem("RigidBody"))
			{
				m_selectedObject->AddComponent(std::make_unique<RigidbodyComponent>());
			}
			
		}
		ui::Separator();
		if(!m_selectedObject->GetComponent<CameraComponent>())
		{
			if (ui::MenuItem("Camera"))
			{
				m_selectedObject->AddComponent(std::make_unique<CameraComponent>());
			}
		}
		ui::Separator();
		// Script não precisa ser verificado pq sempre podem ser adicionados
		if (ui::MenuItem("Script"))
		{
			m_selectedObject->AddComponent(std::make_unique<RotatorScript>());
		}
		ui::EndPopup();
	}

	ImGui::End(); // END INSPECTOR
}
void EditorLayer::DrawHierarchy()
{
	ImGui::Begin("Hieranchy");
	for (auto& object : m_scene->GetGameObjects())
	{
		bool selected = object.get() == m_selectedObject;

		if (ImGui::Selectable(object->GetName().c_str(), selected)) {
			m_selectedObject = object.get();
		}

	}
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0.0f, m_dammySpacing));
	ImGui::End(); // END HIERARCHY
}

void EditorLayer::DrawCreateObjects(float dt)
{
	ImGui::Begin("Create");
	if (ImGui::Button("Create Cube"))
	{
		GameObject* cube = m_app->CreateCube();
		m_selectedObject = cube;

	}
	if (ImGui::Button("Create Plane"))
	{
		GameObject* plane = m_app->CreatePlane();
		m_selectedObject = plane;
	}
	ImGui::Dummy(ImVec2(0.0f, m_dammySpacing));
	ui::Separator();
	ImGui::Dummy(ImVec2(0.0f, m_dammySpacing));

	if (ui::Button("Delete Selected"))
	{
		m_scene->RemoveGameObject(m_selectedObject);
		if (m_scene->GetGameObjects().size() > 0)
		{
			m_selectedObject = m_scene->GetGameObjects()[0].get();

		}
		else
		{
			m_selectedObject = nullptr;
		}
	}
	ImGui::Dummy(ImVec2(0.0f, m_dammySpacing));
	ui::Separator();
	ImGui::Dummy(ImVec2(0.0f, m_dammySpacing));
	if (ImGui::Button("SaveScene", ImVec2(100, 20)))
	{
		m_seceneSerializer->Save(*m_scene, "Scene.txt");
		m_sceneSaved = true;
		m_saveMessageTime = 2.0f;
	}

	if (m_sceneSaved)
	{
		ImGui::Text("Scene Saved!");
	}

	if (m_saveMessageTime > 0.0)
	{
		m_saveMessageTime -= dt;
	}

	if (m_saveMessageTime <= 0.0)
	{
		m_sceneSaved = false;
	}
	ImGui::End(); // END CREATE
}
void EditorLayer::DrawLights()
{
	ImGui::Begin("Lighting");
	ImGui::Text("Directional Light");
	ImGui::DragFloat3("Direction", glm::value_ptr(m_scene->GetDirectionalLight().Direction));
	ImGui::ColorEdit3("D_Color", glm::value_ptr(m_scene->GetDirectionalLight().Color));
	ImGui::DragFloat("D_Intensity", &m_scene->GetDirectionalLight().Intensity, 0.0f, 0.0f, 10.0);
	ImGui::Dummy(ImVec2(0.0f, m_dammySpacing));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0.0f, m_dammySpacing));
	ImGui::Text("Ambiente");
	ImGui::ColorEdit3("A_Color", glm::value_ptr(m_scene->GetAmbienteLight().Color));
	ImGui::DragFloat("A_Intensity", &m_scene->GetAmbienteLight().Intensity, 1.0);
	ImGui::End(); // END LIGHT
}