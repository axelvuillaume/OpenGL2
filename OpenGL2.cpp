// OpenGL2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#define TITLE "Hello OpenGL!"
#define BUFFER_OFFSET(offset) ((GLvoid*)(offset))
using namespace std;
int SCREEN_X = 1024;
int SCREEN_Y = 768;
GLuint vaoCube;
GLuint vboCube;
GLuint passthrough;


float angleX = 0.0f; // rotation around X-axis
float angleY = 0.0f; // rotation around Y-axis
bool isDragging = false; // mouse dragging state
int lastX, lastY;


vector<glm::vec4> vPositions;
vector<glm::vec4> vColors;
vector<glm::vec4> vertexData;

glm::vec4 vertices[8] = {
{-0.5,-0.5, 0.5, 1.0 },
{-0.5, 0.5, 0.5, 1.0 },
{ 0.5, 0.5, 0.5, 1.0 },
{ 0.5,-0.5, 0.5, 1.0 },
{-0.5,-0.5,-0.5, 1.0 },
{-0.5, 0.5,-0.5, 1.0 },
{ 0.5, 0.5,-0.5, 1.0 },
{ 0.5,-0.5,-0.5, 1.0 }
};


glm::vec4 colors[8] = {
{ 1.0, 0.0, 0.0, 1.0 }, // red
{ 1.0, 1.0, 0.0, 1.0 }, // yellow
{ 0.0, 1.0, 0.0, 1.0 }, // green
{ 0.0, 0.0, 1.0, 1.0 }, // blue
{ 1.0, 0.0, 1.0, 1.0 }, // magenta
{ 0.0, 1.0, 1.0, 1.0 }, // cyan
{ 0.0, 0.0, 0.0, 1.0 }, // black
{ 1.0, 1.0, 1.0, 1.0 } // white
};




void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27: case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;
	}
}

void addVertexData(glm::vec4 position, glm::vec4 color) {
    vertexData.push_back(position);
    vertexData.push_back(color);
}

void colorCube() {
	// Face avant (deux triangles)
	vPositions.push_back(vertices[1]); vColors.push_back(colors[1]); // Triangle 1
	vPositions.push_back(vertices[0]); vColors.push_back(colors[0]);
	vPositions.push_back(vertices[3]); vColors.push_back(colors[3]);

	vPositions.push_back(vertices[1]); vColors.push_back(colors[1]); // Triangle 2
	vPositions.push_back(vertices[3]); vColors.push_back(colors[3]);
	vPositions.push_back(vertices[2]); vColors.push_back(colors[2]);

	// Face droite
	vPositions.push_back(vertices[2]); vColors.push_back(colors[2]); // Triangle 1
	vPositions.push_back(vertices[3]); vColors.push_back(colors[3]);
	vPositions.push_back(vertices[7]); vColors.push_back(colors[7]);

	vPositions.push_back(vertices[2]); vColors.push_back(colors[2]); // Triangle 2
	vPositions.push_back(vertices[7]); vColors.push_back(colors[7]);
	vPositions.push_back(vertices[6]); vColors.push_back(colors[6]);

	// Face arrière
	vPositions.push_back(vertices[6]); vColors.push_back(colors[6]); // Triangle 1
	vPositions.push_back(vertices[7]); vColors.push_back(colors[7]);
	vPositions.push_back(vertices[4]); vColors.push_back(colors[4]);

	vPositions.push_back(vertices[6]); vColors.push_back(colors[6]); // Triangle 2
	vPositions.push_back(vertices[4]); vColors.push_back(colors[4]);
	vPositions.push_back(vertices[5]); vColors.push_back(colors[5]);

	// Face gauche
	vPositions.push_back(vertices[5]); vColors.push_back(colors[5]); // Triangle 1
	vPositions.push_back(vertices[4]); vColors.push_back(colors[4]);
	vPositions.push_back(vertices[0]); vColors.push_back(colors[0]);

	vPositions.push_back(vertices[5]); vColors.push_back(colors[5]); // Triangle 2
	vPositions.push_back(vertices[0]); vColors.push_back(colors[0]);
	vPositions.push_back(vertices[1]); vColors.push_back(colors[1]);

	// Face bas
	vPositions.push_back(vertices[3]); vColors.push_back(colors[3]); // Triangle 1
	vPositions.push_back(vertices[0]); vColors.push_back(colors[0]);
	vPositions.push_back(vertices[4]); vColors.push_back(colors[4]);

	vPositions.push_back(vertices[3]); vColors.push_back(colors[3]); // Triangle 2
	vPositions.push_back(vertices[4]); vColors.push_back(colors[4]);
	vPositions.push_back(vertices[7]); vColors.push_back(colors[7]);

	// Face haut
	vPositions.push_back(vertices[5]); vColors.push_back(colors[5]); // Triangle 1
	vPositions.push_back(vertices[1]); vColors.push_back(colors[1]);
	vPositions.push_back(vertices[2]); vColors.push_back(colors[2]);

	vPositions.push_back(vertices[5]); vColors.push_back(colors[5]); // Triangle 2
	vPositions.push_back(vertices[2]); vColors.push_back(colors[2]);
	vPositions.push_back(vertices[6]); vColors.push_back(colors[6]);
}

