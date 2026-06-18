#include "BoxCollider.h"
#include "GameObject.h"
#include <iostream>

BoxCollider::BoxCollider()
{
	UpdateBounds();
}

void BoxCollider::OnAttach()
{
	m_transform = &GetOwner()->GetTransform();
	UpdateBounds();
}

void BoxCollider::UpdateBounds()
{
	glm::vec3 halfSize = size * 0.5f;

	min = -halfSize;
	max = halfSize;
}

std::array<glm::vec3, 8> BoxCollider::GetCorners()
{
    glm::vec3 min = GetMin();
    glm::vec3 max = GetMax();

    return
    {
        glm::vec3(min.x, min.y, min.z),
        glm::vec3(max.x, min.y, min.z),
        glm::vec3(max.x, min.y, max.z),
        glm::vec3(min.x, min.y, max.z),

        glm::vec3(min.x, max.y, min.z),
        glm::vec3(max.x, max.y, min.z),
        glm::vec3(max.x, max.y, max.z),
        glm::vec3(min.x, max.y, max.z)
    };
}

bool BoxCollider::Intersects(BoxCollider& other)
{

    if (GetMax().x < other.GetMin().x) return false;
    if (GetMin().x > other.GetMax().x) return false;

    if (GetMax().y < other.GetMin().y) return false;
    if (GetMin().y > other.GetMax().y) return false;
    

    if (GetMax().z < other.GetMin().z) return false;
    if (GetMin().z > other.GetMax().z) return false;

    return true;
}

glm::vec3 BoxCollider::GetMin()
{
	glm::vec3 worldPosition = glm::vec3(m_transform->position.x, m_transform->position.y, m_transform->position.z);
	return min + worldPosition;
}

glm::vec3 BoxCollider::GetMax()
{
	glm::vec3 worldPosition = glm::vec3(m_transform->position.x, m_transform->position.y, m_transform->position.z);
	return max + worldPosition;
}

