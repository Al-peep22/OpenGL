#include <GUI/Editor.h>

int main(int argc, char* argv[]) {
    // SET DIRECTORY
    neu::file::SetCurrentDirectory("Assets");
    LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

    // INITIALIZE ENGINE
    LOG_INFO("initialize engine...");
    neu::GetEngine().Initialize();
    
    SDL_Event e;
    bool quit = false;

	auto renderTexture = std::make_shared<neu::RenderTexture>();
	renderTexture->Create(512, 512);
	neu::Resources().AddResource("renderTexture", renderTexture);


    // INITIALIZE SCENE
    auto scene = std::make_unique<neu::Scene>();
    scene->Load("scenes/scene03.json");
    scene->Start();

    auto editor = std::make_unique<neu::Editor>();
    
    // MAIN LOOP
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
            ImGui_ImplSDL3_ProcessEvent(&e);
        }

        // UPDATE ENGINE
        neu::GetEngine().Update(); 
        float dt = neu::GetEngine().GetTime().GetDeltaTime();

        // ESC CHECK
        if (neu::GetEngine().GetInput().GetKeyPressed(SDL_SCANCODE_ESCAPE)) quit = true;
                
        // UPDATE SCENE
        scene->Update(dt);

        // UPDATE EDITOR
        editor->Begin();
        editor->UpdateGui(*scene);
        
        // ROTATION CHECK
        /*
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_E)) transform.rotation.y += 90.0f * dt;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_Q)) transform.rotation.y -= 90.0f * dt;
        */

        // CAMERA CONTROLS
        /*
        float speed = 10.0f;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_A)) camera.position.x -= speed * dt;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_A)) eye.x -= speed * dt;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_D)) camera.position.x += speed * dt;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_D)) eye.x += speed * dt;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_W)) camera.position.y += speed * dt;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_W)) eye.y += speed * dt;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_S)) camera.position.y -= speed * dt;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_S)) eye.y -= speed * dt;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_I)) eye.z -= speed * dt;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_O)) eye.z += speed * dt;
        */
        
        // DRAW
        neu::GetEngine().GetRenderer().Clear();

        // PRIMATIVE TYPES
        /*
        * GL_TRIANGLES
        * GL_TRIANGLE_STRIP
        * GL_TRIANGLE_FAN
        * GL_LINES
        * GL_LINE_STRIP
        * GL_LINE_LOOP
        * GL_POINTS
        * GL_QUADS
        * GL_QUAD_STRIP
        * GL_POLYGON
        * etc.
        */
        
        // DRAW SCENE
        scene->Draw(neu::GetEngine().GetRenderer());

        // DRAW IMGUI
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // PRESENT
        neu::GetEngine().GetRenderer().Present();
        
    }

    // SHUTDOWN
    neu::GetEngine().Shutdown();

    return 0;
}
