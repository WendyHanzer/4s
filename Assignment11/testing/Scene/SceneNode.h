#pragma once

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <queue>

#include "Scene.h"
#include "Objects/MoveableObject.h"

/*
event destroy 	(children will register to this and move to this node's parent when it is dead)
				(if no parent, you removed the root, which invalidates the scene. NEVER REMOVE THE ROOT!)
event tick 		(graphics, can do very efficient rendering)
*/

class Scene;

class SceneNode
{
	private:
		Scene& scene;
		SceneNode* parent;
		
		SceneNode* first;		//first child
		SceneNode* last;		//last child
		SceneNode* next;		//next sibling
		SceneNode* prev;		//previous sibling
	
		glm::quat rotation;
		glm::vec3 scale;
		glm::vec3 translation;
		
		glm::mat4 transformation;

	public:
	
		SceneNode(SceneNode& scene);
		SceneNode(SceneNode& parent);
		
		virtual ~SceneNode();
	
		struct iterator
		{
			private:
			
				SceneNode* node;
				queue<SceneNode*> children;
				
			public:
			
				iterator& operator++()
				{
					if (node)
					{
						if (children.empty())
						{
							node = node->first;
							
							if (node)
							{
								children.push(node->first);
							}
						} //if
						else if (node->first != children.front())
						{
							children.push(node->first);
							node = node->next;
						} //else if
					} //if
					
					if (!node && !children.empty())
					{
						node = children.front();
						children.pop();
					} //if
					
					return *this;
				}
				
				inline iterator(SceneNode* node) : node(node) { }
				inline iterator& operator++(const int i) { ++(*this); return *this; }
				inline SceneNode& operator*() { return *node; }
				inline SceneNode* operator->() { return node; }
				inline const bool operator==(const iterator& rhs) { return node == rhs.node; }
				inline const bool operator!=(const iterator& rhs) { return node != rhs.node; }
				
		}; //const_iterator
	
		struct const_iterator
		{
			private:
			
				SceneNode* node;
				queue<SceneNode*> children;
				
			public:
			
				const_iterator& operator++()
				{
					if (node)
					{
						if (children.empty())
						{
							node = node->first;
							children.push(node->first);
						} //if
						else if (node->first != children.front())
						{
							children.push(node->first);
							node = node->next;
						} //else if
					} //if
					
					if (!node && !children.empty())
					{
						node = children.front();
						children.pop();
					} //if
					
					return *this;
				}
				
				inline const_iterator(SceneNode* node) : node(node) { }
				inline const_iterator& operator++(const int i) { ++(*this); return *this; }
				inline const SceneNode& operator*() { return *node; }
				inline const SceneNode* operator->() { return node; }
				inline const bool operator==(const const_iterator& rhs) { return node == rhs.node; }
				inline const bool operator!=(const const_iterator& rhs) { return node != rhs.node; }
				
		}; //const_iterator
		
		iterator begin()
        {
            return iterator(const_cast<SceneNode*>(this));
        } //begin
 
        iterator end()
        {
            return iterator(NULL);
        } //end
 
        const_iterator begin() const
        {
            return const_iterator(const_cast<SceneNode*>(this));
        } //begin
 
        const_iterator end() const
        {
            return const_iterator(NULL);
        } //end
	
		/*
		*	relative transformations
		*/
		void lookAt(const glm::vec3& point);
		void lookAt(const SceneNode& node);
		void lookAt(const SceneNode& node, const glm::vec3& offset = {0, 0, 0});

		void rotate(const float& radians, const glm::vec3& axis);
		void rotate(const glm::quat& orientation);
		
		void translate(const glm::vec3& translation);
		void translate(const float& distance, const glm::quat& orientation);
		
		void scale(const glm::vec3& scale);

		/*
		*	local sets
		*/
		void setRotationX(const float& rotation);
		void setRotationY(const float& rotation);
		void setRotationZ(const float& rotation);
		void setRotation(const glm::vec3& rotation);
		void setOrientation(const glm::quat& orientation);
		
		void setPosition(const glm::vec3& position);
		void setPosition(const float& distance, const glm::quat& orientation);
		
		void setScale(const glm::vec3& scale);

		/*
		*	global sets
		*/
		void setWorldRotationX(const float& rotation);
		void setWorldRotationY(const float& rotation);
		void setWorldRotationZ(const float& rotation);
		void setWorldRotation(const glm::vec3& rotation);
		void setWorldOrientation(const glm::quat& orientation);
		
		void setWorldPosition(const glm::vec3& position);
		void setWorldPosition(const float& distance, const glm::quat& orientation);
		
		void setWorldScale(const glm::vec3& scale);

		const glm::quat& getOrientation();
		const glm::vec3& getPosition();
		const glm::vec3& getScale();
		const glm::mat4 getTransformation();

		const glm::quat& getWorldOrientation();
		const glm::vec3& getWorldPosition();
		const glm::vec3& getWorldScale();
		const glm::mat4 getWorldTransformation();

		inline void addChild(SceneNode& sceneNode);
		inline void remove();

		inline Scene& getScene() { return scene; }
		inline SceneNode* getParent() { return parent; }
		
		inline auto getChildren() { first; }

		inline glm::mat4& getTransformation() { return transformation; }
}; //SceneNode
