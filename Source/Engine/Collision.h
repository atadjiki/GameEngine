#include <unordered_set>
#include <vector>
#include <algorithm> 

#include "Matrix4.h"
#include "AABB.h"
#include "GameObject.h"
#include "Transform.h"
#include "MessagingSystem.h"


namespace Math {

	namespace Collision {

		struct CollisionInfo
		{
			bool hit = false;
			long long collisionTime = -1;
			Vector4 postVelocities[2];
			SmartPointer<GameObject> * collidedObjects[2];

			void PrintInfo() {

				printf("Collision Between:{%s,%s}\n", (*collidedObjects[0])->tag.c_str(), (*collidedObjects[1])->tag.c_str());
				printf("Occured at location - ");
				(*collidedObjects[0])->physics->transform.position.printVector();
				printf("\nTime: %I64d\n", collisionTime);
				printf("Post Velocities:\n");
				postVelocities[0].printVector();
				postVelocities[1].printVector();
				printf("\n----------\n");


			}
		};

		static CollisionInfo BuildCollisionInfo(SmartPointer<GameObject> * &A, SmartPointer<GameObject> * &B, const Vector4 &v1, const Vector4 &v2, const long long &collisionTime, const bool &hit) {

			CollisionInfo collision = CollisionInfo();
			collision.collidedObjects[0] = A;
			collision.collidedObjects[1] = B;

			collision.postVelocities[0] = v1;
			collision.postVelocities[1] = v2;
			collision.collisionTime = collisionTime;

			collision.hit = hit;

			return collision;
		}

		static Matrix4 LocalToWorld(Transform const& A) {

			Matrix4 rotation = RotationMatrix_Z(A.rotation.z);
			Matrix4 translation = TranslationMatrix_Col(A.position);

			Matrix4 ToWorld = translation * rotation;

			return ToWorld;
		}

		static Matrix4 WorldToLocal(Transform const& A) {

			Matrix4 AtoWorld = LocalToWorld(A);
			return Invert_4x4(AtoWorld);
		}


		static Matrix4 AToB(const Transform &A, const Transform &B) {

			return WorldToLocal(B) * LocalToWorld(A);
		}

		static Vector4 TransformFromAtoB(const Transform &A, const Transform &B, const Vector4 &toTransform) {

			Matrix4 A_to_B = AToB(A, B);

			Vector4 A_point = toTransform;
			A_point.w = 1.0f;

			Vector4 A_BB_CenterInB = A_to_B * A_point;

			return A_BB_CenterInB;
		}

		static float AProjectionOntoB_X(const Transform &A, const Transform &B) {

			float result;

			Vector4 AExtentsXInB = AToB(A, B) * Vector4(A.bounds.extents.x, 0, 0, 0);
			Vector4 AExtentsYInB = AToB(A, B) * Vector4(0, A.bounds.extents.y, 0, 0);

			result = fabs(AExtentsXInB.x) + fabs(AExtentsYInB.x);

			return result;
		}

		static float AProjectionOntoB_Y(const Transform &A, const Transform &B) {

			float result;

			Vector4 AExtentsXInB = AToB(A, B) * Vector4(A.bounds.extents.x, 0, 0, 0);
			Vector4 AExtentsYInB = AToB(A, B) * Vector4(0, A.bounds.extents.y, 0, 0);

			result = fabs(AExtentsXInB.y) + fabs(AExtentsYInB.y);

			return result;
		}

		static Vector4 VelocityARelativeToB(const Vector4 &A, const Vector4 &B) {

			Vector4 result;

			result = A - B;

			return result;

		}

		static Vector4 VelocityAInB(SmartPointer<GameObject> * &A, SmartPointer<GameObject> * &B) {


			Vector4 VelARelToB = VelocityARelativeToB((*A)->physics->velocity, (*B)->physics->velocity);

			Vector4 VelAInB = WorldToLocal((*B)->physics->transform) * VelARelToB;

			return VelAInB;

		}

