/*************** Proyecto Final CGIHC 2024-2 Teoría ***************/
/*************** CENTRO COMERCIAL ECO-RENOVABLE ***************/
/*************** Profesor --> Sergio Teodoro Vite ***************/

/*************** INTEGRANTES: ***************/
/*************** ROMERO TRUJILLO JERSON GERARDO ***************/
/*************** MARTINEZ MARTINEZ MARISOL ***************/
/*************** CRUZ HIPOLITO MICHEL ***************/



#include <iostream>
#include <stdlib.h>

// GLAD: Multi-Language GL/GLES/EGL/GLX/WGL Loader-Generator
// https://glad.dav1d.de/
#include <glad/glad.h>

// GLFW: https://www.glfw.org/
#include <GLFW/glfw3.h>

// GLM: OpenGL Math library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Model loading classes
#include <shader_m.h>
#include <camera.h>
#include <model.h>
#include <animatedmodel.h>
#include <material.h>
#include <light.h>
#include <cubemap.h>

#include <irrKlang.h>
using namespace irrklang;

// Functions
bool Start();
bool Update();

// Definición de callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// Gobals
GLFWwindow* window;

// Tamaño en pixeles de la ventana
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

// Definición de cámara (posición en XYZ)
Camera camera(glm::vec3(0.0f, 5.0f, 30.0f));
Camera camera3rd(glm::vec3(0.0f, 0.0f, 0.0f));

// Controladores para el movimiento del mouse
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Variables para la velocidad de reproducción
// de la animación
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float elapsedTime = 0.0f;

glm::vec3 position(-25.0f, 0.0f, 30.0f);
glm::vec3 forwardView(0.0f, 0.0f, -1.0f);
float     trdpersonOffset = 5.0f;
float     scaleV = 0.050f;
float     rotateCharacter = 180.0f;
float	  door_offset = 0.0f;
float	  door_offset2 = 0.0f;
float	  door_rotation = 0.0f;  //Puerta WC Mujeres
float	  door_rotation2 = 0.0f; //Puerta WC Hombres

// Shaders
Shader* mLightsShader;
Shader* proceduralShader;
Shader* wavesShader;

Shader* cubemapShader;
Shader* dynamicShader;

Shader* basicShader;
Shader* staticShader;




Model* UFO;				//UFO Alienígena --> Animacion Procedural 02
Model* gridMesh;			//Pasto --> Animacion Waves 03

/************************<-- MODELOS EXTRA -->************************/
Model* CentroComercial;	//Estructura Centro comercial
Model* DoorsComercial;	//Puertas del Centro Comercial --> Animacion Basica 01
Model* Salas;
Model* PisoExt;
Model* PosteBienvenidos;

/************************<-- MODELOS JERSON -->************************/
Model* Bocina01;
Model* Lampara02;
Model* Maniquie03;		//Creado en MakeHuman
Model* PanelSolar04;
Model* Silla05;
Model* Mesa06;
Model* Sillon07;
Model* MuebleRopa08;
Model* TV09;
Model* Arbol10;
Model* Espectacular;
Model* AuroraBoreal11;	//Animacion Waves

Model* CarroAmarillo;
Model* CarroBlanco;
Model* CarroNaranja;
Model* CarroAzul;


/************************<-- MODELOS MICHEL -->************************/

Model* WCTaza01;
Model* TazaCafe02;
Model* Banco03;
Model* CarritoSuper04;
Model* Dona05;
Model* Espejo06;
Model* Lavamanos07;
Model* Mesa08;
Model* Papel09;
Model* Plato10;
Model* ParedPuerta11;
Model* PuertaBanoM12;
Model* PuertaBanoH13;



/************************<-- MODELOS MARISOL -->************************/

Model* Celular01;
Model* Desayuno02;
Model* Estante03;
Model* Estante04;
Model* Extintor05;
Model* Microondas06;
Model* Nintendo07;
Model* Pantalla08;
Model* Planta09;
Model* Dvd10;








//Indice para las luces
Model* lightDummy;


// Modelos animados
AnimatedModel* Groot;
AnimatedModel* Perrito;
AnimatedModel* Botarga;

float tradius = 10.0f;
float theta = 0.0f;
float alpha = 0.0f;

// Cubemap
CubeMap* mainCubeMap;

// Light gLight;
std::vector<Light> gLights;

// Materiales
Material material01;

float proceduralTime = 0.0f;
float wavesTime = 0.0f;

// Audio
ISoundEngine* SoundEngine = createIrrKlangDevice();

// selección de cámara
bool    activeCamera = 1; // activamos la primera cámara

// Entrada a función principal
int main()
{
	if (!Start())
		return -1;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		if (!Update())
			break;
	}

	glfwTerminate();
	return 0;

}

