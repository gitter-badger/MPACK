#include "PlayGameState.hpp"

#include "Render.hpp"
#include "InputService.hpp"
#include "TimeService.hpp"
#include "PhysicsService.hpp"
#include "DemoApplication.hpp"
#include "TextureMappedFont.hpp"
#include "Global.hpp"
#include "Context.hpp"
#include "Particles.hpp"
#include "Camera2D.hpp"

using namespace MPACK;
using namespace MPACK::Physics;
using namespace MPACK::Graphics;
using namespace MPACK::UI;

namespace Game
{
	PlayGame::PlayGame()
	{
		m_pPGInputController = PGInputController::Initialize();
		m_pPGInputController->SetMaxAcceleration(500.0f);

		Global::pContext->pPhysicsService->callback=Physics_callback;

		m_backgroundTexture = new Texture2D;
		m_backgroundTexture->Load("@Backgrounds/spacebk1.png");
		m_backgroundTexture->SetWrapMode(GL_REPEAT,GL_REPEAT);

		m_background = new TiledBackground;
		m_background->SetTexture(m_backgroundTexture);
		m_background->SetShading(SpriteVertex::NONE);
		m_background->SetDepth(2.0f);
		m_background->SetUVScale(0.5f,0.5f);
		m_background->EnableCamera();

		m_particleTex = new Texture2D;
		m_particleTex->Load("@Sprites/GUI_Button.tga");

		m_pEmitter = new ParticleEmitterCircle;

		m_pEmitter->SetAngularVelocity(15.0f,17.0f);
		m_pEmitter->SetParticleLife(8.0f,10.0f);
		m_pEmitter->SetVelocity(10.0f,15.0f);
		m_pEmitter->SetTexture(m_particleTex);
		m_pEmitter->SetPosition(Vector2f(200.0f,200.0f));
		m_pEmitter->SetSpawnDelay(1.0f/10.0f);
		m_pEmitter->m_modifiers.push_back(new ParticleEffectorGravity());
		m_pEmitter->m_modifiers.push_back(new ParticleEffectorColor(Vector4f(1.0f,1.0f,0.0f,1.0f),Vector4f(0.0f,0.0f,1.0f,0.0f)));


		//Texture loading
		m_enemyTexture=new Texture2D;
		m_playerTexture=new Texture2D;

		m_enemyTexture->Load("@Sprites/Ships/enemy1.png");
		m_playerTexture->Load("@Sprites/Ships/Player.png");

		//Player sprite setup
		m_playerSprite=new Sprite;
		m_playerSprite->SetTexture(m_playerTexture);
		m_playerSprite->SetSize(50.0f,50.0f);
		m_playerSprite->SetShading(SpriteVertex::ALPHA_BLEND);

		//Player setup
		m_playerObject=new Player;
		m_playerObject->SetSprite(m_playerSprite);
		m_playerObject->SetPosition(Vector2f(Render::GetScreenWidth()*0.5,Render::GetScreenHeight()*0.5));

		//Enemy sprite setup
		m_enemySprite=new Sprite;
		m_enemySprite->SetTexture(m_enemyTexture);
		m_enemySprite->SetSize(100.0f,17.0f*4.0f);
		m_enemySprite->SetShading(SpriteVertex::ALPHA_BLEND);

		//Enemy setup
		m_enemyObject=new Enemy;
		m_enemyObject->SetSprite(m_enemySprite);
		m_enemyObject->SetPosition(Vector2f(50.0f,50.0f));

		//Camera setup
		Global::pActiveCamera=new Camera2D();
		Global::pActiveCamera->Link(m_playerObject);

		//should be deleted in final version
		m_testTexture=new Texture2D();
		m_testTexture->Load("@Sprites/Animations/explosion.png");

		m_testSprite=new AnimatedSprite();
		m_testSprite->m_position=Vector2f(Render::GetScreenWidth()*0.5,Render::GetScreenHeight()*0.5);
		m_testSprite->SetTexture(m_testTexture);
		m_testSprite->SetSize(200.0f,200.0f);
		m_testSprite->SetShading(SpriteVertex::ALPHA_BLEND);
		m_testSprite->SetLayer(1.0f);

		m_testSprite->BuildFrameGrid(5,4);
		m_testSprite->SetAnimation(Animation(0,20,10.0f));

		const float FRONT_LAYER=99910000.0f;
		m_UIMatch1Sprite = new Sprite();
		m_UIMatch1Sprite->SetTexture(m_playerTexture);
		m_UIMatch1Sprite->SetSize(300.0f,300.0f);
		m_UIMatch1Sprite->UIMatch(Anchor(Anchor::Get(Anchor::TopLeft)),Anchor(Anchor::Get(Anchor::TopLeft)));
		m_UIMatch1Sprite->SetLayer(FRONT_LAYER);
		m_UIMatch1Sprite->SetShading(SpriteVertex::ALPHA_BLEND);

		m_UIMatch2Sprite = new Sprite();
		m_UIMatch2Sprite->SetTexture(m_playerTexture);
		m_UIMatch2Sprite->SetSize(300.0f,300.0f);
		m_UIMatch2Sprite->UIMatch(Anchor(Anchor::Get(Anchor::BottomLeft)),Anchor(Anchor::Get(Anchor::BottomLeft)));
		m_UIMatch2Sprite->SetLayer(FRONT_LAYER);
		m_UIMatch2Sprite->SetShading(SpriteVertex::ALPHA_BLEND);

		m_UIMatch3Sprite = new Sprite();
		m_UIMatch3Sprite->SetTexture(m_playerTexture);
		m_UIMatch3Sprite->SetSize(300.0f,300.0f);
		m_UIMatch3Sprite->UIMatch(Anchor(Anchor::Get(Anchor::TopRight)),Anchor(Anchor::Get(Anchor::TopRight)));
		m_UIMatch3Sprite->SetLayer(FRONT_LAYER);
		m_UIMatch3Sprite->SetShading(SpriteVertex::ALPHA_BLEND);

		m_UIMatch4Sprite = new Sprite();
		m_UIMatch4Sprite->SetTexture(m_playerTexture);
		m_UIMatch4Sprite->SetSize(300.0f,300.0f);
		m_UIMatch4Sprite->UIMatch(Anchor(Anchor::Get(Anchor::BottomRight)),Anchor(Anchor::Get(Anchor::BottomRight)));
		m_UIMatch4Sprite->SetLayer(FRONT_LAYER);
		m_UIMatch4Sprite->SetShading(SpriteVertex::ALPHA_BLEND);
	}

