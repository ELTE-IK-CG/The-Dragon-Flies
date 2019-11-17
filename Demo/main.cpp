#include <Dragonfly/editor.h>
#include <Dragonfly/detail/buffer.h> //will be replaced
#include <Dragonfly/detail/vao.h> //will be replaced
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

	VaoWrapper demoVao((GLuint)MyVAO, GL_TRIANGLE_STRIP, 3, 0u); // temporary solution

	TextureCube<> testCubemap("Assets/xpos.png", "Assets/xneg.png", "Assets/ypos.png", "Assets/yneg.png", "Assets/zpos.png", "Assets/zneg.png");
	Texture2D<> testTex = testCubemap[TextureType::TEX_CUBE_X_POS]; // 2D view of a cubemap face

	ShaderProgramEditorVF program = "MyShaderProgram";

	program << "Shaders/types.glsl"_fs << "Shaders/sdf.glsl"_fs << "Shaders/main.frag.glsl"_fs;
	program << "Shaders/quad.vert.glsl"_vs << LinkProgram;
	GL_CHECK; //extra opengl error checking in GPU Debug build configuration

	sam.Run([&](float deltaTime)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			cam.Update();

			program << "col_intensity" << 1.f << "gCameraPos" << cam.GetEye(); //unfiorm upload
			program << "testTexture" << testTex; // texture-sampler upload
			program << demoVao;	//Rendering: Ensures that both the vao and program is attached

			GL_CHECK;
			program.Render(); //only the UI!!
		}
	);
	return 0;
}

