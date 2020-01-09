#include <Dragonfly/editor.h>
#include <Dragonfly/detail/buffer.h> //will be replaced
#include <Dragonfly/detail/vao.h> //will be replaced
#include <Dragonfly/detail/Framebuffer/Framebuffer.h> //will be replaced
#include <GL/glew.h>
#include <vector>


int main(int argc, char* args[])
{
	df::Camera cam;
	Sample sam; //handles Events and such
	sam.AddHandlerClass(cam, 5);
	sam.AddStaticHandlerClass<df::ImGuiHandler>(10);

	eltecg::ogl::ArrayBuffer MyVBO;	MyVBO.constructMutable(std::vector<glm::vec2>{ {-1, -1}, { 1, -1 }, { 0, 1 }}, GL_STATIC_DRAW);
	eltecg::ogl::VertexArray MyVAO;	MyVAO.addVBO<glm::vec2>(MyVBO);

	df::VaoArrays myMeshVAO((GLuint)MyVAO, GL_TRIANGLE_STRIP, 3, 0u); // temporary solution
	
	auto myTextureFromFile = df::Texture2D<df::u8vec3>("Assets/xpos.png");
	//auto myCubeMap		   = df::TextureCube<>("xpos.png", "xneg.png", "ypos.png", "yneg.png", "zpos.png", "zneg.png");
	//auto myTextureView	   = myCubeMap[df::X_POS];
	
	df::ShaderProgramEditorVF myMeshProgram = "MyShaderProgram";
	myMeshProgram << "Shaders/vert.vert"_vert << "Shaders/frag.frag"_frag << df::LinkProgram;
	df::ShaderProgramEditorVF myPostProcess = "Postprocess shader program";
	myPostProcess << "Shaders/postprocess.vert"_vert << "Shaders/postprocess.frag"_frag << df::LinkProgram;
	
	//auto myMeshProgram = df::Program("Shaders/postprocess.vert"_vert + "Shaders/postprocess.frag"_frag);
	//auto myPostProcess = df::Program("Shaders/postprocess.vert"_vert + "Shaders/postprocess.frag"_frag);
	
	int w = df::Backbuffer.getWidth(), h = df::Backbuffer.getHeight();
	
	auto myFramebuffer = df::Texture2D<>(w, h, 1) + df::Renderbuffer<df::depth24>(w, h);

	sam.AddHandlerClass(df::Backbuffer);
	sam.AddResize([&](int w, int h) {myFramebuffer = myFramebuffer.MakeResized(w, h); });
	
	sam.Run([&](float deltaTime)
		{
			cam.Update();

			myFramebuffer << df::Clear() << myMeshProgram << "myTexUniform" << myTextureFromFile;
			myMeshProgram << myMeshVAO;

			df::Backbuffer << df::Clear() << myPostProcess << "myLastRender" << myFramebuffer.getColor<0>();
			myPostProcess << df::NoVao(GL_TRIANGLES, 3);
			
			myMeshProgram.Render(); //only the UI!!
			myPostProcess.Render();
		}
	);
	return 0;
}

