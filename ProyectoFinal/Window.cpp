#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	muevex = 2.0f;
	tirar = false;
	numRandom = false;
	avanzar_Tobi = 0.0f;
	desplazar_tobi = 0.0f;
	mover_extremidades = 0.0f;
	desplazar_extremidades = 0.0f;
	rotar_cabeza = 0.0f;
	arriba_cabeza = 0.0f;
	rotar_tobi = 0.0f;
	camara = 1;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Proyecto Final-LCGeIHC", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}




void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	bool mover = true;

	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_Y)
	{
		theWindow-> muevex += 1.0;
	}
	if (key == GLFW_KEY_U)
	{
		theWindow-> muevex -= 1.0;
	}

	if (key == GLFW_KEY_T) {
			theWindow->tirar = true;
			theWindow->numRandom = true;
	}

	if (key == GLFW_KEY_R) {
		theWindow->reset = false;
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}
	if (key == GLFW_KEY_UP) {
		theWindow->avanzar_Tobi += 0.1;
		if (theWindow->mover == true) {
			if (theWindow->mover_extremidades < 40.0f)
				theWindow->mover_extremidades += 1.0f;
			else
				theWindow->mover = false;
		}
		if (theWindow->mover == false) {
			if (theWindow->mover_extremidades > -40.0f)
				theWindow->mover_extremidades -= 1.0f;
			else
				theWindow->mover = true;
		}
	}
	if (key == GLFW_KEY_DOWN) {
		theWindow->avanzar_Tobi -= 0.1;
		if (theWindow->mover == true) {
			if (theWindow->mover_extremidades > -40.0f)
				theWindow->mover_extremidades -= 1.0f;
			else
				theWindow->mover = false;
		}
		if (theWindow->mover == false) {
			if (theWindow->mover_extremidades < +40.0f)
				theWindow->mover_extremidades += 1.0f;
			else
				theWindow->mover = true;
		}
	}
	if (key == GLFW_KEY_RIGHT) {
		theWindow->desplazar_tobi += 0.1;
		if (theWindow->mover_lado == true) {
			if (theWindow->desplazar_extremidades < 40.0f)
				theWindow->desplazar_extremidades += 1.0f;
			else
				theWindow->mover_lado = false;
		}
		if (theWindow->mover_lado == false) {
			if (theWindow->desplazar_extremidades > -40.0f)
				theWindow->desplazar_extremidades -= 1.0f;
			else
				theWindow->mover_lado = true;
		}
	}
	if (key == GLFW_KEY_LEFT) {
		theWindow->desplazar_tobi -= 0.1;
		if (theWindow->mover_lado == true) {
			if (theWindow->desplazar_extremidades > -40.0f)
				theWindow->desplazar_extremidades -= 1.0f;
			else
				theWindow->mover_lado = false;
		}
		if (theWindow->mover_lado == false) {
			if (theWindow->desplazar_extremidades < 40.0f)
				theWindow->desplazar_extremidades += 1.0f;
			else
				theWindow->mover_lado = true;
		}
	}
	if (key == GLFW_KEY_F1)
		theWindow->rotar_tobi += 1.0f;
	if (key == GLFW_KEY_F2)
		theWindow->rotar_tobi -= 1.0f;
	if (key == GLFW_KEY_F3) {
		if (theWindow->arriba_cabeza <= 45.0f)
			theWindow->arriba_cabeza += 1.0f;
	}
	if (key == GLFW_KEY_F4) {
		if (theWindow->arriba_cabeza >= -45.0f)
			theWindow->arriba_cabeza -= 1.0f;
	}
	if (key == GLFW_KEY_F5) {
		if (theWindow->rotar_cabeza <= 90.0f)
			theWindow->rotar_cabeza += 1.0f;
	}
	if (key == GLFW_KEY_F6) {
		if (theWindow->rotar_cabeza >= -90.0f)
			theWindow->rotar_cabeza -= 1.0f;
	}
	if (key == GLFW_KEY_Z)
		theWindow->camara = 1;
	if (key == GLFW_KEY_X)
		theWindow->camara = 2;
	if (key == GLFW_KEY_C)
		theWindow->camara = 3;
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
