#include <string> //for strings

#include "Libraries.h" //includes glm, glew, and glfw
#include "TextureL.h" //custom texture loading
#include <GLFW/glfw3.h> // GLFW

#include "Shader.h" //our shader class
#include "Camera.h" //our camera class
#include "Model.h" //our model loading class

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

//soil for image loading of all formats
#include <SOIL.h>

// Properties
GLuint screenWidth = 2000, screenHeight = 1500;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode); 
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();
void Translate(vec3 translationVec, vec3 scaleVec, bool rotate);
GLuint loadCubemap(vector <const GLchar*> faces);
GLuint loadTexture(GLchar* path);

// Camera
Camera camera(glm::vec3(0.0f, 7000.0f, 0.0f));

bool keys[1024];
GLfloat lastX = 400, lastY = 300; //last x and y of mouse position
bool firstMouse = true; //is it our first mouse


//used for camera movment
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

//custom collision boxes (just for testing)
Collision collision(356.408f, -100.0f, 4271.49f, -3809.22f, 3311.67f, -4580.18f, &camera, vec3(0.0f, 0.0f, 0.0f));
Collision wall1(5130.79f, -5297.63f, 1210.28f, -931.906f, -2850.86f, -2959.25f, &camera, vec3(0.0f, 0.0f, 0.0f));
Collision wall2(2219.4f, -2693.96f, -2499.46f, -3694.83f, 3210.97f, -4727.36f, &camera, vec3(0.0f,0.0f,0.0f));

//how far our view is
float cameraview = 10000000.0f;

//our model view
glm::mat4 model;

// Setup and compile our shaders
Shader shader;
Shader skyboxShader;
Shader shader2;

//values to fiddle around with varoious tasks (used for debugging and testing)
float x = -264.703f;
float z = 16.4501f;
float y = -21.1f;
float r = -0.6f;
float m = 0.017f;

//flag for initally settuping our our models position
int flag = 0;

// The MAIN function, from here we start our application and run our Game loop
int main()
{
	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "FPS", nullptr, nullptr); // Windowed
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, screenWidth, screenHeight);

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);

	//vertices of our skybox (background)
	GLfloat skyboxVertices[] = {
		// Positions          
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};

	//linking the shaders to the corresponding file
	shader.CreateShader("VertexShader.vertexshader", "FragmentShader.fragmentshader");
	shader2.CreateShader("VertexShader.vertexshader", "FragmentShader.fragmentshader");
	skyboxShader.CreateShader("Skybox.vertexshader", "Skybox.fragmentshader");

	// Load models
	Model ourModel("C:\\Users\\w\\Documents\\Visual Studio 2013\\Projects\\opengl\\opengl\\14v9q0hoqu-sniper\\sniper.obj", "C:\\Users\\w\\Documents\\Visual Studio 2013\\Projects\\opengl\\opengl\\14v9q0hoqu-sniper\\", false);
	//Model guy("C:\\Users\\w\\Downloads\\alq5lc8rm3-Abandoned Church\\3ds file.obj", "C:\\Users\\w\\Downloads\\alq5lc8rm3-Abandoned Church\\", false);
	//Model guy("C:\\Users\\w\\Downloads\\MAps\\Mountain\\Mountain.obj","C:\\Users\\w\\Downloads\\MAps\\Mountain\\",  false);
	Model guy("C:\\Users\\w\\Downloads\\MAps\\Lorencia\\Lorencia_obj.obj", "", false);


	// Draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	//generate our skybox vertex and buffer arrays
	GLuint skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);

	//TextureL tex;
	//GLuint TextureID = tex.loadBMP("C:\\Users\\w\\Downloads\\14v9q0hoqu-sniper\\Sniper_Rifle_3P_Diff.bmp");

	//load the skybox textures
	vector <const GLchar*> faces;
	faces.push_back("skybox\\right.jpg");
	faces.push_back("skybox\\left.jpg");
	faces.push_back("skybox\\top.jpg");
	faces.push_back("skybox\\bottom.jpg");
	faces.push_back("skybox\\back.jpg");
	faces.push_back("skybox\\front.jpg");
	GLuint cubemapTexture = loadCubemap(faces);


	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check and call events
		glfwPollEvents();
		Do_Movement();
	
		collision.WriteToFile();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw skybox first
		glDepthMask(GL_FALSE);// Remember to turn depth writing off
		skyboxShader.Use();
		glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		glm::mat4 projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, cameraview);
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(shader.Program, "skybox"), 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthMask(GL_TRUE);

		//translate the gun into the proper position and render
		shader.Use();   // <-- Don't forget this one!
		glm::mat4	Projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, cameraview);
		glm::mat4 View = glm::lookAt(glm::vec3(-5.0f, 0.00f, -.0f), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		glm::mat4 Model;
		Model = glm::scale(vec3(m, m, m));
		Model = glm::rotate(Model, -0.6f, vec3(0.0f, 1.0f, 0.0f));
		Model = glm::translate(Model, vec3(x, y, z));

		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(View));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(Projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(Model));

		ourModel.Draw(shader, &camera, vec3(0.0f, 0.0f, 0.0f));

		view = camera.GetViewMatrix();

		//Draw our map last
		// Transformation matrices
		projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, cameraview);
		view = camera.GetViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		//Translate and scale our map
		Translate(vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 1.0f, 2.0f), true);
		guy.Draw(shader, &camera, vec3(1.0f, 0.0f, 0.0f));

		// Swap the buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

