#include "lab_m1/race/course.h"

using namespace std;
using namespace m1;


Course::Course()
{
	SetPolygonPoints();
	ComputeInnerOuterPoints();
	ComputeCourseMesh();
}


void Course::SetPolygonPoints()
{
	polygon_points =
	{
		glm::vec3(0, 0, 8.7),		// C
		glm::vec3(0.5, 0, 8.7),		// M
		glm::vec3(1.2, 0, 8),		// N
		glm::vec3(1.4, 0, 6.5),		// O
		glm::vec3(1.8, 0, 5),		// P
		glm::vec3(3, 0, 4.5),		// Q
		glm::vec3(7.2, 0, 4.9),		// R
		glm::vec3(7.6, 0, 4.2),		// E
		glm::vec3(7.6, 0, 3.4),		// S
		glm::vec3(6.3, 0, 2.4),		// T
		glm::vec3(5.3, 0, 1.7),		// U
		glm::vec3(4.4, 0, 1),		// V
		glm::vec3(4.2, 0, 0.4),		// W
		glm::vec3(4.4, 0, -0.2),	// Z
		glm::vec3(5.2, 0, -2),		// A1
		glm::vec3(6.3, 0, -4.9),	// C1
		glm::vec3(6, 0, -6.3),		// B
		glm::vec3(4, 0, -6),		// E1
		glm::vec3(2, 0, -4),		// F1
		glm::vec3(0.7, 0, -2),		// G1
		glm::vec3(-0.7, 0, -2),		// H1
		glm::vec3(-2, 0, -4),		// I1
		glm::vec3(-4, 0, -6),		// J1
		glm::vec3(-6, 0, -6.3),		// D
		glm::vec3(-6.3, 0, -4.9),	// K1
		glm::vec3(-5.2, 0, -2),		// L1
		glm::vec3(-3.5, 0, 0),		// M1
		glm::vec3(-1.7, 0, -0.2),	// N1
		glm::vec3(0, 0, -0.7),		// O1
		glm::vec3(1.7, 0, 0),		// P1
		glm::vec3(2.4, 0, 1.4),		// Q1
		glm::vec3(2.2, 0, 3),		// R1
		glm::vec3(1, 0, 4),			// S1
		glm::vec3(-1, 0, 4),		// T1
		glm::vec3(-2.2, 0, 3),		// U1
		glm::vec3(-2.4, 0, 1.4),	// V1
		glm::vec3(-3.6, 0, 1),		// W1
		glm::vec3(-5.3, 0, 1.7),	// Z1
		glm::vec3(-6.3, 0, 2.4),	// B2
		glm::vec3(-7.6, 0, 3.4),	// C2
		glm::vec3(-7.6, 0, 4.2),	// F
		glm::vec3(-7.2, 0, 4.9),	// G
		glm::vec3(-3, 0, 4.5),		// H
		glm::vec3(-1.8, 0, 5),		// I
		glm::vec3(-1.4, 0, 6.5),	// J
		glm::vec3(-1.2, 0, 8),		// K
		glm::vec3(-0.5, 0, 8.7),	// L
		glm::vec3(0, 0, 8.7),		// C
	};
}


void Course::ComputeInnerOuterPoints() 
{
	int size = polygon_points.size();
	glm::vec3 P1, P2;	// End points for a segment
	glm::vec3 D;		// Direction vector from P1 to P2
	glm::vec3 P;		// Perpendicular on D
	glm::vec3 up = glm::vec3(0, 1, 0);	// Perpendicular on XoZ plane
	glm::vec3 inner, outer;				// Resulting points

	for (int i = 0; i < size - 1; i++) {
		P1 = polygon_points[i];
		P2 = polygon_points[i + 1];

		D = P2 - P1;
		P = cross(D, up);

		inner = P1 - inner_dist * P;
		outer = P1 + outer_dist * P;

		inner_points.push_back(inner);
		outer_points.push_back(outer);
	}
}


void Course::ComputeCourseMesh() 
{
	vector<VertexFormat> vertices;
	vector<unsigned int> indices;
	int k = 0;

	course = new Mesh("course");
	course->SetDrawMode(GL_TRIANGLE_STRIP);

	for (int i = 0; i < inner_points.size(); i++) {
		vertices.push_back(VertexFormat(inner_points[i], color));
		vertices.push_back(VertexFormat(outer_points[i], color));

		indices.push_back(k++);
		indices.push_back(k++);
	}
	
	course->InitFromData(vertices, indices);
}


Course::~Course()
{
}