bool Start() {
	// Inicialización de GLFW

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creación de la ventana con GLFW
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Proyecto Final CGIHC 2024-2 - Romero, Martinez, Cruz", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Ocultar el cursor mientras se rota la escena
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: Cargar todos los apuntadores
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	// Activación de buffer de profundidad
	glEnable(GL_DEPTH_TEST);

	// Compilación y enlace de shaders
	mLightsShader = new Shader("shaders/11_PhongShaderMultLights.vs", "shaders/11_PhongShaderMultLights.fs");
	proceduralShader = new Shader("shaders/12_ProceduralAnimation.vs", "shaders/12_ProceduralAnimation.fs");
	wavesShader = new Shader("shaders/13_wavesAnimation.vs", "shaders/13_wavesAnimation.fs");
	cubemapShader = new Shader("shaders/10_vertex_cubemap.vs", "shaders/10_fragment_cubemap.fs");
	dynamicShader = new Shader("shaders/10_vertex_skinning-IT.vs", "shaders/10_fragment_skinning-IT.fs");
	basicShader = new Shader("shaders/10_vertex_simple.vs", "shaders/10_fragment_simple.fs");
	staticShader = new Shader("shaders/10_vertex_simple.vs", "shaders/10_fragment_simple.fs");


	// Máximo número de huesos: 100
	dynamicShader->setBonesIDs(MAX_RIGGING_BONES);

	// Dibujar en malla de alambre
	// glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	// Se cargan los modelos 

	UFO = new Model("models/IllumModels/UFO.fbx");
	gridMesh = new Model("models/IllumModels/grid.fbx");

	/************************<-- CARGA DE MODELOS EXTRA-->************************/
	CentroComercial = new Model("models/IllumModels/ModelosExtra/CentroComercial/CentroComercial.fbx");					//--> Modelo 01
	DoorsComercial = new Model("models/IllumModels/ModelosExtra/PuertaCristal/PuertaDeCristalIzq.obj");					//--> Modelo 02
	Salas = new Model("models/IllumModels/ModelosExtra/SalasCC/Salas.obj");												//--> Modelo 03
	PisoExt = new Model("models/IllumModels/ModelosExtra/PisoExterior/PisoExterior.obj");								//--> Modelo 04
	PosteBienvenidos = new Model("models/IllumModels/ModelosExtra/PosteBienvenido/PosteBienvenidos.obj");				//--> Modelo 05



	/************************<-- CARGA DE MODELOS JERSON -->************************/

	Bocina01 = new Model("models/IllumModels/ModelosJerson/Bocina/Bocina.obj");												//--> Modelo 06
	Lampara02 = new Model("models/IllumModels/ModelosJerson/Lampara/Lampara.obj");											//--> Modelo 07
	Maniquie03 = new Model("models/IllumModels/ModelosJerson/Maniqui/Maniquie.obj");										//--> Modelo 08
	PanelSolar04 = new Model("models/IllumModels/ModelosJerson/Panel Solar/PanelSolar.obj");								//--> Modelo 09
	Silla05 = new Model("models/IllumModels/ModelosJerson/Sofa silla y mesa/Silla.obj");									//--> Modelo 10
	Mesa06 = new Model("models/IllumModels/ModelosJerson/Sofa silla y mesa/Mesa.obj");										//--> Modelo 11
	Sillon07 = new Model("models/IllumModels/ModelosJerson/Sofa silla y mesa/Sillon.obj");									//--> Modelo 12
	MuebleRopa08 = new Model("models/IllumModels/ModelosJerson/Mueble Ropa/MuebleRopa.obj");								//--> Modelo 13
	TV09 = new Model("models/IllumModels/ModelosJerson/TV/TV.obj");															//--> Modelo 14
	Arbol10 = new Model("models/IllumModels/ModelosJerson/Arbol/Arbol.obj");												//--> Modelo 15
	AuroraBoreal11 = new Model("models/IllumModels/ModelosJerson/AuroraBoreal/aurora.fbx"); // Animacion					//--> Modelo 16
	CarroAmarillo = new Model("models/IllumModels/ModelosJerson/Autos/carro amarillo.fbx");
	CarroBlanco = new Model("models/IllumModels/ModelosJerson/Autos/carro blanco.fbx");
	CarroNaranja = new Model("models/IllumModels/ModelosJerson/Autos/carro naranja.fbx");
	CarroAzul = new Model("models/IllumModels/ModelosJerson/Autos/carro azul.fbx");
	Espectacular = new Model("models/IllumModels/ModelosJerson/Espectacular/espectacular.obj");

	/************************<-- CARGA DE MODELOS MARISOL -->************************/

	Celular01 = new Model("models/IllumModels/ModelosMarisol/celular/celular.obj");											//--> Modelo 17
	Desayuno02 = new Model("models/IllumModels/ModelosMarisol/desayuno/desayuno.obj");										//--> Modelo 18
	Estante03 = new Model("models/IllumModels/ModelosMarisol/estante1/estante1.obj");										//--> Modelo 19
	Estante04 = new Model("models/IllumModels/ModelosMarisol/estante2/estante2.obj");										//--> Modelo 20
	Extintor05 = new Model("models/IllumModels/ModelosMarisol/extintor/extintor.obj");										//--> Modelo 21
	Microondas06 = new Model("models/IllumModels/ModelosMarisol/microndas/microndas.obj");									//--> Modelo 22
	Nintendo07 = new Model("models/IllumModels/ModelosMarisol/nintendo/nintendo.obj");										//--> Modelo 23
	Pantalla08 = new Model("models/IllumModels/ModelosMarisol/pantalla/pantalla.obj");										//--> Modelo 24
	Planta09 = new Model("models/IllumModels/ModelosMarisol/planta/planta.obj");											//--> Modelo 25
	Dvd10 = new Model("models/IllumModels/ModelosMarisol/dvd.fbx");															//--> Modelo 26


	/************************<-- CARGA DE MODELOS MICHEL -->************************/

	WCTaza01 = new Model("models/IllumModels/ModelosMichel/Taza/WCTaza.obj");												//--> Modelo 27
	TazaCafe02 = new Model("models/IllumModels/ModelosMichel/tazaCafe/tazaCafe.obj");										//--> Modelo 28
	Banco03 = new Model("models/IllumModels/ModelosMichel/banco/banco.obj");												//--> Modelo 29
	CarritoSuper04 = new Model("models/IllumModels/ModelosMichel/carritoSuper/carritoS.obj");								//--> Modelo 30
	Dona05 = new Model("models/IllumModels/ModelosMichel/dona/dona.obj");													//--> Modelo 31
	Espejo06 = new Model("models/IllumModels/ModelosMichel/espejo/espejo.obj");												//--> Modelo 32
	Lavamanos07 = new Model("models/IllumModels/ModelosMichel/lavamanos/lavamanos.obj");									//--> Modelo 33
	Mesa08 = new Model("models/IllumModels/ModelosMichel/mesa/mesa.obj");													//--> Modelo 34
	Papel09 = new Model("models/IllumModels/ModelosMichel/PapelBano/papelBano.obj");										//--> Modelo 35
	Plato10 = new Model("models/IllumModels/ModelosMichel/plato/plato.obj");	                                            //--> Modelo 36
	ParedPuerta11 = new Model("models/IllumModels/ModelosMichel/paredBano/paredBano.obj");                                  //--> Modelo 37
	PuertaBanoM12 = new Model("models/IllumModels/ModelosMichel/puertaBano/puertaMujer.obj");                               //--> Modelo 38
	PuertaBanoH13 = new Model("models/IllumModels/ModelosMichel/puertaBano/puertaHombre.obj");                              //--> Modelo 39



	//Modelos Animados
	Groot = new AnimatedModel("models/IllumModels/ModelosExtra/Groot3raPersona/Groot3raPersona.fbx");
	Perrito = new AnimatedModel("models/IllumModels/ModelosJerson/Muneco/perrito.fbx");
	Botarga = new AnimatedModel("models/IllumModels/ModelosJerson/Muneco/botarga.fbx");

	// Cubemap
	vector<std::string> faces
	{
		"textures/cubemap/01/posx.jpg",
		"textures/cubemap/01/negx.jpg",
		"textures/cubemap/01/posy.jpg",
		"textures/cubemap/01/negy.jpg",
		"textures/cubemap/01/posz.jpg",
		"textures/cubemap/01/negz.jpg"
	};
	mainCubeMap = new CubeMap();
	mainCubeMap->loadCubemap(faces);

	camera3rd.Position = position;
	camera3rd.Position.y += 1.7f;
	camera3rd.Position -= trdpersonOffset * forwardView;
	camera3rd.Front = forwardView;

	// Lights configuration

	Light light01;
	light01.Position = glm::vec3(5.0f, 2.0f, 5.0f);
	light01.Color = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
	light01.Power = glm::vec4(50.0f, 50.0f, 50.0f, 1.0f);
	light01.alphaIndex = 11;
	gLights.push_back(light01);


	Light light02;
	light02.Position = glm::vec3(-5.0f, 2.0f, 5.0f);
	light02.Color = glm::vec4(0.0f, 0.2f, 0.0f, 1.0f);
	light02.Power = glm::vec4(30.0f, 30.0f, 30.0f, 1.0f);
	gLights.push_back(light02);


	Light light03;
	light03.Position = glm::vec3(5.0f, 2.0f, -5.0f);
	light03.Color = glm::vec4(0.0f, 0.0f, 0.2f, 1.0f);
	light03.Power = glm::vec4(30.0f, 30.0f, 30.0f, 1.0f);
	gLights.push_back(light03);

	Light light04;
	light04.Position = glm::vec3(-5.0f, 2.0f, -5.0f);
	light04.Color = glm::vec4(0.2f, 0.0f, 0.0f, 1.0f);
	light04.Power = glm::vec4(30.0f, 30.0f, 30.0f, 1.0f);
	gLights.push_back(light04);

	lightDummy = new Model("models/IllumModels/lightDummy.fbx");

	// Configuración de propiedades materiales
	// Tabla: http://devernay.free.fr/cours/opengl/materials.html
	material01.ambient = glm::vec4(0.25f, 0.20725f, 0.20725f, 1.0f);
	material01.diffuse = glm::vec4(1.0f, 0.829f, 0.829f, 1.0f);
	material01.specular = glm::vec4(0.296648f, 0.296648f, 0.296648f, 1.0f);
	material01.transparency = 1.0f;

	SoundEngine->play2D("sound/Gears of War 3 - Mad World Instrumental.mp3", true);


	return true;
}


