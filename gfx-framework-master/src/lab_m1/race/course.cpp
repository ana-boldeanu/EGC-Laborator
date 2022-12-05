#include "lab_m1/race/course.h"

using namespace std;
using namespace m1;


Course::Course()
{
	SetPolygonPoints();
	ComputeIntermediaryPoints();
	ComputeInnerOuterPoints();
	ComputeInnerOuterPointsExtended();
	ComputeCourseMesh();
	ComputeLinesMesh();
}


float Course::TriangleArea(glm::vec2 A, glm::vec2 B, glm::vec2 C)
{
	glm::vec2 AB = glm::vec2(B.x - A.x, B.y - A.y);
	glm::vec2 AC = glm::vec2(C.x - A.x, C.y - A.y);
	float cross_prod = AB.x * AC.y - AB.y * AC.x;
	return abs(cross_prod) / 2;
}


bool Course::IsOnRoad(glm::vec2 car_pos) 
{
	glm::vec2 P1, P2, P3;	// A triangle to check if car_pos is inside
	float area_sum, triangle_area;
	int size = (int)inner_points.size();

	// Check for each pair of triangles given by 2 segments (inner & outer [i])
	for (int i = 0; i < size; i++) {
		// First triangle
		P1 = glm::vec2(inner_points[i].x, inner_points[i].z) * road_scale;
		P2 = glm::vec2(outer_points[i].x, outer_points[i].z) * road_scale;

		if (i == size - 1) {
			P3 = glm::vec2(outer_points[0].x, outer_points[0].z) * road_scale;
		} else {
			P3 = glm::vec2(outer_points[i + 1].x, outer_points[i + 1].z) * road_scale;
		}
		
		triangle_area = TriangleArea(P1, P2, P3);

		area_sum = 0;
		area_sum += TriangleArea(P1, P2, car_pos);
		area_sum += TriangleArea(P1, P3, car_pos);
		area_sum += TriangleArea(P2, P3, car_pos);

		if (abs(triangle_area - area_sum) <= 0.001f) {
			return true;
		}

		// Second triangle
		if (i == size - 1) {
			P2 = glm::vec2(inner_points[0].x, inner_points[0].z) * road_scale;
		} else {
			P2 = glm::vec2(inner_points[i + 1].x, inner_points[i + 1].z) * road_scale;
		}
		
		triangle_area = TriangleArea(P2, P3, P1);

		area_sum = 0;
		area_sum += TriangleArea(P1, P2, car_pos);
		area_sum += TriangleArea(P1, P3, car_pos);
		area_sum += TriangleArea(P2, P3, car_pos);

		if (abs(triangle_area - area_sum) <= 0.001f) {
			return true;
		}
	}

	return false;
}



// Value t is in [0, 1]
glm::vec3 IntermediaryPoint(glm::vec3 P1, glm::vec3 P2, float t)
{
	glm::vec3 delta = P2 - P1;
	glm::vec3 direction;
	float distance = glm::length(delta);

	if (distance == 0.0f)
	{
		return P1;
	}
	else
	{
		direction = delta / distance;
		return P1 + direction * (distance * t);
	}
}


void Course::ComputeIntermediaryPoints()
{
	float step = 0.001f;
	float t = 0.0f;
	glm::vec3 point;
	glm::vec3 P1, P2;

	int size = (int)polygon_points.size();

	for (int i = 0; i < size; i++) {
		P1 = polygon_points[i];

		if (i == size - 1) {
			P2 = polygon_points[0];		// For last segment
		} else {
			P2 = polygon_points[i + 1];
		}

		while (t <= 1) {
			point = IntermediaryPoint(P1, P2, t);
			polygon_points_extended.push_back(point);
			t += step;
		}

		t = 0.0f;
	}
}


void Course::ComputeInnerOuterPoints()
{
	glm::vec3 P1, P2;	// End points for a segment
	glm::vec3 D;		// Direction vector from P1 to P2
	glm::vec3 P;		// Perpendicular on D
	glm::vec3 up = glm::vec3(0, 1, 0);	// Perpendicular on XoZ plane
	glm::vec3 inner, outer;				// Resulting points

	int size = (int)polygon_points.size();

	for (int i = 0; i < size; i++) {
		P1 = polygon_points[i];

		if (i == size - 1) {
			P2 = polygon_points[0];		// For last segment
		} else {
			P2 = polygon_points[i + 1];
		}

		D = glm::normalize(P2 - P1);
		P = glm::normalize(cross(D, up));

		inner = P1 - inner_dist * P;
		outer = P1 + outer_dist * P;

		inner_points.push_back(inner);
		outer_points.push_back(outer);
	}
}


