
#ifndef GAME_H_
#define GAME_H_

#include "model_manager.h"
#include "global.h"
#include "platform.h"
#include "utility_math.h"
#include "pipeline.h"
#include "camera.h"
#include "physics.h"
#include "entity.h"

struct GameState
{
    int worldWidth;
    int worldHeight;

    Entity* entities;
    int numEntities;

    int numContactPoints;
    glm::vec3* contactPoints;


    Camera mainCamera;

    Entity* draggedEntity;

    float angle;
    int boxIndex;
};


namespace GameCode
{
    // box2d has it at -10
    glm::vec3 GRAVITY = glm::vec3(0, -10, 0);
    bool appliedForce = false;

    void addRandomBall(GameState* gameState)
    {
        Entity entity;

        int x = utl::randFloat(0, 30);
        int y = utl::randFloat(0, 30);

        int size = utl::randFloat(1, 2);

        entity.entityType = EntityType::Ball;
        entity.flags |= EntityFlag_Collides;
        entity.position = glm::vec3(x, y, 0);
        entity.scale = glm::vec3(size, size, size);
        entity.setModel(global.modelMgr->get(ModelEnum::circle));
        gameState->entities[gameState->numEntities++] = entity;
    }

    void addRandomBox(GameState* gameState)
    {
        Entity entity;

        int x = utl::randFloat(0, 30);
        int y = utl::randFloat(0, 30);

        int size = utl::randFloat(1, 2);

        entity.entityType = EntityType::Ball;
        entity.flags |= EntityFlag_Collides;
        entity.position = glm::vec3(x, y, 0);
        entity.scale = glm::vec3(size, size, size);
        entity.setModel(global.modelMgr->get(ModelEnum::centeredQuad));
        gameState->entities[gameState->numEntities++] = entity;
    }


    void ProcessInputRaycast(GameState* gameState, glm::vec3 raycastDirection)
    {
        for (int i = 0; i < gameState->numEntities; i++)
        {
            Entity* entity = &gameState->entities[i];
            if (!entity->flags & EntityFlag_Static)
            {
                continue;
            }
            
            if (entity->physBody.type == Physics::PhysBodyType::PB_OBB)
            {
                
                if (Physics::testPointInsideOBB2D(raycastDirection, entity->physBody.obb, entity->orientationMat, entity->position))
                {
                    
                    gameState->draggedEntity = entity;
                    
                    /*
                    cout << "selecting entity " << entity->id << endl;
                    gameState->draggedEntity = entity;


//                    glm::vec3 vecToForce = glm::vec3(raycastDirection.x, raycastDirection.y, 0) - entity->position;
                    glm::vec3 vecToForce = glm::vec3(0, -5, 0);

                    glm::vec3 zAxis = glm::vec3(0, 0, 1);
                    glm::vec3 force = 1000.0f * glm::cross(vecToForce, zAxis);

                //    utl::debug("        vecToForce ", vecToForce);
                //    utl::debug("        force is ", force);

                    
                    appliedForce = true;
            //        entity->addForce(force);
                    entity->addTorqueFromForce(force, vecToForce);
                    */


                }                
            }
        }
    }



