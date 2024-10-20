#include "fspch.h"
#include "TransformSystem.h"
#include "Fengshui/ECS/GeneralManager.h"

namespace Fengshui
{
    glm::mat4 TransformSystem::GetLocalTransformMatrix(EntityID entity)
    {
        return GetTransformMatrix(entity);
    }

    glm::mat4 TransformSystem::GetWorldTransformMatrix(EntityID entity)
    {
        EntityID curr = entity;

        glm::mat4 transformMatrix = glm::identity<glm::mat4>();

        while (curr != 0)
        {
            if (GeneralManager::HasComponent<Transform>(curr))
            {
                transformMatrix = GetTransformMatrix(curr) * transformMatrix;
            }
            curr = GeneralManager::GetComponent<Hierarchy>(curr).Parent;
        }

        return transformMatrix;
    }

    glm::mat4 TransformSystem::GetLocalTransformMatrix2D(EntityID entity)
    {
        return GetTransformMatrix2D(entity);
    }

    glm::mat4 TransformSystem::GetWorldTransformMatrix2D(EntityID entity)
    {
        EntityID curr = entity;

        glm::mat4 transformMatrix = glm::identity<glm::mat4>();

        while (curr != 0)
        {
            if (GeneralManager::HasComponent<Transform2D>(curr))
            {
                transformMatrix = GetTransformMatrix2D(curr) * transformMatrix;
            }
            curr = GeneralManager::GetComponent<Hierarchy>(curr).Parent;
        }

        return transformMatrix;
    }

    Transform TransformSystem::GetWorldTransform(EntityID entity)
    {
        //glm::mat4 transformMatrix = GetWorldTransformMatrix(entity);
        //Transform transform;
        //transform.Position = transformMatrix[3];
        //transform.Scale = glm::vec3(glm::length(transformMatrix[0]), glm::length(transformMatrix[1]), glm::length(transformMatrix[2]));
        //glm::mat3 rotationMatrix = glm::mat3(transformMatrix);
        //rotationMatrix[0] /= transform.Scale.x;
        //rotationMatrix[1] /= transform.Scale.y;
        //rotationMatrix[2] /= transform.Scale.z;
        //transform.Rotation = glm::normalize(glm::quat(rotationMatrix));
        //return transform;
        Transform transform = GeneralManager::GetComponent<Transform>(entity);

        EntityID curr = entity;

        Transform trans = Transform();

        while (curr != 0)
        {
            if (GeneralManager::HasComponent<Transform>(curr))
            {
                trans.Position += transform.Position;
                trans.Rotation *= transform.Rotation;
                trans.Scale *= transform.Scale;
            }
            curr = GeneralManager::GetComponent<Hierarchy>(curr).Parent;
            transform = GeneralManager::GetComponent<Transform>(curr);
        }

        return trans;
    }

    glm::mat4 TransformSystem::GetTransformMatrix(EntityID entity)
    {
        const Transform transform = GeneralManager::GetComponent<Transform>(entity);

        return glm::scale(glm::translate(glm::mat4(1.0f), transform.Position) * glm::mat4_cast(transform.Rotation), transform.Scale);
    }

    glm::mat4 TransformSystem::GetTransformMatrix(Transform transform)
    {
        return glm::scale(glm::translate(glm::mat4(1.0f), transform.Position) * glm::mat4_cast(transform.Rotation), transform.Scale);
    }

    glm::mat4 TransformSystem::GetTransformMatrix2D(EntityID entity)
    {
        const Transform2D transform = GeneralManager::GetComponent<Transform2D>(entity);

        if (transform.Rotation == 0)
        {
            return glm::scale(glm::translate(glm::mat4(1.0f), transform.Position), { transform.Scale.x, transform.Scale.y, 1.0f });
        }
        else
        {
            return glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), transform.Position), glm::radians(transform.Rotation), { 0.0f, 0.0f, 1.0f }), { transform.Scale.x, transform.Scale.y, 1.0f });
        }
    }
}
