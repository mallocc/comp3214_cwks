#include "old_cwk1.h"


using namespace glm;

//screens
Screen screen1;

//colours
glm::vec3 WHITE(1, 1, 1), BLACK(0, 0, 0), GREY(.5, .5, .5), OFF_BLACK(.1, .1, .1);

//entities
Entity cube, cone, cylinder, sphere, ground;
std::vector<Entity> spheres;

// composite entities
Composite_Entity rocket;

//light
Light lights = { glm::vec3(100,100,0),glm::vec3(1,1,1),10000,0.5,200 };

vec3 ambient_color = OFF_BLACK;

//Window object  
GLFWwindow* window;
//Shader program
GLuint 	program_id;
//handles to shader
GLuint 	mvp_handle, m_handle, v_handle, p_handle, light_handle, eye_handle;
GLuint lights_handles[1][5], ambient_handle;

Var_Handle mat4_handles[3], light_handles[5];

//Shader paths
const char
* vertex_shader = "tut.vert",
*fragment_shader = "tut.frag";

//proj and view matrix
glm::mat4 Projection, View;

//Window dimensions
const int width = 1280, height = 720;
// eye position
glm::vec3 position = glm::vec3(0, 0, 50);
glm::vec3 eye_direction = glm::vec3(0, -1, 0);
// horizontal angle toward -Z
float horizontalAngle = 3.14f;
// vertical angle : 0, look at the horizon
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;
//Speed variable
float speed = 20.0f; // 3 units / second
					 //Mouse sensitivity
float mouseSpeed = 0.1f;
//Time delta
float dt = 0.05;
//FPS toggle
bool fps_on = 0;
//Camera vectors
glm::vec3 right, up(0, 1, 0), direction;
//wire frame toggle
bool wire_frame = 0;

int test1 = 0;


