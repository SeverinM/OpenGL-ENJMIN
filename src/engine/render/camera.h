#pragma once


class YCamera
{
public:

	typedef enum {
		PROJECTION_ORTHO,
		PROJECTION_PERSPECTIVE
	} PROJTYPE;

	
	YVec3<float> Position; ///< Position de la camera
	YVec3<float> LookAt; ///< Point regarde par la camera
	YVec3<float> Direction; ///< Direction de la camera
	YVec3<float> UpVec; ///< Vecteur up de la camera
	YVec3<float> RightVec; ///< Si on se place dans la camera, indique la droite	
	YVec3<float> UpRef; ///< Ce qu'on considère comme le "haut" dans notre monde (et pas le up de la cam)
	

	PROJTYPE ProjType; ///< Type de projection
	float FovY; ///< fov angle sur y in degrees
	float Ratio; ///< with / height
	float Left; ///< Left YPlane
	float Right;
	float Bottom;
	float Top;
	float Near;
	float Far;
			
	YCamera()
	{
		Position = YVec3<float>(0, -1, 0);
		LookAt = YVec3<float>(0, 0, 0);
		UpRef = YVec3<float>(0, 0, 1);
		UpVec = UpRef;
		FovY = 45.0f;
		Ratio = 800.0f / 600.0f;
		Near = 0.1f;
		Far = 800.0f;
		setProjectionPerspective(FovY, Ratio, Near, Far);
		updateVecs();
	}

	void setProjectionPerspective(float fovy, float ratio, float nearPlane, float farPlane) {
		ProjType = PROJECTION_PERSPECTIVE;
		FovY = fovy;
		Ratio = ratio;
		Near = nearPlane;
		Far = farPlane;
	}

	void setProjectionOrtho(float left, float right, float bottom, float top, float nearPlane, float farPlane) {
		ProjType = PROJECTION_ORTHO;
		Left = left;
		Right = right;
		Bottom = bottom;
		Top = top;
		Near = nearPlane;
		Far = farPlane;
	}


	/**
	* Mise a jour de la camera
	*/
	virtual void update(float elapsed)
	{

	}

	/**
	* Definition du point regarde
	*/
	void setLookAt(YVec3<float> lookat)
	{
		LookAt = lookat;
		updateVecs();
	}

	/**
	* Definition de la position de la camera
	*/
	void setPosition(YVec3<float> pos)
	{
		Position = pos;
		updateVecs();
	}

	/**
	* Definition du haut de notre monde
	*/
	void setUpRef(YVec3<float> upRef)
	{
		UpRef = upRef;
		updateVecs();
	}

	/**
	* Deplacement de la camera d'un delta donné
	*/
	void move(YVec3<float> delta)
	{
		Position += delta;
		LookAt += delta;
		updateVecs();
	}

	void moveOnXY(float value1, float value2, bool fromWorld = false)
	{
		YVec3f output(0, 0, 0);

		if (!fromWorld)
		{
			//Sur Right
			output += RightVec * value1;
			//Sur Up
			output += UpVec * value2;
		}

		else
		{
			output += YVec3f(1, 0, 0) * value1;
			output += YVec3f(0, 1, 0) * value2;
		}
		
		LookAt += output;
		Position += output;
		updateVecs();
	}

	/**
	* Deplacement de la camera à un point donné
	*/
	void moveTo(const YVec3<float> & target)
	{
		
	}

	/**
	* On recalcule les vecteurs utiles au déplacement de la camera (Direction, RightVec, UpVec)
	* on part du principe que sont connus :
	* - la position de la camera
	* - le point regarde par la camera
	* - la vecteur up de notre monde
	*/
	void updateVecs(void)
	{
		Direction = (LookAt - Position).normalize();
		RightVec = Direction.cross(UpRef).normalize();
		UpVec = RightVec.cross(Direction).normalize();
		look();
	}

	/**
	* Rotation droite gauche en subjectif
	*/
	//Rotation en Z et en degrés : convertit en radians
	void rotate(float angle)
	{
		angle *= (M_PI / 180.0f);
		LookAt -= Position;
		LookAt.rotate(UpRef, angle);
		LookAt += Position;
		updateVecs();
	}

	/**
	* Rotation haut bas en subjectif
	*/
	void rotateUp(float angle)
	{
		angle *= (M_PI / 180.0f);
		
		YVec3f previousPos = Position;

		LookAt -= Position;
		LookAt.rotate(RightVec, angle);
		LookAt += Position;

		YVec3f normPos = Position;
		normPos.normalize();
		float newAngle = normPos.dot(UpRef);
		if (newAngle > 0.99 || newAngle < -0.99)
			Position = previousPos;

		updateVecs();
	}

	/**
	* Rotation droite gauche en troisième personne
	*/
	void rotateAround(float angle)
	{
		angle *= (M_PI / 180.0f);
		
		Position -= LookAt;

		//On ne monte pas trop haut pour ne pas passer de l'autre coté
		YVec3f previousPos = Position;
		Position.rotate(RightVec, angle);
		YVec3f normPos = Position;
		normPos.normalize();
		float newAngle = normPos.dot(UpRef);
		if (newAngle > 0.99 || newAngle < -0.99)
			Position = previousPos;

		Position += LookAt;
		updateVecs();
	}

	/**
	* Rotation haut bas en troisième personne
	*/
	void rotateUpAround(float angle)
	{
		angle *= (M_PI / 180.0f);
		YVec3f delta(LookAt - Position);
		YVec3f lookAtOrigin(LookAt);
		LookAt = YVec3f(0, 0, 0);
		Position = -delta;
		Position.rotate(RightVec, angle);
		delta = LookAt - Position;
		LookAt = lookAtOrigin;
		Position = LookAt - delta;
		updateVecs();
	}

	void SetFovY(float value)
	{
		value = abs(value);
		FovY = max(1, min(89, value));
	}

	void IncrementFovY(float delta)
	{
		SetFovY(FovY + delta);
	}

	/**
	* Calcul du bon repère de départ pour la matrice monde
	*/
	void look(void)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if (ProjType == PROJECTION_PERSPECTIVE) {
			gluPerspective(FovY, Ratio, Near, Far);
		}
		if (ProjType == PROJECTION_ORTHO) {
			glOrtho(Left, Right, Bottom, Top, Near, Far);
		}
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(Position.X, Position.Y, Position.Z, LookAt.X, LookAt.Y, LookAt.Z, UpVec.X, UpVec.Y, UpVec.Z);
		
	}
};
