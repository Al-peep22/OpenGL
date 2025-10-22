
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
        { { -0.5,  0.5,  0.0}, {1, 1, 0}, {0.0, 1.0}},
        { {  0.5,  0.5,  0.0}, {1, 0, 1}, {1.0, 1.0}},
        { {  0.5, -0.5,  0.0}, {1, 0, 1}, {1.0, 0.0}},
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
	auto vs = neu::Resources().Get<neu::Shader>("shaders/basics.vert", GL_VERTEX_SHADER);
	auto fs = neu::Resources().Get<neu::Shader>("shaders/basics.frag", GL_FRAGMENT_SHADER);

    // PROGRAM
    auto program = std::make_shared<neu::Program>();
    program->AttachShader(vs);
    program->AttachShader(fs);
    program->Link();
    program->Use();

    //TEXTURE
    neu::res_t<neu::Texture> texture = neu::Resources().Get<neu::Texture>("textures/squid.png");

    // TRANSFORM
	float rotation = 0;
	glm::vec3 eye = { 0, 0, 5 };

    // PROJECTION MATRIX
    float aspect = neu::GetEngine().GetRenderer().GetWidth() / (float)neu::GetEngine().GetRenderer().GetHeight();
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), aspect, 0.01f, 100.0f);
    program->SetUniform("u_projection", projection);

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

        // ESC CHECK
        if (neu::GetEngine().GetInput().GetKeyPressed(SDL_SCANCODE_ESCAPE)) quit = true;

		rotation += neu::GetEngine().GetTime().GetDeltaTime() * 90.0f;

        // MODEL MATRIX
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	    program->SetUniform("u_model", model);

        // VIEW MATRIX
        eye.x += neu::GetEngine().GetInput().GetMouseDelta().x * 0.01f;
        eye.z += neu::GetEngine().GetInput().GetMouseDelta().y * 0.01f;
        glm::mat4 view = glm::lookAt(eye, eye + glm::vec3{ 0, 0, -1 }, glm::vec3{ 0,  1, 0 });
	    program->SetUniform("u_view", view);

		// UPDATE UNIFORMS
		//glUniform1f(uniform, neu::GetEngine().GetTime().GetTime());
		//program->SetUniform("u_time", neu::GetEngine().GetTime().GetTime());

        /*
        -----------------------------------------------------------------------------------------
        -- I can't get my object to show up - Once model was added it dissapeared
        -- Mine is still a triangle
		-- and the colors in the background arent should - how do I make it transparent or tinted?
        -----------------------------------------------------------------------------------------
        */

        // DRAW
        neu::GetEngine().GetRenderer().Clear();

		glBindVertexArray(vao);
		//glDrawArrays(GL_TRIANGLES, 0, (GLsizei)points.size()); //-------------------------------------

		glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);

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

        // PRESENT
        neu::GetEngine().GetRenderer().Present();
        
    }

    // SHUTDOWN
    neu::GetEngine().Shutdown();

    return 0;
}