void colorCube2() {
	// Face avant (rouge uniforme)
	vPositions.push_back(vertices[1]); vColors.push_back(colors[0]); // Triangle 1
	vPositions.push_back(vertices[0]); vColors.push_back(colors[0]);
	vPositions.push_back(vertices[3]); vColors.push_back(colors[0]);

	vPositions.push_back(vertices[1]); vColors.push_back(colors[0]); // Triangle 2
	vPositions.push_back(vertices[3]); vColors.push_back(colors[0]);
	vPositions.push_back(vertices[2]); vColors.push_back(colors[0]);

	// Face droite (jaune uniforme)
	vPositions.push_back(vertices[2]); vColors.push_back(colors[1]); // Triangle 1
	vPositions.push_back(vertices[3]); vColors.push_back(colors[1]);
	vPositions.push_back(vertices[7]); vColors.push_back(colors[1]);

	vPositions.push_back(vertices[2]); vColors.push_back(colors[1]); // Triangle 2
	vPositions.push_back(vertices[7]); vColors.push_back(colors[1]);
	vPositions.push_back(vertices[6]); vColors.push_back(colors[1]);

	// Face arrière (vert uniforme)
	vPositions.push_back(vertices[6]); vColors.push_back(colors[2]); // Triangle 1
	vPositions.push_back(vertices[7]); vColors.push_back(colors[2]);
	vPositions.push_back(vertices[4]); vColors.push_back(colors[2]);

	vPositions.push_back(vertices[6]); vColors.push_back(colors[2]); // Triangle 2
	vPositions.push_back(vertices[4]); vColors.push_back(colors[2]);
	vPositions.push_back(vertices[5]); vColors.push_back(colors[2]);

	// Face gauche (bleu uniforme)
	vPositions.push_back(vertices[5]); vColors.push_back(colors[3]); // Triangle 1
	vPositions.push_back(vertices[4]); vColors.push_back(colors[3]);
	vPositions.push_back(vertices[0]); vColors.push_back(colors[3]);

	vPositions.push_back(vertices[5]); vColors.push_back(colors[3]); // Triangle 2
	vPositions.push_back(vertices[0]); vColors.push_back(colors[3]);
	vPositions.push_back(vertices[1]); vColors.push_back(colors[3]);

	// Face bas (magenta uniforme)
	vPositions.push_back(vertices[3]); vColors.push_back(colors[4]); // Triangle 1
	vPositions.push_back(vertices[0]); vColors.push_back(colors[4]);
	vPositions.push_back(vertices[4]); vColors.push_back(colors[4]);

	vPositions.push_back(vertices[3]); vColors.push_back(colors[4]); // Triangle 2
	vPositions.push_back(vertices[4]); vColors.push_back(colors[4]);
	vPositions.push_back(vertices[7]); vColors.push_back(colors[4]);

	// Face haut (cyan uniforme)
	vPositions.push_back(vertices[5]); vColors.push_back(colors[5]); // Triangle 1
	vPositions.push_back(vertices[1]); vColors.push_back(colors[5]);
	vPositions.push_back(vertices[2]); vColors.push_back(colors[5]);

	vPositions.push_back(vertices[5]); vColors.push_back(colors[5]); // Triangle 2
	vPositions.push_back(vertices[2]); vColors.push_back(colors[5]);
	vPositions.push_back(vertices[6]); vColors.push_back(colors[5]);
}











GLchar* readShaderSource(const char* shaderFile)
{
	FILE* fp;
	fopen_s(&fp, shaderFile, "r");
	GLchar* buf;
	long size;
	if (fp == NULL) return NULL;
	fseek(fp, 0L, SEEK_END);//go to end
	size = ftell(fp); //get size
	fseek(fp, 0L, SEEK_SET);//go to beginning
	buf = (GLchar*)malloc((size + 1) * sizeof(GLchar));
	fread(buf, 1, size, fp);
	buf[size] = 0;
	fclose(fp);
	return buf;
}


