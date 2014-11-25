#pragma once

#include "../../../Engine/Entity/Component/Component.h"
#include "../../../Graphics/GraphicsSystem.h"
#include "../../../Graphics/mesh.h"
#include "../../../Graphics/shaderProgram.h"
#include "../Actor.h"

class ActorGraphicsComponent : public Component
{
	protected:
	
		virtual void onChangeController()
		{
		} //onChangeController

		virtual void onTick(const long double& delta)
		{
			if (mesh)
			{
				Actor& controller = (Actor&)getController();
				GraphicsSystem& graphics = ((GraphicsSystem&)(System&)mapping);
				ShaderProgram& program = *graphics.program;
				
				GLuint mvp = program.uniform("MVP");
				GLuint m = program.uniform("M");
				//GLuint n = program.uniform("N");
				
				glm::mat4 transform = controller.getTransformation();
				
				if (!ignoreWorldRotation)
				{
					transform = graphics.calculateM(transform);
				} //if

				glUniformMatrix4fv(mvp, 1, GL_FALSE, glm::value_ptr(graphics.calculateMVP(transform)));
				glUniformMatrix4fv(m, 1, GL_FALSE, glm::value_ptr(transform));
				//glUniformMatrix4fv(n, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(controller.getTransformation())))));
			
				mesh->render(program, texture);
			} //if

			Component::onTick(delta);
		} //onTick

	public:

		/*
		*	this is the mesh to be rendered
		*/
		Mesh* mesh;
		
		/*
		*	this overrides all textures in the mesh
		*	global override isn't good, should override specific materials
		*/
		Texture* texture;
		
		bool ignoreWorldRotation;
		
		ActorGraphicsComponent(Actor& controller, const string& name, const bool& ignoreWorldRotation = false) :
			Component(controller, (System&)controller.getManager().context[typeid(GraphicsSystem)], typeid(GraphicsSystem), name),
			mesh(NULL),
			texture(NULL),
			ignoreWorldRotation(ignoreWorldRotation)
		{
		} //ActorGraphicsComponent

		virtual ~ActorGraphicsComponent()
		{
			destroy();
		} //~ActorGraphicsComponent
}; //ActorGraphicsComponent
