// not working
void generate_sphere_buffer(int lats, int longs, GLfloat ** buffer_data, int * verts)
{
	int i, j;
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;
	int indicator = 0;
	for (i = 0; i <= lats; i++) {
		double lat0 = glm::pi<double>() * (-0.5 + (double)(i - 1) / lats);
		double z0 = sin(lat0);
		double zr0 = cos(lat0);

		double lat1 = glm::pi<double>() * (-0.5 + (double)i / lats);
		double z1 = sin(lat1);
		double zr1 = cos(lat1);

		for (j = 0; j <= longs; j++) {
			double lng = 2 * glm::pi<double>() * (double)(j - 1) / longs;
			double x = cos(lng);
			double y = sin(lng);

			vertices.push_back(x * zr0);
			vertices.push_back(y * zr0);
			vertices.push_back(z0);
			indices.push_back(indicator);
			indicator++;

			vertices.push_back(x * zr1);
			vertices.push_back(y * zr1);
			vertices.push_back(z1);
			indices.push_back(indicator);
			indicator++;
		}
		indices.push_back(GL_PRIMITIVE_RESTART_FIXED_INDEX);
	}
	*verts = indices.size();
	*buffer_data = &vertices[0];
}

void draw_new(GLuint * vert_b, GLfloat * vert_b_data, GLuint * colour_b, GLfloat * colour_b_data, int n)
{
	glGenBuffers(1, vert_b);

	glBindBuffer(GL_ARRAY_BUFFER, *vert_b);
	glBufferData(GL_ARRAY_BUFFER, n*sizeof(glm::vec3), vert_b_data, GL_STATIC_DRAW);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, *vert_b);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	
	glGenBuffers(1, colour_b);
	glBindBuffer(GL_ARRAY_BUFFER, *colour_b);
	glBufferData(GL_ARRAY_BUFFER, n*sizeof(glm::vec3), colour_b_data, GL_STATIC_DRAW);

	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, *colour_b);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, n*sizeof(glm::vec3)); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void init(GLuint * vao, GLuint * vert_b, GLfloat * vert_b_data, GLuint * colour_b, GLfloat * colour_b_data, int n)
{
	glGenVertexArrays(1, vao);
	glBindVertexArray(*vao);

	glGenBuffers(1, vert_b);

	glBindBuffer(GL_ARRAY_BUFFER, *vert_b);
	glBufferData(GL_ARRAY_BUFFER, n * sizeof(glm::vec3), vert_b_data, GL_STATIC_DRAW);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, *vert_b);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glGenBuffers(1, colour_b);
	glBindBuffer(GL_ARRAY_BUFFER, *colour_b);
	glBufferData(GL_ARRAY_BUFFER, n * sizeof(glm::vec3), colour_b_data, GL_STATIC_DRAW);

	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, *colour_b);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);
	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void draw(GLuint * vao, int n)
{
	glBindVertexArray(*vao);
	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, n * sizeof(glm::vec3)); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glBindVertexArray(0);
}

//Holds buffers, array object and data in one structure.
//Able to draw itself and initilise buffers.
struct Entity2
{
private:
	GLuint vert_b, colour_b, vao;

public:
	GLfloat * v_b, * c_b;
	int n;
	Particle p;
	bool is_static;

	Entity2() {}

	Entity2(GLfloat * v, GLfloat * c, int _n);

	void init();

	void draw();
};

Entity2::Entity2(GLfloat * v, GLfloat * c, int _n)
{
	v_b = v;
	c_b = c;
	n = _n;
}
void Entity2::init()
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
	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}
void Entity2::draw()
{
	glBindVertexArray(vao);
	// Draw the triangle !
	glDrawArrays(GL_TRIANGLE_FAN, 0, n * sizeof(glm::vec3)); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glBindVertexArray(0);
}
//Randomises the colour buffer passed
void random_colour_buffer_floats(GLfloat ** buffer_data, int n)
{
	*buffer_data = (GLfloat*)std::malloc(n * 3 * sizeof(GLfloat));
	for (int v = 0; v < n; v++)
		for (int i = 0; i < 3; i++)
			(*buffer_data)[3 * v + i] = randf();
}

int generate_cone2(GLfloat ** buffer_data)
{
	int lod = 32;
	*buffer_data = (GLfloat*)std::malloc((lod+3) * 3 * sizeof(GLfloat));
	(*buffer_data)[0] = 0.;
	(*buffer_data)[1] = 0.;
	(*buffer_data)[2] = 0.;
	int i = 3;
	float step = 2. * 3.141596 / float(lod);
	float Radius = 1.;
	for (float a = 0; a <= (2. * 3.141596 + step); a += step) {
		float c = Radius * cos(a);
		float s = Radius * sin(a);
		(*buffer_data)[i] = c;
		(*buffer_data)[i + 1] = s;
		(*buffer_data)[i + 2] = 2.0;
		i += 3;
	}
	return i;
}

struct Entity_Generator
{
	Entity clone;
	std::vector<Entity> entities;
	int max;
	glm::vec3 * pos;
	bool gen_on = 1;

	Entity_Generator() {};

	Entity_Generator(Entity clone_, int max_, glm::vec3 * pos_)
	{
		max = max_;
		pos = pos_;
		clone = clone_;
		Entity e;
		for (int i = 0; i < max; ++i)
		{
			e = clone.clone();
			e.p.pos = *pos;
			e.p.vel += glm::vec3(randf(), randf(), randf());
			entities.push_back(e);
			//Print(e.p.vel);
			std::cout << &e << std::endl;
		}
		for (Entity e : entities)
			std::cout << &e << std::endl;

	}

