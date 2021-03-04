#include "common.h"
#include "Image.h"
#include "Player.h"

#include "Sprite.h"
#include "GameMap.h"

#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <ctime>

constexpr GLsizei WINDOW_WIDTH = W_WIDTH, WINDOW_HEIGHT = W_HEIGHT;

const std::string PATH {"../resources/"};
const std::string HERO_0 {PATH + "Char_one/"};

struct InputState
{
  bool keys[1024]{}; //массив состояний кнопок - нажата/не нажата
  GLfloat lastX = 400, lastY = 300; //исходное положение мыши
  bool firstMouse = true;
  bool captureMouse         = true;  // Мышка захвачена нашим приложением или нет?
  bool capturedMouseJustNow = false;
} static Input;


GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;


void OnKeyboardPressed(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		break;
  case GLFW_KEY_1:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    break;
  case GLFW_KEY_2:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    break;
	default:
		if (action == GLFW_PRESS)
      Input.keys[key] = true;
		else if (action == GLFW_RELEASE)
      Input.keys[key] = false;
	}
}

void processPlayerMovement(Player &player)
{
	bool X = Input.keys[GLFW_KEY_X];
	if (X)player.Attack();
	
	static bool E_down = false;
	bool E = Input.keys[GLFW_KEY_E];
	if (E && !E_down)GameMap::GetCur()->PressE();
	E_down = E;

	static bool I_down = false;
	bool I = Input.keys[GLFW_KEY_I];
	if (I && !I_down)Player::Get().PressI();
	I_down = I;

	static bool LA_down = false;
	bool L_ANG = Input.keys[GLFW_KEY_COMMA];
	if (L_ANG && !LA_down)Player::Get().PressLA();
	LA_down = L_ANG;

	static bool RA_down = false;
	bool R_ANG = Input.keys[GLFW_KEY_PERIOD];
	if (R_ANG && !RA_down)Player::Get().PressRA();
	RA_down = R_ANG; 

	static bool USE_down = false;
	bool USE = Input.keys[GLFW_KEY_U];
	if (USE && !USE_down)Player::Get().PressU();
	USE_down = USE;

	//thrOw
	static bool O_down = false; 
	bool O = Input.keys[GLFW_KEY_O];
	if (O && !O_down)Player::Get().PressO();
	O_down = O;

	bool U = Input.keys[GLFW_KEY_W];
	bool D = Input.keys[GLFW_KEY_S];
	bool L = Input.keys[GLFW_KEY_A];
	bool R = Input.keys[GLFW_KEY_D];
	bool l_ctrl = Input.keys[GLFW_KEY_LEFT_CONTROL];
	player.RefreshMoveState(U, D, L, R, l_ctrl);
}

void OnMouseButtonClicked(GLFWwindow* window, int button, int action, int mods)
{
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    Input.captureMouse = !Input.captureMouse;

  if (Input.captureMouse)
  {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    Input.capturedMouseJustNow = true;
  }
  else
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

}

void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
  if (Input.firstMouse)
  {
    Input.lastX = float(xpos);
    Input.lastY = float(ypos);
    Input.firstMouse = false;
  }

  GLfloat xoffset = float(xpos) - Input.lastX;
  GLfloat yoffset = Input.lastY - float(ypos);

  Input.lastX = float(xpos);
  Input.lastY = float(ypos);
}


void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
  // ...
}


int initGL()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}

	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

  std::cout << "Controls: "<< std::endl;
  std::cout << "press right mouse button to capture/release mouse cursor  "<< std::endl;
  std::cout << "W, A, S, D - movement  "<< std::endl;
  std::cout << "press ESC to exit" << std::endl;

	return 0;
}

inline Pixel lin_interp(Pixel from, Pixel to, double proc)
{
	uint8_t r = (uint8_t)(from.r + (to.r - from.r) * proc);
	uint8_t g = (uint8_t)(from.g + (to.g - from.g) * proc);
	uint8_t b = (uint8_t)(from.b + (to.b - from.b) * proc);
	uint8_t a = 255;// (uint8_t)(from.a + (to.a - from.a) * proc);
	return Pixel {r,g,b,a};
}

Pixel only_r_color(Pixel from, double proc)
{
	uint8_t r = from.r;
	uint8_t g = (uint8_t)(from.g * (1 - proc));
	uint8_t b = (uint8_t)(from.b * (1 - proc));
	uint8_t a = 255;// (uint8_t)(from.a + (to.a - from.a) * proc);
	return Pixel {r,g,b,a};
}

Pixel win_fn(Point p, Pixel from, double proc)
{
	double white_r = proc * W2_DIAG;

	int d = 0;
	if (p.x < W2_WIDTH) d += (W2_WIDTH - p.x) * (W2_WIDTH - p.x);
	if (p.x > W2_WIDTH) d += (p.x - W2_WIDTH) * (p.x - W2_WIDTH);
	if (p.y < W2_HEIGHT) d += (W2_HEIGHT - p.y) * (W2_HEIGHT - p.y);
	if (p.y > W2_HEIGHT) d += (p.y - W2_HEIGHT) * (p.y - W2_HEIGHT);
	if (d <= white_r)return KW_COLOR;
	return lin_interp(from, KW_COLOR, white_r / d);
}