//shape generators non indexed triangles
//Cube vertex data array
GLfloat cube_v_b[] = {
	-1.0f,-1.0f,-1.0f, // triangle 1 : begin
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, // triangle 1 : end
	1.0f, 1.0f,-1.0f, // triangle 2 : begin
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f, // triangle 2 : end
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f
};
std::vector<glm::vec3> generate_cube()
{
	std::vector<glm::vec3> v;
	for (int i = 0; i < 36; i++)
		v.push_back(glm::vec3(cube_v_b[i * 3], cube_v_b[i * 3 + 1], cube_v_b[i * 3 + 2]));
	return v;
}
std::vector<glm::vec3> generate_cone(int k)
{
	std::vector<glm::vec3> v;
	float step = 2. * 3.141596 / float(k);
	float Radius = 1., c = 0., s = 0.;
	float len = 2.;
	for (float a = 0; a <= (2. * 3.141596); a += step)
	{
		v.push_back(vec3());
		c = Radius * cos(a);
		s = Radius * sin(a);
		v.push_back(vec3(c, s, len));
		c = Radius * cos(a - step);
		s = Radius * sin(a - step);
		v.push_back(vec3(c, s, len));
	}
	for (float a = 0; a <= (2. * 3.141596); a += step)
	{
		c = Radius * cos(a - step);
		s = Radius * sin(a - step);
		v.push_back(vec3(c, s, len));
		c = Radius * cos(a);
		s = Radius * sin(a);
		v.push_back(vec3(c, s, len));
		v.push_back(vec3(0., 0., len));
	}
	return v;
}
std::vector<glm::vec3> generate_cylinder(int k, float len)
{
	std::vector<glm::vec3> v;
	glm::vec3 t1, t2;
	float step = 2. * 3.141596 / float(k);
	float Radius = 1., c = 0., s = 0.;
	for (float a = 0; a <= (2. * 3.141596); a += step)
	{
		v.push_back(vec3(0., 0., 0));
		c = Radius * cos(a);
		s = Radius * sin(a);
		v.push_back(vec3(c, s, 0));
		c = Radius * cos(a - step);
		s = Radius * sin(a - step);
		v.push_back(vec3(c, s, 0));
	}
	for (float a = 0; a > -(2. * 3.141596); a -= step)
	{
		c = Radius * cos(a);
		s = Radius * sin(a);
		t1 = vec3(c, s, 0);
		t2 = vec3(c, s, len);
		c = Radius * cos(a - step);
		s = Radius * sin(a - step);

		v.push_back(t1);
		v.push_back(t2);
		v.push_back(vec3(c, s, len));
		v.push_back(vec3(c, s, len));
		v.push_back(vec3(c, s, 0));
		v.push_back(t1);
	}
	for (float a = 0; a <= (2. * 3.141596); a += step)
	{
		c = Radius * cos(a - step);
		s = Radius * sin(a - step);
		v.push_back(vec3(c, s, len));
		c = Radius * cos(a);
		s = Radius * sin(a);
		v.push_back(vec3(c, s, len));
		v.push_back(vec3(0., 0., len));
	}
	return v;
}
std::vector<glm::vec3> generate_sphere(int lats, int longs)
{
	std::vector<glm::vec3> v;
	glm::vec3 t1, t2;
	float len = 1.;
	float step_lats = 2. * 3.141596 / float(lats);
	float step_longs = 2. * 3.141596 / float(longs);
	float Radius = 1., x, y, z;
	for (float a = step_lats; a <= (2. * 3.141596); a += step_lats)
	{
		for (float b = step_longs; b <= (2. * 3.141596); b += step_longs)
		{
			x = Radius * cos(a) * cos(b);
			y = Radius * cos(a) * sin(b);
			z = Radius * sin(a);
			v.push_back(glm::vec3(x, y, z));
			x = Radius * cos(a + step_lats) * cos(b);
			y = Radius * cos(a + step_lats) * sin(b);
			z = Radius * sin(a + step_lats);
			v.push_back(glm::vec3(x, y, z));
			x = Radius * cos(a + step_lats) * cos(b + step_longs);
			y = Radius * cos(a + step_lats) * sin(b + step_longs);
			z = Radius * sin(a + step_lats);
			v.push_back(glm::vec3(x, y, z));

			v.push_back(glm::vec3(x, y, z));
			x = Radius * cos(a) * cos(b + step_longs);
			y = Radius * cos(a) * sin(b + step_longs);
			z = Radius * sin(a);
			v.push_back(glm::vec3(x, y, z));
			x = Radius * cos(a) * cos(b);
			y = Radius * cos(a) * sin(b);
			z = Radius * sin(a);
			v.push_back(glm::vec3(x, y, z));
		}
	}

	return v;
}
std::vector<glm::vec3> generate_normals(std::vector<glm::vec3> v)
{
	std::vector<glm::vec3> n;
	for (int i = 0; i < v.size(); i += 3)
	{
		glm::vec3 nm = glm::normalize(glm::cross(v[i + 1] - v[i], v[i + 2] - v[i]));
		for (int j = 0; j < 3; ++j)
			//n.push_back(-v[i]);
			n.push_back(nm);
	}
	return n;
}

std::vector<glm::vec3> generate_rect()
{
	std::vector<glm::vec3> n;
	n.push_back(glm::vec3(1, 0, 1));
	n.push_back(glm::vec3(1, 0, -1));
	n.push_back(glm::vec3(-1, 0, -1));
	n.push_back(glm::vec3(-1, 0, -1));
	n.push_back(glm::vec3(-1, 0, 1));
	n.push_back(glm::vec3(1, 0, 1));
	return n;
}

