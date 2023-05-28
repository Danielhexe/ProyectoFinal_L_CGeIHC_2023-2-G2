/*
Semestre 2023-2
Desarrollo del proyecto final de computación gráfica 

*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//Horario
float horario;
//variables para animación
float toffsetu = 0.0f;
float toffsetv = 0.0f;
float reproduciranimacion, habilitaranimacion,
guardoFrame, reinicioFrame, ciclo, ciclo2, contador = 0;



/*   Animacion básica ----------------- Snitch--------------*/
float offsetSnitch;
float vueloSnitch;
float rotateSnitch;
bool inicioV;
bool rotFlag;
bool frente;
float offsetAlas;
float alas;
bool aleteo;
/* -------------------------- Biju ----------------*/
float offsetBiju;
float cuerpoMovBiju;
bool colitaCaminar;
float movBiju;
bool inicioBiju;
float offsetBijumov;
float creceBiju;
float lanzaBiju;
/* --------------- Animación gargantua -----------------*/
float crece;
float angulo;
float xg;
float zg;
float gC;


/*    Ilumicanción   */
bool fueraLuces;


Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Sphere sp = Sphere(1.0, 20, 20);		//Creacion de bijudamas

Camera camera;
Texture LavaT;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;

Skybox skyboxNoche;
Skybox skyboxDia;

Model silla;
Model DeathStar;
Model Kunai;
Model Kama;
Model Kubikiribocho;
Model Shuriken4Dagas;
Model Bandana;
//Shukaku
Model shukakuBijuTorzo, shukakuBijuColita, shukakuBijuBraDer, shukakuBiju, shukakuBijuBraIzq, shukakuBijuPieDer, shukakuBijuPieIzq, bijudama;


Model restaurante;
Model Naruto;
Model TrebolC;
Model Mesa;
Model Katana;
Model GargantuaC;
Model GargantuaA;


//TOBI
Model Torso_Tobi, Pierna_DER, Pierna_IZQ, Brazo_DER, Brazo_Izq, Cabeza;

//Snithc
Model Snitch;
Model SnitchAlaI;
Model SnitchAlaD;



//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLightNoche;
DirectionalLight mainLightDia;

//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";
// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";
//PARA INPUT CON KEYFRAMES 
void inputKeyframes(bool* keys);
//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};
	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	
	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();		//Piso A desarrrollar
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

}
void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}
///////////////////////////////KEYFRAMES/////////////////////
bool animacion = false;
//NEW// Keyframes
float posXavion = 2.0, posYavion = 5.0, posZavion = -3.0; //Estado inicial
float	movAvion_x = 0.0f, movAvion_y = 0.0f;
float giroAvion = 0;
#define MAX_FRAMES 30
int i_max_steps = 350;		//Fluides de la animación, más grande, mas fluido =! consume más recuros //Default 90
int i_curr_steps = 5;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float movAvion_x;		//Variable para PosicionX
	float movAvion_y;		//Variable para PosicionY
	float movAvion_xInc;		//Variable para IncrementoX
	float movAvion_yInc;		//Variable para IncrementoY
	float giroAvion;
	float giroAvionInc;
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 5;			//introducir datos	Que son los 
bool play = false;
int playIndex = 0;

void saveFrame(void)
{

	printf("frameindex %d\n", FrameIndex);


	KeyFrame[FrameIndex].movAvion_x = movAvion_x;
	KeyFrame[FrameIndex].movAvion_y = movAvion_y;
	KeyFrame[FrameIndex].giroAvion = giroAvion;
	//Almacenar los datos
	//Opcion 1 idesal: Escribir en un archivo txt
	//opcion 2 mas rapido: Imprimir a consola y copiar los datos y copiar un archivo txt
	printf("KeyFrame[%d].movAvion_x = %f;\n",FrameIndex,movAvion_x);
	FrameIndex++;
}
void resetElements(void)
{

	movAvion_x = KeyFrame[0].movAvion_x;
	movAvion_y = KeyFrame[0].movAvion_y;
	giroAvion = KeyFrame[0].giroAvion;
}
void interpolation(void)
{
	KeyFrame[playIndex].movAvion_xInc = (KeyFrame[playIndex + 1].movAvion_x - KeyFrame[playIndex].movAvion_x) / i_max_steps;
	KeyFrame[playIndex].movAvion_yInc = (KeyFrame[playIndex + 1].movAvion_y - KeyFrame[playIndex].movAvion_y) / i_max_steps;
	KeyFrame[playIndex].giroAvionInc = (KeyFrame[playIndex + 1].giroAvion - KeyFrame[playIndex].giroAvion) / i_max_steps;

}
void animate(void)
{
	//Movimiento del objeto
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			printf("playindex : %d\n", playIndex);
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("Frame index= %d\n", FrameIndex);
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				//printf("entro aquí\n");
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//printf("se quedó aqui\n");
			//printf("max steps: %f", i_max_steps);
			//Draw animation
			movAvion_x += KeyFrame[playIndex].movAvion_xInc;
			movAvion_y += KeyFrame[playIndex].movAvion_yInc;
			giroAvion += KeyFrame[playIndex].giroAvionInc;
			i_curr_steps++;
		}

	}
}