    void demo1Init(GameState* gameState)
    {
        gameState->worldWidth = 50;
        gameState->worldHeight = 50;
        gameState->numEntities = 0;
        gameState->entities = new Entity[4096];

        gameState->numContactPoints = 0;
        gameState->contactPoints = new glm::vec3[1024];

        gameState->angle = 0;
        gameState->draggedEntity = NULL;

        float scale = 100.0;
        int index = gameState->numEntities++;
        Entity* entity = &gameState->entities[index];
        entity->id = gameState->numEntities;
        entity->entityType = EntityType::XYZAxis;
        entity->scale = glm::vec3(scale, scale, scale);
        entity->flags = EntityFlag_Static;
        entity->setModel(global.modelMgr->get(ModelEnum::xyzAxis));

        if (global.modelMgr->get(ModelEnum::xyzAxis) == NULL)
        {
            cout << "XYZ is null" << endl;
        }
        else
        {
            cout << "XYZ is not null" << endl;
        }

        cout << "name is " << gameState->entities[0].entityType << endl;
        if (gameState->entities[0].m_model != NULL)
        {
            cout << "   Can render0" << endl;
        }
        else
        {
            cout << "   Cant render0" << endl;
        }

        if (gameState->entities[0].m_model != NULL)
        {
            cout << "   Can render01" << endl;
        }
        else
        {
            cout << "   Cant render01" << endl;
        }

        //        gameState->entities[gameState->numEntities++] = entity;


        
        // the box
        int x = 0;
        int y = 10;
        int size = 1;
        int w = size * 2;
        int h = size * 2;
        int d = 1;
        index = gameState->numEntities++;
        entity = &gameState->entities[index];
        entity->id = index;
        entity->entityType = EntityType::Box;
        entity->flags = EntityFlag_Collides;
        entity->SetAwake(true);
        entity->mass = 1;
        entity->invMass = 1 / (float)entity->mass;

        entity->position = glm::vec3(x, y, 0);
    //    glm::mat4 om = glm::rotate(30.0f, glm::vec3(0, 0, 1));
        glm::mat4 om = glm::rotate(0.0f, glm::vec3(0, 0, 1));

        /*
        glm::vec3 xAxis = glm::vec3(0, 1, 0);
        xAxis = glm::normalize(xAxis);
        glm::vec3 yAxis = glm::vec3(-1, 0, 0);
        yAxis = glm::normalize(yAxis);
        glm::vec3 zAxis = glm::vec3(0, 0, 1);

        // world to contact local matrix
        glm::mat4 om = utl::axes2GLMMat(xAxis, yAxis, zAxis);
        */




        entity->orientation = glm::toQuat(om);
        entity->SyncOrientationMat();

        entity->scale = glm::vec3(w, h, d);

        entity->velocityDamping = 0.95f;
        entity->angularDamping = 0.80f;

        entity->inertiaTensor = Physics::GetBoxInertiaTensor(entity->mass, w * 2, h * 2, d * 2);
        entity->transformInertiaTensor();
        
        
        
        entity->setModel(global.modelMgr->get(ModelEnum::unitCenteredQuad));
        
        entity->physBody.type = Physics::PhysBodyType::PB_OBB;
        entity->physBody.obb.center = glm::vec3(0, 0, 0);
        entity->physBody.obb.axes[0] = glm::vec3(1, 0, 0);
        entity->physBody.obb.axes[1] = glm::vec3(0, 1, 0);
        entity->physBody.obb.axes[2] = glm::vec3(0, 0, 1);
        entity->physBody.obb.halfEdges = glm::vec3(w, h, d);
        




        gameState->boxIndex = index;

        // the floor 
        index = gameState->numEntities++;
        entity = &gameState->entities[index];
        entity->id = index;
        entity->entityType = EntityType::Floor;
        entity->position = glm::vec3(0, 0, 0);
        entity->flags = EntityFlag_Collides | EntityFlag_Static;
        entity->scale = glm::vec3(gameState->worldWidth, 0.2, 1);
        entity->setModel(global.modelMgr->get(ModelEnum::unitCenteredQuad));

        entity->physBody.type = Physics::PhysBodyType::PB_PLANE;
        entity->physBody.plane.normal = glm::vec3(0, 1, 0);
        entity->physBody.plane.offset = 0;       // dot(glm::vec3(0, 1, 0),  glm::vec3(0, 0, 0));


        // the wall
        index = gameState->numEntities++;
        entity = &gameState->entities[index];
        entity->id = index;
        entity->entityType = EntityType::Wall;
        entity->position = glm::vec3(gameState->worldWidth / 2, gameState->worldHeight / 2, 0);
        entity->flags = EntityFlag_Collides | EntityFlag_Static;
        entity->scale = glm::vec3(2, gameState->worldHeight, 1);
        entity->setModel(global.modelMgr->get(ModelEnum::unitCenteredQuad));


        index = gameState->numEntities++;
        entity = &gameState->entities[index];
        entity->id = index;
        entity->entityType = EntityType::Wall;
        entity->position = glm::vec3(-gameState->worldWidth / 2, gameState->worldHeight / 2, 0);
        entity->flags = EntityFlag_Collides | EntityFlag_Static;
        entity->scale = glm::vec3(2, gameState->worldHeight, 1);
        entity->setModel(global.modelMgr->get(ModelEnum::unitCenteredQuad));
    }