		//time it takes the left edge of A's projection onto B to pass the right edge of B's projection
		static long long  GetOpenGapTime_X(SmartPointer<GameObject> * &A, SmartPointer<GameObject> * &B) {

			Vector4 B_Center = (*B)->physics->transform.bounds.center;
			Vector4 B_Extents = (*B)->physics->transform.bounds.extents;

			Vector4 A_CenterInB = TransformFromAtoB((*A)->physics->transform, (*B)->physics->transform, (*A)->physics->transform.bounds.center);
			float AOnBX = AProjectionOntoB_X((*A)->physics->transform, (*B)->physics->transform);

			Vector4 AVelocityInB = VelocityAInB(A, B);

			if (AVelocityInB.x == 0) {
				return 0;
			}

			return static_cast<long long>(((B_Center.x + B_Extents.x) - (A_CenterInB.x - AOnBX)) / AVelocityInB.x);

		}

		//time it takes the left edge of A's projection onto B to pass the right edge of B's projection
		static long long  GetOpenGapTime_Y(SmartPointer<GameObject> * &A, SmartPointer<GameObject> * &B) {

			Vector4 B_Center = (*B)->physics->transform.bounds.center;
			Vector4 B_Extents = (*B)->physics->transform.bounds.extents;

			Vector4 A_CenterInB = TransformFromAtoB((*A)->physics->transform, (*B)->physics->transform, (*A)->physics->transform.bounds.center);
			float AOnBY = AProjectionOntoB_Y((*A)->physics->transform, (*B)->physics->transform);

			Vector4 AVelocityInB = VelocityAInB(A, B);

			if (AVelocityInB.y == 0) {
				return 0;
			}

			return  static_cast<long long>(((B_Center.y + B_Extents.y) - (A_CenterInB.y - AOnBY)) / AVelocityInB.y);

		}

		static long long  GetCloseGapTime_X(SmartPointer<GameObject> * &A, SmartPointer<GameObject> * &B) {

			Vector4 B_Center = (*B)->physics->transform.bounds.center;
			Vector4 B_Extents = (*B)->physics->transform.bounds.extents;

			Vector4 A_CenterInB = TransformFromAtoB((*A)->physics->transform, (*B)->physics->transform, (*A)->physics->transform.bounds.center);
			float AOnBX = AProjectionOntoB_X((*A)->physics->transform, (*B)->physics->transform);

			Vector4 AVelocityInB = VelocityAInB(A, B);

			if (AVelocityInB.x == 0) {
				return 0;
			}

			return  static_cast<long long>((B_Center.x - (B_Extents.x + AOnBX) - A_CenterInB.x) / AVelocityInB.x);

		}

		static long long GetCloseGapTime_Y(SmartPointer<GameObject> * &A, SmartPointer<GameObject> * &B) {

			Vector4 B_Center = (*B)->physics->transform.bounds.center;
			Vector4 B_Extents = (*B)->physics->transform.bounds.extents;

			Vector4 A_CenterInB = TransformFromAtoB((*A)->physics->transform, (*B)->physics->transform, (*A)->physics->transform.bounds.center);
			float AOnBY = AProjectionOntoB_Y((*A)->physics->transform, (*B)->physics->transform);

			Vector4 AVelocityInB = VelocityAInB(A, B);

			if (AVelocityInB.y == 0) {
				return 0;
			}

			return   static_cast<long long>((B_Center.y - (B_Extents.y + AOnBY) - A_CenterInB.y) / AVelocityInB.y);

		}

		static bool CheckSeparatingAxis(const AABB &A, const AABB &B) {

			//we know the BBs dont intersect if there is a separating axis
			//there is a separation if: distance between centers > sum of extens
			//this assumes that both AABB's are in the same coordinate system to make sure you have already adjusted them

			float distance_between_centers_x = fabs(A.center.x - B.center.x);
			float distance_extents_A_x = fabs(A.extents.x);
			float distance_extents_B_x = fabs(B.extents.x);

			bool AB;

			if (distance_between_centers_x >= distance_extents_A_x + distance_extents_B_x) {

				AB = true;
			}
			else {
				AB = false;
			}

			float distance_between_centers_y = fabs(A.center.y - B.center.y);
			float distance_extents_A_y = fabs(A.extents.y);
			float distance_extents_B_y = fabs(B.extents.y);

			bool BA;

			if (distance_between_centers_y >= distance_extents_A_y + distance_extents_B_y) {

				BA = true;
			}
			else {
				BA = false;
			}

			if (AB || BA) {
				return true;
			}
			else {
				return false;
			}
		}