//entity struct bodies
void Entity::init()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vert_b);
	glBindBuffer(GL_ARRAY_BUFFER, vert_b);
	glBufferData(GL_ARRAY_BUFFER, n * sizeof(glm::vec3), v_b, GL_STATIC_DRAW);
	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vert_b);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glGenBuffers(1, &colour_b);
	glBindBuffer(GL_ARRAY_BUFFER, colour_b);
	glBufferData(GL_ARRAY_BUFFER, n * sizeof(glm::vec3), c_b, GL_STATIC_DRAW);
	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colour_b);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	glGenBuffers(1, &normal_b);
	glBindBuffer(GL_ARRAY_BUFFER, normal_b);
	glBufferData(GL_ARRAY_BUFFER, n * sizeof(glm::vec3), n_b, GL_STATIC_DRAW);
	// 3rd attribute buffer : nromals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normal_b);
	glVertexAttribPointer(
		2,                                // attribute. No particular reason for 2, but must match the layout in the shader.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glFlush();

	printf("%i %i %i\n", vert_b, colour_b, normal_b);
}
void Entity::draw()
{
	glm::mat4 Model = glm::translate(glm::mat4(1.), p.pos) * glm::rotate(glm::mat4(1.), theta, rotation) * glm::scale(glm::mat4(1.), scale);
	glm::mat4 mvp = Projection * View * Model;
	mat4_handles[0].load(Model);
	mat4_handles[1].load(View);
	mat4_handles[2].load(Projection);

	glUniform3f(ambient_handle, ambient_color.x, ambient_color.y, ambient_color.z);

	light_handles[0].load(lights.pos);
	light_handles[1].load(lights.color);
	light_handles[2].load(lights.brightness);
	light_handles[3].load(lights.shininess);
	light_handles[4].load(lights.specular_scale);

	if (p.life > 0)
	{
		glBindVertexArray(vao);
		glDrawArrays(wire_frame ? GL_LINE_LOOP : GL_TRIANGLES, 0, n);
		glBindVertexArray(0);
	}
}
void Entity::draw_array(int wire_frame)
{
	if (p.life > 0)
	{
		glBindVertexArray(vao);
		glDrawArrays(wire_frame ? GL_LINE_LOOP : GL_TRIANGLES, 0, n);
		glBindVertexArray(0);
	}
}

//composite entity bodies
void Composite_Entity::draw(int wire_frame)
{
	glm::mat4 comp_Model = glm::translate(glm::mat4(1.), p.pos) * glm::rotate(glm::mat4(1.), theta, rotation) * glm::scale(glm::mat4(1.), scale);
	mat4_handles[1].load(View);
	mat4_handles[2].load(Projection);

	glUniform3f(ambient_handle, ambient_color.x, ambient_color.y, ambient_color.z);

	light_handles[0].load(lights.pos);
	light_handles[1].load(lights.color);
	light_handles[2].load(lights.brightness);
	light_handles[3].load(lights.shininess);
	light_handles[4].load(lights.specular_scale);

	for (Entity e : entities)
	{
		glm::mat4 Model = glm::translate(glm::mat4(1.), e.p.pos) * glm::rotate(glm::mat4(1.), e.theta, e.rotation);
		Model = comp_Model * Model;
		//glUniformMatrix4fv(m_handle, 1, GL_FALSE, &Model[0][0]);
		mat4_handles[0].load(Model);
		e.draw_array(wire_frame);
	}
}
void Composite_Entity::add(Entity e)
{
	entities.push_back(e);
}

//Randomises the colour buffer passed
void random_colour_buffer(glm::vec3 ** buffer_data, int n)
{
	*buffer_data = (glm::vec3*)std::malloc(n * sizeof(glm::vec3));
	for (int v = 0; v < n; v++)
		(*buffer_data)[v] = glm::vec3(randf(), randf(), randf());
}
//Randomises the colour buffer passed
void random_alpha_colour_buffer(glm::vec3 ** buffer_data, int n, glm::vec3 colour)
{
	*buffer_data = (glm::vec3*)std::malloc(n * sizeof(glm::vec3));
	for (int v = 0; v < n; v++)
		(*buffer_data)[v] = glm::vec3(colour.x*randf(), colour.y*randf(), colour.z*randf());
}
//Randomises the colour buffer passed
void generate_colour_buffer(glm::vec3 ** buffer_data, int n, glm::vec3 colour)
{
	*buffer_data = (glm::vec3*)std::malloc(n * sizeof(glm::vec3));
	for (int v = 0; v < n; v++)
		(*buffer_data)[v] = colour;
}

//Randomises the colour buffer passed
void random_alpha_uniform_colour_buffer(glm::vec3 ** buffer_data, int n, glm::vec3 colour1, glm::vec3 colour2)
{
	*buffer_data = (glm::vec3*)std::malloc(n * sizeof(glm::vec3));
	for (int v = 0; v < n; v++)
		(*buffer_data)[v] = (colour1 + (colour2 - colour1)*randf());
}


