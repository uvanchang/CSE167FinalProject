#include "BezierCurve.h"

BezierCurve::BezierCurve(glm::vec3 controlPoints[4], GLuint shader)
	: program(shader)
{
	setBasisVectors();
	setControlPoints(controlPoints);
}

BezierCurve::~BezierCurve()
{

}

void BezierCurve::setControlPoints(glm::vec3 controlPoints[4])
{
	glm::vec3 p0 = controlPoints[0];
	glm::vec3 p1 = controlPoints[1];
	glm::vec3 p2 = controlPoints[2];
	glm::vec3 p3 = controlPoints[3];

	float control_points[16] = // Column major
	{
		p0.x, p0.y, p0.z, 0,
		p1.x, p1.y, p1.z, 0,
		p2.x, p2.y, p2.z, 0,
		p3.x, p3.y, p3.z, 0,
	};
	Gbez = glm::make_mat4(control_points);
	C = Gbez * Bbez;
}

void BezierCurve::updateControlPoint(int index, glm::vec3 controlPoint)
{
	Gbez[index] = glm::vec4(controlPoint, 0.0f);
	C = Gbez * Bbez;
}

void BezierCurve::setBasisVectors()
{
	float basis_vectors[16] = // Row major = Column major
	{
		-1, 3, -3, 1,
		3, -6, 3, 0,
		-3, 3, 0, 0,
		1, 0, 0, 0
	};
	Bbez = glm::make_mat4(basis_vectors);
}

glm::vec3 BezierCurve::parametricCurve(float t)
{
	glm::vec4 T = glm::vec4(pow(t, 3), pow(t, 2), t, 1);
	return C * T;
}

glm::vec3 BezierCurve::getPoint(float t)
{
	return parametricCurve(t);
}

void BezierCurve::update()
{
	for (unsigned int i = 0; i < 150; ++i) {
		points[i] = getPoint(i/149.0f);
	}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void BezierCurve::render()
{
	glUseProgram(program);

	glm::mat4 model = glm::mat4(1);
	glm::vec3 color = glm::vec3(0, 0, 0);
	GLuint modelLoc = glGetUniformLocation(program, "model");
	GLuint colorLoc = glGetUniformLocation(program, "color");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(colorLoc, 1, glm::value_ptr(color));

	// Bind to the VAO.
	glBindVertexArray(vao);
	// Draw points 
	glDrawArrays(GL_LINE_STRIP, 0, sizeof(points)/sizeof(points[0]));
	// Unbind from the VAO.
	glBindVertexArray(0);
}