		static bool CheckForCollision(const Transform &A, const Transform &B) {

			//first project A into B and check for collision

			if (debug) printf("Projecting A onto B..\n\n");
			AABB A_on_B(A.bounds);
			A_on_B.center = TransformFromAtoB(A, B, A.bounds.center);
			bool AB;
			if (CheckSeparatingAxis(A_on_B, B.bounds) == false) { //if there is not a separation, we have a collision!
				
				AB = true;
			}
			else {
				AB = false;
			}

			if (debug) printf("\n\nProjecting A onto B..\n\n");
			AABB B_on_A(B.bounds);
			B_on_A.center = TransformFromAtoB(B, A, B.bounds.center);

			bool BA;
			if (CheckSeparatingAxis(B_on_A, A.bounds) == false) {
				BA = true;
			}
			else {
				BA = false;
			}
			

			//if we did not detect a gap on either projection, there was no collision

			if (AB && BA) {
				return true;
			}
			else {
				return false;
			}
		}

		static Vector4 PostCollisionVelocity(const float &m1, const float &m2, Vector4 &v1, Vector4 &v2) {

			Vector4 vel_final;

			vel_final = v1 * ((m1 - m2) / (m1 + m2)) + v2 * ((2 * m2) / (m1 + m2));

			return vel_final;
		}

		//𝑚_1 𝑣_1+𝑚_2 𝑣_2=𝑚_1 𝑣_1^′+𝑚_2 𝑣_2^′
		static std::vector<Vector4> CalculatePostVelocities(SmartPointer<GameObject> * &A, SmartPointer<GameObject> * &B) {

			float m1 = (*A)->physics->mass;
			float m2 = (*B)->physics->mass;
			Vector4 v1 = (*A)->physics->velocity;
			Vector4 v2 = (*B)->physics->velocity;

			std::vector<Vector4> velocities = std::vector<Vector4>(2);

			velocities[0] = PostCollisionVelocity(m1, m2, v1, v2);
			velocities[1] = PostCollisionVelocity(m2, m1, v2, v1);

			return velocities;
		}

		static CollisionInfo CheckCollisionsInFrame(SmartPointer<GameObject> * &A, SmartPointer<GameObject> * &B, const long long t_frame) {

			long long t_start = 0; //start of frame
			long long t_opens[4] = { 0  }; long long t_closes[4] = { 0 };

			//A in B coordinate system - x,y axes
			t_opens[0] = GetOpenGapTime_X(A, B);
			t_closes[0] = GetCloseGapTime_X(A, B);

			t_opens[1] = GetOpenGapTime_Y(A, B);
			t_closes[1] = GetCloseGapTime_Y(A, B);

			//B in A coordinate system - x,y axes
			t_opens[2] = GetOpenGapTime_X(B, A);
			t_closes[2] = GetCloseGapTime_X(B, A);

			t_opens[3] = GetOpenGapTime_Y(B, A);
			t_closes[3] = GetCloseGapTime_Y(B, A);


			//find the latest t_close and the earlier t_open
			long long t_open = *std::min_element(t_opens, t_opens + 4);
			long long t_close = *std::max_element(t_closes, t_closes + 4);
			

			std::vector<Vector4> postVelocities = CalculatePostVelocities(A, B);

			if (!GameObject::Equals(*(*A).operator->(), *(*B).operator->()) && CheckForCollision((*A)->physics->transform, (*B)->physics->transform)) {

				return BuildCollisionInfo(A, B, postVelocities[0], postVelocities[1], t_close, true);
			}

			if (t_close > t_open) {

				return BuildCollisionInfo(A, B, postVelocities[0], postVelocities[1], t_close, false);
			}

			if (t_close < t_open) {

				return BuildCollisionInfo(A, B, postVelocities[0], postVelocities[1], t_close, true);
			}
			else {
				return BuildCollisionInfo(A, B, postVelocities[0], postVelocities[1], t_close, false);
			}

		}