//Error callback  
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}
//Loads shaders from their files into a shader program (from opengl-tutorials.org)
GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

void reset_rocket()
{
	rocket.p.pos = glm::vec3();
	rocket.p.vel = glm::vec3();
}

//Initilise custom objects
void init_objects()
{
	//std::vector<vec3> v = generate_cube();
	//cube.n_b = generate_normals(v).data();
	//cube.v_b = v.data();
	//cube.n = v.size();
	////printf("%i", cube.n);
	//random_colour_buffer(&cube.c_b, cube.n);
	//cube.p.pos = glm::vec3(0, 0., 0);
	//cube.init();

	// setup rocket model shapes

	std::vector<vec3> v1 = generate_cone(100);
	cone.n_b = generate_normals(v1).data();
	cone.v_b = v1.data();
	cone.n = v1.size();
	generate_colour_buffer(&cone.c_b, cone.n, WHITE);
	cone.p.pos = glm::vec3(0, 0, 0);
	cone.p.vel = glm::vec3(0, 0, 0);
	cone.rotation = vec3(1, 0, 0);
	cone.theta = 3.141596 / 2;
	cone.init();

	rocket.add(cone);

	std::vector<vec3> v2 = generate_cylinder(100, 5);
	cylinder.n_b = generate_normals(v2).data();
	cylinder.v_b = v2.data();
	cylinder.n = v2.size();
	generate_colour_buffer(&cylinder.c_b, cylinder.n, WHITE);
	cylinder.p.pos = glm::vec3(0, -2, 0);
	cylinder.rotation = vec3(1, 0, 0);
	cylinder.theta = 3.141596 / 2;
	cylinder.init();

	rocket.add(cylinder);

	std::vector<vec3> v3 = generate_sphere(30, 31);
	sphere.n_b = generate_normals(v3).data();
	sphere.v_b = v3.data();
	sphere.n = v3.size();
	generate_colour_buffer(&sphere.c_b, sphere.n, WHITE);
	sphere.p.pos = glm::vec3(0, -7, 0);
	sphere.init();

	rocket.add(sphere);

	v1 = generate_cone(100);
	cone.n_b = generate_normals(v1).data();
	cone.v_b = v1.data();
	cone.n = v1.size();
	generate_colour_buffer(&cone.c_b, cone.n, WHITE);
	cone.p.pos = glm::vec3(0, -7, 0);
	cone.p.vel = glm::vec3(0, 0, 0);
	cone.rotation = vec3(1, 0, 0);
	cone.theta = 3.141596 / 2;
	cone.init();

	rocket.add(cone);

	rocket.scale *= 1.0f;
	rocket.p.pos = vec3(0, 0, 0);

	// setup ground shape

	std::vector<vec3> v4 = generate_rect();
	ground.n_b = generate_normals(v4).data();
	ground.v_b = v4.data();
	ground.n = v4.size();
	generate_colour_buffer(&ground.c_b, ground.n, WHITE);
	ground.p.pos = glm::vec3(0, -10, 0);
	ground.scale *= 10.0f;
	ground.init();

	//int dist = 15;
	//for (int i = 0; i < 100; ++i)
	//{
	//	std::vector<vec3> v3 = generate_cube();//generate_sphere(16, 16);
	//	Entity s;
	//	s.n_b = generate_normals(v3).data();
	//	s.v_b = v3.data();
	//	s.n = v3.size();
	//	generate_colour_buffer(&s.c_b, s.n, glm::vec3(.5, .5, .5));
	//	s.p.pos = glm::vec3(randf()*dist- dist/2, randf()*dist - dist/2, randf() * dist - dist/2);
	//	s.init();
	//	spheres.push_back(s);
	//}


}
//Key input callback  
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	//strafing is always orthoganol to the eye position vector
	// Move forward
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		position += direction * dt * speed;
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		position -= direction * dt * speed;
	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		position += right * dt * speed;
	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		position -= right * dt * speed;
	}

	// FPS toggle
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
		fps_on = !fps_on;
	}

	// FPS toggle
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		reset_rocket();
	}

	// FPS toggle
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		wire_frame = !wire_frame;
	}
	// FPS toggle
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		test1++;
	}
}
//Custom graphics loop
void loop()
{

	// update and draw rocket model
	rocket.p.vel += glm::vec3(0,0.01,0) * dt;
	rocket.p.update(dt);
	rocket.draw(wire_frame);
	eye_direction = rocket.p.pos;

	ground.draw();
	//screen1.draw();
}


