/*---------------------------------------------------------*/
/*-----------------  Práctica  11 -------------------------*/
/*-----------------    2022-1   ---------------------------*/
/*----- Alumno: Leonardo Alonso González Pacheco ----------*/
/*------------- No. Cuenta 313221136 ----------------------*/
/*---------------- Eduardo Ruiz Aguilar -------------------*/
/*------------- No. Cuenta 313555268 ----------------------*/
/*---------------------------------------------------------*/
#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>

//#pragma comment(lib, "winmm.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor *monitors;

void getResolution(void);

// camera
Camera camera(glm::vec3(0.0f, 40.0f, 200.0f));//posicion de la camara 40 300
float MovementSpeed = 0.1f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(-80.0, 0.0f, 0.0f);
glm::vec3 lightDirection(0.0f, -1.0f, 0.0f); //dirección de rayos luminosos
glm::vec3 colorambient(0.2f, 0.3f, 0.5f);
glm::vec3 colordiffuse(0.2f, 0.4f, 0.0f);
glm::vec3 colorspecular(0.0f, 0.0f, 0.0f);
glm::vec3 lightPosition2(350.0f, 0.0f, 80.0f);

double myVariable = 0.0;
double myVariable2 = 0.0;
double variacionLuz = 0.0;

// posiciones
//float x = 0.0f;
//float y = 0.0f;
float	movAuto_x = -100.0f,//0
		movAuto_z = -60.0f,//0
		orienta = 180.0f,
        giroLlanta = 0.0f;

int     estado = 0,
        movimiento = 0,
		movimientoAvion = 0;
        

bool	animacion = false,
		animacion1 = false,
		recorrido1 = true,
		recorrido2 = false,
		recorrido3 = false,
		recorrido4 = false,
		recorrido5 = false,
		animacion2 = false,
		r1_anim2 = true,
		r2_anim2 = false,
		r3_anim2 = false,
		r4_anim2 = false,
		r5_anim2 = false,
		r6_anim2 = false,
		r7_anim2 = false,
		r8_anim2 = false,
		r9_anim2 = false;

//Keyframes (Manipulación y dibujo)
float	posX = 0.0f,
		posY = 0.0f,
		posZ = 0.0f,
		rotRodIzq = 0.0f,
		giroMonito = 0.0f,
		movBrazoIzq = 0.0f,
		movBrazoDer = 0.0f,
		movPieIzq = 0.0f,
		movPieDer = 0.0f,
		movCabeza = 0.0f,
		rejas_x = 0.0f,
		bmw_x = 0.0f,
		bmw_rot = 0.0f,
		bmw_z = 0.0f,
		movAvion_x = 10.0f,
		movAvion_z = 120.0f,
		movAvion_y = 0.0f;

float	incX = 0.0f,
		incY = 0.0f,
		incZ = 0.0f,
		rotInc = 0.0f,
		giroMonitoInc = 0.0f,
		movBrazoIzqInc = 0.0f,
		movBrazoDerInc = 0.0f,
		movPieIzqInc = 0.0f,
		movPieDerInc = 0.0f,
		movCabezaInc = 0.0f;

#define MAX_FRAMES 9
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float rotRodIzq;
	float giroMonito;
	float movBrazoIzq;
	float movBrazoDer;
	float movPieIzq;
	float movPieDer;
	float movCabeza;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].giroMonito = giroMonito;

	KeyFrame[FrameIndex].movBrazoIzq = movBrazoIzq;
	KeyFrame[FrameIndex].movBrazoDer = movBrazoDer;
	KeyFrame[FrameIndex].movPieIzq = movPieIzq;
	KeyFrame[FrameIndex].movPieDer = movPieDer;
	KeyFrame[FrameIndex].movCabeza = movCabeza;

	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	giroMonito = KeyFrame[0].giroMonito;

	movBrazoIzq = KeyFrame[0].movBrazoIzq;
	movBrazoDer = KeyFrame[0].movBrazoDer;
	movPieIzq = KeyFrame[0].movPieIzq;
	movPieDer = KeyFrame[0].movPieDer;
	movCabeza = KeyFrame[0].movCabeza;
}

