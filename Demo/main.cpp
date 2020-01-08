#include <Dragonfly/editor.h>
#include <Dragonfly/detail/buffer.h> //will be replaced
#include <Dragonfly/detail/vao.h> //will be replaced
#include <Dragonfly/detail/Framebuffer/Framebuffer.h> //will be replaced
#include <GL/glew.h>
#include <vector>

using namespace df;

int main(int argc, char* args[])
{
	Sample sam; //handles Events and such
	Camera cam;
	sam.AddHandlerClass(cam, 5);
	sam.AddStaticHandlerClass<ImGuiHandler>(10);

	eltecg::ogl::ArrayBuffer MyVBO;
	MyVBO.constructMutable(std::vector<glm::vec2>{ {-1, -1}, { 1, -1 }, { 0, 1 }}, GL_STATIC_DRAW);

	eltecg::ogl::VertexArray MyVAO;
	MyVAO.addVBO<glm::vec2>(MyVBO);

	VaoArrays demoVao((GLuint)MyVAO, GL_TRIANGLE_STRIP, 3, 0u); // temporary solution
	NoVao noVao = NoVao(GL_TRIANGLES, 3);

	TextureCube<> testCubemap("Assets/xpos.png", "Assets/xneg.png", "Assets/ypos.png", "Assets/yneg.png", "Assets/zpos.png", "Assets/zneg.png");
	Texture2D<> testTex = testCubemap[TextureType::TEX_CUBE_X_POS]; // 2D view of a cubemap face

	ShaderProgramEditorVF program = "MyShaderProgram";
	program << "Shaders/vert.vert"_vs << "Shaders/frag.frag"_fs << LinkProgram;
	ShaderProgramEditorVF postprocess = "Postprocess shader program";
	postprocess << "Shaders/postprocess.vert"_vs << "Shaders/postprocess.frag"_fs << LinkProgram;

	auto frameBuff = Renderbuffer<depth24>(Backbuffer.getWidth(), Backbuffer.getHeight()) + Texture2D<>(Backbuffer.getWidth(), Backbuffer.getHeight(), 1);

	sam.AddHandlerClass(Backbuffer);
	sam.AddResize([&](int w, int h) {frameBuff = frameBuff.MakeResized(w, h); });
	
	GL_CHECK; //extra opengl error checking in GPU Debug build configuration

	sam.Run([&](float deltaTime)
		{
			cam.Update();

			frameBuff << ClearColor<0>(0.f, 0.1f, 0.5f) << ClearDepth() << program << "texImg" << testTex;
			program << demoVao;	//Rendering: Ensures that both the vao and program is attached

			Backbuffer << ClearColor(0.f, 0.1f, 0.5f) << ClearDepth() << postprocess << "texFrame" << frameBuff.getColor<0>();
			postprocess << noVao;

			GL_CHECK;
			program.Render(); //only the UI!!
			postprocess.Render();
		}
	);
	return 0;
}