void Course::ComputeInnerOuterPointsExtended() 
{
	glm::vec3 P1, P2;	// End points for a segment
	glm::vec3 D;		// Direction vector from P1 to P2
	glm::vec3 P;		// Perpendicular on D
	glm::vec3 up = glm::vec3(0, 1, 0);	// Perpendicular on XoZ plane
	glm::vec3 inner, outer;				// Resulting points
	glm::vec3 tree_0, tree_1, tree_2, tree_3;		// Same for tree locations

	int size = (int)polygon_points_extended.size();

	for (int i = 0; i < size; i++) {
		P1 = polygon_points_extended[i];

		if (i == size - 1) {
			P2 = polygon_points_extended[0];
		} else {
			P2 = polygon_points_extended[i + 1];
		}

		D = glm::normalize(P2 - P1);
		P = glm::normalize(cross(D, up));

		inner = P1 - inner_dist * P;
		outer = P1 + outer_dist * P;

		inner_points_extended.push_back(inner);
		outer_points_extended.push_back(outer);

		// Same for tree locations
		tree_0 = P1 - tree_dist_0 * P;
		tree_1 = P1 + tree_dist_0 * P;
		tree_2 = P1 - tree_dist_1 * P;
		tree_3 = P1 + tree_dist_1 * P;

		tree_locations_0.push_back(tree_0);
		tree_locations_1.push_back(tree_1); 
		tree_locations_2.push_back(tree_2);
		tree_locations_3.push_back(tree_3);
	}

	// Compute vector to hold which (random) side to place a model (tree) on
	srand(static_cast <unsigned> (time(0)));
	int side;
	for (int i = 0; i < size; i++) {
		side = rand() % 4;
		locations.push_back(side);
	}

	// Same for model rotations
	int rotation;
	for (int i = 0; i < size; i++) {
		rotation = rand() % 12;
		rotations.push_back(rotation);
	}
}


void Course::ComputeLinesMesh() 
{
	lines = new Mesh("lines");

	vector<VertexFormat> vertices;
	vector<unsigned int> indices;
	glm::vec3 color = glm::vec3(1, 1, 1);
	int k = 0;

	lines->SetDrawMode(GL_LINES);
	glLineWidth(5);

	int size = (int)polygon_points_extended.size();
	for (int i = 0; i < size - 300; i += 300) {
		vertices.push_back(VertexFormat(polygon_points_extended[i], color));
		i += 300;
		vertices.push_back(VertexFormat(polygon_points_extended[i], color));

		indices.push_back(k++);
		indices.push_back(k++);
	}

	lines->InitFromData(vertices, indices);
}


void Course::ComputeCourseMesh() 
{
	int size = (int)inner_points_extended.size(); 
	vector<VertexFormat> vertices;
	vector<unsigned int> indices;
	int k = 0;

	course = new Mesh("course");
	course->SetDrawMode(GL_TRIANGLE_STRIP);

	for (int i = 0; i < size; i++) {
		vertices.push_back(VertexFormat(inner_points_extended[i], color));
		vertices.push_back(VertexFormat(outer_points_extended[i], color));

		indices.push_back(k++);
		indices.push_back(k++);
	}

	indices.push_back(0);
	indices.push_back(1);
	
	course->InitFromData(vertices, indices);
}


