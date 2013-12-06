
/* 
 * Auteur Anis Benyoub
 * Classe qui permet de gérer la camera
 */
 
#ifndef CAMERA
#define CAMERA

#include "math/matrix4.h"
#include "math/vector3.h"
#include "defines.h"

class Camera
{
	public:
		Camera();
		~Camera();

		void Yaw(double parAngle);
		void Pitch(double parAngle);
		void Translate(const Vector3& parDir);
		void InjectFixedValues(GLuint parShaderID);
		void UpdateValues(GLuint parShaderID);

	protected:
		Matrix4 FTransformation;
		double FLens;
		double FAngleView;
		double FHauteurEcran;
		double FPasX;
		double FPasY;

};

#endif //CAMERA