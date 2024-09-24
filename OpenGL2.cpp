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
int SCREEN_Y = 769;
GLuint vaoCube;
GLuint vaoCube2;
GLuint vboCube;
GLuint vboInterlacedCube;
GLuint passthrough;
vector<glm::vec4> interlacedData;

float angleX = 0.0f; // rotation around X-axis
float angleY = 0.0f; // rotation around Y-axis
bool isDragging = false; // mouse dragging state
int lastX, lastY;

vector<glm::vec4> vPositions;
vector<glm::vec4> vColors;  

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

void createCube(vector<glm::vec4>& positions, vector<glm::vec4>& cubeColors, glm::vec3 offset) {
	for (int i = 0; i < 8; i++) {
		//glm::vec4 vertex = vertices[i];
		positions.push_back(vertices[i]);
		cubeColors.push_back(colors[i]); // Utilise directement les couleurs définies
	}
}


void createInterlacedCube(vector<glm::vec4>& interlaced, glm::vec3 offset, glm::vec4 uniformColor = glm::vec4(1.0, 0.0, 0.0, 1.0)) {
	for (int i = 0; i < 8; i++) {
		glm::vec4 vertex = vertices[i] + glm::vec4(offset, 0.0);
		interlaced.push_back(vertex);
		interlaced.push_back(uniformColor);
	}
}

// Modified to use createCube for both colored cubes
void colorCube() {
	// Create positions and colors for the first cube
	createCube(vPositions, vColors, glm::vec3(0.0, 0.0, 0.0));

	createInterlacedCube(interlacedData, glm::vec3(1.0, 1.0, 0.0), glm::vec4(0.0, 0.0, 1.0, 1.0)); // Bleu pour le deuxième cube

}


void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27: case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;
	}
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

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use shader and draw first cube
	glUseProgram(passthrough);
	glBindVertexArray(vaoCube);
	updateMVP();  // Update MVP for the first cube
	glDrawArrays(GL_TRIANGLES, 0, 36);  // Draw the first cube (36 vertices for the cube)

	//// Use shader and draw second cube
	//glBindVertexArray(vaoCube2);
	//updateMVP();  // Update MVP for the second cube
	//glDrawArrays(GL_TRIANGLES, 0, interlacedData.size() / 2);  // Draw the second cube
	//	
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
	 (program);
	return program;
}

void init() {
	passthrough = initShaders("passthrough.vert", "passthrough.frag");
	colorCube(); // Set up vertices and colors
	glEnable(GL_DEPTH_TEST);
	
	glGenBuffers(1, &vboCube);
	glBindBuffer(GL_ARRAY_BUFFER, vboCube);
	size_t positionSize = vPositions.size() * sizeof(glm::vec4); 
	size_t colorSize = vColors.size() * sizeof(glm::vec4);
	glBufferData(GL_ARRAY_BUFFER, positionSize+ colorSize, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, positionSize, vPositions.data()); // Charger les positions
	glBufferSubData(GL_ARRAY_BUFFER, positionSize, colorSize, vColors.data());

	glGenVertexArrays(1, &vaoCube);

	// Setup first cube VAO
	glBindVertexArray(vaoCube);

	GLuint vPosition = glGetAttribLocation(passthrough, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(passthrough, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(positionSize)); // Couleurs pour le premier cube


	// Création du deuxième cube avec des données interlacées
	glGenBuffers(1, &vboInterlacedCube);

	glBindBuffer(GL_ARRAY_BUFFER, vboInterlacedCube);
	size_t interlacedSize = interlacedData.size() * sizeof(glm::vec4);
	glBufferData(GL_ARRAY_BUFFER, interlacedSize, interlacedData.data(), GL_STATIC_DRAW);

	// Setup second cube VAO

	glGenVertexArrays(1, &vaoCube2);
	glBindVertexArray(vaoCube2);

	GLuint vPositionInterlaced = glGetAttribLocation(passthrough, "vPosition");
	glEnableVertexAttribArray(vPositionInterlaced);
	glVertexAttribPointer(vPositionInterlaced, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 2, BUFFER_OFFSET(0)); // Positions interlacées

	GLuint vColorInterlaced = glGetAttribLocation(passthrough, "vColor");
	glEnableVertexAttribArray(vColorInterlaced);
	glVertexAttribPointer(vColorInterlaced, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 2, BUFFER_OFFSET(sizeof(glm::vec4))); // Couleurs interlacées


	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-1.0, 3.0, -1.0, 2.0, -2.0, 2.0); // Adjust for second cube's position
	glViewport(0, 0, SCREEN_X, SCREEN_Y);
}


void cleanup()
{
		//glDeleteVertexArrays(2, vaoCube); // Supprime les deux VAOs
		//glDeleteBuffers(1, &vboCube); // Supprime le VBO
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