void updateMVP() {
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_X / (float)SCREEN_Y, 0.1f, 100.0f);
	glm::mat4 View = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 Model = glm::mat4(1.0f); // identity matrix
	Model = glm::rotate(Model, glm::radians(angleX), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X-axis
	Model = glm::rotate(Model, glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y-axis

	glm::mat4 MVP = Projection * View * Model;

	glUniformMatrix4fv(glGetUniformLocation(passthrough, "MVP"), 1, GL_FALSE, &MVP[0][0]);
}

void mouseButton(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		isDragging = (state == GLUT_DOWN);
		lastX = x;
		lastY = y;
	}
}

void mouseMotion(int x, int y) {
	if (isDragging) {
		int deltaX = x - lastX;
		int deltaY = y - lastY;

		// Update angles based on mouse movement
		angleY += deltaX * 0.5f; // adjust sensitivity as needed
		angleX -= deltaY * 0.5f; // adjust sensitivity as needed

		lastX = x; // update last position
		lastY = y;

		// Update the MVP matrix after changing the angles
		updateMVP();
		glutPostRedisplay(); // request to redraw the window
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(passthrough); // make shader current
	glBindVertexArray(vaoCube); // make vao current
	// initiate rendering:
	// the current shader program draws the current VAO
	glDrawArrays(GL_TRIANGLES, 0, vPositions.size());
	glutSwapBuffers();
}



GLuint initShaders(const char* vShaderFile, const char* fShaderFile)
{
	struct Shader {
		const char* filename;
		GLenum type;
		GLchar* source;
	} shaders[2] = {
	{ vShaderFile, GL_VERTEX_SHADER, NULL },
	{ fShaderFile, GL_FRAGMENT_SHADER, NULL }
	};
	GLuint program = glCreateProgram();
	for (int i = 0; i < 2; ++i) {
		Shader& s = shaders[i];
		s.source = readShaderSource(s.filename);
		if (shaders[i].source == NULL) {
			printf("Failed to read %s\n", s.filename);
			exit(EXIT_FAILURE);
		}
		GLuint shader = glCreateShader(s.type);
		glShaderSource(shader, 1, (const GLchar**)&s.source, NULL);
		glCompileShader(shader);
		GLint compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			printf("%s failed to compile:\n", s.filename);
			GLint logSize;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
			char* logMsg = new char[logSize];
			glGetShaderInfoLog(shader, logSize, NULL, logMsg);
			printf("%s\n", logMsg);
			delete[] logMsg;
			exit(EXIT_FAILURE);
		}
		delete[] s.source;
		glAttachShader(program, shader);
	}
	/* link and error check */
	glLinkProgram(program);
	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked) {
		printf("Shader program failed to link:\n");
		GLint logSize;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
		char* logMsg = new char[logSize];
		glGetProgramInfoLog(program, logSize, NULL, logMsg);
		printf("%s\n", logMsg);
		delete[] logMsg;
		exit(EXIT_FAILURE);
	}
	/* use program object */
	glUseProgram(program);
	return program;
}

void init()
{
	colorCube();
	colorCube2();
	glEnable(GL_DEPTH_TEST);

	glGenBuffers(1, &vboCube);
	glBindBuffer(GL_ARRAY_BUFFER, vboCube);
	int sp = vPositions.size() * sizeof(glm::vec4);
	int sc = vColors.size() * sizeof(glm::vec4);
	glBufferData(GL_ARRAY_BUFFER, sp + sc, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sp, &vPositions[0]); //load
	glBufferSubData(GL_ARRAY_BUFFER, sp, sc, &vColors[0]); // load


	glGenVertexArrays(1, &vaoCube);
	glBindVertexArray(vaoCube);

	passthrough = initShaders("passthrough.vert", "passthrough.frag");
	/*glUseProgram(passthrough);*/


	GLuint vPosition =
		glGetAttribLocation(passthrough, "vPosition");
		glEnableVertexAttribArray(vPosition);
		glVertexAttribPointer(vPosition, 4, GL_FLOAT,GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vColor =
		glGetAttribLocation(passthrough, "vColor");glEnableVertexAttribArray(vColor); 
		glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sp));


	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -2.0, 2.0);
	glViewport(0, 0, SCREEN_X, SCREEN_Y);
}

void cleanup()
{
	glDeleteVertexArrays(1, &vaoCube);
	glDeleteBuffers(1, &vboCube);
}




void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(SCREEN_X, SCREEN_Y);
	glutCreateWindow(TITLE);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
		GLUT_ACTION_GLUTMAINLOOP_RETURNS);



	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* glewInit failed*/
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}
	fprintf(stdout, "Using GLEW %s\n", glewGetString(GLEW_VERSION));
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMotion);
	glutMainLoop();
	cleanup();	
}