    void demo2Init(GameState* gameState)
    {
        gameState->worldWidth = 50;
        gameState->worldHeight = 50;
        gameState->numEntities = 0;
        gameState->entities = new Entity[4096];

        gameState->numContactPoints = 0;
        gameState->contactPoints = new glm::vec3[1024];

        float scale = 100.0;
        Entity entity;
        entity.entityType = EntityType::XYZAxis;
        entity.scale = glm::vec3(scale, scale, scale);
        entity.flags = EntityFlag_Static;
        entity.setModel(global.modelMgr->get(ModelEnum::xyzAxis));
        gameState->entities[gameState->numEntities++] = entity;

        for (int i = 0; i < 50; i++)
        {
            addRandomBall(gameState);
        }

        for (int i = 0; i < 50; i++)
        {
            addRandomBox(gameState);
        }

        entity.entityType = EntityType::Floor;
        entity.position = glm::vec3(0, 0, 0);
        entity.flags |= EntityFlag_Collides | EntityFlag_Static;
        entity.scale = glm::vec3(gameState->worldWidth, 2, 1);
        entity.setModel(global.modelMgr->get(ModelEnum::centeredQuad));
        gameState->entities[gameState->numEntities++] = entity;

        entity.entityType = EntityType::Wall;
        entity.position = glm::vec3(gameState->worldWidth / 2, gameState->worldHeight / 2, 0);
        entity.flags = EntityFlag_Collides | EntityFlag_Static;
        entity.scale = glm::vec3(2, gameState->worldHeight, 1);
        entity.setModel(global.modelMgr->get(ModelEnum::centeredQuad));
        gameState->entities[gameState->numEntities++] = entity;

        entity.entityType = EntityType::Wall;
        entity.position = glm::vec3(-gameState->worldWidth / 2, gameState->worldHeight / 2, 0);
        entity.flags = EntityFlag_Collides | EntityFlag_Static;
        entity.scale = glm::vec3(2, gameState->worldHeight, 1);
        entity.setModel(global.modelMgr->get(ModelEnum::centeredQuad));
        gameState->entities[gameState->numEntities++] = entity;
    }


    glm::vec3 screenToWorldPoint(GameState* gameState, glm::vec2 screenPoint)
    {
        glm::vec4 viewPort = glm::vec4(0, 0, utl::SCREEN_WIDTH, utl::SCREEN_HEIGHT);
        glm::vec3 temp = glm::vec3(screenPoint.x, screenPoint.y, 0);

        glm::vec3 worldPoint = glm::unProject(temp, (gameState->mainCamera.getPipeline().getModelViewMatrix()), gameState->mainCamera.getPipeline().getProjectionMatrix(), viewPort);
        return worldPoint;
    }


    glm::vec3 worldToScreen(GameState* gameState, glm::vec3 pos)
    {
        glm::vec4 viewPort = glm::vec4(0, 0, utl::SCREEN_WIDTH, utl::SCREEN_HEIGHT);
        //	glm::vec3 screenPos = glm::project(pos, glm::inverse(m_pipeline.getModelViewMatrix()), m_pipeline.getProjectionMatrix(), viewPort);
        glm::vec3 screenPos = glm::project(pos, gameState->mainCamera.getPipeline().getModelViewMatrix(), gameState->mainCamera.getPipeline().getProjectionMatrix(), viewPort);
        return screenPos;
    }



    void init(GameState* gameState)
    {
        demo1Init(gameState);        
    }

    float angle = 0;




