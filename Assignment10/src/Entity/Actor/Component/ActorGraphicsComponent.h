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

				glUniformMatrix4fv(mvp, 1, GL_FALSE, glm::value_ptr(graphics.calculateMVP(controller.getTransformation())));
				glUniformMatrix4fv(m, 1, GL_FALSE, glm::value_ptr(controller.getTransformation()));
				//glUniformMatrix4fv(n, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(controller.getTransformation())))));
			
				mesh->render(program);
			} //if

			Component::onTick(delta);
		} //onTick

	public:

		Mesh* mesh;
		
		ActorGraphicsComponent(Actor& controller, const string& name) :
			Component(controller, (System&)controller.getManager().context[typeid(GraphicsSystem)], typeid(GraphicsSystem), name),
			mesh(NULL)
		{
		} //ActorGraphicsComponent

		virtual ~ActorGraphicsComponent()
		{
			destroy();
		} //~ActorGraphicsComponent
}; //ActorGraphicsComponent
