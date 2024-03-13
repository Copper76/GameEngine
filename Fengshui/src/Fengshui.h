#pragma once

/**
* For external application usage
**/

//Application
#include "Fengshui/Core/Application.h"

//Time
#include "Fengshui/Core/Time.h"

//Logging
#include "Fengshui/Logging/Log.h"

//Layers
#include "Fengshui/Layers/Layer.h"
#include "Fengshui/IMGUI/ImGuiLayer.h"

//Renderer
#include "Fengshui/Renderer/Renderer.h"
#include "Fengshui/Renderer/RenderCommand.h"
#include "Fengshui/Renderer/Shader.h"
#include "Fengshui/Renderer/Buffer.h"
#include "Fengshui/Renderer/VertexArray.h"
#include "Fengshui/Renderer/Texture.h"

//Inputs with mouse button and key codes
#include "Fengshui/Core/Input.h"
#include "Fengshui/Core/KeyCode.h"
#include "Fengshui/Core/MouseButtonCode.h"

//Scene
#include "Fengshui/Core/Scene.h"

//Entity
#include "Fengshui/ECS/GameEntity.h"

//Components
#include "Fengshui/ECS/Components/TransformComponent.h"
#include "Fengshui/ECS/Components/RenderComponent.h"
#include "Fengshui/ECS/Components/CameraComponent.h"

//Entry Point, place headers above this
#include "Fengshui/Core/EntryPoint.h"