void Course::SetPolygonPoints()
{
	polygon_points =
	{
		glm::vec3(0, 0, 1),			// C
		glm::vec3(0.4, 0, 0.99),	// D
		glm::vec3(0.84, 0, 0.91),	// E
		glm::vec3(1.29, 0, 0.7),	// F
		glm::vec3(1.99, 0, 0.27),	// G
		glm::vec3(2.76, 0, -0.24),	// J3
		glm::vec3(3.58, 0, -0.79),	// K3
		glm::vec3(4.38, 0, -1.33),	// L3
		glm::vec3(5.3, 0, -1.94),	// H
		glm::vec3(5.89, 0, -2.32),	// I
		glm::vec3(6.29, 0, -2.45),	// J
		glm::vec3(6.81, 0, -2.45),	// K
		glm::vec3(7.25, 0, -2.28),	// L
		glm::vec3(7.66, 0, -1.98),	// M
		glm::vec3(7.87, 0, -1.51),	// N
		glm::vec3(7.85, 0, -1),		// O
		glm::vec3(7.77, 0, -0.52),	// P
		glm::vec3(7.46, 0, 0.5),	// M3
		glm::vec3(7.15, 0, 1.52),	// N3
		glm::vec3(6.81, 0, 2.63),	// O3
		glm::vec3(6.49, 0, 3.7),	// Q
		glm::vec3(6.35, 0, 4.17),	// R
		glm::vec3(6.31, 0, 4.66),	// S
		glm::vec3(6.39, 0, 5.12),	// T
		glm::vec3(6.55, 0, 5.6),	// U
		glm::vec3(6.8, 0, 6.03),	// V
		glm::vec3(7.18, 0, 6.43),	// W

		glm::vec3(7.74, 0, 6.81),	// Z
		glm::vec3(8.7, 0, 7.46),	// P3
		glm::vec3(9.76, 0, 8.18),	// Q3
		glm::vec3(10.65, 0, 8.79),	// R3
		glm::vec3(11.52, 0, 9.38),	// A1
		glm::vec3(11.83, 0, 9.81),	// B1
		glm::vec3(11.88, 0, 10.28),	// C1
		glm::vec3(11.82, 0, 10.71),	// D1
		glm::vec3(11.62, 0, 11.12),	// E1
		glm::vec3(11.31, 0, 11.52),	// F1
		glm::vec3(10.95, 0, 11.82),	// G1
		glm::vec3(10.57, 0, 12.05),	// H1
		glm::vec3(10.16, 0, 12.11),	// I1
		glm::vec3(9.09, 0, 12.13),	// S3
		glm::vec3(8, 0, 12.16),		// T3
		glm::vec3(6.74, 0, 12.18),	// U3
		glm::vec3(5.58, 0, 12.21),	// J1
		glm::vec3(5.25, 0, 12.26),	// K1
		glm::vec3(4.84, 0, 12.39),	// L1
		glm::vec3(4.47, 0, 12.58),	// M1
		glm::vec3(4.12, 0, 12.83),	// N1
		glm::vec3(3.76, 0, 13.23),	// O1
		glm::vec3(3.45, 0, 13.66),	// P1
		glm::vec3(3.03, 0, 14.71),	// V3
		glm::vec3(2.58, 0, 15.82),	// W3
		glm::vec3(2.15, 0, 16.88),	// Z3
		glm::vec3(1.65, 0, 18.12),	// Q1
		glm::vec3(1.5, 0, 18.54),	// R1
		glm::vec3(1.31, 0, 18.89),	// S1
		glm::vec3(0.92, 0, 19.16),	// T1
		glm::vec3(0.54, 0, 19.31),	// U1
		glm::vec3(0, 0, 19.35),		// V1

		glm::vec3(-0.54, 0, 19.31),		// -U1
		glm::vec3(-0.92, 0, 19.16),		// -T1
		glm::vec3(-1.31, 0, 18.89),		// -S1
		glm::vec3(-1.5, 0, 18.54),		// -R1
		glm::vec3(-1.65, 0, 18.12),		// -Q1
		glm::vec3(-2.15, 0, 16.88),		// Z3
		glm::vec3(-2.58, 0, 15.82),		// W3
		glm::vec3(-3.03, 0, 14.71),		// V3
		glm::vec3(-3.45, 0, 13.66),		// -P1
		glm::vec3(-3.76, 0, 13.23),		// -O1
		glm::vec3(-4.12, 0, 12.83),		// -N1
		glm::vec3(-4.47, 0, 12.58),		// -M1
		glm::vec3(-4.84, 0, 12.39),		// -L1
		glm::vec3(-5.25, 0, 12.26),		// -K1
		glm::vec3(-5.58, 0, 12.21),		// -J1
		glm::vec3(-6.74, 0, 12.18),		// -U3
		glm::vec3(-8, 0, 12.16),		// -T3
		glm::vec3(-9.09, 0, 12.13),		// -S3
		glm::vec3(-10.16, 0, 12.11),	// -I1
		glm::vec3(-10.57, 0, 12.05),	// -H1
		glm::vec3(-10.95, 0, 11.82),	// -G1
		glm::vec3(-11.31, 0, 11.52),	// -F1
		glm::vec3(-11.62, 0, 11.12),	// -E1
		glm::vec3(-11.82, 0, 10.71),	// -D1
		glm::vec3(-11.88, 0, 10.28),	// -C1
		glm::vec3(-11.83, 0, 9.81),		// -B1
		glm::vec3(-11.52, 0, 9.38),		// -A1
		glm::vec3(-10.65, 0, 8.79),		// R3
		glm::vec3(-9.76, 0, 8.18),		// Q3
		glm::vec3(-8.7, 0, 7.46),		// P3
		glm::vec3(-7.74, 0, 6.81),		// -Z

		glm::vec3(-7.12, 0, 6.8),	// H3
		glm::vec3(-6.43, 0, 6.77),	// G3
		glm::vec3(-5.85, 0, 6.83),	// F3
		glm::vec3(-5.3, 0, 6.93),	// E3
		glm::vec3(-4.8, 0, 7.07),	// D3
		glm::vec3(-4.26, 0, 7.32),	// C3
		glm::vec3(-3.87, 0, 7.7),	// B3
		glm::vec3(-3.67, 0, 8.09),	// A3

		glm::vec3(-3.58, 0, 8.53),	// -P2
		glm::vec3(-3.27, 0, 9.09),	// -Q2
		glm::vec3(-2.88, 0, 9.57),	// -R2
		glm::vec3(-2.48, 0, 9.92),	// -S2
		glm::vec3(-1.93, 0, 10.23),	// -T2
		glm::vec3(-1.33, 0, 10.4),	// -U2
		glm::vec3(-0.75, 0, 10.49),	// -V2
		glm::vec3(0, 0, 10.56),		// W2

		glm::vec3(0.75, 0, 10.49),	// V2
		glm::vec3(1.33, 0, 10.4),	// U2
		glm::vec3(1.93, 0, 10.23),	// T2
		glm::vec3(2.48, 0, 9.92),	// S2
		glm::vec3(2.88, 0, 9.57),	// R2
		glm::vec3(3.27, 0, 9.09),	// Q2
		glm::vec3(3.58, 0, 8.53),	// P2

		glm::vec3(3.7, 0, 7.87),	// O2
		glm::vec3(3.72, 0, 7.22),	// N2
		glm::vec3(3.64, 0, 6.45),	// M2
		glm::vec3(3.44, 0, 5.83),	// L2
		glm::vec3(3.17, 0, 5.32),	// K2

		glm::vec3(2.65, 0, 4.99),	// -F2
		glm::vec3(2.11, 0, 4.63),	// -G2
		glm::vec3(1.39, 0, 4.23),	// -H2
		glm::vec3(0.57, 0, 3.96),	// -I2
		glm::vec3(0, 0, 3.88),		// J2

		glm::vec3(-0.57, 0, 3.96),	// I2
		glm::vec3(-1.39, 0, 4.23),	// H2
		glm::vec3(-2.11, 0, 4.63),	// G2
		glm::vec3(-2.65, 0, 4.99),	// F2
		glm::vec3(-3.24, 0, 5.35),	// E2
		glm::vec3(-3.89, 0, 5.43),	// D2
		glm::vec3(-4.57, 0, 5.32),	// C2
		glm::vec3(-5.27, 0, 5.06),	// B2
		glm::vec3(-5.79, 0, 4.71),	// A2
		glm::vec3(-6.17, 0, 4.33),	// Z1

		glm::vec3(-6.49, 0, 3.7),	// -Q
		glm::vec3(-6.81, 0, 2.63),	// -O3
		glm::vec3(-7.15, 0, 1.52),	// -N3
		glm::vec3(-7.46, 0, 0.5),	// -M3
		glm::vec3(-7.77, 0, -0.52),	// -P
		glm::vec3(-7.85, 0, -1),	// -O
		glm::vec3(-7.87, 0, -1.51),	// -N
		glm::vec3(-7.66, 0, -1.98),	// -M
		glm::vec3(-7.25, 0, -2.28),	// -L
		glm::vec3(-6.81, 0, -2.45),	// -K
		glm::vec3(-6.29, 0, -2.45),	// -J
		glm::vec3(-5.89, 0, -2.32),	// -I
		glm::vec3(-5.3, 0, -1.94),	// -H
		glm::vec3(-4.38, 0, -1.33),	// -L3
		glm::vec3(-3.58, 0, -0.79),	// -K3
		glm::vec3(-2.76, 0, -0.24),	// -J3
		glm::vec3(-1.99, 0, 0.27),	// -G
		glm::vec3(-1.29, 0, 0.7),	// -F
		glm::vec3(-0.84, 0, 0.91),	// -E
		glm::vec3(-0.4, 0, 0.99),	// -D
	};
}


Course::~Course()
{
}