//
//  Broadphase.cpp
//
#include "fspch.h"

#include "Broadphase.h"
#include "glm/glm.hpp"

//struct psuedoBody_t {
//	int id;
//	float value;
//	bool ismin;
//};
//
//int CompareSAP(const void* a, const void* b) {
//	const psuedoBody_t* ea = (const psuedoBody_t*)a;
//	const psuedoBody_t* eb = (const psuedoBody_t*)b;
//
//	if (ea->value < eb->value) {
//		return -1;
//	}
//	return 1;
//}
//
//void SortBodiesBounds(const Body* bodies, const int num, psuedoBody_t* sortedArray, const float dt_sec) {
//	//use this as base axis to avoid all the objects on the same axis;
//	Vec3 axis = Vec3(1, 1, 1);
//	axis.Normalize();
//	glm::vec3 test = glm::vec3();
//	
//
//	for (int i = 0; i < num; i++) {
//		const Body& body = bodies[i];
//		Bounds bounds = body.m_shape->GetBounds(body.m_position, body.m_orientation);
//
//		//expand bounds by velocity and a little extra
//		bounds.Expand(bounds.mins + body.m_linearVelocity * dt_sec);
//		bounds.Expand(bounds.maxs + body.m_linearVelocity * dt_sec);
//
//		const float epsilon = 0.01f;
//		bounds.Expand(bounds.mins + Vec3(-1, -1, -1) * epsilon);
//		bounds.Expand(bounds.maxs + Vec3(1, 1, 1) * epsilon);
//
//		sortedArray[i * 2 + 0].id = i;
//		sortedArray[i * 2 + 0].value = axis.Dot(bounds.mins);
//		sortedArray[i * 2 + 0].ismin = true;
//
//		sortedArray[i * 2 + 1].id = i;
//		sortedArray[i * 2 + 1].value = axis.Dot(bounds.maxs);
//		sortedArray[i * 2 + 1].ismin = false;
//	}
//
//	qsort(sortedArray, num * 2, sizeof(psuedoBody_t), CompareSAP);
//}
//
//void BuildPairs(std::vector< collisionPair_t >& collisionPairs, const psuedoBody_t* sortedBodies, const int num) {
//	collisionPairs.clear();
//
//	//build collision pairs from sorted bodies
//	for (int i = 0; i < num * 2; i++) {
//		const psuedoBody_t& a = sortedBodies[i];
//		if (!a.ismin) {
//			continue;
//		}
//
//		collisionPair_t pair;
//		pair.a = a.id;
//
//		for (int j = i + 1; j < num * 2; j++) {
//			const psuedoBody_t& b = sortedBodies[j];
//			//b's bound is a's max bound, ie we hit the max bound of a and the current item and future items will not collide with a
//			if (b.id == a.id) {
//				break;
//			}
//
//			if (!b.ismin) {
//				continue;
//			}
//			pair.b = b.id;
//			collisionPairs.push_back(pair);
//		}
//	}
//}
//
//void SweepAndPrune1D(const Body* bodies, const int num, std::vector< collisionPair_t >& finalPairs, const float dt_sec) {
//	psuedoBody_t* sortedBodies = (psuedoBody_t*)alloca(sizeof(psuedoBody_t) * num * 2);
//
//	SortBodiesBounds(bodies, num, sortedBodies, dt_sec);
//	BuildPairs(finalPairs, sortedBodies, num);
//}
//
///*
//====================================================
//BroadPhase
//====================================================
//*/
//void BroadPhase(const Body* bodies, const int num, std::vector< collisionPair_t >& finalPairs, const float dt_sec) {
//	finalPairs.clear();//why clear twice
//
//	SweepAndPrune1D(bodies, num, finalPairs, dt_sec);
//}

namespace Fengshui
{
	struct psuedoBody_t {
		EntityID id;
		float value;
		bool ismin;
	};

	int CompareSAP(const void* a, const void* b) {
		const psuedoBody_t* ea = (const psuedoBody_t*)a;
		const psuedoBody_t* eb = (const psuedoBody_t*)b;

		if (ea->value < eb->value) {
			return -1;
		}
		return 1;
	}

	void SortBodiesBounds(const std::set<EntityID> entities, const int num, psuedoBody_t* sortedArray, const float dt_sec) {
		//use this as base axis to avoid all the objects on the same axis;
		glm::vec3 axis = glm::vec3(1, 1, 1);
		axis = glm::normalize(axis);

		int i = 0;

		for (EntityID entity : entities) {
			const Transform transform = GeneralManager::GetComponent<Transform>(entity);
			const Rigidbody rb = GeneralManager::GetComponent<Rigidbody>(entity);
			const Collider collider = GeneralManager::GetComponent<Collider>(entity);
			Bounds bounds = collider.Shape->GetBounds(transform.Position, transform.Rotation);

			//expand bounds by velocity and a little extra
			bounds.Expand(bounds.mins + rb.LinearVelocity * dt_sec);
			bounds.Expand(bounds.maxs + rb.LinearVelocity * dt_sec);

			const float epsilon = 0.01f;
			bounds.Expand(bounds.mins + glm::vec3(-1, -1, -1) * epsilon);
			bounds.Expand(bounds.maxs + glm::vec3(1, 1, 1) * epsilon);

			//Sorting needs changing, I am not using an array here
			sortedArray[i * 2 + 0].id = entity;
			sortedArray[i * 2 + 0].value = glm::dot(axis, bounds.mins);
			sortedArray[i * 2 + 0].ismin = true;

			sortedArray[i * 2 + 1].id = entity;
			sortedArray[i * 2 + 1].value = glm::dot(axis, bounds.maxs);
			sortedArray[i * 2 + 1].ismin = false;
			i++;
		}

		qsort(sortedArray, num * 2, sizeof(psuedoBody_t), CompareSAP);
	}

	void BuildPairs(std::vector< collisionPair_t >& collisionPairs, const psuedoBody_t* sortedBodies, const int num) {
		collisionPairs.clear();

		//build collision pairs from sorted bodies
		for (int i = 0; i < num * 2; i++) {
			const psuedoBody_t& a = sortedBodies[i];
			if (!a.ismin) {
				continue;
			}

			collisionPair_t pair;
			pair.a = a.id;

			for (int j = i + 1; j < num * 2; j++) {
				const psuedoBody_t& b = sortedBodies[j];
				//b's bound is a's max bound, ie we hit the max bound of a and the current item and future items will not collide with a
				if (b.id == a.id) {
					break;
				}

				if (!b.ismin) {
					continue;
				}
				pair.b = b.id;
				collisionPairs.push_back(pair);
			}
		}
	}

	void BroadPhase(const std::set<EntityID> entities, const int num, std::vector<collisionPair_t>& finalPairs, const float dt_sec) {
		finalPairs.clear();//why clear twice
		psuedoBody_t* sortedBodies = (psuedoBody_t*)_malloca(sizeof(psuedoBody_t) * num * 2);

		SortBodiesBounds(entities, num, sortedBodies, dt_sec);
		BuildPairs(finalPairs, sortedBodies, num);
	}

}