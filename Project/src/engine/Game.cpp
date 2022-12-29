﻿#include "Game.h"
#include "BGSpriteComponent.h"
#include "GL/glew.h"
#include "Mesh.h"
#include "Shader.h"
#include "SpriteComponent.h"
#include "Texture.h"
#include "VertexArray.h"
#include "engine/Math.h"
#include "engine/Renderer.h"
#include "engine/Texture.h"
#include "game/Asteroid.h"
#include "game/Ship.h"
#include <SDL.h>
#include <SDL_error.h>
#include <SDL_log.h>
#include <SDL_render.h>
#include <SDL_surface.h>
#include <SDL_timer.h>
#include <SDL_video.h>
#include <algorithm>
#include <iostream>

Game::Game()
{
	mIsRunning = true;
	mTicksCount = 0;
	mShipRespawnCooldown = 3.0f;
	mShipDead = false;
	timer = 0;
}

Game::~Game()
{
	while (!mActors.empty())
	{
		delete mActors.back();
	}
}

void Game::LoadData()
{
	// Create player's ship
	mShip = new Ship(this);

	// Create asteroids
	const int numAsteroids = 20;
	for (int i = 0; i < numAsteroids; i++)
	{
		new Asteroid(this);
	}
}

void Game::UnloadData()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!mActors.empty())
	{
		delete mActors.back();
	}
}

bool Game::Initialize()
{
	mRenderer = new Renderer();
	mRenderer->Initialize(1024, 768);

	LoadData();
	mTicksCount = SDL_GetTicks();
	return true;
}


void Game::Update()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::Shutdown()
{
	UnloadData();
	mRenderer->Shutdown();
	delete mRenderer;
}
void Game::AddActor(Actor* actor)
{
	if (mUpdatingActors)
		mPendingActors.emplace_back(actor);
	else
		mActors.emplace_back(actor);
}

void Game::RemoveActor(Actor *actor)
{
	auto iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
		mActors.erase(iter);
}


void Game::ProcessInput()
{
	SDL_Event event;

	// returns true if there are events in the q
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->ProcessInput(state);
	}
	mUpdatingActors = false;
}

void Game::UpdateGame()
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16)); // limits to 60 fps

	// calc delta time
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.f;
	mTicksCount = SDL_GetTicks();

	timer += deltaTime;
	// clamp delta time to avoid big simulation jumps during debugging
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	mUpdatingActors = true;
	for(auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	for(auto pending : mPendingActors)
	{
		pending->CalculateWorldTransform();
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	std::vector<Actor*> actorsPendingRemoval;
	for(auto actor : mActors)
	{
		if (actor->GetState() == Actor::EPendingRemoval)
			actorsPendingRemoval.emplace_back(actor);
	}

	for(auto actor : actorsPendingRemoval)
	{
		delete actor;
	}
	if (mShipDead)
	{
		mShipRespawnCooldown -= deltaTime;
		if (mShipRespawnCooldown <= 0.0f)
			RespawnShip();
	}
}
void Game::AddAsteroid(Asteroid *ast)
{
	mAsteroids.emplace_back(ast);
}
void Game::RemoveAsteroid(Asteroid *ast)
{
	auto iter = std::find(mAsteroids.begin(), mAsteroids.end(), ast);
	if (iter != mAsteroids.end())
		mAsteroids.erase(iter);
}
void Game::NotifyShipDeath()
{
	mShipRespawnCooldown = 3.0f;
	mShipDead = true;
}

void Game::RespawnShip()
{
	mShipDead = false;
	mShipRespawnCooldown = 3.0f;
	mShip = new Ship(this);
	mShip->SetPosition(Vector2(512.0f, 384.0f));
	mShip->SetScale(1.5f);
}

std::vector<Asteroid *> Game::GetAsteroids() const
{
	return mAsteroids;
}
void Game::GenerateOutput()
{
	mRenderer->Draw();
}
