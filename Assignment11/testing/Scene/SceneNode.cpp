#include "SceneNode.h"

//scene node is an entity
//has graphics aspect
SceneNode::SceneNode(SceneNode& scene) :
	scene(scene),
	parent(NULL),
	first(NULL),
	last(NULL),
	next(NULL),
	prev(NULL)
{
	scene.root = *this;
} //SceneNode

SceneNode::SceneNode(SceneNode& parent) :
	scene(scene)
	parent(NULL),
	first(NULL),
	last(NULL),
	next(NULL),
	prev(NULL)
{
	parent.add(const_cast<SceneNode&>(*this));
} //Parent

virtual ~SceneNode::SceneNode()
{
	remove();
} //SceneNode

/*
*	measurements
*/
//http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/#How_do_I_find_the_rotation_between_2_vectors__
glm::quat quatBetweenVectors(const glm::vec3& start, const glm::vec3& dest)
{
	const glm::vec3 startN = glm::normalize(start);
	const glm::vec3 destN = glm::normalize(dest);
 
	const float cosTheta = glm::dot(startN, destN);
	glm::vec3 rotationAxis;
 
	if (cosTheta < -1 + 0.001f)
	{
		rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), startN);
		
		if (gtx::norm::length2(rotationAxis) < 0.01 )
		{
			rotationAxis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), startN);
		} //if
 
		rotationAxis = glm::normalize(rotationAxis);
		return gtx::quaternion::angleAxis(180.0f, rotationAxis);
    } //if
 
	rotationAxis = glm::cross(startN, destN);
 
	const float s = glm::sqrt((1 + cosTheta)*2);
	const float invs = 1/s;
 
    return glm::quat(s*0.5f, rotationAxis.x*invs, rotationAxis.y*invs, rotationAxis.z*invs);
} //quatBetweenVectors

inline glm::quat quatBetweenQuats(const glm::quat& start, const glm::quat& end)
{
	//q' = end*start
	//q'*start^-1 = end
	return end*glm::inverse(start));
} //quatBetweenQuats

/*
*	conversions
*/
inline glm::vec3 offset2Vector(const float& distance, const glm::quat& orientation)
{
	return orientation*(distance*glm::vec3(0, 0, -1));
} //offset2Vector

/*
*	transformations
*/
inline glm::quat rotate(const glm::quat& orienation, const glm::quat& transformation)
{
	return transformation*orientation;
} //rotate

/*******************************************************************************************************/
//
//	glm::quat quatBetweenVectors(const glm::vec3& start, const glm::vec3& dest)
//	glm::quat quatBetweenQuats(const glm::quat& start, const glm::quat& end)
//
//	glm::quat gtx::quaternion::angleAxis(const float& radians, const glm::vec3& axis);
//
//	inline glm::vec3 offset2Vector(const float& distance, const glm::quat& orientation)
//
//	glm::quat glm::rotate(const glm::quat& orientation, const float& radians, const glm::vec3& axis)
//	glm::quat rotate(const glm::quat& orienation, const glm::quat& transformation)

/*
*	relative transformations
*/
void SceneNode::lookAt(const glm::vec3& point)
{
	rotation *= quatBetweenVectors(position, point);
} //lookat
void SceneNode::lookAt(const SceneNode& node)
{
	lookAt(node.position);
} //lookAt
void SceneNode::lookAt(const SceneNode& node, const glm::vec3& offset)
{
	lookAt(node.position + offset);
} //lookAt

void SceneNode::rotate(const float& radians, const glm::vec3& axis)
{
	rotation = glm::rotate(rotation, radians, axis);
} //rotate

void SceneNode::rotate(const glm::quat& orientation)
{
	rotation = rotate(rotation, orientation);
} //rotate

void SceneNode::translate(const glm::vec3& translation)
{
	position += translation;
} //translate
void SceneNode::translate(const float& distance, const glm::quat& orientation)
{
	position += offset2Vector(distance, orientation);
} //translate

void SceneNode::scale(const glm::vec3& scale)
{
	scale += scale;
} //scale

