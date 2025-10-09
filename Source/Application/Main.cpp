
int main(int argc, char* argv[]) {
    neu::file::SetCurrentDirectory("Assets");
    LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

    // initialize engine
    LOG_INFO("initialize engine...");
    neu::GetEngine().Initialize();

    // initialize scene


    // create vectors
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

        // CLEAR SCREEN
        neu::vec3 bgColor{ 0, 0, 0 };
        neu::GetEngine().GetRenderer().SetColor(bgColor.r, bgColor.g, bgColor.b);
        neu::GetEngine().GetRenderer().Clear();

        // GET TIME
        float time = neu::GetEngine().GetTime().GetTime();

        // GET MOUSE POSITION
        neu::vec2 mousePos = neu::GetEngine().GetInput().GetMousePosition();

        // Normalize mouse position
        float tx = (mousePos.x / 800.0f) * 2.0f - 1.0f;
        float ty = -((mousePos.y / 600.0f) * 2.0f - 1.0f); // flip Y

        // Calculate rotation and scale
        float angle = time * 50.0f; // degrees per second
        float scale = std::sin(time) * 0.5f + 1.0f; // 0.5 to 1.5

        // Apply transformations
        glPushMatrix(); // Save current transform

        glTranslatef(tx, ty, 0.0f);         // mouse translation
        glRotatef(angle, 0.0f, 1.0f, 0.0f); // rotation around y-axis
        glScalef(scale, scale, scale);     // scaling

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
        glBegin(GL_TRIANGLES); 

        for (size_t i = 0; i < points.size(); ++i) {
            glColor3f(colors[i].r, colors[i].g, colors[i].b);
            glVertex3f(points[i].x, points[i].y, points[i].z);
        }

        glEnd();
        glPopMatrix(); // Restore transform state

        // Present frame
        neu::GetEngine().GetRenderer().Present();
    }

    neu::GetEngine().Shutdown();

    return 0;
}