void setup_program_handles()
{
	ambient_handle = glGetUniformLocation(program_id, "ambient_color");

	mat4_handles[0] = Var_Handle("M");
	mat4_handles[1] = Var_Handle("V");
	mat4_handles[2] = Var_Handle("P");
	for (int i = 0; i < 3; ++i)
		mat4_handles[i].init(program_id);

	light_handles[0] = Var_Handle("light", &lights.pos);
	light_handles[1] = Var_Handle("diffuse_color", &lights.color);
	light_handles[2] = Var_Handle("brightness", &lights.brightness);
	light_handles[3] = Var_Handle("shininess", &lights.shininess);
	light_handles[4] = Var_Handle("specular_scale", &lights.specular_scale);

	for (int i = 0; i < 5; ++i)
		light_handles[i].init(program_id);
}
//GL window initialose
int initWindow()
{
	//Set the error callback  
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW  
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional  
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version  
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version  
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing  
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create a window and create its OpenGL context  
	window = glfwCreateWindow(width, height, "Test Window", NULL, NULL);

	//If the window couldn't be created  
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread.   
	glfwMakeContextCurrent(window);

	//Sets the key callback  
	glfwSetKeyCallback(window, key_callback);

	//Initialize GLEW  
	GLenum err = glewInit();

	//If GLEW hasn't initialized  
	if (err != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		return -1;
	}

	program_id = LoadShaders(vertex_shader, fragment_shader);
	//glUseProgram(program_id);

	setup_program_handles();

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	init_objects();

	std::vector < Var_Handle> h;
	for (int i = 0; i < 5; i++)
		h.push_back(light_handles[i]);
	std::vector < Entity> e;
	e.push_back(ground);
	std::vector < Composite_Entity> ce;
	ce.push_back(rocket);

	screen1 = Screen(
		program_id,
		h,
		e,
		ce,
		width, height,
		position,
		eye_direction,
		up,
		dt);
	screen1.init();

}
//GL graphics loop
void glLoop(void(*graphics_loop)())
{
	//Main Loop  
	do
	{
		auto start = std::chrono::high_resolution_clock::now();

		direction = -glm::normalize(position);
		right = glm::normalize(glm::cross(direction, up));

		//position = glm::quat(glm::vec3(0.05 * dt, -0.1*dt, 0)) * position;

		//lights[0].pos = glm::quat(glm::vec3(0.0 * dt, 0.1*dt, -0.0*dt)) * lights[0].pos;

		// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		Projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 1000.0f);

		// Or, for an ortho camera :
		//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

		// Camera matrix
		View = glm::lookAt(
			position,
			eye_direction,
			up
		);

		//Clear color buffer  
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::vec3 back_color = ambient_color * 1.0f;
		glClearColor(back_color.x, back_color.y, back_color.z, 1.);

		graphics_loop();

		//Swap buffers  
		glfwSwapBuffers(window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...  
		glfwPollEvents();


		auto finish = std::chrono::high_resolution_clock::now();
		int ms = float(std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count());
		long newWait = 5 - ms;// -(gm.gameSpeed);
		newWait = newWait < 0 ? 0 : newWait;
		std::this_thread::sleep_for(std::chrono::milliseconds(newWait));

	} //Check if the ESC key had been pressed or if the window had been closed  
	while (!glfwWindowShouldClose(window));

	//Close OpenGL window and terminate GLFW  
	glfwDestroyWindow(window);
	//Finalize and clean up GLFW  
	glfwTerminate();

	exit(EXIT_SUCCESS);
}

//cwk1 main function
int old_cwk1_main()
{
	initWindow();
	glLoop(loop);

	return 0;
}