void SetLightUniformInt(Shader* shader, const char* propertyName, size_t lightIndex, int value) {
	std::ostringstream ss;
	ss << "allLights[" << lightIndex << "]." << propertyName;
	std::string uniformName = ss.str();

	shader->setInt(uniformName.c_str(), value);
}
void SetLightUniformFloat(Shader* shader, const char* propertyName, size_t lightIndex, float value) {
	std::ostringstream ss;
	ss << "allLights[" << lightIndex << "]." << propertyName;
	std::string uniformName = ss.str();

	shader->setFloat(uniformName.c_str(), value);
}
void SetLightUniformVec4(Shader* shader, const char* propertyName, size_t lightIndex, glm::vec4 value) {
	std::ostringstream ss;
	ss << "allLights[" << lightIndex << "]." << propertyName;
	std::string uniformName = ss.str();

	shader->setVec4(uniformName.c_str(), value);
}
void SetLightUniformVec3(Shader* shader, const char* propertyName, size_t lightIndex, glm::vec3 value) {
	std::ostringstream ss;
	ss << "allLights[" << lightIndex << "]." << propertyName;
	std::string uniformName = ss.str();

	shader->setVec3(uniformName.c_str(), value);
}


bool Update() {
	// Cálculo del framerate
	float currentFrame = (float)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	// Procesa la entrada del teclado o mouse
	processInput(window);

	// Renderizado R - G - B - A
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 projection;
	glm::mat4 view;

	if (activeCamera) {
		// Cámara en primera persona
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		view = camera.GetViewMatrix();
	}
	else {
		// cámara en tercera persona
		projection = glm::perspective(glm::radians(camera3rd.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		view = camera3rd.GetViewMatrix();
	}

	// Cubemap (fondo)
	{
		mainCubeMap->drawCubeMap(*cubemapShader, projection, view);
	}

	{
		mLightsShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// MODELO01 --> EXTRA --> Estructura Centro Comercial
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 2.54756f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 6.25f, 2.5f));	// it's a bit too big for our scene, so scale it down
		mLightsShader->setMat4("model", model);

		// Configuramos propiedades de fuentes de luz
		mLightsShader->setInt("numLights", (int)gLights.size());
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}

		mLightsShader->setVec3("eye", camera.Position);

		// Aplicamos propiedades materiales
		mLightsShader->setVec4("MaterialAmbientColor", material01.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material01.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material01.specular);
		mLightsShader->setFloat("transparency", material01.transparency);

		CentroComercial->Draw(*mLightsShader);

		//MODEL10 --> EXTRA --> PISO
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		mLightsShader->setMat4("model", model);
		PisoExt->Draw(*mLightsShader);

		//MODEL10 --> EXTRA --> PISO
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		mLightsShader->setMat4("model", model);
		PisoExt->Draw(*mLightsShader);

		//MODELO 02 --> EXTRA  --> PUERTAS CORREDIZAS

		model = glm::mat4(1.0f);
		//Efecto de puerta corrediza Lado Derecho - Left
		model = glm::translate(model, glm::vec3(0.017679f + door_offset, 0.0, -5.52512f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
		mLightsShader->setMat4("model", model);
		DoorsComercial->Draw(*mLightsShader);

		model = glm::mat4(1.0f);
		//Efecto de puerta corrediza Lado Izquierdo - Left
		model = glm::translate(model, glm::vec3(-5.49812f + door_offset, 0.0f, -5.52512f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
		mLightsShader->setMat4("model", model);
		DoorsComercial->Draw(*mLightsShader);

		model = glm::mat4(1.0f);
		//Efecto de puerta corrediza Lado Derecho - Right
		model = glm::translate(model, glm::vec3(5.49812f + door_offset2, 0.0f, 16.5467f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5, 0.5f, 1.0f));
		mLightsShader->setMat4("model", model);
		DoorsComercial->Draw(*mLightsShader);

		model = glm::mat4(1.0f);
		//Efecto de puerta corrediza Lado Izquierdo - Right
		model = glm::translate(model, glm::vec3(-0.030028f + door_offset2, 0.0f, 16.5467f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5, 0.5f, 1.0f));
		mLightsShader->setMat4("model", model);
		DoorsComercial->Draw(*mLightsShader);

		//MODEL03 -->  EXTRA --> SALAS
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		mLightsShader->setMat4("model", model);
		Salas->Draw(*mLightsShader);

		//MODELO 04 --> JERSON --> Bocina01 --> IZQUIERDA
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-5.50834f, 0.1f, -6.44789f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		mLightsShader->setMat4("model", model);
		Bocina01->Draw(*mLightsShader);

		//MODEL04 --> JERSON --> Bocina02 --> CENTRO
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.1f, -6.44789f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		mLightsShader->setMat4("model", model);
		Bocina01->Draw(*mLightsShader);

		//MODEL04 --> JERSON --> Bocina02 --> DERECHA
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(5.6f, 0.1f, -6.44789f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		mLightsShader->setMat4("model", model);
		Bocina01->Draw(*mLightsShader);

		//MODEL05 --> JERSON --> Lampara01 --> DERECHA DELANTERA
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(4.49922f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.6f, 1.0f, 1.0f));
		mLightsShader->setMat4("model", model);
		Lampara02->Draw(*mLightsShader);

		//MODEL05 --> JERSON --> Lampara02 -- IZQUIERDA DELANTERA
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-20.2599f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.6f, 1.0f, 1.0f));
		mLightsShader->setMat4("model", model);
		Lampara02->Draw(*mLightsShader);

		//MODEL06 --> JERSON --> PanelSolar -- IZQ 01
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-7.57558f, 6.0735f, -0.690915f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		mLightsShader->setMat4("model", model);
		PanelSolar04->Draw(*mLightsShader);

		//MODEL06 --> JERSON --> PanelSolar -- IZQ 02
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-2.57074f, 6.0735f, -0.690915f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		mLightsShader->setMat4("model", model);
		PanelSolar04->Draw(*mLightsShader);

		//MODEL06 --> JERSON --> PanelSolar -- DER 03
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.5928f, 6.0735f, -0.690915f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		mLightsShader->setMat4("model", model);
		PanelSolar04->Draw(*mLightsShader);

		//MODEL06 --> JERSON --> PanelSolar -- DER 04
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(7.75092f, 6.0735f, -0.690915f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		mLightsShader->setMat4("model", model);
		PanelSolar04->Draw(*mLightsShader);

		//MODEL07 --> JERSON --> Silla
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-2.0f, 0.0f, -5.5f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		mLightsShader->setMat4("model", model);
		Silla05->Draw(*mLightsShader);

		//MODEL08 --> JERSON --> Mesa
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-4.82806f, 0.036699f, -5.69903f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.621379f));
		mLightsShader->setMat4("model", model);
		Mesa06->Draw(*mLightsShader);

		//MODEL09 --> JERSON --> Sillon
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.31204f, 0.0f, -4.6575f));
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f));
		mLightsShader->setMat4("model", model);
		Sillon07->Draw(*mLightsShader);

		//MODEL09 --> JERSON --> MuebleRopa
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(-53.0815f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0));
		mLightsShader->setMat4("model", model);
		MuebleRopa08->Draw(*mLightsShader);

		//MODEL10 --> JERSON --> TV
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.98373f, 0.036699f, -5.40524f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.930686f));
		mLightsShader->setMat4("model", model);
		TV09->Draw(*mLightsShader);

		//MODEL10 --> JERSON --> ARBOL
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		mLightsShader->setMat4("model", model);
		Arbol10->Draw(*mLightsShader);

		//MODEL10.1 --> JERSON --> ARBOL
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-40.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		mLightsShader->setMat4("model", model);
		Arbol10->Draw(*mLightsShader);

		//MODELAUTOAMARILLO--> JERSON 
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(31.506f, -0.039065f, 2.1343f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		mLightsShader->setMat4("model", model);
		CarroAmarillo->Draw(*mLightsShader);

		//MODELAUTOBLANCO--> JERSON 
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(31.506f, -0.039065f, -2.2546f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.338f));
		mLightsShader->setMat4("model", model);
		CarroBlanco->Draw(*mLightsShader);

		//MODELAUTONARANJA--> JERSON 
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(31.174f, -0.039065f, -6.4189f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.579f));
		mLightsShader->setMat4("model", model);
		CarroNaranja->Draw(*mLightsShader);


		//MODELAUTOAZUL--> JERSON 
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(32.073f, -0.039065f, 10.606f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.801f));
		mLightsShader->setMat4("model", model);
		CarroAzul->Draw(*mLightsShader);

		//ESPECTACULAR--> JERSON 
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(24.037f, 1.8033f, -29.646f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		mLightsShader->setMat4("model", model);
		Espectacular->Draw(*mLightsShader);

		//MODEL11 -->  EXTRA  --> POSTE BIENVENIDOS
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-19.0f, 0.0f, 27.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		mLightsShader->setMat4("model", model);
		PosteBienvenidos->Draw(*mLightsShader);

		//MODEL12 --> MARISOL --> CELULAR
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(3.24879f, 1.62848f, -3.83944f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.120988f));
		mLightsShader->setMat4("model", model);
		Celular01->Draw(*mLightsShader);

		//MODEL13 --> MARISOL --> HotCakes
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-7.69858f, 1.14691f, 3.46061f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.207493f));
		mLightsShader->setMat4("model", model);
		Desayuno02->Draw(*mLightsShader);

		//MODEL14 --> MARISOL --> EstanteMueble
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.7433f, 0.508558f, -1.63823f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		mLightsShader->setMat4("model", model);
		Estante03->Draw(*mLightsShader);

		//MODEL15 --> MARISOL --> Estante02 CAFETERIA
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-7.40346f, 0.439357f, 3.76578f));
		model = glm::rotate(model, glm::radians(46.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.39355f));
		mLightsShader->setMat4("model", model);
		Estante04->Draw(*mLightsShader);

		//MODEL16 --> MARISOL --> Extintor
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-5.50739f, 1.59069f, 9.88178f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.148739f));
		mLightsShader->setMat4("model", model);
		Extintor05->Draw(*mLightsShader);

		//MODEL17 --> MARISOL --> Microondas
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-7.29098f, 1.33222f, 3.98596f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.187072f));
		mLightsShader->setMat4("model", model);
		Microondas06->Draw(*mLightsShader);

		//MODEL18 --> MARISOL --> Nintendo S
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.48112f, 1.51278f, -3.83989f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.129984f));
		mLightsShader->setMat4("model", model);
		Nintendo07->Draw(*mLightsShader);

		//MODEL19 --> MARISOL --> Pantalla
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-8.82149f, 2.79994f, 2.90781f));
		model = glm::rotate(model, glm::radians(-44.213f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.546864f));
		mLightsShader->setMat4("model", model);
		Pantalla08->Draw(*mLightsShader);

		//MODEL20 --> MARISOL --> Planta
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(7.47723f, 0.177379f, 7.44974f));
		//model = glm::rotate(model, glm::radians(-44.213f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.124345f));
		mLightsShader->setMat4("model", model);
		Planta09->Draw(*mLightsShader);

		//MODEL20.1 --> MARISOL --> Planta
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-7.47723f, 0.177379f, 7.44974f));
		//model = glm::rotate(model, glm::radians(-44.213f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.124345f));
		mLightsShader->setMat4("model", model);
		Planta09->Draw(*mLightsShader);

		//MODEL21 --> MARISOL --> dvd
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.78347f, 0.62585f, -3.45959f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.068686f));
		mLightsShader->setMat4("model", model);
		Dvd10->Draw(*mLightsShader);

		//MODEL22 --> MICHEL --> BancoExt
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(9.46297f, 0.83214f, 10.7407f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.419935f, 0.229935f, 0.249935f));
		mLightsShader->setMat4("model", model);
		Banco03->Draw(*mLightsShader);

		//MODEL22.1 --> MICHEL --> BancoExt2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(6.86297f, 0.83214f, 10.7407f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.419935f, 0.229935f, 0.249935f));
		mLightsShader->setMat4("model", model);
		Banco03->Draw(*mLightsShader);

		//MODEL22.3 -- > MICHEL-- > BancoExt3
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-6.86297f, 0.83214f, 10.7407f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.419935f, 0.229935f, 0.249935f));
		mLightsShader->setMat4("model", model);
		Banco03->Draw(*mLightsShader);

		//MODEL22.1 --> MICHEL --> BancoExt4
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-9.46297f, 0.83214f, 10.7407f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.419935f, 0.229935f, 0.249935f));
		mLightsShader->setMat4("model", model);
		Banco03->Draw(*mLightsShader);

		//MODEL23 --> MICHEL --> CarritoSuper
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(5.59477f, 0.719956, 16.6735f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.521788f));
		mLightsShader->setMat4("model", model);
		CarritoSuper04->Draw(*mLightsShader);


		//MODEL24 --> MICHEL --> Dona
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-7.65067f, 0.812502f, 2.90766f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		mLightsShader->setMat4("model", model);
		Dona05->Draw(*mLightsShader);

		//MODEL25 --> MICHEL --> Espejo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-8.83135f, 2.01728f, -3.62347f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.376624f, 0.376624f, 0.246624f));
		mLightsShader->setMat4("model", model);
		Espejo06->Draw(*mLightsShader);


		//MODEL25.1 --> MICHEL --> Espejo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(6.33302f, 2.01728f, -3.62347f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.376624f, 0.376624f, 0.246624f));
		mLightsShader->setMat4("model", model);
		Espejo06->Draw(*mLightsShader);


		//MODEL26 --> MICHEL --> LavaManos
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-8.41267f, 0.63214f, -3.62767f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.288465f, 0.548465f, 0.288465f));
		mLightsShader->setMat4("model", model);
		Lavamanos07->Draw(*mLightsShader);

		//MODEL26.1 --> MICHEL --> LavaManos
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(6.68267f, 0.63214f, -3.62767f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.288465f, 0.548465f, 0.288465f));
		mLightsShader->setMat4("model", model);
		Lavamanos07->Draw(*mLightsShader);

		//MODEL27 --> MICHEL --> Mesa
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(8.08764f, 0.413136f, 11.34f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.541802f));
		mLightsShader->setMat4("model", model);
		Mesa08->Draw(*mLightsShader);

		//MODEL27.1 --> MICHEL --> Mesa
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-8.08764f, 0.413136f, 11.34f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.541802f));
		mLightsShader->setMat4("model", model);
		Mesa08->Draw(*mLightsShader);

		//MODEL28 --> MICHEL --> Papel de baño
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-6.12f, 1.2471f, -5.237f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.093));
		mLightsShader->setMat4("model", model);
		Papel09->Draw(*mLightsShader);

		//MODEL28.1 --> MICHEL --> Papel de baño
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(8.7046f, 1.2471f, -5.237f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.093));
		mLightsShader->setMat4("model", model);
		Papel09->Draw(*mLightsShader);

		//MODEL29 --> MICHEL --> Plato
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-7.64733f, 0.794784f, 2.90048f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.050477f));
		mLightsShader->setMat4("model", model);
		Plato10->Draw(*mLightsShader);


		//MODEL30 --> MICHEL --> Pared del Baño M
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-7.6471f, 2.0441f, -2.2779f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.197f, 0.459f, 0.543f));
		mLightsShader->setMat4("model", model);
		ParedPuerta11->Draw(*mLightsShader);

		//MODEL31 --> MICHEL --> Pared del Baño H
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(7.3810f, 2.0435f, -2.2679f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.190f, 0.459f, 0.543f));
		mLightsShader->setMat4("model", model);
		ParedPuerta11->Draw(*mLightsShader);


		//MODEL32 --> MICHEL --> Puerta del Baño M

		// Limita el ángulo de rotación entre 0 y 90 grados
		door_rotation = glm::clamp(door_rotation, 0.0f, 90.0f);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-7.6471f, 1.789f, -2.2779f)); // Posición de la puerta
		model = glm::rotate(model, glm::radians(door_rotation), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación de la puerta
		model = glm::scale(model, glm::vec3(0.197f, 0.489f, 0.543f)); // Escalado de la puerta
		mLightsShader->setMat4("model", model);
		PuertaBanoM12->Draw(*mLightsShader);


		//MODEL33 --> MICHEL --> Puerta del Baño H
		// Limita el ángulo de rotación entre 0 y 90 grados
		door_rotation2 = glm::clamp(door_rotation2, 0.0f, 90.0f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(7.3810f, 1.778f, -2.2679f)); //2.0435 en y
		model = glm::rotate(model, glm::radians(door_rotation2), glm::vec3(0.0f, 1.0f, 0.0f));

		model = glm::scale(model, glm::vec3(0.190f, 0.459f, 0.543f));
		mLightsShader->setMat4("model", model);
		PuertaBanoH13->Draw(*mLightsShader);

	}

	glUseProgram(0);


	//Elementos Solidos que les afecta la iluminacion mucho
	{
		// Activamos el shader del plano
		staticShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		staticShader->setMat4("projection", projection);
		staticShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		//MODEL 0 --> JERSON --> Maniquie
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(-107.184f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		mLightsShader->setMat4("model", model);
		Maniquie03->Draw(*mLightsShader);

		//MODEL01 --> MICHEL --> TAZA WC --> Mujeres
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		mLightsShader->setMat4("model", model);
		WCTaza01->Draw(*mLightsShader);

		//MODEL01 --> MICHEL --> TAZA WC --> Hombres
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(15.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		mLightsShader->setMat4("model", model);
		WCTaza01->Draw(*mLightsShader);

		//MODEL01 --> MICHEL --> TAZA WC --> Hombres
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		mLightsShader->setMat4("model", model);
		TazaCafe02->Draw(*mLightsShader);


	}

	glUseProgram(0);

	/**************************** Animacion UFO - Procedural ****************************/
	{
		// Activamos el shader procedural
		proceduralShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		proceduralShader->setMat4("projection", projection);
		proceduralShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		proceduralShader->setMat4("model", model);

		proceduralShader->setFloat("time", proceduralTime);
		proceduralShader->setFloat("radius", 18.0f);
		proceduralShader->setFloat("height", 12.0f);

		UFO->Draw(*proceduralShader);
		proceduralTime += 0.001;

	}

	glUseProgram(0);


	/**************************** Animacion Waves - Aurora Boreal ****************************/
	{
		// Activamos el shader de Phong
		wavesShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		wavesShader->setMat4("projection", projection);
		wavesShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 50.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(50.0f, 150.0f, 50.0f));
		wavesShader->setMat4("model", model);

		wavesShader->setFloat("time", wavesTime);
		wavesShader->setFloat("radius", 1.0f);
		wavesShader->setFloat("height", 1.0f);

		//gridMesh->Draw(*wavesShader);
		AuroraBoreal11->Draw(*wavesShader);
		wavesTime += 0.001;

	}

	glUseProgram(0);


	// Personaje Animado

	{
		Groot->UpdateAnimation(deltaTime);
		Perrito->UpdateAnimation(deltaTime);
		Botarga->UpdateAnimation(deltaTime);


		// Activación del shader del personaje
		dynamicShader->use();

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		dynamicShader->setMat4("projection", projection);
		dynamicShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(rotateCharacter), glm::vec3(0.0, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.002f));	// it's a bit too big for our scene, so scale it down

		dynamicShader->setMat4("model", model);

		dynamicShader->setMat4("gBones", MAX_RIGGING_BONES, Groot->gBones);

		Groot->Draw(*dynamicShader);

		// Aplicamos transformaciones del modelo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 15.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.003f));	// it's a bit too big for our scene, so scale it down

		dynamicShader->setMat4("model", model);

		dynamicShader->setMat4("gBones", MAX_RIGGING_BONES, Perrito->gBones);

		Perrito->Draw(*dynamicShader);

		// Aplicamos transformaciones del modelo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-3.42806f, 1.0f, -3.59903f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.009f));	// it's a bit too big for our scene, so scale it down

		dynamicShader->setMat4("model", model);

		dynamicShader->setMat4("gBones", MAX_RIGGING_BONES, Botarga->gBones);

		Botarga->Draw(*dynamicShader);


	}

	glUseProgram(0);

	// Deplegamos los indicadores auxiliares de cada fuente de iluminación
	{
		basicShader->use();

		basicShader->setMat4("projection", projection);
		basicShader->setMat4("view", view);

		glm::mat4 model;

		for (size_t i = 0; i < gLights.size(); ++i) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, gLights[i].Position);
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			basicShader->setMat4("model", model);

			lightDummy->Draw(*basicShader);
		}

	}

	glUseProgram(0);




	// glfw: swap buffers 
	glfwSwapBuffers(window);
	glfwPollEvents();

	return true;
}