int main()
{
	int x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12;
	int y1, y2, y3, y4, y5, y6, y7, y8, y9, y10, y11, y12;
	int z1, z2, z3, z4, z5, z6, z7, z8, z9, z10, z11, z12;

	bool Desaparecer = false;

	x1 = x2 = x3 = x4 = x5 = x6 = x7 = x8 = x9 = x10 = x11 = x12 = 0;
	y1 = y2 = y3 = y4 = y5 = y6 = y7 = y8 = y9 = y10 = y11 = y12 = 0;
	z1 = z2 = z3 = z4 = z5 = z6 = z7 = z8 = z9 = z10 = z11 = z12 = 0;

	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();
	CreateObjects();
	CreateShaders();
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);
	/* ----------------- Texturas -------------------*/
	pisoTexture = Texture("Textures/AldeaHoja.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	FlechaTexture = Texture("Textures/flechas.tga");
	FlechaTexture.LoadTextureA();
	LavaT = Texture("Textures/lava2TGA.tga");
	LavaT.LoadTextureA();
	/*---------------------------- MODELOS -----------------------------------*/
	silla = Model();
	silla.LoadModel("Models/sillaRestaurante.obj");
	DeathStar = Model();
	DeathStar.LoadModel("Models/DeathStar.obj");
	Kunai = Model();
	Kunai.LoadModel("Models/Kunai.obj");
	Kama = Model();
	Kama.LoadModel("Models/KAma.obj");
	Kubikiribocho = Model();
	Kubikiribocho.LoadModel("Models/Kubikiribocho.obj");
	Shuriken4Dagas = Model();
	Shuriken4Dagas.LoadModel("Models/Shuriken4Dagas.obj");
	//Shukau
	shukakuBijuTorzo = Model();
	shukakuBijuTorzo.LoadModel("Models/torzo.obj");
	shukakuBijuColita = Model();
	shukakuBijuColita.LoadModel("Models/colita.obj");
	shukakuBijuBraIzq = Model();
	shukakuBijuBraIzq.LoadModel("Models/brazoIzq.obj");
	shukakuBijuBraDer = Model();
	shukakuBijuBraDer.LoadModel("Models/brazoDer.obj");
	shukakuBijuPieIzq = Model();
	shukakuBijuPieIzq.LoadModel("Models/pieIzq.obj");
	shukakuBijuPieDer = Model();
	shukakuBijuPieDer.LoadModel("Models/pieDer.obj");
	bijudama = Model();
	bijudama.LoadModel("Models/bijudamaOBJ.obj");
	Bandana = Model();
	Bandana.LoadModel("Models/Bandana.obj");

	restaurante = Model();
	restaurante.LoadModel("Models/RestauranteCompleto.obj");
	Naruto = Model();
	Naruto.LoadModel("Models/Bandage Naruto.obj");
	TrebolC = Model();
	TrebolC.LoadModel("Models/trebolCarmesi.obj");
	Mesa = Model();
	Mesa.LoadModel("Models/MESA.obj");
	Katana = Model();
	Katana.LoadModel("Models/KAtana.obj");
	GargantuaC = Model();
	GargantuaC.LoadModel("Models/gargantuaCentro.obj");
	GargantuaA = Model();
	GargantuaA.LoadModel("Models/gargantuaCom.obj");
	Torso_Tobi = Model();
	Torso_Tobi.LoadModel("Models/Torso.obj");
	Pierna_DER = Model();
	Pierna_DER.LoadModel("Models/Pierna_DER.obj");
	Pierna_IZQ = Model();
	Pierna_IZQ.LoadModel("Models/Pierna_IZQ.obj");
	Brazo_DER = Model();
	Brazo_DER.LoadModel("Models/Brazo_Der.obj");
	Brazo_Izq = Model();
	Brazo_Izq.LoadModel("Models/Brazo_Izq.obj");
	Cabeza = Model();
	Cabeza.LoadModel("Models/Cabeza.obj");

	//Snitch jerarquía
	Snitch = Model();
	Snitch.LoadModel("Models/snitchOBJ.obj"); //El centro de la snitch
	SnitchAlaI = Model();
	SnitchAlaI.LoadModel("Models/snitchAlaI.obj"); //El centro de la snitch
	SnitchAlaD = Model();
	SnitchAlaD.LoadModel("Models/snitchAlaD.obj"); //El centro de la snitch


	/* Variable a utilizar */
	horario = 0.0f;

	/* Aquí es para colocar el fondo de todo el proyecto además de colocar el día y la noche todo se carga*/
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/Noche/Aldea_RT.tga");			//Right
	skyboxFaces.push_back("Textures/Skybox/Noche/Aldea_LT.tga");			//Left
	skyboxFaces.push_back("Textures/Skybox/Noche/Aldea_DT.tga");			//Down
	skyboxFaces.push_back("Textures/Skybox/Noche/Aldea_UT.tga");			//Up
	skyboxFaces.push_back("Textures/Skybox/Noche/Aldea_BT.tga");			//Back				//CA	
	skyboxFaces.push_back("Textures/Skybox/Noche/Aldea_FT.tga");			//Front	256x256		
	skyboxNoche = Skybox(skyboxFaces);

	//Se coloca el siguiente horario
	std::vector<std::string> skyboxFacesDia;
	skyboxFacesDia.push_back("Textures/Skybox/Dia/Aldea_RT.tga");
	skyboxFacesDia.push_back("Textures/Skybox/Dia/Aldea_LT.tga");
	skyboxFacesDia.push_back("Textures/Skybox/Dia/Aldea_DT.tga");
	skyboxFacesDia.push_back("Textures/Skybox/Dia/Aldea_UT.tga");
	skyboxFacesDia.push_back("Textures/Skybox/Dia/Aldea_LT.tga");
	skyboxFacesDia.push_back("Textures/Skybox/Dia/Aldea_FT.tga");
	skyboxDia = Skybox(skyboxFacesDia);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	//luz direccional, sólo 1 y siempre debe de existir
	mainLightNoche = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.1f, 0.1f,
		0.0f, 0.0f, -1.0f);
	
	mainLightDia = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.8f, 0.8f,
		0.0f, 0.0f, -1.0f);



	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Luces del restaurante
	pointLights[0] = PointLight(1.0f, 0.9647f, 0.3373f,
		0.0f, 30.0f,
		130.0f, 60.f, 120.f,
		0.007f, 0.007f, 0.006f);
	pointLightCount++;

	pointLights[1] = PointLight(1.0f, 0.9647f, 0.3373f,
		0.0f, 30.0f,
		130.0f, 60.f, 290.f,
		0.007f, 0.007f, 0.006f); //0.001f, 0.004f, 0.005
	pointLightCount++;

	//Luz naranja
	pointLights[2] = PointLight(1.0f, 0.3176, 0.0196,
		0.0f, 30.0f,
		80.0f, 70.f, 80.f,
		0.001f, 0.004f, 0.005f);
	pointLightCount++;


	/* Spotlighs*/
	unsigned int spotLightCount = 0;
	
	//Bijudama
	spotLights[0] = SpotLight(0.4784, 0.0588, 0.8196,
		1.0f, 2.0f,
		5.0f, 20.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		35.0f);
	spotLightCount++;

	//Rayo de la muerte
	spotLights[1] = SpotLight(0.051, 1, 0.4118,
		1.0f, 2.0f,
		-280.0f, 230.0f, -450.0f,
		1.f, -1.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;


	spotLights[2] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	//Verificacion

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	glm::vec3 posblackhawk = glm::vec3(2.0f, 0.0f, 0.0f);
	//KEYFRAMES DECLARADOS INICIALES
	KeyFrame[0].movAvion_x = 0.0f;
	KeyFrame[0].movAvion_y = 0.0f;
	KeyFrame[0].giroAvion = 0;

	KeyFrame[1].movAvion_x = 1.0f;
	KeyFrame[1].movAvion_y = 2.0f;
	KeyFrame[1].giroAvion = 0;

	KeyFrame[2].movAvion_x = 2.0f;
	KeyFrame[2].movAvion_y = 0.0f;
	KeyFrame[2].giroAvion = 0;

	KeyFrame[3].movAvion_x = 3.0f;
	KeyFrame[3].movAvion_y = -2.0f;
	KeyFrame[3].giroAvion = 0;

	KeyFrame[4].movAvion_x = 4.0f;
	KeyFrame[4].movAvion_y = 0.0f;
	KeyFrame[4].giroAvion = 180.0f;


	//Agregar Kefyrame[5] para que el avión regrese al inicio



	//Para cargar esferas esto funcionaría como las primitivas geométricas
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader


	/*-----------------   Se declaran las variables snitch------------- */
	offsetSnitch = 0.3f;
	offsetAlas = 0.1;
	vueloSnitch = 0.0f;
	rotateSnitch = 0.0f;
	alas = 0.0f;
	aleteo = true;
	inicioV = true;
	frente = true;
	rotFlag = false;
	/* ----------------------------------------Biju----------------------------------*/
	cuerpoMovBiju = 0.0f;
	offsetBiju = 0.005;
	colitaCaminar = true;
	movBiju = 0.0f;
	inicioBiju = true;
	offsetBijumov = 0.04;
	creceBiju = 0.0f;
	lanzaBiju = 0.0f;

	/* ------------------------------Gargantua----------------------------------------------*/
	crece = 0.0f;
	angulo = 0.0f;
	xg = 300.f;
	zg = -500.0f;
	gC = 0.0f;


	/* Luces */
	//fueraLuces = false;


	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		//para keyframes
		inputKeyframes(mainWindow.getsKeys());
		animate();
		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		/* Este ciclo es el encargado de dar el día y la noche en el escenario*/
		if (horario <= 7.0f) {			//Se ve la noche
			skyboxNoche.DrawSkybox(camera.calculateViewMatrix(), projection);			//Se dibuja el cielo
			horario += 0.01f;
			fueraLuces = false;
		}
		else {
			skyboxDia.DrawSkybox(camera.calculateViewMatrix(), projection);
			if (horario > 14.f) {
				horario = 0.0f;
			}
			horario += 0.01f;
			fueraLuces = true;
		}
		//printf("Horario -> %f\n",horario);
				
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[2].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación

		//
		//shaderList[0].SetDirectionalLight(&mainLightDia);
		
		if (fueraLuces) {
			shaderList[0].SetPointLights(pointLights, pointLightCount - 3);
			shaderList[0].SetDirectionalLight(&mainLightDia);
			shaderList[0].SetSpotLights(spotLights, spotLightCount-1);
		}
		else {
			shaderList[0].SetPointLights(pointLights, pointLightCount);
			shaderList[0].SetDirectionalLight(&mainLightNoche);
			shaderList[0].SetSpotLights(spotLights, spotLightCount);
		}
		
		//Las spotlights
		//shaderList[0].SetSpotLights(spotLights, spotLightCount);


		/* Se declaran algunas matrices auxiliares*/
		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::mat4 modelBiju(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec2 toffset = glm::vec2(0.0f, 0.0f);


		/*---------------------  Piso  --------------------*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(25.0f, 1.0f, 45.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		meshList[2]->RenderMesh();

		//AGAVE ¿qué sucede si lo renderizan antes del coche y de la pista?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -2.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();


		/*------------------ Gargantua ------------------------------*/
		if (xg >= 0.0f) {
			xg -= 0.2;
			zg += 0.32;
		}
		if (gC <= 35.0f) {
			gC+=0.005f;
		}
		if (angulo < 360) angulo += 1.f;
		else angulo = 0.0f;
		model = glm::mat4(1.0);				//x	= 300f;		z=-500.0f;
		model = glm::translate(model, glm::vec3(xg, 50.0f, zg));
		model = glm::scale(model, glm::vec3(20.0f + gC, 20.0f + gC, 20.0f + gC));
		model = glm::rotate(model, angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		GargantuaC.RenderModel();

		model = modelaux;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, -1 + alas, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		GargantuaA.RenderModel();
		//printf("Crecimiento Gargantua: %f\n", gC);

		if (xg >= 0.0f) {

			/* --------------------------- Restaurante ----------------------------*/
			model = glm::mat4(1.0);				//:vec3(-22.0f, 28.0f, 15.0f)
			model = glm::translate(model, glm::vec3(170.0f, -15.0f, 200.0f));
			model = glm::scale(model, glm::vec3(20.0f, 25.0f, 50.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			restaurante.RenderModel();

			/* ------------------ Silla para el restaurante ----------------*/
			model = glm::mat4(1.0);	//Mesa 2 silla 1
			model = glm::translate(model, glm::vec3(160.0f, -1.0f, -10.0f));
			model = glm::scale(model, glm::vec3(14.0f, 14.0f, 14.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			silla.RenderModel();

			model = glm::mat4(1.0);	//Mesa 2 silla 2
			model = glm::translate(model, glm::vec3(160.0f, -1.0f, -50.0f));
			model = glm::scale(model, glm::vec3(14.0f, 14.0f, 14.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			silla.RenderModel();

			model = glm::mat4(1.0);	//Mesa 2 silla 3
			model = glm::translate(model, glm::vec3(110.0f, -1.0f, -50.0f));
			model = glm::scale(model, glm::vec3(14.0f, 14.0f, 14.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			silla.RenderModel();

			model = glm::mat4(1.0);	//Mesa 2 silla 4
			model = glm::translate(model, glm::vec3(110.0f, -1.0f, -10.0f));
			model = glm::scale(model, glm::vec3(14.0f, 14.0f, 14.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			silla.RenderModel();

			model = glm::mat4(1.0);	//Mesa 1 silla 1
			model = glm::translate(model, glm::vec3(50.0f, -1.0f, -10.0f));
			model = glm::scale(model, glm::vec3(14.0f, 14.0f, 14.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			silla.RenderModel();

			model = glm::mat4(1.0);	//Mesa 1 silla 2
			model = glm::translate(model, glm::vec3(50.0f, -1.0f, -50.0f));
			model = glm::scale(model, glm::vec3(14.0f, 14.0f, 14.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			silla.RenderModel();

			model = glm::mat4(1.0);	//Mesa 1 silla 3
			model = glm::translate(model, glm::vec3(10.0f, -1.0f, -50.0f));
			model = glm::scale(model, glm::vec3(14.0f, 14.0f, 14.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			silla.RenderModel();

			model = glm::mat4(1.0);	//Mesa 1 silla 4
			model = glm::translate(model, glm::vec3(10.0f, -1.0f, -10.0f));
			model = glm::scale(model, glm::vec3(14.0f, 14.0f, 14.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			silla.RenderModel();

			/*-------------------------------- MESA -----------------------------*/
			model = glm::mat4(1.0);		//Mesa 1	//80 , 0, 60
			model = glm::translate(model, glm::vec3(30.0f, -1.0f, -30.0f));
			model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Mesa.RenderModel();

			model = glm::mat4(1.0);		//Mesa 2
			model = glm::translate(model, glm::vec3(130.0f, -1.0f, -30.0f));
			model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Mesa.RenderModel();

			/* ------------------------------ Trebol ---------------------------*/
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(20.0f, 0.0f, -90.0f));
			model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			TrebolC.RenderModel();

			/*------------------ DEATHSTAR ------------------------------*/
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-250.0f, 100.0f, -500.0f));
			model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
			model = glm::rotate(model, -150 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			DeathStar.RenderModel();

			/*--------------------- KUNAI ---------------------------------*/
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-160.0f, -15.0f, 30.0f));
			model = glm::scale(model, glm::vec3(1.8f, 1.8f, 1.8f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Kunai.RenderModel();
			/*-------------------------- Shuriken --------------------------*/
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-160.0f, 0.0f, 30.0f));
			model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Shuriken4Dagas.RenderModel();

			/*----------------------- Katana Sasuke -----------------------------*/
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-100.0f, 0.0f, 140.0f));
			model = glm::scale(model, glm::vec3(2.5f, 0.5f, 2.5f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Katana.RenderModel();

			/*----------------------- Kubikiri ----------------------------*/
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-150.0f, -2.0f, 140.0f));
			model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Kubikiribocho.RenderModel();

			/*----------------------- Kama --------------------------------*/
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-220.0f, 0.0f, 190.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Kama.RenderModel();

			/*------------------------ Shukaku biju--------------------------*/
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-22.0f, 28.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			shukakuBiju.RenderModel();

			/*-------------------------- BANDANA -------------------------------*/
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-200.0f, 10.0f, 10.0f));
			model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Bandana.RenderModel();

			/*---------------------------------- Naruto ------------------*/
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-22.0f, 58.0f, 15.0f));
			model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Naruto.RenderModel();

			/*------------------------ Snitch --------------------------*/
			if (vueloSnitch < 70.0f && frente) {		//Esto representa la distancia que debe recorrer
				vueloSnitch += offsetSnitch * deltaTime;	//Para ajustar nuestro ciclo de reloj y se vea bien
				if (rotateSnitch >= 0.0f && rotFlag) {
					rotateSnitch -= 0.05;	//Para hacer que vuelva a su estado de ver hacia adelante
				}
			}
			else {
				vueloSnitch -= offsetSnitch * deltaTime;
				frente = false;
				if (vueloSnitch < -30.5f) { frente = true; }
				if (rotateSnitch <= (180.0 * toRadians)) {
					rotateSnitch += 0.05;
				}
				else { rotFlag = true; }		//Si no tenemos una bandera auxiliar cuando recién inicie el programa va a hacer el (rotateSnitch >= 0.0f && rotFlag)
			}

			if (vueloSnitch > 1.0f && inicioV) {	//inicioV bandera para ajustar la posición  a 0.5 y de ahí arrancar sino vueloSnitch en el incio es > 40 y se desaparece xd
				vueloSnitch = 0.5;
				inicioV = false;
			}
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(8 * sin(vueloSnitch / 4), 50 + (2.5 * sin(vueloSnitch)), vueloSnitch));
			model = glm::rotate(model, rotateSnitch, glm::vec3(0.0f, 1.0f, 0.0f));
			modelaux = model;	//Para hacer jerarquía
			model = glm::scale(model, glm::vec3(2.5f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Snitch.RenderModel();

			if (aleteo && alas <= 1.2f) {
				alas += offsetAlas * deltaTime;
			}
			else {
				aleteo = false;
				alas -= offsetAlas * deltaTime;
				if (alas > 1.5) alas = 1.4;
				if (alas < 0.8f) {
					aleteo = true;
				}
			}

			//AlaI
			model = modelaux;
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			model = glm::translate(model, glm::vec3(-1.5f, 1.0f, -1.f));
			model = glm::rotate(model, -1 + alas, glm::vec3(0.0f, 0.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			SnitchAlaI.RenderModel();

			//AlaD
			model = modelaux;
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			model = glm::translate(model, glm::vec3(1.5f, 1.0f, -1.f));
			model = glm::rotate(model, -alas + 1, glm::vec3(0.0f, 0.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			SnitchAlaD.RenderModel();

			/*-------------------------------- TOBI ----------------------------*/
			//Torso
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 4.0f, 200.0f + mainWindow.avanzar_tobi()));
			model = glm::translate(model, glm::vec3(mainWindow.derecha_tobi(), 0.0f, 0.0f));
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, mainWindow.retroceder_tobi()));
			model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
			model = glm::rotate(model, mainWindow.Rotar_Tobi() * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			modelaux = model;
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Torso_Tobi.RenderModel();

			//Pierna Derecha
			model = modelaux;
			model = glm::translate(model, glm::vec3(3.0f, -1.5f, 0.0f));
			model = glm::rotate(model, mainWindow.Mover_pierna_brazo() * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, mainWindow.deLado_Tobi() * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Pierna_DER.RenderModel();

			//Pierna Izquierda
			model = modelaux;
			model = glm::translate(model, glm::vec3(-3.0f, -1.5f, 0.0f));
			model = glm::rotate(model, -mainWindow.Mover_pierna_brazo() * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, -mainWindow.deLado_Tobi() * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Pierna_IZQ.RenderModel();

			//Brazo Izquierdo
			model = modelaux;
			model = glm::translate(model, glm::vec3(13.05f, 11.5f, 0.0f));
			model = glm::rotate(model, -mainWindow.Mover_pierna_brazo() * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, -mainWindow.deLado_Tobi() * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Brazo_Izq.RenderModel();

			//Brazo Derecho
			model = modelaux;
			model = glm::translate(model, glm::vec3(-13.05f, 11.5f, 0.0f));
			model = glm::rotate(model, mainWindow.Mover_pierna_brazo() * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, mainWindow.deLado_Tobi() * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Brazo_DER.RenderModel();

			//Cabeza
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, 40.0f, 0.0f));
			model = glm::rotate(model, mainWindow.ARRIBA_cabeza() * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, mainWindow.Rotar_Cabeza() * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Cabeza.RenderModel();

			/*------------------------ Shukaku biju--------------------------*/
			//Torzo
			movBiju += offsetBijumov * deltaTime;

			if (movBiju > 1.0f && inicioBiju) {	//inicioV bandera para ajustar la posición  a 0.5 y de ahí arrancar sino vueloSnitch en el incio es > 40 y se desaparece xd
				movBiju = 0.5;
				inicioBiju = false;
			}

			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-30.0f, -1.0f, movBiju - 200));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, .0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			modelaux = model;
			modelBiju = modelaux;

			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			shukakuBijuTorzo.RenderModel();

			//cuerpoMovBiju += 0.01f;
			if (colitaCaminar && cuerpoMovBiju <= 0.4f) {
				cuerpoMovBiju += offsetBiju * deltaTime;
			}
			else {
				colitaCaminar = false;
				cuerpoMovBiju -= offsetBiju * deltaTime;
				if (cuerpoMovBiju > 1.5) cuerpoMovBiju = .5;
				if (cuerpoMovBiju < -0.15f) {
					colitaCaminar = true;
				}
			}

			//Colita
			model = modelaux;
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			model = glm::translate(model, glm::vec3(-3.4f, 1.6f, 0.0f));
			model = glm::rotate(model, cuerpoMovBiju, glm::vec3(0.0f, 0.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			shukakuBijuColita.RenderModel();

			//Brazo Derecho
			model = modelaux;
			model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
			model = glm::translate(model, glm::vec3(-2.0f, 3.f, -2.8f));
			model = glm::rotate(model, -60 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, cuerpoMovBiju, glm::vec3(0.0f, 1.0f, .0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			shukakuBijuBraDer.RenderModel();

			//Brazo Izquierdo
			model = modelaux;
			model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
			model = glm::translate(model, glm::vec3(0.0f, 4.f, 5.f));
			model = glm::rotate(model, 60 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, cuerpoMovBiju, glm::vec3(0.0f, 1.0f, .0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			shukakuBijuBraIzq.RenderModel();

			//Pie Derecho
			model = modelaux;
			model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
			model = glm::translate(model, glm::vec3(0.0f, -1.0f, -.7f));
			model = glm::rotate(model, cuerpoMovBiju, glm::vec3(0.0f, 0.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			shukakuBijuPieDer.RenderModel();

			//Pie Izquierdo
			model = modelaux;
			model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
			model = glm::translate(model, glm::vec3(0.0f, -1.2f, 3.5f));
			model = glm::rotate(model, -cuerpoMovBiju, glm::vec3(0.0f, 0.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			shukakuBijuPieIzq.RenderModel();


			/* ------------------------- Textura con movimiento------------------------*/
			//Importantes porque la variable uniform no podemos modificarla directamente
			toffsetu += 0.001;
			toffsetv += 0.001;
			if (toffsetu > 1.0)
				toffsetu = 0.0;
			toffset = glm::vec2(toffsetu, toffsetv);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.2f, -6.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			FlechaTexture.UseTexture();
			//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[4]->RenderMesh();
			glDisable(GL_BLEND);

			//Bijudama tiene textura

			if (creceBiju < 3.0f) creceBiju += 0.01;
			else if (lanzaBiju < 50.0) lanzaBiju += 0.05;
			else {
				lanzaBiju = 50.0;
				creceBiju += 0.01;
			}

			//printf("crece -> %f | lanza -> %f\n", creceBiju, lanzaBiju);

			model = modelBiju;
			model = glm::translate(model, glm::vec3(10.0f + lanzaBiju, 7.0f, 0.0f));
			model = glm::scale(model, glm::vec3(creceBiju, creceBiju, creceBiju));
					glm::vec3 muevePoint = glm::vec3(-35, 60.0f, movBiju - 200.f + 70.0f + (4.5 * lanzaBiju));
					spotLights[0].SetPos(muevePoint);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			bijudama.RenderModel();
		}
		else {

			/* --------------------------- Restaurante ----------------------------*/
			model = glm::mat4(1.0);				//:vec3(-22.0f, 28.0f, 15.0f)
			model = glm::translate(model, glm::vec3(170.0f - x1, -15.0f + y1, 200.0f - z1));
			model = glm::scale(model, glm::vec3(20.0f, 25.0f, 50.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			if (x1 <= 170.0f)
				x1 += 1.0f;
			if (y1 <= 50.0f)
				y1 += 1.0f;
			if (z1 <= 200.0f)
				z1 += 1.0f;
			if (x1 <= 170.0f && y1 <= 50.0f && z1 <= 200.0f)
				restaurante.RenderModel();

			
			/*------------------ DEATHSTAR ------------------------------*/
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-250.0f + x2, 100.0f - y2, -500.0f + z2));
			model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
			model = glm::rotate(model, -150 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			if (x2 <= 250.0f)
				x2 += 1.0f;
			if (y2 <= 100.0f)
				y2 += 1.0f;
			if (z2 <= 500.0f)
				z2 += 1.0f;
			if (x2 <= 250.0f && y2 <= 100.0f && z2 <= 500.0f)
				DeathStar.RenderModel();

			/*--------------------- KUNAI ---------------------------------*/
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-160.0f + x3, -15.0f + y3, 30.0f - z3));
			model = glm::scale(model, glm::vec3(1.8f, 1.8f, 1.8f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			if (x3 <= 160.0f)
				x3 += 1.0f;
			if (y3 <= 15.0f)
				y3 += 1.0f;
			if (z3 <= 30.0f)
				z3 += 1.0f;
			if (x3 <= 160.0f && y3 <= 15.0f && z3 <= 30.0f)
				Kunai.RenderModel();

			/*-------------------------- Shuriken --------------------------*/
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-160.0f + x4, 0.0f, 30.0f - z4));
			model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			if (x4 <= 160.0f)
				x4 += 1.0f;
			if (z4 <= 30.0f)
				z4 += 1.0f;
			if (x4 <= 160.0f && z4 <= 30.0f)
				Shuriken4Dagas.RenderModel();

			/*----------------------- Katana Sasuke -----------------------------*/
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-100.0f + x5, 0.0f, 140.0f - z5));
			model = glm::scale(model, glm::vec3(2.5f, 0.5f, 2.5f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			if (x5 <= 100.0f)
				x5 += 1.0f;
			if (z5 <= 140.0f)
				z5 += 1.0f;
			if (x5 <= 100.0f && z5 <= 140.0f)
				Katana.RenderModel();

			/*----------------------- Kubikiri ----------------------------*/
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-150.0f + x6, -2.0f + y6, 140.0f - z6));
			model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			if (x6 <= 150.0f)
				x6 += 1.0f;
			if (y6 <= 2.0f)
				y6 += 1.0f;
			if (z6 <= 140.0f)
				z6 += 1.0f;
			if (x6 <= 150.0f && y6 <= 2.0f && z6 <= 140.0f)
				Kubikiribocho.RenderModel();

			/*----------------------- Kama --------------------------------*/
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-220.0f + x7, 0.0f, 190.0f - z7));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			if (x7 <= 220.0f)
				x7 += 1.0f;
			if (z7 <= 190.0f)
				z7 += 1.0f;
			if (x7 <= 220.0f && z7 <= 190.0f)
				Kama.RenderModel();

			/*------------------------ Shukaku biju--------------------------*/
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-22.0f + x8, 28.0f - y8, 0.0f));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			if (x8 <= 22.0f)
				x8 += 1.0f;
			if (y8 <= 28.0f)
				y8 += 1.0f;
			if (x8 <= 22.0f && z8 <= 28.0f)
				shukakuBiju.RenderModel();


			/*-------------------------------- TOBI ----------------------------*/
			//Torso
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 4.0f - y9, 200.0f + mainWindow.avanzar_tobi() + z9));
			model = glm::translate(model, glm::vec3(mainWindow.derecha_tobi() + x9, 0.0f, 0.0f));
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, mainWindow.retroceder_tobi()));
			model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
			model = glm::rotate(model, mainWindow.Rotar_Tobi() * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			modelaux = model;
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			
			if (mainWindow.avanzar_tobi() >= 0.0f) {
				if (x9 <= mainWindow.derecha_tobi())
					x10 = x9 += 1.0f;
			}
			else {
				if (x9 <= mainWindow.derecha_tobi())
					x9 += 1.0f;
				x10 = 0.0f-x9;
			}
			if (y9 <= 4.0f)
				y9 += 1.0f;
			if (mainWindow.derecha_tobi() >= 0.0f) {
				if (z9 <= (200.0f + mainWindow.avanzar_tobi()))
					z10 = z9 += 1.0f;
			}
			else {
				if (z9 <= (200.0f + mainWindow.avanzar_tobi()))
					z9 += 1.0f;
				z10 = 0.0f-z9;
			}
			if ((x10 + mainWindow.derecha_tobi()) >= 0.0f && (z10 + 200.0f + mainWindow.avanzar_tobi()) >= 0.0f && y9 <= 4.0f)
				Torso_Tobi.RenderModel();

			//Pierna Derecha
			model = modelaux;
			model = glm::translate(model, glm::vec3(3.0f, -1.5f, 0.0f));
			model = glm::rotate(model, mainWindow.Mover_pierna_brazo() * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, mainWindow.deLado_Tobi() * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			if ((x10 + mainWindow.derecha_tobi()) >= 0.0f && (z10 + 200.0f + mainWindow.avanzar_tobi()) >= 0.0f && y9 <= 4.0f)
				Pierna_DER.RenderModel();

			//Pierna Izquierda
			model = modelaux;
			model = glm::translate(model, glm::vec3(-3.0f, -1.5f, 0.0f));
			model = glm::rotate(model, -mainWindow.Mover_pierna_brazo() * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, -mainWindow.deLado_Tobi() * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			if ((x10 + mainWindow.derecha_tobi()) >= 0.0f && (z10 + 200.0f + mainWindow.avanzar_tobi()) >= 0.0f && y9 <= 4.0f)
				Pierna_IZQ.RenderModel();

			//Brazo Izquierdo
			model = modelaux;
			model = glm::translate(model, glm::vec3(13.05f, 11.5f, 0.0f));
			model = glm::rotate(model, -mainWindow.Mover_pierna_brazo() * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, -mainWindow.deLado_Tobi() * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			if ((x10 + mainWindow.derecha_tobi()) >= 0.0f && (z10 + 200.0f + mainWindow.avanzar_tobi()) >= 0.0f && y9 <= 4.0f)
				Brazo_Izq.RenderModel();

			//Brazo Derecho
			model = modelaux;
			model = glm::translate(model, glm::vec3(-13.05f, 11.5f, 0.0f));
			model = glm::rotate(model, mainWindow.Mover_pierna_brazo() * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, mainWindow.deLado_Tobi() * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			if ((x10 + mainWindow.derecha_tobi()) >= 0.0f && (z10 + 200.0f + mainWindow.avanzar_tobi()) >= 0.0f && y9 <= 4.0f)
				Brazo_DER.RenderModel();

			//Cabeza
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, 40.0f, 0.0f));
			model = glm::rotate(model, mainWindow.ARRIBA_cabeza() * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, mainWindow.Rotar_Cabeza() * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			if ((x10 + mainWindow.derecha_tobi()) >= 0.0f && (z10 + 200.0f + mainWindow.avanzar_tobi()) >= 0.0f && y9 <= 4.0f)
				Cabeza.RenderModel();

			/* ------------------------- Textura con movimiento------------------------*/
			//Importantes porque la variable uniform no podemos modificarla directamente
			toffsetu += 0.001;
			toffsetv += 0.001;
			if (toffsetu > 1.0)
				toffsetu = 0.0;
			toffset = glm::vec2(toffsetu, toffsetv);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.2f - y11, -6.0f + z11));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			FlechaTexture.UseTexture();
			//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			if (y11 <= 0.2f)
				y11 += 0.1f;
			if (z11 <= 6.0f)
				z11 += 1.0f;
			if (y11 <= 0.2f && z11 <= 6.0f) {
				meshList[4]->RenderMesh();
				glDisable(GL_BLEND);
			}

			//Bijudama tiene textura

			if (creceBiju < 3.0f) creceBiju += 0.01;
			else if (lanzaBiju < 50.0) lanzaBiju += 0.05;
			else {
				lanzaBiju = 50.0;
				creceBiju += 0.01;
			}

			//printf("crece -> %f | lanza -> %f\n", creceBiju, lanzaBiju);

			model = modelBiju;
			model = glm::translate(model, glm::vec3(10.0f + lanzaBiju - x12, 7.0f - y12, 0.0f));
			model = glm::scale(model, glm::vec3(creceBiju, creceBiju, creceBiju));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			if (x12 <= (10.0f + lanzaBiju))
				x12 += 1.0f;
			if (y12 <= 7.0f)
				y12 += 1.0f;
			if (x12 <= (10.0f + lanzaBiju) && y12 <= 7.0f)
				bijudama.RenderModel();



			if (gC >= 35.0f) {
				gC=0.0f;
				xg = 300.0f;
				zg = -500.0f;
				lanzaBiju = 0.0f;
				creceBiju = 0.0f;
				x1 = x2 = x3 = x4 = x5 = x6 = x7 = x8 = x9 = x10 = x11 = x12 = 0;
				y1 = y2 = y3 = y4 = y5 = y6 = y7 = y8 = y9 = y10 = y11 = y12 = 0;
				z1 = z2 = z3 = z4 = z5 = z6 = z7 = z8 = z9 = z10 = z11 = z12 = 0;
				movBiju = 0.0f;
			}
		}
		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}

void inputKeyframes(bool* keys)
{
	if (keys[GLFW_KEY_SPACE])
	{
		if (reproduciranimacion < 1)
		{
			if (play == false && (FrameIndex > 1))
			{
				resetElements();
				//First Interpolation				
				interpolation();
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
				reproduciranimacion++;
				printf("\n presiona 0 para habilitar reproducir de nuevo la animación'\n");
				habilitaranimacion = 0;

			}
			else
			{
				play = false;
			}
		}
	}
	if (keys[GLFW_KEY_0])
	{
		if (habilitaranimacion < 1)
		{
			reproduciranimacion = 0;
		}
	}

	if (keys[GLFW_KEY_L])
	{
		if (guardoFrame < 1)
		{
			saveFrame();
			//printf("movAvion_x es: %f\n", movAvion_x);
			//printf("movAvion_y es: %f\n", movAvion_y);
			printf(" \npresiona P para habilitar guardar otro frame'\n");
			guardoFrame++;
			reinicioFrame = 0;
		}
	}
	if (keys[GLFW_KEY_P])
	{
		if (reinicioFrame < 1)
		{
			guardoFrame = 0;
		}
	}


	if (keys[GLFW_KEY_1])
	{
		if (ciclo < 1)
		{
			//printf("movAvion_x es: %f\n", movAvion_x);
			movAvion_x += 1.0f;
			printf("\n movAvion_x es: %f\n", movAvion_x);
			ciclo++;
			ciclo2 = 0;
			printf("\n reinicia con 2\n");
		}

	}
	if (keys[GLFW_KEY_2])
	{
		if (ciclo2 < 1)
		{
			ciclo = 0;
		}
	}

}

