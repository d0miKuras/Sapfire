#include "engine/Test.h"
#include "GL/glew.h"
#include "engine/Log.h"
#include "engine/events/WindowEvent.h"
#include "engine/renderer/Buffer.h"
#include "engine/renderer/Shader.h"
#include "engine/renderer/Window.h"

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
TestApp::TestApp()
{
	mWindow = std::unique_ptr<Window>(Window::Create());
	mWindow->SetEventCallback(BIND_EVENT_FN(TestApp::OnEvent));
	mLayerStack.PushLayer(new Layer());
	Log::Init();

	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	float vertices[3 * 3] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};
	uint32_t indices[] = {0, 1, 2};

	// Specify the vertex attributes
	// (For now, assume one vertex format)
	// Position is 3 floats starting at offset 0
	/* glEnableVertexAttribArray(0); */
	/* glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0); */
	/* glEnableVertexAttribArray(1); */
	/* glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, reinterpret_cast<void *>(sizeof(float) *
	 * 3)); */
	/* glEnableVertexAttribArray(2); */
	/* glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, reinterpret_cast<void *>(sizeof(float) *
	 * 6)); */
	mVB.reset(VertexBuffer::Create(vertices, sizeof(vertices) / sizeof(float)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

	mIB.reset(IndexBuffer::Create(indices, 3));
	mShader.reset(Shader::Create());
	mShader->Load("../Shaders/Triangle.vert", "../Shaders/Triangle.frag");
}

TestApp::~TestApp()
{
}

void TestApp::OnEvent(Event &event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(TestApp::OnWindowClose));

	for (auto it = mLayerStack.end(); it != mLayerStack.begin();)
	{
		(*--it)->OnEvent(event);
		if (event.Handled)
			break;
	}
}

void TestApp::Tick()
{
	while (mRunning)
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		mShader->Bind();
		glBindVertexArray(mVertexArray);
		glDrawElements(GL_TRIANGLES, mIB->GetCount(), GL_UNSIGNED_INT, nullptr);
		mWindow->OnUpdate();
	}
}

bool TestApp::OnWindowClose(WindowCloseEvent &e)
{
	mRunning = false;
	return true;
}