    void PreIntegrate(Entity* entity, float dt_s)
    {
        std::cout << "awake " << entity->isAwake << std::endl;

        if (!entity->isAwake)
            return;


        // velocity has damping
        // you can consider removing the damping term altogether if you have lots of entities

    //    entity->lastAcceleration = entity->acceleration;
        entity->acceleration = entity->forceAccum / entity->mass;
        entity->velocity += entity->acceleration * dt_s;
        if (glm::length(entity->velocity) < 0.01)
        {
            entity->velocity = glm::vec3(0.0);
        }

        entity->angularAcceleration = entity->inverseInertiaTensor * entity->torqueAccum;
        entity->angularVelocity += entity->angularAcceleration * dt_s;
        entity->angularVelocity = entity->angularDamping * entity->angularVelocity;
        if (glm::length(entity->angularVelocity) < 0.01)
        {
            entity->angularVelocity = glm::vec3(0.0);
        }
    }




    void integrate(Entity* entity, float dt_s)
    {
        if (!entity->isAwake)
            return;


        if (glm::length(entity->velocity) < 0.01)
        {
            entity->velocity = glm::vec3(0.0);
        }

        entity->position += dt_s * entity->velocity;



        if (glm::length(entity->angularVelocity) < 0.01)
        {
            entity->angularVelocity = glm::vec3(0.0);
        }

        /*
        if (entity->velocity != glm::vec3(0.0))
        {
            utl::debug(">>>> resolving position ", entity->position);
            utl::debug("        velocity ", entity->velocity);
            utl::debug("        angularVelocity ", entity->angularVelocity);
            utl::debug("        orientation ", entity->orientation);
            utl::debug("        orientationMat ", entity->orientationMat);
        }
        */


        float angularMag = glm::length(entity->angularVelocity);
        if (angularMag != 0)
        {
            // https://math.stackexchange.com/questions/22437/combining-two-3d-rotations
            entity->updateOrientation(entity->angularVelocity, dt_s);
            entity->transformInertiaTensor();
        }


        // update matrices with the new position and orientation
        entity->forceAccum = glm::vec3(0, 0, 0);
        entity->torqueAccum = glm::vec3(0, 0, 0);

        std::cout << "      motion " << entity->motion << std::endl;


        // recency-weighted average (RWA)
        float currentMotion = glm::dot(entity->velocity, entity->velocity) + glm::dot(entity->angularVelocity, entity->angularVelocity);

        // this makes RWA dependent on the duration of the frame. 
        float bias = pow(0.5, dt_s);
        entity->motion = bias * entity->motion + (1 - bias) * currentMotion;



        if (entity->motion < sleepEpislon)
        {
            entity->SetAwake(false);
        }
        else if (entity->motion > 10 * sleepEpislon)
        {
            entity->motion = 10 * sleepEpislon;
        }

        std::cout << "      motion " << entity->motion << std::endl;

    }


#if 0
    // Game Physics Enginer Development 
    // although the proper formula is x' = x + vt + 0.5 * a * t^2
    // 3.31 the acceleration is often neglecgible. so we just ignore the acceleration term
    // so we just ignore the acceleration term
    void integrate(Entity* entity, float dt_s)
    {        
        // velocity has damping
        // you can consider removing the damping term altogether if you have lots of entities
        
    //    entity->lastAcceleration = entity->acceleration;
        entity->acceleration = entity->forceAccum / entity->mass;

        entity->velocity += entity->acceleration * dt_s;

       // utl::debug("        acceleration ", entity->acceleration * dt_s);
       // utl::debug("        velocity ", entity->velocity);


        // damping 
        // entity->velocity = entity->velocityDamping * entity->velocity;
        
       // utl::debug("        after damping velocity ", entity->velocity);


        if ( glm::length(entity->velocity) < 0.01)
        {
            entity->velocity = glm::vec3(0.0);
        }

        /*
        if (entity->velocity != glm::vec3(0.0))
        {
            utl::debug(">>>> resolving position ", entity->position);
            utl::debug("        velocity ", entity->velocity);
            utl::debug("        orientation ", entity->orientation);
            utl::debug("        orientationMat ", entity->orientationMat);
        }
        */

        entity->position += dt_s * entity->velocity;

        
        entity->angularAcceleration = entity->inverseInertiaTensor * entity->torqueAccum;
        entity->angularVelocity += entity->angularAcceleration * dt_s;

        // damping
        entity->angularVelocity = entity->angularDamping * entity->angularVelocity;

        if (glm::length(entity->angularVelocity) < 0.01)
        {
            entity->angularVelocity = glm::vec3(0.0);
        }

        float angularMag = glm::length(entity->angularVelocity);
        if (angularMag != 0)
        {
            // https://math.stackexchange.com/questions/22437/combining-two-3d-rotations
            entity->updateOrientation(entity->angularVelocity, dt_s);
            entity->transformInertiaTensor();
        }
        

        // update matrices with the new position and orientation
        entity->forceAccum = glm::vec3(0, 0, 0);
        entity->torqueAccum = glm::vec3(0, 0, 0);        

        entity->normalizeQuat();
    }
#endif