	int PlayGame::Update()
	{
		float lTimeStep = Global::pContext->pTimeService->Elapsed();
		float delta=lTimeStep;

		m_testSprite->Update(delta);

		m_pPGInputController->Update(lTimeStep);

		m_playerObject->SetLinearAcceleration(m_pPGInputController->GetMovementDirection());

		Object::UpdateAll(delta);
		ParticleEmitter::UpdateAll(delta);
		Particle::UpdateAll(delta);
		Camera2D::UpdateAll(delta);

		if(m_pPGInputController->IsUserRequestingExit())
		{
			return EVENT_PLAYGAME_PAUSE;
		}
		return EVENT_NOTHING;
	}

	void PlayGame::Render()
	{
		//Debug::Print(Global::pFont,"X = %lf  Y = %lf",m_playerObject->GetLinearAcceleration().x,m_playerObject->GetLinearAcceleration().y);

		m_background->Render();
		m_testSprite->Render();
		m_UIMatch1Sprite->Render();
		m_UIMatch2Sprite->Render();
		m_UIMatch3Sprite->Render();
		m_UIMatch4Sprite->Render();

		Batcher::EnableCamera();
		Object::RenderAll();
		Particle::RenderAll();
		Batcher::DisableCamera();

		m_pPGInputController->Render();
	}

	void PlayGame::Pause()
	{
		Global::pContext->pPhysicsService->callback=NULL;
		m_pPGInputController->Pause();
	}

	void PlayGame::Continue()
	{
		Global::pContext->pPhysicsService->callback=Physics_callback;
		m_pPGInputController->Continue();
	}

	PlayGame::~PlayGame()
	{
		delete m_pPGInputController;

		delete m_particleTex;
		delete m_pEmitter;

		delete m_enemyTexture;
		delete m_playerTexture;

		delete m_enemySprite;
		delete m_playerSprite;

		delete m_enemyObject;
		delete m_playerObject;

		delete m_backgroundTexture;
		delete m_background;

		delete Global::pActiveCamera;
		Global::pActiveCamera=NULL;

		//should be deleted in final version
		delete m_testSprite;
		delete m_testTexture;

		delete m_UIMatch1Sprite;
		delete m_UIMatch2Sprite;
		delete m_UIMatch3Sprite;
		delete m_UIMatch4Sprite;

		Particle::Clear();
	}

	void PlayGame::Physics_callback(void *param1, void *param2)
	{
		/* 		This  piece of code is here to show you how NOT to cast pointer in diamond-structure inheritance
		 * Object *pobj1p=(Object*)param1;
		 * Object *pobj2p=(Object*)param2;
 	 	 *
		 * PObject *pobj1=(PObject*)param1;
		 * PObject *pobj2=(PObject*)param2;
		 * Object *obj1=(Object*)pobj1->GetUserData();
		 * Object *obj2=(Object*)pobj2->GetUserData();
		 */

		//ALLWAYS use dynamic_cast properly in situations like this!!
		PObject *pobj1=(PObject*)param1;
		PObject *pobj2=(PObject*)param2;
		PhysicalObject *tobj1=(PhysicalObject*)pobj1->GetUserData();
		PhysicalObject *tobj2=(PhysicalObject*)pobj2->GetUserData();
		Object *obj1=dynamic_cast<Object*>(tobj1);
		Object *obj2=dynamic_cast<Object*>(tobj2);

		obj1->m_debugInCollision=true;
		obj2->m_debugInCollision=true;
	}
}