void interpolation(void)
{
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;

	movBrazoIzqInc = (KeyFrame[playIndex + 1].movBrazoIzq - KeyFrame[playIndex].movBrazoIzq) / i_max_steps;
	movBrazoDerInc = (KeyFrame[playIndex + 1].movBrazoDer - KeyFrame[playIndex].movBrazoDer) / i_max_steps;
	movPieIzqInc = (KeyFrame[playIndex + 1].movPieIzq - KeyFrame[playIndex].movPieIzq) / i_max_steps;
	movPieDerInc = (KeyFrame[playIndex + 1].movPieDer - KeyFrame[playIndex].movPieDer) / i_max_steps;
	movCabezaInc = (KeyFrame[playIndex + 1].movCabeza - KeyFrame[playIndex].movCabeza) / i_max_steps;

}

void animate(void)
{
	//movimiento circular, ecuaciones de gometría Analítica
	//LUZ AZUL
	lightPosition.x = 50.0 * cos(myVariable);
	lightPosition.z = 50.0 * sin(myVariable);
	myVariable += 0.1;

	//cambio de color en luz
	colorambient.x = 25.0 * cos(variacionLuz);
	colorambient.y = 60.0 * sin(variacionLuz);
	colorambient.z = 10.0 * cos(variacionLuz);
	colordiffuse.x = 20.0 * sin(variacionLuz);
	colordiffuse.y = 50.0 * cos(variacionLuz);
	colordiffuse.z = 30.0 * sin(variacionLuz);
	variacionLuz += 0.01;

	////animación de LUZ ROSA
	//lightPosition2.x = 50 * sin(myVariable2);
	//lightPosition2.z = 50 * sin(myVariable2) + cos(myVariable2);
	//myVariable2 += 0.1;


	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += incX;
			posY += incY;
			posZ += incZ;

			rotRodIzq += rotInc;
			giroMonito += giroMonitoInc;

			movBrazoIzq += movBrazoIzqInc;
			movBrazoDer += movBrazoDerInc;
			movPieIzq += movPieIzqInc;
			movPieDer += movPieDerInc;
			movCabeza += movCabezaInc;

			i_curr_steps++;
		}
	}

	////Vehículo
	if (animacion)
	{
		if (estado == 0)
		{
			movAuto_z += 3.0f;
			giroLlanta += 9.0f;
			if (movAuto_z >= 200.0f)
				estado = 1;	
		}
		else
		{
			movAuto_z -= 3.0f;
			giroLlanta -= 9.0f;
			if (movAuto_z <= 0.0f)
				estado = 0;
		}
			//animacion = false;	
	}

	/*switch (movimiento) {
	case 1:
		if (movAuto_z >= -70.0f) {
			movAuto_z -= 0.5f;
			giroLlanta -= 9.0f;
		}
		else movimiento = 2;
		break;

	case 2:
		if (movAuto_x <= 20.75f) {
			movAuto_x += 0.5f;
			giroLlanta += 9.0f;
		}
		else movimiento = 3;
		break;

	case 3:
		if (movAuto_z <= 9.5f) {
			movAuto_z += 0.5f;
			giroLlanta += 9.0f;
		}
		else movimiento = 4;
		break;

	case 4:
		if (movAuto_x >= -3.75) {
			movAuto_x -= 0.5f;
			giroLlanta -= 9.0f;
		}
		else movimiento = 5;
		break;

	case 5:
		if (movAuto_z <= 30.5f) {
			movAuto_z += 0.5f;
			giroLlanta += 9.0f;
		}
		else movimiento = 0;
		break;

	default:
		break;
	}*/

	/*if (animacion1)
	{
		if (recorrido1)
		{
			movAuto_z += 3.0f;
			giroLlanta += 9.0f;
			orienta = 0.0f;
			if (movAuto_z >= 200.0f)
			{
				recorrido1 = false;
				recorrido2 = true;
			}
		}
		if (recorrido2)
		{
			movAuto_x -= 3.0f;
			giroLlanta += 9.0f;
			orienta = -90.0f;
			if (movAuto_x <= -250.0f)
			{
				recorrido2 = false;
				recorrido3 = true;
			}
		}
		if (recorrido3)
		{
			movAuto_z -= 3.0f;
			giroLlanta += 9.0f;
			orienta = 180.0f;
			if (movAuto_z <= -200.0f)
			{
				recorrido3 = false;
				recorrido4 = true;
			}
		}
		if (recorrido4)
		{
			movAuto_z += 3.0f;
			giroLlanta += 9.0f;
			orienta = -270.0f;
			if (movAuto_x >= 0.0f)
			{
				recorrido4 = false;
				recorrido1 = true;
			}
		}
	}*/
	
	if (animacion1)
	{
		if (recorrido1)
		{
			movAuto_z -= 3.0f;
			giroLlanta += 9.0f;
			orienta = 180.0f;
			if (movAuto_z <= -200.0f)
			{
				recorrido1 = false;
				recorrido4 = true;
			}
		}
		if (recorrido2)
		{
			movAuto_x += 3.0f;
			giroLlanta += 9.0f;
			orienta = -270.0f;
			if (movAuto_x >= 0.0f)
			{
				recorrido2 = false;
				recorrido1 = true;
			}
		}
		if (recorrido3)
		{
			movAuto_z += 3.0f;
			giroLlanta += 9.0f;
			orienta = 270.0f;
			if (movAuto_z <= 200.0f)
			{
				recorrido3 = false;
				recorrido2 = true;
			}
		}
		if (recorrido4)
		{
			movAuto_x -= 3.0f;
			giroLlanta += 9.0f;
			orienta = -90.0f;
			if (movAuto_x <= -250.0f)
			{
				recorrido4 = false;
				recorrido5 = true;
			}
		}
		if (recorrido5)
		{
			movAuto_x += 2.5f;
			movAuto_z += 4.0f;
			orienta = 32.00538;
			if (movAuto_z >= 200.0f && movAuto_x >= 0.0f)
			{
				recorrido5 = false;
				recorrido1 = true;
			}

		}
	}

	if (animacion2)
	{
		if (r1_anim2)
		{
			rejas_x -= 2.0f;
			bmw_z += 0.05f;
			if (rejas_x <= -80.0f)
			{
				r1_anim2 = false;
				r2_anim2 = true;
			}
		}
		if (r2_anim2)
		{
			bmw_z += 1.0f;
			if (bmw_z >= 30.0f)
			{
				r2_anim2 = false;
				r3_anim2 = true;
			}
		}
		if (r3_anim2)
		{
			bmw_z += 1.0f;
			bmw_rot -= 1.0f;
			bmw_x -= 0.3;
			if (bmw_rot <= -90.0f)
			{
				r3_anim2 = false;
				r4_anim2 = true;
			}
		}
		if (r4_anim2)
		{
			bmw_x -= 1.0f;
			rejas_x += 1.0f;
			if (rejas_x >= 0.0f)
			{
				bmw_x -= 1.0f;
				rejas_x = 0.0f;
				if (bmw_x <= -650.0f)
				{
					r4_anim2 = false;
					r5_anim2 = true;
				}
			}
		}
		if (r5_anim2)
		{
			bmw_x -= 1.0;
			bmw_z -= 0.7f;
			bmw_rot += 0.6f;
			if (bmw_x <= -800.0f)
			{
				r5_anim2 = false;
				r6_anim2 = true;
			}
		}
		if (r6_anim2)
		{
			bmw_x -= 1.0;
			bmw_z += 1.0f;
			bmw_rot += 0.6f;
			if (bmw_x <= -900.0f)
			{
				r6_anim2 = false;
				r7_anim2 = true;
			}
		}
		if (r7_anim2)
		{
			bmw_x += 1.0;
			bmw_z += 1.5f;
			bmw_rot += 0.6f;
			if (bmw_x >= -800.0f)
			{
				r7_anim2 = false;
				r8_anim2 = true;
			}
		}
		if (r8_anim2)
		{
			bmw_x += 1.0;
			bmw_z -= 0.6f;
			bmw_rot -= 0.2f;
			if (bmw_x >= -650.0f)
			{
				r8_anim2 = false;
				r9_anim2 = true;
			}
		}
		if (r9_anim2)
		{
			bmw_x += 1.0;
			if (bmw_x >= 130.0f)
			{
				r9_anim2 = false;
				bmw_rot = 0.0f;
				bmw_x = 0.0f;
				bmw_z = 0.0f;

			}
		}
	}


	switch (movimientoAvion) {
		case 1:
			if (movAvion_x >= -80.0f) {
				movAvion_x -= 0.5f;
			}
			else movimientoAvion = 2;
			break;

		case 2:
			if (movAvion_y <= 50.0f) {
				movAvion_y += 0.5f;
			}
			else movimientoAvion = 3;
			break;

		case 3:
			if (movAvion_x <= -50.0f) {
				movAvion_x -= 0.5f;
			}
			else movimientoAvion = 4;
			break;

		case 4:
			if (movAvion_y >= -3.75) {
				movAvion_y -= 0.5f;
			}
			else movimientoAvion = 5;
			break;

		case 5:
			if (movAvion_x <= 10.0f) {
				movAvion_x += 0.5f;
			}
			else movimientoAvion = 0;
			break;

		default:
			break;
	}
}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CGeIHC", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs"); //dibuja elementos estáticos del escenario
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");//Se encarga de dibujar el fondo(cielo y piso) se encuentra estático
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");//modelos con animaciones asociadas

	vector<std::string> faces
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	// -----------

	/*Model botaDer("resources/objects/Personaje/bota.obj");
=======
	/*Model piso("resources/objects/piso/piso.obj");*/
	Model botaDer("resources/objects/Personaje/bota.obj");
	Model piernaDer("resources/objects/Personaje/piernader.obj");
	Model piernaIzq("resources/objects/Personaje/piernader.obj");
	Model torso("resources/objects/Personaje/torso.obj");
	Model brazoDer("resources/objects/Personaje/brazoder.obj");
	Model brazoIzq("resources/objects/Personaje/brazoizq.obj");
	Model cabeza("resources/objects/Personaje/cabeza.obj");
	Model carro("resources/objects/lambo/carroceria.obj");
	Model llanta("resources/objects/lambo/Wheel.obj");
	Model avionGuerra("resources/objects/Avion/Avion.obj");
	Model casaMinimalista("resources/objects/casa2021/casa.obj");
	Model camastro("resources/objects/sunbed/camastrof.obj");
	Model Cama("resources/objects/cama/single_bed.obj");
	Model Mesa("resources/objects/mesa_baja/mesa.obj");
	Model Pasto("resources/objects/casa2021/pasto.obj");
	Model Cristales_casa("resources/objects/casa2021/cristales.obj");
	Model Rejas("resources/objects/casa2021/rejas.obj");
	Model Perro("resources/objects/perro/cherry.obj");
	Model Tesla("resources/objects/tesla/tesla.obj");
	Model Calle("resources/objects/camino/calle.obj");
	Model BMW("resources/objects/bmw/bmw.obj");
	Model Tienda("resources/objects/tienda/tienda.obj");
	Model Mueble_Tienda("resources/objects/comics/mueble.obj");
	Model Puerta_Tienda("resources/objects/tienda/puerta.obj");
	Model Cristales_Tienda("resources/objects/tienda/cristales.obj");
	Model Arbusto("resources/objects/arbusto/Poplar_Tree.obj");
	Model Lampara_Calle("resources/objects/lamp_calle/lampara.obj");
	Model Rotonda("resources/objects/Rotonda/roundabout.obj");
	Model Planta("resources/objects/Planta/objPlant.obj");
	Model Sofa("resources/objects/sofa/sofa.obj");
	Model Sillon("resources/objects/Sillon/jean_obj.obj");
	Model TV("resources/objects/tv/jean_obj.obj");
	Model Closet("resources/objects/closet/ConvaliaBouquet.obj");
	Model Florero("resources/objects/florero/ConvaliaBouquet.obj");
	Model Retrete("resources/objects/retrete/retrete.obj");
	Model Almohada("resources/objects/Almohada/pillow.obj");
	Model Lampara("resources/objects/lampara/lmp005.obj");
	Model Cocina("resources/objects/cocina/fkc.obj");
	Model MesaRedonda("resources/objects/MesaRedonda/diningtable.obj");
	Model Cuadro1("resources/objects/cuadros/nacimiento.obj");
	Model Cuadro2("resources/objects/cuadros/noche_estrellada.obj");
	Model Cuadro3("resources/objects/cuadros/van-gogh.obj");
	Model Garage("resources/objects/garage/double_garage.obj");
	Model Librero("resources/objects/librero/librero.obj");
	Model Tapete("resources/objects/tapete/tapete.obj");


	/*ModelAnim animacionPersonaje("resources/objects/Personaje1/PersonajeBrazo.dae");
	animacionPersonaje.initShaders(animShader.ID);*/

	/*ModelAnim ninja("resources/objects/ZombieWalk/ZombieWalk.dae");
	ninja.initShaders(animShader.ID);*/

	/*ModelAnim personajeTwist("resources/objects/Twist/Twist.dae");
	personajeTwist.initShaders(animShader.ID);*/

	//Inicialización de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].giroMonito = 0;
	}

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);
		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection); //direción de rayos luminosos
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.4f, 0.4f, 0.4f)); //caras menos iluminadas se apaga la fuente de luz(cantidad de luz en el ambiente)
		staticShader.setVec3("dirLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f)); //caras mas iluminadas
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f)); //brillo de la superficie

		////staticShader.setVec3("pointLight[0].position", glm::vec3(-90.0f, 4.0f, -10.0f)); //Recibe atenuación
		//staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		//staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		//staticShader.setVec3("pointLight[0].position", glm::vec3(-90.0f, 4.0f, -10.0f)); //Recibe atenuación
		//staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		//staticShader.setFloat("pointLight[0].constant", 0.08f); //ATENUACIÓN
		//staticShader.setFloat("pointLight[0].linear", 0.0009f);
		//staticShader.setFloat("pointLight[0].quadratic", 0.0032f);//se indica mas potencia a la fuente (SUBO POTENCIA) al reducir atenuación

		//Luz Azul
		staticShader.setVec3("pointLight[0].position", glm::vec3(-80.0, 0.0f, 160.0f));
		/*staticShader.setVec3("pointLight[1].position", lightPosition);*/
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.7f));  // aca se pone el color de la luz
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.3f, 1.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 1.0f); //atenuaciones para la intensidad de la luz
		staticShader.setFloat("pointLight[0].linear", 0.0009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.0032f);

		////Luz que cambia de colores
		staticShader.setVec3("pointLight[1].position", glm::vec3(-10.0, 0.0f, 30.0f));
		staticShader.setVec3("pointLight[1].ambient", colorambient);  // aca se pone el color de la luz
		staticShader.setVec3("pointLight[1].diffuse", colordiffuse);
		staticShader.setVec3("pointLight[1].specular", colorspecular);
		staticShader.setFloat("pointLight[1].constant", 1.0f); //atenuaciones para la intensidad de la luz
		staticShader.setFloat("pointLight[1].linear", 0.0009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.0032f);

		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(0.6f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);

		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		//Remember to activate the shader with the animation
		animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);
		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-40.3f, 1.75f, 0.3f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.2f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		//animacionPersonaje.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Segundo Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------

		model = glm::translate(glm::mat4(1.0f), glm::vec3(40.3f, 1.75f, 0.3f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.5f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		/*ninja.Draw(animShader);*/
		/*personajeTwist.Draw(animShader);*/

		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//CASA
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Se agrega transparencia
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // -70 200
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		casaMinimalista.Draw(staticShader);
		glEnable(GL_BLEND);

		//CRISTALES
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // -70 200
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		Cristales_casa.Draw(staticShader);

		//TIENDA
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 375.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(15.0f));
		staticShader.setMat4("model", model);
		Tienda.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 375.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(15.0f));
		staticShader.setMat4("model", model);
		Puerta_Tienda.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 375.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(15.0f));
		staticShader.setMat4("model", model);
		Cristales_Tienda.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 375.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(15.0f));
		staticShader.setMat4("model", model);
		Mueble_Tienda.Draw(staticShader);

		//PASTO
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		Pasto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(325.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		Pasto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 325.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		Pasto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(325.0f, 0.0f, 325.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		Pasto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-325.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		Pasto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-325.0f, 0.0f, 325.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		Pasto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(650.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		Pasto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(650.0f, 0.0f, 350.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		Pasto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(650.0f, 0.0f, 650.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		Pasto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(325.0f, 0.0f, 650.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		Pasto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 650.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		Pasto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-325.0f, 0.0f, 650.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		Pasto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(975.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		Pasto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(975.0f, 0.0f, 325.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		Pasto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(975.0f, 0.0f, 650.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		Pasto.Draw(staticShader);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Se agrega trnasparencia
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // -70 200
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		casaMinimalista.Draw(staticShader);
		glEnable(GL_BLEND);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // -70 200
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		Pasto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(325.0f, 0.0f, 0.0f)); // -70 200
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		Pasto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 325.0f)); // -70 200
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		Pasto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(325.0f, 0.0f, 325.0f)); // -70 200
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		Pasto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-325.0f, 0.0f, 0.0f)); // -70 200
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		Pasto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-325.0f, 0.0f, 325.0f)); // -70 200
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		Pasto.Draw(staticShader);

		//ARBUSTO
		model = glm::translate(glm::mat4(1.0f), glm::vec3(200.0f, 0.0f, 100.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(50.0f));
		staticShader.setMat4("model", model);
		Arbusto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(200.0f, 0.0f, 50.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(50.0f));
		staticShader.setMat4("model", model);
		Arbusto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(200.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(50.0f));
		staticShader.setMat4("model", model);
		Arbusto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(200.0f, 0.0f, -50.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(50.0f));
		staticShader.setMat4("model", model);
		Arbusto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(200.0f, 0.0f, -100.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(50.0f));
		staticShader.setMat4("model", model);
		Arbusto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(200.0f, 0.0f, -150.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(50.0f));
		staticShader.setMat4("model", model);
		Arbusto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(190.0f, 0.0f, -200.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(50.0f));
		staticShader.setMat4("model", model);
		Arbusto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-220.0f, 0.0f, 100.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(50.0f));
		staticShader.setMat4("model", model);
		Arbusto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-220.0f, 0.0f, 50.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(50.0f));
		staticShader.setMat4("model", model);
		Arbusto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-200.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(50.0f));
		staticShader.setMat4("model", model);
		Arbusto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-200.0f, 0.0f, -50.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(50.0f));
		staticShader.setMat4("model", model);
		Arbusto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-200.0f, 0.0f, -100.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(50.0f));
		staticShader.setMat4("model", model);
		Arbusto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-200.0f, 0.0f, -150.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(50.0f));
		staticShader.setMat4("model", model);
		Arbusto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-190.0f, 0.0f, -200.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(50.0f));
		staticShader.setMat4("model", model);
		Arbusto.Draw(staticShader);

		//PLANTA
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-35.0f, 0.0f, 130.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(35.0f));
		staticShader.setMat4("model", model);
		Planta.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(35.0f, 0.0f, 130.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(35.0f));
		staticShader.setMat4("model", model);
		Planta.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-35.0f, 0.0f, 100.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(35.0f));
		staticShader.setMat4("model", model);
		Planta.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(35.0f, 0.0f, 100.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(35.0f));
		staticShader.setMat4("model", model);
		Planta.Draw(staticShader);

		//REJAS
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // -70 200
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		Rejas.Draw(staticShader);

		//REJAS EXTERIOR
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-40.0f, 0.0f, 140.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		Rejas.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, 0.0f, 140.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		Rejas.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(195.0f+rejas_x, 0.0f, 140.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		Rejas.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(230.0f+rejas_x, 0.0f, 140.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		Rejas.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-25.0f, 0.0f, -240.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		Rejas.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 0.0f, -240.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		Rejas.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(127.0f, 0.0f, -240.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		Rejas.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(162.0f, 0.0f, -240.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		Rejas.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(208.0f, 0.0f, -240.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		Rejas.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(500.0f+movAvion_x, 0.0f+movAvion_y, 120.0f+movAvion_z));//70
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		staticShader.setMat4("model", model);
		avionGuerra.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(20.0f, 0.0f, 120.0f));//70
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		staticShader.setMat4("model", model);
		Perro.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(40.0f, 0.0f,-150.0f));//70,50
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(8.0f));
		staticShader.setMat4("model", model);
		camastro.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(60.0f, 0.0f, -150.0f));//70,50
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(8.0f));
		staticShader.setMat4("model", model);
		camastro.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(80.0f, 0.0f, -150.0f));//70,50
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(8.0f));
		staticShader.setMat4("model", model);
		camastro.Draw(staticShader);

		//Camastros del otro lado
		model = glm::translate(glm::mat4(1.0f), glm::vec3(40.0f, 0.0f, -60.0f));//70,50
		model = glm::rotate(model, glm::radians(360.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(8.0f));
		staticShader.setMat4("model", model);
		camastro.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(60.0f, 0.0f, -60.0f));//70
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		Perro.Draw(staticShader);

		//CAMINO
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-900.0f, 0.5f, 220.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(80.0f));
		staticShader.setMat4("model", model);
		Calle.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-430.0f, 1.0f, 220.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(80.0f));
		staticShader.setMat4("model", model);
		Calle.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(40.0f, 1.0f, 220.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(80.0f));
		staticShader.setMat4("model", model);
		Calle.Draw(staticShader);

		//ROTONDA
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-650.0f, 2.0f, 205.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		staticShader.setMat4("model", model);
		Rotonda.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(1100.0f, 2.0f, 205.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		staticShader.setMat4("model", model);
		Rotonda.Draw(staticShader);

		//LAMPARAS CALLE
		model = glm::translate(glm::mat4(1.0f), glm::vec3(200.0f, 0.0f, 160.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(25.0f));
		staticShader.setMat4("model", model);
		Lampara_Calle.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(200.0f, 0.0f, 160.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(25.0f));
		staticShader.setMat4("model", model);
		Lampara_Calle.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-200.0f, 0.0f, 160.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(25.0f));
		staticShader.setMat4("model", model);
		Lampara_Calle.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-200.0f, 0.0f, 160.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(25.0f));
		staticShader.setMat4("model", model);
		Lampara_Calle.Draw(staticShader);

		//MESA
		model = glm::translate(glm::mat4(1.0f), glm::vec3(75.0f, 0.0f, -3.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(15.0f));
		staticShader.setMat4("model", model);
		Mesa.Draw(staticShader);

		//SOFA
		model = glm::translate(glm::mat4(1.0f), glm::vec3(45.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(12.0f));
		staticShader.setMat4("model", model);
		Sofa.Draw(staticShader);

		//SOFA
		model = glm::translate(glm::mat4(1.0f), glm::vec3(75.0f, 0.0f, 28.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(12.0f));
		staticShader.setMat4("model", model);
		Sofa.Draw(staticShader);

		//SILLON
		model = glm::translate(glm::mat4(1.0f), glm::vec3(65.0f, 0.0f, -15.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f));
		staticShader.setMat4("model", model);
		Sillon.Draw(staticShader);

		//CLOSET
		model = glm::translate(glm::mat4(1.0f), glm::vec3(85.0f, 0.0f, -5.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.005f));
		staticShader.setMat4("model", model);
		Closet.Draw(staticShader);

		//FLORERO
		model = glm::translate(glm::mat4(1.0f), glm::vec3(85.0f, 0.0f, -20.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		Florero.Draw(staticShader);

		//RETRETE_1
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-35.0f, 3.0f, -86.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model);
		Retrete.Draw(staticShader);

		//RETRETE_2
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-80.0f, 3.0f, -111.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model);
		Retrete.Draw(staticShader);

		//RETRETE_3
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-42.0f, 53.0f, 10.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model);
		Retrete.Draw(staticShader);

		//TV
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Se agrega transparencia
		model = glm::translate(glm::mat4(1.0f), glm::vec3(95.0f, 25.0f, 6.0f));
		model = glm::rotate(model, glm::radians(-95.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-2.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model);
		TV.Draw(staticShader);
		glEnable(GL_BLEND);

		//ALMOHADA
		model = glm::translate(glm::mat4(1.0f), glm::vec3(30.0f, 50.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.09f));
		staticShader.setMat4("model", model);
		Almohada.Draw(staticShader);

		//CAMA_1
		model = glm::translate(glm::mat4(1.0f), glm::vec3(14.0f, 50.0f, 18.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f));
		staticShader.setMat4("model", model);
		Cama.Draw(staticShader);

		//CAMA_2
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-40.0f, 0.0f, -52.5f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(8.0f));
		staticShader.setMat4("model", model);
		Cama.Draw(staticShader);

		//CAMA_3
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-40.0f, 0.0f, -145.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(8.0f));
		staticShader.setMat4("model", model);
		Cama.Draw(staticShader);

		//LAMPARA
		model = glm::translate(glm::mat4(1.0f), glm::vec3(45.0f, 50.0f, 200.0f));
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f));
		staticShader.setMat4("model", model);
		Lampara.Draw(staticShader);

		//MESA REDONDA
		model = glm::translate(glm::mat4(1.0f), glm::vec3(55.0f, -5.0f, 25.0f));
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.07f));
		staticShader.setMat4("model", model);
		MesaRedonda.Draw(staticShader);

		//COCINA
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-45.0f, 0.0f, 15.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		staticShader.setMat4("model", model);
		Cocina.Draw(staticShader);

		//CUADROS
		model = glm::translate(glm::mat4(1.0f), glm::vec3(7.0f, 35.0f, -15.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f));
		staticShader.setMat4("model", model);
		Cuadro1.Draw(staticShader);

		//CUADROS
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-55.0f, 35.0f, -117.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(4.0f));
		staticShader.setMat4("model", model);
		Cuadro2.Draw(staticShader);

		//CUADROS
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-55.0f, 30.0f, -80.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.7f));
		staticShader.setMat4("model", model);
		Cuadro3.Draw(staticShader);

		//GARAGE
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-140.0f, 0.0f, 100.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(14.0f));
		staticShader.setMat4("model", model);
		Garage.Draw(staticShader);

		//LIBRERO_1
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-70.0f, 5.0f, -45.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		Librero.Draw(staticShader);

		//LIBRERO_2
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-60.0f, 5.0f, -156.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		Librero.Draw(staticShader);

		//LIBRERO_3
		model = glm::translate(glm::mat4(1.0f), glm::vec3(20.0f, 55.0f, 30.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		Librero.Draw(staticShader);

		//TAPETE
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-45.0f, 250.0f, 15.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		staticShader.setMat4("model", model);
		Tapete.Draw(staticShader);


		// -------------------------------------------------------------------------------------------------------------------------
		// Carro
		// -------------------------------------------------------------------------------------------------------------------------
		//model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::translate(model, glm::vec3(15.0f + movAuto_x, -1.0, 325.0 + movAuto_z)); //-1.0f en y
		//tmp = model = glm::rotate(model, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//staticShader.setMat4("model", model);
		//carro.Draw(staticShader);

		//model = glm::translate(tmp, glm::vec3(8.5f, 2.5f, 12.9f));
		//model = glm::rotate(model, glm::radians(giroLlanta), glm::vec3(1.0f, 0.0f, 0.0f)); // animacion de llanta sobre x
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//staticShader.setMat4("model", model);
		//llanta.Draw(staticShader);	//llanta Izq delantera

		//model = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, 12.9f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//model = glm::rotate(model, glm::radians(giroLlanta), glm::vec3(1.0f, 0.0f, 0.0f)); // animacion de llanta sobre x
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//staticShader.setMat4("model", model);
		//llanta.Draw(staticShader);	//llanta Der delantera

		//model = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, -14.5f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//model = glm::rotate(model, glm::radians(giroLlanta), glm::vec3(1.0f, 0.0f, 0.0f)); // animacion de llanta sobre x
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//staticShader.setMat4("model", model);
		//llanta.Draw(staticShader);	//llanta Der trasera

		//model = glm::translate(tmp, glm::vec3(8.5f, 2.5f, -14.5f));
		//model = glm::rotate(model, glm::radians(giroLlanta), glm::vec3(1.0f, 0.0f, 0.0f)); // animacion de llanta sobre x
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//staticShader.setMat4("model", model);
		//llanta.Draw(staticShader);	//llanta Izq trase

		//TESLA
		model = glm::translate(glm::mat4(1.0f), glm::vec3(150.0f, 0.0f, 350.0f));//70
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(8.0f));
		staticShader.setMat4("model", model);
		Tesla.Draw(staticShader);

		//BMW
		model = glm::translate(glm::mat4(1.0f), glm::vec3(160.0f+bmw_x, 0.0f, 60.0f+bmw_z)); // -70 200
		model = glm::rotate(model, glm::radians(bmw_rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(15.0f));
		staticShader.setMat4("model", model);
		BMW.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Caja Transparente --- Siguiente Práctica
		// -------------------------------------------------------------------------------------------------------------------------
		/*glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -70.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		cubo.Draw(staticShader);
		glEnable(GL_BLEND);*/
		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// ------------------------------------------------------------------------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	//To Configure Model
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		posZ++;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		posZ--;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		posX--;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		posX++;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		rotRodIzq--;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		rotRodIzq++;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		giroMonito--;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		giroMonito++;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		movBrazoIzq += 3.0f;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		movBrazoIzq -= 3.0f;
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		posY--;
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
		animacion2 ^= true;
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
		movimientoAvion = 1;
	if (key == GLFW_KEY_Q && action == GLFW_PRESS && movAvion_x > 30.0f) //Con esta tecla el auto regresa 
		movAvion_x = 0.0f;
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		lightDirection.x += 0.5f;
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		lightDirection.x -= 0.5f;
	
	//Car animation
	//if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	//	movimiento = 1;
	//if (key == GLFW_KEY_L && action == GLFW_PRESS && movAuto_z > 30.5f) //Con esta tecla el auto regresa 
	//	movAuto_z = 0.0f;

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		animacion1 ^= true; //XOR que cambia el valor 
	if (key == GLFW_KEY_L && action == GLFW_PRESS && movAuto_z > 200.0f) //no se detecta la tecla dos veces al mismo tiempo por cada ciclo de dibujo se detecta una vez, llegando la final puedo resetear
		movAuto_z = 0.0f;


	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//To Save a KeyFrame
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}