    void CopyContactPoints(GameState* gameState, Physics::CollisionData* contact)
    {
        for (int i = 0; i < contact->numContactPoints; i++)
        {
            gameState->contactPoints[gameState->numContactPoints++] = contact->contactPoints[i].position;
        }
    }
    

    void tick(GameInput gameInput, GameState* gameState)
    {
        gameState->numContactPoints = 0;

        // cout << "gameState->numEntities " << gameState->numEntities << endl;
        for (int i = 0; i < gameState->numEntities; i++)
        {
            if (!(gameState->entities[i].flags & EntityFlag_Static) && &gameState->entities[i] != gameState->draggedEntity)
            {
                Entity* entity = &gameState->entities[i];
                {
                    gameState->entities[i].addForce(gameState->entities[i].mass * GRAVITY, false);


                    // adding drag
                    glm::vec3 drag = gameState->entities[i].velocity;


                    if (glm::length(drag) != 0)
                    {
                        float dragCoeff = glm::length(drag);
                        dragCoeff = 0.1 * dragCoeff;

                        drag = glm::normalize(drag);
                        drag *= -dragCoeff;

                        // gameState->entities[i].forceAccum += drag;
                        gameState->entities[i].addForce(drag, false);



                    //    utl::debug("forceAccum ", gameState->entities[i].forceAccum);
                    }
                }
            }
        }
        

        for (int i = 0; i < gameState->numEntities; i++)
        {
            if (!(gameState->entities[i].flags & EntityFlag_Static))
            {
                PreIntegrate(&gameState->entities[i], gameInput.dt_s);
            }
        }


        for (int i = 0; i < gameState->numEntities; i++)
        {
            if (!gameState->entities[i].flags & EntityFlag_Static)
            {
                continue;
            }

            for (int j = i + 1; j < gameState->numEntities; j++)
            {                
                Physics::CollisionData contact;

                /*
                if (Physics::TestContactInfo(gameState->entities[i], gameState->entities[j], contact))
                {
                    CopyContactPoints(gameState, &contact);

                    cout << "Resolving contact" << endl;
                    Physics::Resolve(contact, &gameState->entities[i], &gameState->entities[j]);
                }
                */
                
                Physics::GenerateContactInfo(gameState->entities[i], gameState->entities[j], contact);
                if (contact.numContactPoints > 0)
                {                    
                    CopyContactPoints(gameState, &contact);

                    cout << "\n\nResolving contact" << endl;

                    if (gameState->entities[j].flags & EntityFlag_Static)
                    {
                        Physics::Resolve(contact, &gameState->entities[i], NULL, gameInput.dt_s);
                    }
                    else
                    {
                        Physics::Resolve(contact, &gameState->entities[i], &gameState->entities[j], gameInput.dt_s);
                    }
                }
                
                
            }
        }


        for (int i = 0; i < gameState->numEntities; i++)
        {
            if (!(gameState->entities[i].flags & EntityFlag_Static))
            {
                integrate(&gameState->entities[i], gameInput.dt_s);
            }
        }
    }
};


#endif