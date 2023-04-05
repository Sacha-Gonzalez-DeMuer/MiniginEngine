#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextObject.h"
#include "Scene.h"
#include "Input.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "FPSCounter.h"
#include "Orbiter.h"
#include "CacheTrasher.h"
#include "CacheTrashDebugger.h"
#include "HealthComponent.h"

#include <chrono>
#include <iostream>


void AddFPSCounter(dae::Scene& scene, std::shared_ptr<dae::Font> font);
void AddOrbitSystem(dae::Scene& scene);
void AddCacheTrasher(dae::Scene& scene);
void AddInputAssignment(dae::Scene& scene);

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	// background
	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>()->SetTexture("background.tga");
	scene.Add(go);

	// logo
	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>()->SetTexture("logo.tga");
	go->SetLocalPosition(216, 180);
	scene.Add(go);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	auto go_assignment = std::make_shared<dae::GameObject>();
	go_assignment->SetLocalPosition(80, 20);
	go_assignment->AddComponent<dae::TextObject>("Programming 4 Assignment", font);
	scene.Add(go_assignment);

	// FPS counter
	AddFPSCounter(scene, font);


	// setup player
	auto go_player = std::make_shared<dae::GameObject>();
	go_player->AddComponent<dae::RenderComponent>()->SetTexture("player.png");
	auto health = go_player->AddComponent<dae::HealthComponent>(100.0f, 3);
	go_player->SetLocalPosition(250, 250);

	// setup input
	auto kill_command = std::make_shared<dae::KillCommand>(health);
	dae::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_D, SDL_KEYDOWN), kill_command);


	scene.Add(go_player);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}

void AddFPSCounter(dae::Scene& scene, std::shared_ptr<dae::Font> font)
{
	auto go_fps_counter = std::make_shared<dae::GameObject>();
	go_fps_counter->AddComponent<dae::FPSCounter>();
	go_fps_counter->AddComponent<dae::TextObject>("FPS", font);
	go_fps_counter->SetLocalPosition(0, 0);
	scene.Add(go_fps_counter);
}

void AddOrbitSystem(dae::Scene& scene)
{
	// === Scenegraph objects
	// create sun
	auto go_sun = std::make_shared<dae::GameObject>();
	go_sun->AddComponent<dae::RenderComponent>()->SetTexture("sun.png");
	go_sun->SetLocalPosition(250, 250);

	// create earth
	auto go_earth = std::make_shared<dae::GameObject>();
	go_earth->AddComponent<dae::RenderComponent>()->SetTexture("earth.png");
	go_earth->AddComponent<dae::Orbiter>(150.0f);

	// create moon
	auto go_moon = std::make_shared<dae::GameObject>();
	go_moon->AddComponent<dae::RenderComponent>()->SetTexture("moon.png");
	go_moon->AddComponent<dae::Orbiter>(100.0f);

	// create hierarchy
	//go_moon->SetParent(go_earth, false);
	go_sun->AddChild(go_earth);

	// add celestial bodies to scene
	scene.Add(go_sun);
}

void AddCacheTrasher(dae::Scene& scene)
{
	auto cache_trasher_go{ std::make_shared<dae::GameObject>() };
	cache_trasher_go->AddComponent<dae::CacheTrashDebugger>();
	scene.Add(cache_trasher_go);
}

void AddInputAssignment(dae::Scene& scene)
{
	// set up objects
	auto controller0{ std::make_shared<dae::XBox360Controller>(0) };
	auto pacman{ std::make_shared<dae::GameObject>() };
	auto pacwoman{ std::make_shared<dae::GameObject>() };
	pacman->AddComponent<dae::RenderComponent>()->SetTexture("pacman.png");
	pacwoman->AddComponent<dae::RenderComponent>()->SetTexture("pacwoman.png");
	float speed{ 10.0f };

	// pacman commands
	auto move_right_command{ std::make_shared<dae::MoveCommand>(pacman, glm::vec2(1,0), speed) };
	auto move_left_command{ std::make_shared<dae::MoveCommand>(pacman, glm::vec2(-1,0), speed) };
	auto move_up_command{ std::make_shared<dae::MoveCommand>(pacman, glm::vec2(0,-1), speed) };
	auto move_down_command{ std::make_shared<dae::MoveCommand>(pacman, glm::vec2(0,1), speed) };

	// Controller binds
	dae::Input::GetInstance().AddController(controller0);
	dae::Input::GetInstance().AddCommand(std::make_pair(controller0->GetControllerIndex(), dae::XBox360Controller::ControllerButton::DPadRight), move_right_command);
	dae::Input::GetInstance().AddCommand(std::make_pair(controller0->GetControllerIndex(), dae::XBox360Controller::ControllerButton::DPadLeft), move_left_command);
	dae::Input::GetInstance().AddCommand(std::make_pair(controller0->GetControllerIndex(), dae::XBox360Controller::ControllerButton::DPadUp), move_up_command);
	dae::Input::GetInstance().AddCommand(std::make_pair(controller0->GetControllerIndex(), dae::XBox360Controller::ControllerButton::DPadDown), move_down_command);

	// pacwoman commands
	speed *= 2;
	move_right_command = std::make_shared<dae::MoveCommand>(pacwoman, glm::vec2(1, 0), speed);
	move_left_command = std::make_shared<dae::MoveCommand>(pacwoman, glm::vec2(-1, 0), speed);
	move_up_command = std::make_shared<dae::MoveCommand>(pacwoman, glm::vec2(0, -1), speed);
	move_down_command = std::make_shared<dae::MoveCommand>(pacwoman, glm::vec2(0, 1), speed);

	// keyboard binds
	dae::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_D, SDL_KEYDOWN), move_right_command);
	dae::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_W, SDL_KEYDOWN), move_up_command);
	dae::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_A, SDL_KEYDOWN), move_left_command);
	dae::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_S, SDL_KEYDOWN), move_down_command);

	scene.Add(pacman);
	scene.Add(pacwoman);
}