		struct CollidableDetails : public MessagingSystem::MessageDetails {

			SmartPointer<GameObject> * collidable;

			static CollidableDetails * Build(SmartPointer<GameObject> * A) {

				CollidableDetails * tmp = new CollidableDetails();
				tmp->collidable = A;
				return tmp;
			}

			~CollidableDetails() {

			}

		};

		class CollisionManager : public MessagingSystem::IMessageHandler {

		private:
			//we only support this many collidables at a time
			std::string addCollidableString = "AddCollidable";
			std::string removeCollidableString = "RemoveCollidable";

		public:

			std::vector<SmartPointer<GameObject> *> collidables;
			size_t addHash;
			size_t remHash;

			CollisionManager() : MessagingSystem::IMessageHandler() {
				RegisterMessageHandler(std::hash<std::string>{}(addCollidableString), this);
				RegisterMessageHandler(std::hash<std::string>{}(removeCollidableString), this);
				collidables = std::vector<SmartPointer<GameObject> *>();
				addHash = std::hash<std::string>{}(addCollidableString);
				remHash = std::hash<std::string>{}(removeCollidableString);

			}

			~CollisionManager() {

			}

			void HandleMessage(const std::size_t & i_Message, MessagingSystem::MessageDetails * details) override {


				if (i_Message == std::hash<std::string>{}(addCollidableString)) {

					printf("Handling Message - %s	", addCollidableString.c_str());
					SmartPointer<GameObject> * A = reinterpret_cast <CollidableDetails *>(details)->collidable;
					printf("For GameObject - %s\n", (*A)->tag.c_str());
					AddCollidable(A);
					delete details;

				}
				else if (i_Message == std::hash<std::string>{}(removeCollidableString)) {
					printf("Handling Message - %s	", removeCollidableString.c_str());
					SmartPointer<GameObject> * A = reinterpret_cast <CollidableDetails *>(details)->collidable;
					printf("For GameObject - %s\n", (*A)->tag.c_str());
					RemoveCollidable(A);
					delete details;
				}
			}

			bool AddCollidable(SmartPointer<GameObject> * &A) {


				collidables.push_back(A);
				return true;
				

			}

			bool RemoveCollidable(SmartPointer<GameObject> * &A) {

				if (collidables.size() <= 0) {

					return false;
				}
				else {
					for (size_t i = 0; i < collidables.size(); i++) {

						if (GameObject::Equals(*(*A).operator->(), *(*collidables[i]).operator->())) {
							collidables[i] = NULL;
							return true;
						}
					}

					return false;
				}
			}

			bool Clear() {

				collidables.clear();
			}

			std::vector<CollisionInfo> ProcessCollisions(long long t_frame) {

				std::vector<CollisionInfo> collisions = std::vector<CollisionInfo>();

				for (size_t i = 0; i < collidables.size(); i++) {

					for (size_t j = i; j <  collidables.size(); j++) {


						std::vector<Vector4> PostVelocities = CalculatePostVelocities(collidables[i] , collidables[j]);

					    CollisionInfo collisionInfo = CheckCollisionsInFrame(collidables[i] , collidables[j], t_frame);

						if (collisionInfo.hit && collisionInfo.collisionTime >= 0) {
							if (CheckForCollision((*collidables[i])->physics->transform, (*collidables[j])->physics->transform)) {
								collisions.insert(collisions.begin(), collisionInfo);
							}
						}

					}
				}

				return collisions;

			}
		};
		static CollisionManager _CollisionManager = CollisionManager();

		static void DisposeCollisionManager() {

			_CollisionManager.~CollisionManager();
		}
	}

}