/*
*	local sets
*/
void SceneNode::setRotation(const float& radians, const glm::vec3& axis)
{
	rotation = gtx::quaternion::angleAxis(radians, axis);
} //setRotation
void SceneNode::setOrientation(const glm::quat& orientation)
{
	rotation = orientation;
} //setOrientation

void SceneNode::setPosition(const glm::vec3& position)
{
	this->position = position;
} //setPosition
void SceneNode::setPosition(const float& distance, const glm::quat& orientation)
{
	position = offset2Vector(distance, orientation);
} //setPosition

void SceneNode::setScale(const glm::vec3& scale)
{
	this->scale = scale;
} //setScale

/*
*	global sets
*/
void SceneNode::setWorldRotation(const float& radians, const glm::vec3 axis)
{
	setWorldOrientation(gtx::quaternion::angleAxis(radians, axis));
} //setWorldRotation
void SceneNode::setWorldOrientation(const glm::quat& orientation)
{
	if (parent)
	{
		rotation = quatBetweenQuats(parent->getWorldOrientation(), orientation);
	} //if
	else
	{
		rotation = rotate(rotation, orientation);
	} //else
} //setWorldOrientation

void SceneNode::setWorldPosition(const glm::vec3& position)
{
	if (parent)
	{
		this->position = position - parent->getWorldPosition();
	} //if
	else
	{
		this->position = position;
	} //else
} //setWorldPosition
void SceneNode::setWorldPosition(const float& distance, const glm::quat& orientation)
{
	setWorldPosition(offset2Vector(distance, orientation));
} //setWorldPosition

void SceneNode::setWorldScale(const glm::vec3& scale)
{
	if (parent)
	{
		this->scale = scale - parent->getWorldScale();
	} //if
	else
	{
		this->scale = scale;
	} //else
} //setWorldScale

const glm::quat& SceneNode::getOrientation()
{
	return orientation;
} //getOrientation
const glm::vec3& SceneNode::getPosition()
{
	return position;
} //getPosition
const glm::vec3& SceneNode::getScale()
{
	return scale;
} //getScale
const glm::mat4 SceneNode::getTransformation()
{
	return glm::scale(glm::toMat4(orientation)*glm::translate(glm::mat4(1.0f), position), scale);
} //getTransformation

const glm::quat& SceneNode::getWorldOrientation()
{
	if (parent)
	{
		return rotate(parent->getWorldOrientation(), rotation);
	} //if
	else
	{
		return rotation;
	} //else
} //getWorldOrientation

const glm::vec3& SceneNode::getWorldPosition()
{
	if (parent)
	{
		return parent->getWorldPosition() + position;
	} //if
	else
	{
		return position;
	} //else
} //getWorldPosition

const glm::vec3& SceneNode::getWorldScale()
{
	if (parent)
	{
		return parent->getWorldScale() + scale;
	} //if
	else
	{
		return scale;
	} //else
} //getWorldScale

const glm::mat4 SceneNode::getWorldTransformation()
{
	return glm::scale(glm::toMat4(getWorldOrientation())*glm::translate(glm::mat4(1.0f), getWorldPosition()), getWorldScale());
} //getWorldTransformation

inline void SceneNode::add(SceneNode& sceneNode)
{
	sceneNode.parent = this;
	
	sceneNode.next = NULL;
	sceneNode.prev = last;

	if (last == NULL)
	{
		first = &sceneNode;
	} //if
	else
	{
		last->next = &sceneNode;
	} //else

	last = &sceneNode;
} //add

inline void SceneNode::remove()
{
	if (next)
	{
		next->prev = prev;
	} //if
	else if (parent)
	{
		parent->last = prev;
	} //else
	if (prev)
	{
		prev->next = next;
	}
	else if (parent)
	{
		parent->first = next;
	} //else
} //remove

inline Scene& SceneNode::getScene() { return scene; }
inline SceneNode* SceneNode::getParent() { return parent; }

inline auto SceneNode::getChildren() { first; }

inline glm::mat4& SceneNode::getTransformation() { return transformation; }