void Image_Draw_SpeedUp_Die(Image& screen_save, Image &canvas, double proc) 
{
	int c_w = screen_save.Width();
	int c_h = screen_save.Height();

	#pragma omp parallel for
	for (int y = 0; y < c_h; y++)
		for (int x = 0; x < c_w; x++) {
			auto from = screen_save.GetPixel(x, y);
			uint8_t r = (uint8_t)(from.r + (127 - from.r) * proc);
			uint8_t g = (uint8_t)(from.g + (20 - from.g) * proc);
			uint8_t b = (uint8_t)(from.b + (27 - from.b) * proc);
			uint8_t a = 255;
			canvas.SetPixel(x, y, Pixel {r,g,b,a});
		}
}

int main(int argc, char **argv)
{
	if (!glfwInit())return -1;

	//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "task1 base project", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, OnKeyboardPressed);
	glfwSetCursorPosCallback(window, OnMouseMove);
	glfwSetMouseButtonCallback(window, OnMouseButtonClicked);
	glfwSetScrollCallback(window, OnMouseScroll);

	if (initGL() != 0)
		return -1;

	//Reset any OpenGL errors which could be present for some reason
	GLenum gl_error = glGetError();
	while (gl_error != GL_NO_ERROR)
		gl_error = glGetError();

	std::srand(std::time(nullptr));

	GameMap gmap {};

	LiveObjSprite player_img {HERO_0, SpritePixSz{16}, 125, 2};
	Player player {gmap.GetPos(GameMap::E_MapPos::Center, Size{.w = 30, .h = 32}), player_img};

	Image screenBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 4);

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  GL_CHECK_ERRORS;
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); GL_CHECK_ERRORS;

	double sec = GameTime::Now().GetTime();
	int frames = 0;

	bool is_alive = true;
	Image for_die = Image::Image(0, 0, 0);

	double die_time = 0;
	double die_duration = 3;
	Player::E_DieType die_type;
	bool last_die_draw = false;
	Image lose_img {PATH + "lose.png"};

	bool win = false;
	double win_time = 0;
	const double win_duration = 3;
	Image win_img {PATH + "win.png"};

	while (!glfwWindowShouldClose(window)) {
		GameTime::Now().SetCur(glfwGetTime());

		if (GameTime::Now().GetTime() - sec > 1) {
			sec = GameTime::Now().GetTime();
			std::cout << "fps : " << frames << std::endl;
			frames = 0;
		}
		frames++;

		glfwPollEvents();

		if (!win) {
			win = gmap.CheckWin();
			if (win) {
				for_die = Image {screenBuffer};
				win_time = GameTime::Now().GetTime();
			}
		}

		if (!win && is_alive) {
			gmap.CheckChangeMap();//TODO: add if(...) for effect
			gmap.EnemiesMove(player.GetCenter());

			gmap.Draw(screenBuffer);

			processPlayerMovement(player);
			is_alive = !player.GetIsDied();

			if (!is_alive) { // die
				for_die = Image {screenBuffer};
				die_time = GameTime::Now().GetTime();
				die_type = player.GetDiedType();
			} else {
				player.Draw(screenBuffer);
				player.InventoryDraw(screenBuffer);
			}
		}
		if (!is_alive) {
			double proc = GameTime::Now().GetSecAfter(die_time) / die_duration;
			if (proc >= 1) {
				if (last_die_draw) break;
				last_die_draw = true;
			}

			if (die_type == Player::E_DieType::Kill)
				for_die.Draw(screenBuffer, [&](auto x) {return only_r_color(x, proc); });
			else if (die_type == Player::E_DieType::EmptyStay)
				for_die.Draw(screenBuffer, [&](auto x) {return lin_interp(x, Pixel {27, 20, 27, 255}, proc); });
			//Image_Draw_SpeedUp_Die(for_die, screenBuffer, proc);

			player.DieDraw(screenBuffer, proc);
			//lose_img.Draw(screenBuffer, Point {.x = (W_WIDTH - lose_img.Width()) / 2, .y = (W_HEIGHT - lose_img.Height()) / 2}, true);
		}

		if (win) {
			double proc = GameTime::Now().GetSecAfter(win_time) / win_duration;
			if (proc >= 1)break;
			for_die.Draw(screenBuffer, [&](auto pos, auto pix) {return win_fn(pos, pix, proc); });
			win_img.Draw(screenBuffer, Point {.x = (W_WIDTH - win_img.Width()) / 2, .y = (W_HEIGHT - win_img.Height()) / 2}, true);

		}
		/*TODO:DEL +++*/
		//auto pos = player.GetPos();
		//screenBuffer.SetPixel(pos.x + 12, pos.y + 1, Pixel {.r = 255,  .g = 0, .b = 0, .a = 255,});
		//screenBuffer.SetPixel(pos.x + 19, pos.y + 1, Pixel {.r = 255,  .g = 0, .b = 0, .a = 255,});
		/*TODO:DEL ---*/

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
		glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}
