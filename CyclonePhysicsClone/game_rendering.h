#ifndef GAME_RENDERING_U_
#define GAME_RENDERING_U_


#include "global.h"
#include "renderer_manager.h"
#include "game.h"


namespace GameRendering
{
    void render(Model* model, Pipeline &p, glm::vec3 position, glm::mat4 orientation, glm::vec3 scale, Renderer* r)
    {
        p.pushMatrix();
            p.translate(position);
            p.addMatrix(orientation);
            p.scale(scale);
            r->setUniLocs(p);
            model->render();
        p.popMatrix();
    }

    void render(GameState* gameState)
    {
        gameState->mainCamera.getPipeline().setMatrixMode(MODEL_MATRIX);
        glBindFramebuffer(GL_FRAMEBUFFER, RENDER_TO_SCREEN);
        glViewport(0, 0, utl::SCREEN_WIDTH, utl::SCREEN_HEIGHT);

        gameState->mainCamera.getPipeline().setMatrixMode(VIEW_MATRIX);
        gameState->mainCamera.getPipeline().loadIdentity();
        gameState->mainCamera.getPipeline().translate(0.0f, 0.0f, 5.0f);


        gameState->mainCamera.getPipeline().setMatrixMode(MODEL_MATRIX);

        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glDepthMask(true);

        glCullFace(GL_BACK);
        glClear(GL_DEPTH_BUFFER_BIT);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        //	mapView.render(mainCamera.getPipeline());

        Entity* entities = gameState->entities;


        Renderer* p_renderer = &global.rendererMgr->r_fullColor;
        p_renderer->enableShader();
        p_renderer->setData(R_FULL_COLOR::u_color, COLOR_RED);

        for (int i = 1; i < gameState->numEntities; i++)
        {
            if (entities[i].flags & EntityFlag_Static)
            {
                p_renderer->setData(R_FULL_COLOR::u_color, COLOR_GRAY);
            }
            else if (entities[i].entityType == EntityType::Box)
            {
                if (entities[i].isAwake)
                {
                    p_renderer->setData(R_FULL_COLOR::u_color, COLOR_BLUE);
                }
                else
                {
                    p_renderer->setData(R_FULL_COLOR::u_color, COLOR_LIGHT_BLUE_2);
                }
            }

            gameState->entities[i].renderCore(gameState->mainCamera.getPipeline(), p_renderer);
        }
        
        for (int i = 0; i < gameState->numContactPoints; i++)
        {
            p_renderer->setData(R_FULL_COLOR::u_color, COLOR_RED);

            render(global.modelMgr->get(ModelEnum::centeredQuad), gameState->mainCamera.getPipeline(), 
                gameState->contactPoints[i], 
                glm::mat4(1.0), 
                glm::vec3(0.5), 
                p_renderer);
        }




        // Rendering wireframes
        p_renderer = &global.rendererMgr->r_fullVertexColor;
        p_renderer->enableShader();

        gameState->entities[0].renderCore(gameState->mainCamera.getPipeline(), p_renderer);

        for (int i = 1; i < gameState->numEntities; i++)
        {
            render(global.modelMgr->get(ModelEnum::xyzAxis), gameState->mainCamera.getPipeline(),
                gameState->entities[i].position,
                gameState->entities[i].orientationMat,
                gameState->entities[i].scale,
                p_renderer);
        }
        p_renderer->disableShader();



        /*
        if (currentRay != NULL && currentRay->canRender())
        {
        p_renderer->setData(R_FULL_COLOR::u_color, COLOR_RED);
        currentRay->renderGroup(mainCamera.getPipeline(), p_renderer);
        }

        if (sourcePoint.canRender())
        {
        p_renderer->setData(R_FULL_COLOR::u_color, COLOR_TEAL);
        sourcePoint.renderGroup(mainCamera.getPipeline(), p_renderer);
        }

        if (endPoint.canRender())
        {
        p_renderer->setData(R_FULL_COLOR::u_color, COLOR_GREEN);
        endPoint.renderGroup(mainCamera.getPipeline(), p_renderer);
        }
        */
        p_renderer->disableShader();

    }
};
#endif // ! GAME_RENDERING_U_