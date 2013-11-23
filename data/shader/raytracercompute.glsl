#version 430


//Destination du rendu
layout(rgba8) uniform image2D renderCanvas;

layout (local_size_x = 16, local_size_y = 16) in;

const int MAX_PRIM = 20;
// Camera informations
uniform vec3 cameraPosition;
//Z axis
uniform vec3 cameraDirection;
// Up axis
uniform vec3 cameraUpAxis;
// Third Axis
uniform vec3 cameraXAxis;
//View Angle
uniform float viewAngle;
// Distance focale
uniform float focalDistance;
//Attributs précalcules pour soucis de perf
uniform vec3 coinSupGauche;
uniform float unitX;
uniform float unitY;

// Primitives
struct Triangle
{
	vec3 p0;
	vec3 p1;
	vec3 p2;
};

struct Plan
{
	vec3 p1;
	vec3 p2;
	vec3 p3;
	vec3 p4;
};

struct Quadrique
{
	float A;
	float B;
	float C;
	float D;
	float E;
	float F;
	float G;
	float H;	
	float I;
	float J;			
};

// Rayon
struct Ray
{
	vec3 origin;
	vec3 direction;				
};

// Rayon
struct Intersection
{
	bool isValid;
	vec3 point;
	float distance;				
	vec3 normale;
	Primitive obj;	
};

//Materiau

struct Materiau
{
	vec4 color;
};
struct Primitive
{
	int type;
	int index;
};
// Objets
struct ObjectQ
{
	Quadrique quadric;
	Materiau material;
};

struct ObjectT
{
	Triangle triangle;
	Materiau material;
};

struct ObjectP
{
	Plan plan;
	Materiau material;
};


Intersection IntersectWithScene(Ray parRay, Primitive[MAX_PRIM] parPrim)
{
	Intersection intersect;
	return intersect
}
vec4 computeBRDF(Ray parRay, Intersection parIntersect)
{
	vec4 color;
	return color;
}

Primitive[MAX_PRIM] getPrimitives(Ray parRay)
{
	Primitive listePrim[MAX_PRIM]
	return  listePrim;
}

vec4 Reflect(Ray parRay, Intersection parIntersect)
{
	vec4 color;
	if(parIntersect.isValid)
	{
		Ray reflected;
		reflected.origin = parIntersect.point - EPSILON*parIntersect.normal;		
		refracted.direction = reflect(parRay.direction,parIntersect.normal);
		color = CouleurPixel(refracted);
	}
	return color;
}
vec4 Refract(Ray parRay, Intersection parIntersect,)
{
	vec4 color;
	if(parIntersect.isValid)
	{
		Ray refracted;
		refracted.origin = parIntersect.point + EPSILON*parIntersect.normal;	
		// Indice de réraction a changer	
		refracted.direction = refract(parRay.direction,parIntersect.normal,0.5);
		color = CouleurPixel(refracted);
	}
	return color;
}


vec4 computeBRDF(Ray parRay, Intersection parIntersect)
{
	vec4 color;
	return color;
}
vec4 CouleurPixel(Ray parRayon)
{
	vec4 finalColor;
	Primitive listePrim[MAX_PRIM] = getPrimitives(parRayon);
	Intersection intersect = IntersectWithScene(parRayon,listePrim);
	finalColor = computeBRDF(Ray, intersect);
	finalColor*=Reflect(Ray, Intersection);
	finalColor*=Refract(Ray, Intersection);
	return finalColor;
}

vec4 RayTrace(vec2 storePos)
{
	//Calcul du rayon a lancer
	Ray rayon;
	rayon.origin = cameraPosition;
	vec3 screenPoint = coinSupGauche + unitX * storePos.x + unitY * storePos.y;
	rayon.direction = screenPoint - cameraPosition;
	// Lancer
	return CouleurPixel(rayon);
}
void main() 
{
	 // Recuperation de la taille de l'image
     ivec2 sizeXY = imageSize(renderCanvas);
     
     // On récupère le point de la texture qui nous a été attribue
     ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);
     imageStore(renderCanvas, storePos, RayTrace(storePos));
}