	void update(float dt)
	{
		for (Entity e : entities)
		{
			e.p.update(dt);
			if(0)
			if (e.p.life < 0)
			{
				e.p.pos = *pos;
				e.p.life = e.p.life_time;
			}
		}
	}

	void draw()
	{
		for (Entity e : entities)
			e.draw();
	}


};


void init(GLuint * vao, GLuint * vbo, std::vector<Vertex> * object)
{
	glGenVertexArrays(1, vao);
	glBindVertexArray(*vao);
	/* Allocate and assign One Vertex Buffer Object to our handle */
	glGenBuffers(1, vbo);
	/* Bind our VBO as being the active buffer and storing vertex attributes (coordinates + colors) */
	glBindBuffer(GL_ARRAY_BUFFER, *vbo);
	/* Copy the vertex data from tetrahedron to our buffer */
	/* 12 * sizeof(GLfloat) is the size of the tetrahedrom array, since it contains 12 Vertex values */
	glBufferData(GL_ARRAY_BUFFER, object->size() * sizeof(struct Vertex), object->data(), GL_STATIC_DRAW);
	/* Specify that our coordinate data is going into attribute index 0, and contains three doubles per vertex */
	/* Note stride = sizeof ( struct Vertex ) and pointer = ( const GLvoid* ) 0 */
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (const GLvoid*)offsetof(struct Vertex, position));
	/* Enable attribute index 0 as being used */
	glEnableVertexAttribArray(0);
	/* Specify that our color data is going into attribute index 1, and contains three floats per vertex */
	/* Note stride = sizeof ( struct Vertex ) and pointer = ( const GLvoid* ) ( 3 * sizeof ( GLdouble ) ) i.e. the size (in bytes)
	occupied by the first attribute (position) */
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (const GLvoid*)offsetof(struct Vertex, color)); // bug );
																																   /* Enable attribute index 1 as being used */
	glEnableVertexAttribArray(1);/* Bind our second VBO as being the active buffer and storing vertex attributes (colors) */

	glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (const GLvoid*)offsetof(struct Vertex, normal)); // bug );
																																	/* Enable attribute index 1 as being used */
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glFlush();
}
void draw(GLuint * vao, std::vector<Vertex> * object)
{
	glm::mat4 Model = glm::translate(glm::mat4(1.), glm::vec3()) * glm::rotate(glm::mat4(1.), (float)(3.141f/2.0f), glm::vec3(1, 0, 0));
	mat4_handles[0].load(Model);
	mat4_handles[1].load();
	mat4_handles[2].load();

	ambient_color_handle.load();

	for (Var_Handle v : light_handles)
		v.load();

	glBindVertexArray(*vao);
	glDrawArrays(wire_frame ? GL_LINE_LOOP : GL_TRIANGLES, 0, object->size());
	glBindVertexArray(NULL);

	glFinish();
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
void random_alpha_uniform_colour_buffer(glm::vec3 ** buffer_data, int n, glm::vec3 colour1,glm::vec3 colour2)
{
	*buffer_data = (glm::vec3*)std::malloc(n * sizeof(glm::vec3));
	for (int v = 0; v < n; v++)
		(*buffer_data)[v] = (colour1 + (colour2 - colour1)*randf());
}
//Randomises the colour buffer passed


struct CModel
{
	glm::vec3
		rotation = glm::vec3(1, 0, 0),
		scale = glm::vec3(1, 1, 1);
	GLfloat theta;
	Particle p;
	std::vector<GLuint> vaos;
	std::vector<int> indecies;
	TinyMesh * tm;

	CModel() {}

	CModel(TinyMesh * _tm)
	{
		tm = _tm;
	}

	void init();
	void render();
};


void CModel::init()
{
	GLuint vbo, vao;
	for (int i = 0; i< tm->Number_Shapes(); i++) {
		/* Vertex Array Object  */
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		/* Vertex Buffer Object */
		glGenBuffers(1, &vbo);
		/* */
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		GLuint data_count = tm->Number_Positions(i) * sizeof(float);
		glBufferData(GL_ARRAY_BUFFER, data_count, tm->Positions(i), GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);

		Check("making vao");
		GLuint elementbuffer;
		glGenBuffers(1, &elementbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
		GLuint element_count = tm->shapes[0].mesh.indices.size() * sizeof(unsigned int);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, element_count, tm->shapes[i].mesh.indices.data(), GL_STATIC_DRAW);
		glBindVertexArray(0);
		indecies.push_back(tm->Number_Indices(i));
		vaos.push_back(vao);
	}
}
void CModel::render()
{
	glm::mat4 Model =
		glm::translate(glm::mat4(1.), p.pos) *
		glm::rotate(glm::mat4(1.), theta, rotation) *
		glm::scale(glm::mat4(1.), scale);
	mat4_handles[0].load(Model);
	mat4_handles[1].load();
	mat4_handles[2].load();

	ambient_color_handle.load();

	for (Var_Handle v : light_handles)
		v.load();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for (int i = 0; i < tm->Number_Shapes(); i++) {
		glBindVertexArray(vaos[i]);
		glDrawElements(
			GL_TRIANGLES, // mode
			indecies.data()[i], // count
			GL_UNSIGNED_INT, // type
			(void*)0 // element array buffer offset
		); /* Invoke glDrawArrays telling that our data consists of individual triangles */
		glBindVertexArray(0);
	}
}