//loads a texture via soil
GLuint loadTexture(GLchar* path)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char *image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);

	cout << width << endl;

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return textureID;

}

//loads a cubemap texture via soil
GLuint loadCubemap(vector <const GLchar*> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	//glActiveTexture(GL_TEXTURE0);

	int width, height;
	unsigned char * image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		cout << faces[i] << endl;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}

//Used for translating models
void Translate(vec3 translateVec, vec3 scaleVec, bool rotate = false)
{
	//reset all transfomrations and scaling
	model = glm::translate(model, vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, vec3(1.0f, 1.0f, 1.0f));

	//if this is called the first time to get it into its inital position
	if (flag == 0 && rotate)
	{
		model = glm::rotate(model, -90.0f, vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, scaleVec);
		flag = 1;
	}

	//translate the model if needed
	model = glm::translate(model, translateVec); // Translate it down a bit so it's at the center of the scene
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model)); //this is our model matrix
}

// Moves/alters the camera positions based on user input
void Do_Movement()
{

	//then process movment based around the flags
	// Camera controls
	if (keys[GLFW_KEY_W])
	{
		camera.ProcessFutureKeyboard(FORWARD, deltaTime);

	}
	if (keys[GLFW_KEY_S])
	{
		camera.ProcessFutureKeyboard(BACKWARD, deltaTime);

	}
	if (keys[GLFW_KEY_A])
	{
		camera.ProcessFutureKeyboard(LEFT, deltaTime);

	}
	if (keys[GLFW_KEY_D])
	{
		camera.ProcessFutureKeyboard(RIGHT, deltaTime);

	}

	//these keys were just used to create custom bounding boxes for collisions, they dont do anything useful for movement
	if (keys[GLFW_KEY_R])
	{

		//x += 0.5f;
		//cout << "x" << x << endl;
		//groundtrans += 10.0f;
		//cout << groundtrans << endl;
		collision.CreateCustomBoundingBox(&camera, 'r');
		cout << "x" << camera.Position.x << endl;
	
	}
	if (keys[GLFW_KEY_F])
	{
		//x -= 0.5f;
		//cout << "x" << x << endl;
		//groundtrans -= 10.0f;
		//cout << groundtrans << endl;
		collision.CreateCustomBoundingBox(&camera, 'f');
		cout << "x" << camera.Position.x << endl;
	}
	if (keys[GLFW_KEY_T])
	{
		//z += 0.5f;
		//cout << "z" << x << endl;
		collision.CreateCustomBoundingBox(&camera, 't');
		cout << "z" << camera.Position.z << endl;
	}
	if (keys[GLFW_KEY_G])
	{
		//z -= 0.5f;
		//cout << "z" << x << endl;
		collision.CreateCustomBoundingBox(&camera, 'g');
		cout << "z" << camera.Position.z << endl;
	}
	if (keys[GLFW_KEY_Y])
	{
		//y += 0.5f;
		//cout << "y" << x << endl;
		collision.CreateCustomBoundingBox(&camera, 'y');
		cout << "y" << camera.Position.y << endl;
	}
	if (keys[GLFW_KEY_H])
	{
		//y -= 0.5f;
		//cout << "y" << x << endl;
		collision.CreateCustomBoundingBox(&camera, 'h');
		cout << "y" << camera.Position.y << endl;
	}
	if (keys[GLFW_KEY_P])
	{
		collision.CreateCustomBoundingBox(&camera, 'p');
		//m += 0.001f;
		//cout << "m" << m << endl;
	}
	if (keys[GLFW_KEY_L])
	{
		collision.CreateCustomBoundingBox(&camera, 'l');
		collision.closeFile = 1;
	}

	collision.ResetFlags(); //reset the flags to 0
	collision.CreateCustomBoundingBox(&camera, 'p'); //set the flags if we have been in a collision
	wall1.ResetFlags(); //reset the flags to 0
	wall1.CreateCustomBoundingBox(&camera, 'p');
	wall2.ResetFlags();
	wall2.CreateCustomBoundingBox(&camera, 'p');

	camera.ProcessKeyboard();
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

//process the mouse movment if it has been moved
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

//called whenever the scroll wheel is moved
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

//#pragma endregion