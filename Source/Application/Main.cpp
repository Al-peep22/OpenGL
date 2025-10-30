
int main(int argc, char* argv[]) {
    neu::file::SetCurrentDirectory("Assets");
    LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

    // initialize engine
    LOG_INFO("initialize engine...");
    neu::GetEngine().Initialize();

    // initialize scene


	// OPEN_GL SETUP
    std::vector<neu::vec3> points = {
		//  x ,   y ,   z ,
        {  0.0,  0.5,  0.0 },
        { -0.5, -0.5, -0.5 },
        {  0.5, -0.5,  0.5 },
    };
    std::vector<neu::vec3> colors = {
        { 0.0, 1.0, 1.0 },  // teal
        { 1.0, 1.0, 0.0 },  // yellow
		{ 1.0, 0.0, 1.0 },  // pink
    };
	std::vector<neu::vec2> texcoord = {
		{0.5, 1.0},
		{0.0, 0.0},
		{1.0, 0.0}
	};

    struct Vertex {
        neu::vec3 position;
        neu::vec3 color;
        neu::vec2 texcoord;
    };

    std::vector<Vertex> vertices{ 
        { { -0.5, -0.5,  0.0}, {0, 1, 1}, {0.0, 0.0}},
        { { -0.5,  0.5,  0.5}, {1, 1, 0}, {0.0, 1.0}},
        { {  0.5,  0.5, -0.5}, {1, 0, 1}, {1.0, 1.0}},
        { {  0.5, -0.5,  0.5}, {1, 0, 1}, {1.0, 0.0}},
    };

	std::vector<GLuint> indices = { 0, 1, 2, 2, 3, 0 };

	// VERTEX BUFFER
    GLuint vbo;
    glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	// INDEX BUFFER
	GLuint ibo;
	glGenBuffers(1, &ibo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

	// VERTEX ARRAY
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, color));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texcoord));

    // SHADERS
	/*auto vs = neu::Resources().Get<neu::Shader>("shaders/basics_lit.vert", GL_VERTEX_SHADER);
	auto fs = neu::Resources().Get<neu::Shader>("shaders/basics_lit.frag", GL_FRAGMENT_SHADER);*/

    // PROGRAM
    auto program = neu::Resources().Get<neu::Program>("shaders/basics_lit.prog");
    program->Use();

    //TEXTURE
    neu::res_t<neu::Texture> texture = neu::Resources().Get<neu::Texture>("textures/spot_diffuse.png");

    //set light uniforms
    program->SetUniform("u_light.color", glm::vec3(0.5f));
    program->SetUniform("u_ambient_light", glm::vec3(0.2f));
    neu::Transform lightTransform{ {2,4,3} };

    // TRANSFORM
	float rotation = 0;
	glm::vec3 eye = { 0, 0, 2 };

    neu::Transform transform{ { 0, 0, 0 } };
    neu::Transform camera{ { 0, 0, 2 } };

    // PROJECTION MATRIX
    float aspect = neu::GetEngine().GetRenderer().GetWidth() / (float)neu::GetEngine().GetRenderer().GetHeight();
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), aspect, 0.01f, 100.0f);
    program->SetUniform("u_projection", projection);

	// MODEL LOAD (Vertex buffer code replacement? ----------------------)
    auto model3d = std::make_shared<neu::Model>();
    model3d->Load("models/spot.obj");


    SDL_Event e;
    bool quit = false;
    
    // MAIN LOOP
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }

        // UPDATE ENGINE
        neu::GetEngine().Update();
 
        float dt = neu::GetEngine().GetTime().GetDeltaTime();

        // ESC CHECK
        if (neu::GetEngine().GetInput().GetKeyPressed(SDL_SCANCODE_ESCAPE)) quit = true;

        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_E)) transform.rotation.y += 90.0f * dt;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_Q)) transform.rotation.y -= 90.0f * dt;
        program->SetUniform("u_model", transform.GetMatrix());

        // MODEL MATRIX
     //   glm::mat4 model = glm::mat4(1.0f);
     //   model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
     //   model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
     //   model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	    //program->SetUniform("u_model", model);

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
        // fill in the rest of the controls (WS and QE)
        /*glm::mat4 view = glm::lookAt(camera.position, camera.position + glm::vec3{ 0, 0, -1 }, glm::vec3{ 0, 1, 0 });
        program->SetUniform("u_view", view);*/

        // VIEW MATRIX
        //eye.x += neu::GetEngine().GetInput().GetMouseDelta().x * 0.01f;
        //eye.z += neu::GetEngine().GetInput().GetMouseDelta().y * 0.01f;
        glm::mat4 view = glm::lookAt(eye, eye + glm::vec3{ 0, 0, -1 }, glm::vec3{ 0,  1, 0 });
	    program->SetUniform("u_view", view);

        program->SetUniform("u_light.position", (glm::vec3)(view* glm::vec4(lightTransform.position,1)));


		// UPDATE UNIFORMS
		//glUniform1f(uniform, neu::GetEngine().GetTime().GetTime());
		//program->SetUniform("u_time", neu::GetEngine().GetTime().GetTime());

        // DRAW
        neu::GetEngine().GetRenderer().Clear();

        // start new ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        // set ImGui
        ImGui::Begin("Editor");
        ImGui::Text("Hello World");
        ImGui::Text("Press 'Esc' to quit.");
        ImGui::End();

		//glBindVertexArray(vao);
		//glDrawArrays(GL_TRIANGLES, 0, (GLsizei)points.size()); //-------------------------------------

		//glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);

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


        //Model Stuff--------------------------------
        model3d->Draw(GL_TRIANGLES);
        
        // draw ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // PRESENT
        neu::GetEngine().GetRenderer().Present();
        
    }

    // SHUTDOWN
    neu::GetEngine().Shutdown();

    return 0;
}
