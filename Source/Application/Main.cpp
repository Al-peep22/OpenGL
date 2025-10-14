
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
        { {  0.0,  0.5,  0.0}, {0, 1, 1}, {0.5, 1.0}},
        { { -0.5, -0.5, -0.5}, {1, 1, 0}, {0.0, 0.0}},
        { {  0.5, -0.5,  0.5}, {1, 0, 1}, {1.0, 0.0}},
    };

	std::vector<GLuint> indices = { 0, 1, 2 };

    // vertex buffer
    GLuint vbo;
    glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    // index buffer
	GLuint ibo;
	glGenBuffers(1, &ibo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // Vertex Array
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

    /*
    GLuint vbo[3];
    glGenBuffers(3, vbo);

    // Vertex Buffer (position)
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(neu::vec3) * points.size(), points.data(), GL_STATIC_DRAW);

    // Vertex Buffer (color)
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(neu::vec3) * colors.size(), colors.data(), GL_STATIC_DRAW);

    // Vertex Array
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Position
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // Color
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // Color
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    */

    // Vertex Shader
    std::string vs_source;
    neu::file::ReadTextFile("Shaders/basics.vert", vs_source);
    const char* vs_cstr = vs_source.c_str();

    GLuint vs;
    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vs_cstr, NULL);
    glCompileShader(vs);
    int success;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::string infoLog(512, '\0');  // pre-allocate space
        GLsizei length;
        glGetShaderInfoLog(vs, (GLsizei)infoLog.size(), &length, &infoLog[0]);
        infoLog.resize(length);

        LOG_WARNING("Shader compilation failed: {}", infoLog);
    }

    // Fragment shader
    std::string fs_source;
    neu::file::ReadTextFile("Shaders/basics.frag", fs_source);
    const char* fs_cstr = fs_source.c_str();

    GLuint fs;
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_cstr, NULL);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::string infoLog(512, '\0');  // pre-allocate space
        GLsizei length;
        glGetShaderInfoLog(fs, (GLsizei)infoLog.size(), &length, &infoLog[0]);
        infoLog.resize(length);

        LOG_WARNING("Shader compilation failed: {}", infoLog);
    }

    // Program
    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        std::string infoLog(512, '\0');  // pre-allocate space
        GLsizei length;
        glGetProgramInfoLog(program, (GLsizei)infoLog.size(), &length, &infoLog[0]);
        infoLog.resize(length);

        LOG_WARNING("Program link failed: {}", infoLog);
    }
    glUseProgram(program);

    // TEXTURE
    neu::res_t<neu::Texture> texture = neu::Resources().Get<neu::Texture>("Textures/squid.png");

    // Uniform
    GLint uniform = glGetUniformLocation(program, "u_time");
    //ASSERT_MSG(uniform != -1, "Could not find uniform u_time.");

	GLint tex_uniform = glGetUniformLocation(program, "u_texture");
    glUniform1i(tex_uniform, 0);

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

		// UPDATE UNIFORMS
		glUniform1f(uniform, neu::GetEngine().GetTime().GetTime());

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


        //// GET TIME
        //float time = neu::GetEngine().GetTime().GetTime();
        //
        //// GET MOUSE POSITION
        //neu::vec2 mousePos = neu::GetEngine().GetInput().GetMousePosition();
        //
        //// Normalize mouse position
        //float tx = (mousePos.x / 800.0f) * 2.0f - 1.0f;
        //float ty = -((mousePos.y / 600.0f) * 2.0f - 1.0f); // flip Y
        //
        //// Calculate rotation and scale
        //float angle = time * 50.0f; // degrees per second
        //float scale = std::sin(time) * 0.5f + 1.0f; // 0.5 to 1.5
        //
        //// Apply transformations
        //glPushMatrix(); // Save current transform
        //
        //glTranslatef(tx, ty, 0.0f);         // mouse translation
        //glRotatef(angle, 0.0f, 1.0f, 0.0f); // rotation around y-axis
        //glScalef(scale, scale, scale);     // scaling

        neu::GetEngine().GetRenderer().Present();
        
    }

    neu::GetEngine().Shutdown();

    return 0;
}
