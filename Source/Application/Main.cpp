
int main(int argc, char* argv[]) {
    neu::file::SetCurrentDirectory("Assets");
    LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

    // initialize engine
    LOG_INFO("initialize engine...");
    neu::GetEngine().Initialize();

    // initialize scene


	// OPEN_GL SETUP
    std::vector<neu::vec3> points = {
        {0, 0.5, 0},
        {-0.5, -0.5, -0.5},
        {0.5, -0.5, 0.5}
    };
    std::vector<neu::vec3> colors = {
        {0, 1, 1},  // teal
        {1, 1, 0},  // yellow
		{1, 0, 1}   // pink
    };

    GLuint vbo[2];
    glGenBuffers(2, vbo);

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

    // Vertex Shader
    std::string vs_source;
    neu::file::ReadTextFile("Shaders/basic.vert", vs_source);
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
    neu::file::ReadTextFile("Shaders/basic.frag", fs_source);
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
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        std::string infoLog(512, '\0');  // pre-allocate space
        GLsizei length;
        glGetProgramInfoLog(shaderProgram, (GLsizei)infoLog.size(), &length, &infoLog[0]);
        infoLog.resize(length);

        LOG_WARNING("Program link failed: {}", infoLog);
    }
    glUseProgram(shaderProgram);

    // Uniform
    GLint uniform = glGetUniformLocation(shaderProgram, "u_time");
    ASSERT_MSG(uniform != -1, "Could not find uniform u_time.");

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

  //      // CLEAR SCREEN
  //      neu::vec3 bgColor{ 0, 0, 0 };
  //      neu::GetEngine().GetRenderer().SetColor(bgColor.r, bgColor.g, bgColor.b);
  //      neu::GetEngine().GetRenderer().Clear();

		//// UPDATE SCENE
        /*
        float angle = neu::GetEngine().GetTime().GetTime() * 90.0f;
        float scale = neu::math::Remap(-1.0f, 1.0f, 0.3f, 1.5f, neu::math::sin(neu::GetEngine().GetTime().GetTime()));
        neu::vec2 mouse = neu::GetEngine().GetInput().GetMousePosition();
        neu::vec2 position;
        position.x = neu::math::Remap(0.0f, (float)neu::GetEngine().GetRenderer().GetWidth(), -1.0f, 1.0f, mouse.x);
        position.y = -neu::math::Remap(0.0f, (float)neu::GetEngine().GetRenderer().GetHeight(), -1.0f, 1.0f, mouse.y);
        */


        // draw
        neu::vec3 color{ 0, 0, 0 };
        neu::GetEngine().GetRenderer().SetColor(color.r, color.g, color.b);
        neu::GetEngine().GetRenderer().Clear();

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)points.size());

        /*glLoadIdentity();
        glPushMatrix();

        glTranslatef(position.x, position.y, 0);
        glRotatef(angle, 0, 0, 1);
        glScalef(scale, scale, 1);*/

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
        /*glBegin(GL_TRIANGLES);

        for (size_t i = 0; i < points.size(); ++i) {
            glColor3f(colors[i].r, colors[i].g, colors[i].b);
            glVertex3f(points[i].x, points[i].y, points[i].z);
        }

        glEnd();

        glPopMatrix();*/

        neu::GetEngine().GetRenderer().Present();

        //// GET TIME
        //float time = neu::GetEngine().GetTime().GetTime();

        //// GET MOUSE POSITION
        //neu::vec2 mousePos = neu::GetEngine().GetInput().GetMousePosition();

        //// Normalize mouse position
        //float tx = (mousePos.x / 800.0f) * 2.0f - 1.0f;
        //float ty = -((mousePos.y / 600.0f) * 2.0f - 1.0f); // flip Y

        //// Calculate rotation and scale
        //float angle = time * 50.0f; // degrees per second
        //float scale = std::sin(time) * 0.5f + 1.0f; // 0.5 to 1.5

        //// Apply transformations
        //glPushMatrix(); // Save current transform

        //glTranslatef(tx, ty, 0.0f);         // mouse translation
        //glRotatef(angle, 0.0f, 1.0f, 0.0f); // rotation around y-axis
        //glScalef(scale, scale, scale);     // scaling

        
    }

    neu::GetEngine().Shutdown();

    return 0;
}
