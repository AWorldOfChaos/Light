#include "light.hpp"

#include "imgui.h"

class ExampleLayer : public Light::Layer
{
public:
	ExampleLayer(): Light::Layer("TestLayer"), camera(45.0f, 1.6f/0.9f, 1.0f, 100.0f)
	{
		shader.reset(Light::Shader::create("../Light/shadersrc/test.vs", "../Light/shadersrc/test.fs"));

		vao.reset(Light::VertexArray::create());
		squareVao.reset(Light::VertexArray::create());

		float vertices[] = {
			0, 0, 0, 1.0, 0.0, 0.0, 1.0,
			0, 0.5, 0, 0.0, 1.0, 0.0, 1.0,
			0.5, 0, 0, 0.0, 0.0, 1.0, 1.0
		};

		float squareVertices[] = {
			-0.2, -0.2, 0, 0.3, 0.2, 0.8, 1.0,
			-0.2, 0.7, 0, 0.3, 0.2, 0.8, 1.0,
			0.7, -0.2, 0, 0.3, 0.2, 0.8, 1.0,
			0.7, 0.7, 0, 0.3, 0.2, 0.8, 1.0
		};

		vbo.reset(Light::VertexBuffer::create(vertices, sizeof(vertices)));
		squareVbo.reset(Light::VertexBuffer::create(squareVertices, sizeof(squareVertices)));

		Light::BufferLayout layout = {
			{ Light::ShaderDataType::Float3, "a_Position" },
			{ Light::ShaderDataType::Float4, "a_Color"}
		};

		vbo->setLayout(layout);
		squareVbo->setLayout(layout);

		unsigned int indices[3] = { 0, 1, 2};
		unsigned int squareIndices[6] = {0, 1, 2, 2, 1, 3};
		
		ibo.reset(Light::IndexBuffer::create(indices, sizeof(indices)/sizeof(unsigned int)));
		squareIbo.reset(Light::IndexBuffer::create(squareIndices, sizeof(squareIndices)/sizeof(unsigned int)));

		vao->addVertexBuffer(vbo);
		vao->setIndexBuffer(ibo);
		squareVao->addVertexBuffer(squareVbo);
		squareVao->setIndexBuffer(squareIbo);

		position = camera.getPosition();

	}
	~ExampleLayer() {}

	void onUpdate(Light::Timestep ts) override
	{

		if(Light::Input::isKeyPressed(LIGHT_KEY_W))
		{
			position.z += cameraPositionSpeed * ts.getSeconds();
		}
		else if(Light::Input::isKeyPressed(LIGHT_KEY_S))
		{
			position.z -= cameraPositionSpeed * ts.getSeconds();
		}
		else if(Light::Input::isKeyPressed(LIGHT_KEY_A))
		{
			position.x -= cameraPositionSpeed * ts.getSeconds();
		}
		else if(Light::Input::isKeyPressed(LIGHT_KEY_D))
		{
			position.x += cameraPositionSpeed * ts.getSeconds();
		}

		camera.setPosition(position);

		Light::Renderer::beginScene(camera);
		
		Light::Renderer::submit(shader, squareVao);
		Light::Renderer::submit(shader, vao);

		Light::Renderer::endScene();
	}

	void onEvent(Light::Event& e) override
	{

	}

	void onImguiRender() override
	{
	}

private:
	std::shared_ptr<Light::VertexArray> vao;
	std::shared_ptr<Light::VertexArray> squareVao;
	std::shared_ptr<Light::Shader> shader;
	std::shared_ptr<Light::VertexBuffer> vbo;
	std::shared_ptr<Light::VertexBuffer> squareVbo;
	std::shared_ptr<Light::IndexBuffer> ibo;
	std::shared_ptr<Light::IndexBuffer> squareIbo;
	Light::PerspectiveCamera camera;

	glm::vec3 position;
	float cameraPositionSpeed = 0.01f;
};

class Editor : public Light::Application
{
public:
	Editor()
	{
		pushLayer(new ExampleLayer());
	}
	~Editor() {}
	
};

Light::Application* Light::createApplication()
{
	return new Editor();
}