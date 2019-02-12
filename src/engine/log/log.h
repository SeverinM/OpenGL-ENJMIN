#ifndef	__YOCTO_LOG__
#define	__YOCTO_LOG__

#include "engine/utils/types.h"
#include "engine/utils/utils.h"
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class YLog	
{
	public:
		
		typedef enum {
			USER_INFO,
			USER_ERROR,
			ENGINE_INFO,
			ENGINE_ERROR,
			ENGINE_WARNING
		}MSG_TYPE;

		struct Ray
		{
			YVec3f origin;
			YVec3f direction;
			YColor color;
			float length;
			float displayLeft;
		};

		void Update(float elapsed)
		{
			for (vector<Ray>::iterator it = allDebugRay.begin(); it != allDebugRay.end(); it++)
			{
				it->displayLeft -= elapsed;
			}
		}

		void Add(YVec3f position, YVec3f direction, float length = 1, float duration = 1, YColor colr = YColor(1, 1, 1, 1))
		{
			Ray ray;
			ray.origin = position;
			ray.direction = direction;
			ray.length = length;
			ray.displayLeft = duration;
			ray.color = colr;
			allDebugRay.push_back(ray);
		}

		void Render()
		{
			for (vector<Ray>::iterator it = allDebugRay.begin(); it != allDebugRay.end(); it++)
			{
				if (it->displayLeft > 0)
				{
					glColor3d(it->color.R, it->color.V, it->color.B);
					glVertex3d(it->origin.X, it->origin.Y, it->origin.Z);
					glVertex3d(it->origin.X + (it->length * it->direction.X), it->origin.Y + (it->length * it->direction.Y), it->origin.Z + (it->length * it->direction.Z));
				}
			}
		}

		static YLog * getInstance()
		{
			return _Instance;
		}

	protected:
		vector<Ray> allDebugRay;
		bool _HideEngineLog;
		unsigned long _LastMessageTime;
		inline static YLog * _Instance;
		YLog()
		{
			_HideEngineLog = false;
			_LastMessageTime = GetTickCount();
		}

		bool mustDelete(const Ray & ray)
		{
			return ray.displayLeft < 0;
		}

		virtual void logMsg(MSG_TYPE type, const char *message, bool intro=true, bool ret=true) = 0;
	public:

		static void log(MSG_TYPE type, const char *message, bool intro=true, bool ret=true)
		{
			if(!_Instance)
				return;
			if(_Instance->_HideEngineLog && (type == ENGINE_ERROR || type == ENGINE_INFO))
				return;
			_Instance->logMsg(type,message,intro,ret);
			_Instance->_LastMessageTime = GetTickCount();
		}

		static void showEngineLog(bool show)
		{
			_Instance->_HideEngineLog = !show;
		}

};


#endif