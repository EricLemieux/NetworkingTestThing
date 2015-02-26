#define SFML_STATIC

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include <SFML\Graphics\Image.hpp>

void init();
void myDrawFunction();
void floor();

GLfloat xTrans = 0.0f;
GLfloat yTrans = 0.0f;


sf::Image img_data;

GLuint texture_handle[3];

sf::Image mipmap_data;

GLfloat LightAmbient[]=		{ 0.1f, 0.1f, 0.1f, 1.0f };
GLfloat LightDiffuse[]=		{ 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[]=	{ 0.0f, 00.0f, 100.0f, 1.0f };


int main()
{
    // Create the main window
    sf::Window App(sf::VideoMode(800, 800, 32), "My OpenGL Window",2);//, sf::Style::Fullscreen, 2); // 2 levels of antialiasing

	init(); //Init OpenGL states 

	sf::Event Event;

    ///// MAIN LOOP /////
    while (App.isOpen())
    {
		// Clear color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

		///// Real-time input handling
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			xTrans -= .2f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			xTrans += .2f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			yTrans += .2f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			yTrans -= .2f;
		/////

        while (App.pollEvent(Event))
        {
            // Close window : exit
            if (Event.type == sf::Event::Closed)
                App.close();

            // Escape key : exit
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
                App.close();


            // Resize event : adjust viewport
            if (Event.type == sf::Event::Resized) {
					glViewport(0, 0, Event.size.width, Event.size.height);
					glMatrixMode(GL_PROJECTION);
					glLoadIdentity();
					gluPerspective(45.f, Event.size.width/Event.size.height, 1.f, 500.f);
					glMatrixMode(GL_MODELVIEW);
			}
        }

		App.setActive();
				
		// Setup a perspective projection
		glViewport(0, 0, 800, 800);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.f, 1.f, 1.f, 10000.f);
		
		myDrawFunction();
		floor();

        App.display();
    }

    return EXIT_SUCCESS;
}

			

void init()
{
	///// INIT OpenGL /////
    // Set color and depth clear value
    glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations


	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		// Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		// Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);	// Position The Light
	glEnable(GL_LIGHT1);								// Enable Light One

	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	// Enable texture
	glEnable(GL_TEXTURE_2D);
	
	///// LOAD TEXTURE FROM FILE
	img_data.loadFromFile("images/box.jpg");

	//Generate OpenGL texture object
	
	glGenTextures(2, &texture_handle[0]);
	glBindTexture(GL_TEXTURE_2D, texture_handle[0]);

	// Upload data to GPU
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA,
		img_data.getSize().x, img_data.getSize().y, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, img_data.getPixelsPtr()
	);
	// some texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	////////////////////
	// FLOOR TEXTURE
	img_data.loadFromFile("images/checker.jpg");
	
	glBindTexture(GL_TEXTURE_2D, texture_handle[1]);

	// Upload data to GPU
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA,
		img_data.getSize().x, img_data.getSize().y, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, img_data.getPixelsPtr()
	);
	// some texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//FLOOR MIPMAPS
	glBindTexture(GL_TEXTURE_2D, texture_handle[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, img_data.getSize().x, img_data.getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, img_data.getPixelsPtr());


}

void myDrawFunction() {
	
	// Clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glTranslatef(0.f, 0.f, -300.f);
	glTranslatef(xTrans, 0.f, 0.f);
	glTranslatef(0.f, yTrans, 0.f);

	glBindTexture(GL_TEXTURE_2D, texture_handle[0]);

    // Draw a cube
	glScalef(0.1f, 0.1f, 0.1f);
	glBegin(GL_QUADS);
		// Front Face
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-100.0f, -100.0f,  100.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 100.0f, -100.0f,  100.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 100.0f,  100.0f,  100.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-100.0f,  100.0f,  100.0f);
		// Back Face
		glNormal3f( 0.0f, 0.0f,-1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-100.0f, -100.0f, -100.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-100.0f,  100.0f, -100.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 100.0f,  100.0f, -100.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 100.0f, -100.0f, -100.0f);
		// Top Face
		glNormal3f( 0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-100.0f,  100.0f, -100.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-100.0f,  100.0f,  100.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 100.0f,  100.0f,  100.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 100.0f,  100.0f, -100.0f);

	glEnd();
		// Bottom Face
	glBegin(GL_QUADS);
		glNormal3f( 0.0f,-1.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-100.0f, -100.0f, -100.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 100.0f, -100.0f, -100.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 100.0f, -100.0f,  100.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-100.0f, -100.0f,  100.0f);
		// Right face
		glNormal3f( 1.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 100.0f, -100.0f, -100.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 100.0f,  100.0f, -100.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 100.0f,  100.0f,  100.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 100.0f, -100.0f,  100.0f);
		// Left Face
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-100.0f, -100.0f, -100.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-100.0f, -100.0f,  100.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-100.0f,  100.0f,  100.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-100.0f,  100.0f, -100.0f);
	glEnd();



}

void floor() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBindTexture(GL_TEXTURE_2D, texture_handle[2]);
	
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();

	glScalef(4.f, 4.f, 1.f);
	glBegin(GL_QUADS);
		glNormal3f( 0.0f, 1.0f, 0.0f);
	    glTexCoord2f(1.0f, 1.0f); glVertex3f(-200.0, -200.0, -400.0);
	    glTexCoord2f(1.0f, 0.0f); glVertex3f(200.0, -200.0, -400.0);
	    glTexCoord2f(0.0f, 0.0f); glVertex3f(200.0, 200.0, -400.0);
	    glTexCoord2f(0.0f, 1.0f); glVertex3f(-200.0, 200.0, -400.0);
	glEnd();

	glPopMatrix();
}