// Procesamos entradas del teclado
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);


	/***************PUERTAS CORREDIZAS***************/
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		if (door_offset < 0.01f)
			door_offset += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		if (door_offset > -1.0f)
			door_offset -= 0.01f;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		if (door_offset2 < 1.0f)
			door_offset2 += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		if (door_offset2 > 0.01f)
			door_offset2 -= 0.01f;


	/****************PUERTAS DE LOS BAÑOS ***********/

	//PUERTA WC MUJERES
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		door_rotation += 5.0f;

	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		door_rotation -= 5.0f;


	//PUERTA WC HOMBRES
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		door_rotation2 += 5.0f;

	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		door_rotation2 -= 5.0f;


	// Character movement
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {

		position = position + scaleV * forwardView;
		camera3rd.Front = forwardView;
		camera3rd.ProcessKeyboard(FORWARD, deltaTime);
		camera3rd.Position = position;
		camera3rd.Position.y += 1.0f;
		camera3rd.Position -= trdpersonOffset * forwardView;

	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		position = position - scaleV * forwardView;
		camera3rd.Front = forwardView;
		camera3rd.ProcessKeyboard(BACKWARD, deltaTime);
		camera3rd.Position = position;
		camera3rd.Position.y += 1.0f;
		camera3rd.Position -= trdpersonOffset * forwardView;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		rotateCharacter += 0.5f;

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(rotateCharacter), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec4 viewVector = model * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		forwardView = glm::vec3(viewVector);
		forwardView = glm::normalize(forwardView);

		camera3rd.Front = forwardView;
		camera3rd.Position = position;
		camera3rd.Position.y += 1.0f;
		camera3rd.Position -= trdpersonOffset * forwardView;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		rotateCharacter -= 0.5f;

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(rotateCharacter), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec4 viewVector = model * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		forwardView = glm::vec3(viewVector);
		forwardView = glm::normalize(forwardView);

		camera3rd.Front = forwardView;
		camera3rd.Position = position;
		camera3rd.Position.y += 1.0f;
		camera3rd.Position -= trdpersonOffset * forwardView;
	}

	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
		activeCamera = 0;
	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
		activeCamera = 1;

}

// glfw: Actualizamos el puerto de vista si hay cambios del tamaño
// de la ventana
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// glfw: Callback del movimiento y eventos del mouse
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos;

	lastX = (float)xpos;
	lastY = (float)ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: Complemento para el movimiento y eventos del mouse
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll((float)yoffset);
}

